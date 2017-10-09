#include <stdio.h>
#include <string.h>

#include "fsl.h"
#include "lwip/err.h"
#include "lwip/tcp.h"

#include "config.h"
#include "pcie_to_tcp.h"
#include "axis_pcie.h"

#ifdef __arm__
extern volatile int TcpFastTimer;
extern volatile int TcpSlowTimer;
#endif

#define SEND_BUFF_SIZE 0x2000

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

    return 0;
}

err_t recv_callback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
    int i;
    PCIE_CTL *ctl_rx, ctl_tx;
    u8 *send_buff;

    // do not read the packet if we are not in ESTABLISHED state
    if (!p) 
    {
        tcp_close(tpcb);
        tcp_recv(tpcb, NULL);

        return ERR_OK;
    }

    // indicate that the packet has been received
    tcp_recved(tpcb, p->len);

    if ((send_buff = (u8 *)malloc(SEND_BUFF_SIZE)) == NULL)
    {
        xil_printf("malloc() ERROR\r\n");
        goto _end;
    }

#ifdef VERBOSE

    xil_printf("recv_callback(): len = %d (max = %d)\r\n", p->len, tcp_sndbuf(tpcb));

#endif

    if (p->len % sizeof(PCIE_CTL) != 0)
    {
        xil_printf("recv_callback() ERROR: bad length\r\n");

        ctl_tx.flags = PCIE_F_ERROR;

#ifdef VERBOSE

        xil_printf("TX: flags = 0x%.2x, data = 0x%.8x\r\n", ctl_tx.flags, ctl_tx.data);
#endif
        if ((err = tcp_write(tpcb, (void *)&ctl_tx, sizeof(ctl_tx), 1)) != ERR_OK)
        {
            xil_printf("tcp_write() ERROR %d\r\n", err);   
        }

        goto _end;
    }    

    ctl_rx = (PCIE_CTL *)p->payload;

    for (i = 0; i < p->len / sizeof(PCIE_CTL); i++, ctl_rx++)    
    {

#ifdef VERBOSE

        xil_printf("RX: flags = 0x%.2x, data = 0x%.8x\r\n", ctl_rx->flags, ctl_rx->data);
#endif   
        if (ctl_rx->flags & PCIE_F_STATUS)
        {
            ctl_tx.flags = PCIE_F_HAS_DATA | PCIE_F_STATUS;            
            ctl_tx.data = axis_pcie_status();

            if ((err = tcp_write(tpcb, (void *)&ctl_tx, sizeof(ctl_tx), 1)) != ERR_OK)
            {
                xil_printf("tcp_write() ERROR %d\r\n", err);   
            }
        }     

        if (ctl_rx->flags & PCIE_F_HAS_DATA)
        {
            axis_pcie_write(ctl_rx->data, (ctl_rx->flags & PCIE_F_TLAST) ? AXIS_PCIE_F_TLAST : 0);
        }        
        
        if (ctl_rx->flags & PCIE_F_RECV_REPLY)
        {
            PCIE_CTL *send_data = (PCIE_CTL *)send_buff;
            u16 send_len = 0;
            u32 flags = (ctl_rx->flags & PCIE_F_TIMEOUT) ? AXIS_PCIE_F_WITH_TIMEOUT : 0;

            while (1)
            {
                u32 read_data = 0, read_flags = flags;

                if (axis_pcie_read(&read_data, &read_flags) == -1)
                {
                    ctl_tx.flags = PCIE_F_TIMEOUT | PCIE_F_ERROR;
                    ctl_tx.data = 0;
#ifdef VERBOSE
                    xil_printf("TX: flags = 0x%.2x, data = 0x%.8x\r\n", ctl_tx.flags, ctl_tx.data);
#endif
                    if ((err = tcp_write(tpcb, (void *)&ctl_tx, sizeof(ctl_tx), 1)) != ERR_OK)
                    {
                        xil_printf("tcp_write() ERROR %d\r\n", err);   
                    }

                    goto _end;
                }

                flags &= ~AXIS_PCIE_F_WITH_TIMEOUT;
                
                send_data->flags = PCIE_F_HAS_DATA;
                send_data->flags |= (read_flags & AXIS_PCIE_F_TLAST) ? PCIE_F_TLAST : 0;
                send_data->data = read_data;
#ifdef VERBOSE
                xil_printf("TX: flags = 0x%.2x, data = 0x%.8x\r\n", send_data->flags, read_data);
#endif
                send_len += sizeof(ctl_tx);
                send_data++;

                if (send_len + sizeof(ctl_tx) >= SEND_BUFF_SIZE)
                {
                    if ((err = tcp_write(tpcb, (void *)send_buff, send_len, 1)) != ERR_OK)
                    {
                        xil_printf("tcp_write() ERROR %d\r\n", err);   
                    }

                    send_data = (PCIE_CTL *)send_buff;
                    send_len = 0;
                }                

                if (read_flags & AXIS_PCIE_F_TLAST)
                {
                    break;
                }
            }

            if (send_len > 0)
            {
                if ((err = tcp_write(tpcb, (void *)send_buff, send_len, 1)) != ERR_OK)
                {
                    xil_printf("tcp_write() ERROR %d\r\n", err);   
                }
            }
        }
    }

_end:

    if (send_buff)
    {
        free(send_buff);
    }

    // free the received pbuf
    pbuf_free(p);

    return ERR_OK;
}

err_t accept_callback(void *arg, struct tcp_pcb *newpcb, err_t err)
{
    static int connection = 1;

    xil_printf("accept_callback(): Connection %d accepted\r\n", connection);

    // set the receive callback for this connection
    tcp_recv(newpcb, recv_callback);

    // just use an integer number indicating the connection id as the callback argument
    tcp_arg(newpcb, (void *)connection);

    // increment for subsequent accepted connections
    connection++;

    return ERR_OK;
}

int start_application(u16 port)
{
    struct tcp_pcb *pcb;
    err_t err;

    // create new TCP PCB structure
    if (!(pcb = tcp_new())) 
    {
        xil_printf("tcp_new() ERROR\r\n");
        return -1;
    }

    // bind to specified port
    if ((err = tcp_bind(pcb, IP_ADDR_ANY, port)) != ERR_OK) 
    {
        xil_printf("tcp_bind() ERROR %d\r\n", err);
        return -2;
    }

    // we do not need any arguments to callback functions
    tcp_arg(pcb, NULL);

    // listen for connections
    if (!(pcb = tcp_listen(pcb))) 
    {
        xil_printf("tcp_listen() ERROR\r\n");
        return -3;
    }

    // specify callback to use for incoming connections
    tcp_accept(pcb, accept_callback);

    xil_printf("start_application(): TCP server is started at port %d\r\n", port);

    return 0;
}
