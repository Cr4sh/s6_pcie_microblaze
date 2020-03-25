#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <fsl.h>
#include <lwip/err.h>
#include <lwip/tcp.h>
#include <xparameters.h>
#include <xstatus.h>
#include <xil_cache.h>

#include "config.h"
#include "pcie_to_tcp.h"
#include "pcie_tlp.h"
#include "axi_dma.h"
#include "axis_pcie.h"
#include "flash.h"

#define MIN(_a_, _b_) (((_a_) < (_b_)) ? (_a_) : (_b_))
#define MAX(_a_, _b_) (((_a_) > (_b_)) ? (_a_) : (_b_))

#ifdef __arm__

extern volatile int TcpFastTimer;
extern volatile int TcpSlowTimer;

#endif

struct tcp_pcb *m_tpcb = NULL;

int m_connection = 1;
bool m_resident = false, m_verbose = false;

u8 *m_option_rom = (u8 *)BASE_ADDR_OPTION_ROM;

// DMA transmit/receive buffers
u8 m_buffer_rx[PROT_MAX_PACKET_SIZE];
u8 m_buffer_tx[PROT_MAX_PACKET_SIZE];

// protocol receive buffer
u8 m_buffer_recv[sizeof(PROT_CTL) + PROT_MAX_PACKET_SIZE];

u32 m_bytes_have = 0;
u32 m_bytes_need = sizeof(PROT_CTL);

void dma_rx_callback(void);
void dma_tx_callback(void);

int transfer_data(void) 
{

#ifdef __arm__

    if (TcpFastTimer) 
    {
        TcpFastTimer = 0;
        tcp_fasttmr();
    }

    if (TcpSlowTimer) 
    {
        TcpSlowTimer = 0;
        tcp_slowtmr();
    }

#endif

    // send pending TLPs
    tlp_handle_pending();

    return 0;
}

u32 option_rom_erase(void)
{
    // erase option ROM from flash
    return flash_write(OPTION_ROM_FLASH_ADDR, NULL, OPTION_ROM_MAX_SIZE);
}

int dma_rx_queue(void)
{
    // start DMA transfer
    if (axi_dma_queue_rx((void *)&m_buffer_rx, PROT_MAX_PACKET_SIZE, dma_rx_callback) != XST_SUCCESS)
    {
        return XST_FAILURE;   
    }

    return XST_SUCCESS;
}

int dma_tx_queue(u8 *data, u32 size)
{
    memcpy(&m_buffer_tx, data, size);

    // flush cache before the transfer
    Xil_DCacheFlushRange((u32)&m_buffer_tx, PROT_MAX_PACKET_SIZE);

    // start DMA transfer
    if (axi_dma_queue_tx((void *)&m_buffer_tx, size, dma_tx_callback) != XST_SUCCESS)
    {
        return XST_FAILURE;   
    }

    return XST_SUCCESS;
}

void dma_rx_callback(void)
{
    u32 size = 0;

    // invalidate cache after the transfer
    Xil_DCacheInvalidateRange((u32)&m_buffer_rx, PROT_MAX_PACKET_SIZE);

    // calculate TLP length
    if ((size = tlp_size(m_buffer_rx)) == 0)
    {
        xil_printf("dma_rx_callback(): Unknown TLP format, ignoring...\n");

        // don't transfer this TLP to the client
        goto _end;
    }

#ifdef USE_OPTION_ROM

    if (m_resident != 0)
    {
        // handle TLP on the soft processor size
        if (tlp_handle(m_buffer_rx, size) == TLP_IGNORE)
        {
            // don't transfer this TLP to the client
            goto _end;
        }    
    }

#endif // USE_OPTION_ROM

    if (m_tpcb)
    {
        err_t err;
        PROT_CTL reply;

        if (size > PROT_MAX_PACKET_SIZE)
        {
            xil_printf("dma_rx_callback() ERROR: TLP is too large, ignoring...\n");
            goto _end;
        }

        reply.code = PROT_CTL_TLP_RECV;
        reply.size = (u8)size;

#ifdef VERBOSE

        xil_printf("dma_rx_callback(): size = %d\n", reply.size);
#endif
        // send reply to the client
        if ((err = tcp_write(m_tpcb, (void *)&reply, sizeof(PROT_CTL), TCP_WRITE_FLAG_COPY)) == ERR_OK)
        {
            if ((err = tcp_write(m_tpcb, (void *)&m_buffer_rx, reply.size, TCP_WRITE_FLAG_COPY)) != ERR_OK)
            {
                xil_printf("tcp_write() ERROR %d\n", err);   
            }
        }
        else
        {
            xil_printf("tcp_write() ERROR %d\n", err);   
        }
    }
    else
    {
        xil_printf("dma_rx_callback() ERROR: Not connected\n");
    }

_end:

#ifndef RECV_AUTO

    if (m_resident)

#endif

    {
        // continue receiving TLPs
        dma_rx_queue();
    }
}

void dma_tx_callback(void)
{
    
}

void handle_request(struct tcp_pcb *tpcb, PROT_CTL *request)
{    
    u8 buffer[sizeof(PROT_CTL) + PROT_MAX_PACKET_SIZE];
    PROT_CTL *reply = (PROT_CTL *)&buffer;
    bool ignore = false;

    reply->code = PROT_CTL_ERROR_FAILED;
    reply->size = 0;

    // dispatch client request
    switch (request->code)
    {
    case PROT_CTL_PING:
        {

#ifdef VERBOSE

            xil_printf("recv_callback(): PROT_CTL_PING\n");
#endif
            reply->code = PROT_CTL_SUCCESS;
            break;
        }

    case PROT_CTL_RESET:
        {
            xil_printf("recv_callback(): PROT_CTL_RESET\n");

            // ...

            break;
        }

    case PROT_CTL_STATUS:
        {
            u32 device_status = 0;

#ifdef VERBOSE

            xil_printf("recv_callback(): PROT_CTL_STATUS\n");
#endif
            // get PCI-E device status
            device_status = axis_pcie_status();

            memcpy(reply->data, &device_status, sizeof(u32));

            reply->size = sizeof(u32);
            reply->code = PROT_CTL_SUCCESS;
            break;
        }

    case PROT_CTL_TLP_SEND:
        {

#ifdef VERBOSE

            xil_printf("recv_callback(): PROT_CTL_TLP_SEND\n");
#endif
            axi_dma_wait_tx();

            // send TLP to the PCI-E device
            if (dma_tx_queue(request->data, (u32)request->size) == XST_SUCCESS)
            {
                reply->code = PROT_CTL_SUCCESS;
            }

            break;
        }

    case PROT_CTL_TLP_RECV:
        {

#ifdef VERBOSE

            xil_printf("recv_callback(): PROT_CTL_TLP_RECV\n");
#endif

#ifndef RECV_AUTO

            if (!axi_dma_queued_rx())
            {
                // start receiving TLPs
                dma_rx_queue();
            }
#endif
            // don't send any reply
            ignore = true;  
            break;
        }

    case PROT_CTL_CONFIG:
        {

#ifdef VERBOSE

            xil_printf("recv_callback(): PROT_CTL_CONFIG\n");
#endif
            if (axis_pcie_status_bus_id() != 0)
            {
                u32 cfg_addr = 0, cfg_data = 0;

                memcpy(&cfg_addr, request->data, sizeof(u32));

                // read PCI-E config space
                cfg_data = axis_pcie_read_config(cfg_addr);

                memcpy(reply->data, &cfg_data, sizeof(u32));

                reply->size = sizeof(u32);
                reply->code = PROT_CTL_SUCCESS;
            }
            else
            {
                xil_printf("recv_callback(): PROT_CTL_CONFIG fails, device is not initialized\n");
            }

            break;
        }

    case PROT_CTL_TEST:
        {
            reply->size = PROT_MAX_PACKET_SIZE;
            reply->code = PROT_CTL_SUCCESS;

            break;
        }

#ifdef USE_OPTION_ROM

    case PROT_CTL_RESIDENT_ON:
        {
            xil_printf("recv_callback(): PROT_CTL_RESIDENT_ON\n");            

            m_resident = true;

            if (!axi_dma_queued_rx())
            {
                // start receiving TLPs
                dma_rx_queue();
            }

            // don't send any reply
            ignore = true;  
            break;
        }

    case PROT_CTL_RESIDENT_OFF:
        {
            xil_printf("recv_callback(): PROT_CTL_RESIDENT_OFF\n");

            m_resident = false;

            // don't send any reply
            ignore = true;  
            break;
        }

    case PROT_CTL_ROM_WRITE:
        {
            PROT_CTL_ROM *rom_write = (PROT_CTL_ROM *)&request->data;

            request->size -= sizeof(PROT_CTL_ROM);

            xil_printf(
                "recv_callback(): PROT_CTL_ROM_WRITE: offset = 0x%x, size = 0x%x\n",
                rom_write->offset, request->size
            );

            if (rom_write->offset + request->size < OPTION_ROM_MAX_SIZE)
            {
                // write option ROM contents into the flash
                if (flash_write(
                    OPTION_ROM_FLASH_ADDR + rom_write->offset, rom_write->data, request->size) == XST_SUCCESS)
                {
                    reply->code = PROT_CTL_SUCCESS;
                }                
            }
            else
            {
                xil_printf("recv_callback() ERROR: Bad option ROM offset\n");       
            }

            break;
        }

    case PROT_CTL_ROM_ERASE:
        {
            xil_printf("recv_callback(): PROT_CTL_ROM_ERASE\n");

            // erase option ROM contents
            if (option_rom_erase() == XST_SUCCESS)
            {
                reply->code = PROT_CTL_SUCCESS;
            }
            
            break;
        }

    case PROT_CTL_ROM_LOG_ON:
        {
            xil_printf("recv_callback(): PROT_CTL_ROM_LOG_ON\n");

            // enable option ROM access logging
            m_verbose = true;
            
            // don't send any reply
            ignore = true;  
            break;
        }

    case PROT_CTL_ROM_LOG_OFF:
        {
            xil_printf("recv_callback(): PROT_CTL_ROM_LOG_OFF\n");

            // enable option ROM access logging
            m_verbose = false;
            
            // don't send any reply
            ignore = true;  
            break;
        }

#endif // USE_OPTION_ROM

    default:
        {
            xil_printf("recv_callback() ERROR: Unknown code\n");
            break;
        }
    }

    if (!ignore)
    {
        err_t err;

        // send reply to the client
        if ((err = tcp_write(tpcb, (void *)buffer, sizeof(PROT_CTL) + reply->size, TCP_WRITE_FLAG_COPY)) != ERR_OK)
        {
            xil_printf("tcp_write() ERROR %d\n", err);   
        }
    }

    // receive next request
    m_bytes_have = 0;
    m_bytes_need = sizeof(PROT_CTL);
}

err_t recv_callback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
    u32 copied = 0;

    // do not read the packet if we are not in ESTABLISHED state
    if (!p) 
    {
        tcp_close(tpcb);
        tcp_recv(tpcb, NULL);

        return ERR_OK;
    }

    // indicate that the packet has been received
    tcp_recved(tpcb, p->len);

#ifdef VERBOSE

    xil_printf("recv_callback(): len = %d (max = %d)\n", p->len, tcp_sndbuf(tpcb));

#endif

    // handle received data
    while (copied < p->len)
    {
        PROT_CTL *request = (PROT_CTL *)&m_buffer_recv;
        u32 len = MIN(m_bytes_need - m_bytes_have, p->len - copied);

        // copy available amount of bytes to the request buffer
        memcpy(m_buffer_recv + m_bytes_have, p->payload + copied, len);
        m_bytes_have += len;

        if (m_bytes_have == sizeof(PROT_CTL))
        {
            // determine full request length
            m_bytes_need += request->size;
        }

        if (m_bytes_need == m_bytes_have)
        {
            // handle received request
            handle_request(tpcb, request);
        }
        
        copied += len;
    }

    // free the received pbuf
    pbuf_free(p);

    return ERR_OK;
}

void error_callback(void *arg, err_t err)
{
    xil_printf("error_callback(): Connection closed\n");  
    
    m_connection -= 1;
    m_tpcb = NULL; 
}

err_t accept_callback(void *arg, struct tcp_pcb *newpcb, err_t err)
{
    xil_printf("accept_callback(): Connection accepted\n");

    // set the receive callback for this connection
    tcp_recv(newpcb, recv_callback);

    // set the error callback for this connection
    tcp_err(newpcb, error_callback);

    // just use an integer number indicating the connection id as the callback argument
    tcp_arg(newpcb, (void *)m_connection);

    // increment for subsequent accepted connections
    m_connection += 1;
    m_tpcb = newpcb;

    return ERR_OK;
}

int start_application(u16 port)
{
    err_t err;
    struct tcp_pcb *pcb = NULL;    

    m_tpcb = NULL; 
    m_connection = 1;    
    m_verbose = false;

#ifdef USE_OPTION_ROM

    m_resident = true;

    xil_printf("Option ROM support is present\n");

#else

    m_resident = false;

#endif

    tlp_handle_init();

    // initialize DMA engine
    if (axi_dma_initialize() != XST_SUCCESS)
    {
        return -1;
    }

    // create new TCP PCB structure
    if (!(pcb = tcp_new())) 
    {
        xil_printf("tcp_new() ERROR\n");
        return -1;
    }

    // bind to specified port
    if ((err = tcp_bind(pcb, IP_ADDR_ANY, port)) != ERR_OK) 
    {
        xil_printf("tcp_bind() ERROR %d\n", err);
        return -1;
    }

    // we do not need any arguments to callback functions
    tcp_arg(pcb, NULL);

#if defined(RECV_AUTO) || defined(USE_OPTION_ROM)

    // start receiving TLPs
    dma_rx_queue();    

#endif

    // listen for connections
    if (!(pcb = tcp_listen(pcb))) 
    {
        xil_printf("tcp_listen() ERROR\n");
        return -1;
    }

    // specify callback to use for incoming connections
    tcp_accept(pcb, accept_callback);

    xil_printf("start_application(): TCP server is started at port %d\n", port);

    return ERR_OK;
}
