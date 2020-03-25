#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <xil_types.h>
#include <xparameters.h>

#include "config.h"
#include "pcie_tlp.h"
#include "pcie_to_tcp.h"
#include "axi_dma.h"
#include "axis_pcie.h"

#define RING_BUFF_SIZE 64
#define RING_BUFF_INC(_val_) (((_val_) + 1) % RING_BUFF_SIZE)

typedef struct _BUFF
{
    u32 busy;
    u8 data[PROT_MAX_PACKET_SIZE];

} BUFF;

// defined in pcie_to_tcp.c
extern u8 *m_option_rom;
extern bool m_verbose;

BUFF *m_buff_list = (BUFF *)BASE_ADDR_TX_BUFF;

BUFF *m_ring_buff[RING_BUFF_SIZE];

int m_ring_buff_len = 0;
int m_ring_buff_rd = 0, m_ring_buff_wr = 0;

void ring_buff_init(void)
{
    m_ring_buff_len = 0;
    m_ring_buff_rd = m_ring_buff_wr = 0;
}

bool ring_buff_push(BUFF *val)
{
    if (m_ring_buff_len == RING_BUFF_SIZE)
    {
        // ring buffer is full
        return false;
    }

    m_ring_buff[m_ring_buff_wr] = val;

    m_ring_buff_wr = RING_BUFF_INC(m_ring_buff_wr);
    m_ring_buff_len += 1;

    return true;
}

bool ring_buff_pop(BUFF **val)
{
    if (m_ring_buff_len == 0)
    {
        // ring buffer is empty
        return false;
    }

    if (val)
    {
        *val = m_ring_buff[m_ring_buff_rd];
    }

    m_ring_buff_rd = RING_BUFF_INC(m_ring_buff_rd);
    m_ring_buff_len -= 1;

    return true;
}

BUFF *buff_get(void)
{
    int i = 0;
    BUFF *buff = m_buff_list;

    for (i = 0; i < RING_BUFF_SIZE; i += 1)
    {
        // check for the free buffer
        if (!buff->busy)
        {
            // mark buffer as busy
            buff->busy = true;

            return buff;
        }

        buff += 1;
    }

    return NULL;
}

void buff_free(BUFF *buff)
{
    // mark buffer as free
    buff->busy = false;
}

void buff_init(void)
{
    int i = 0;
    BUFF *buff = m_buff_list;

    for (i = 0; i < RING_BUFF_SIZE; i += 1)
    {
        buff_free(buff);
        buff += 1;
    }
}

int tlp_size(u8 *data)
{
    u32 header = 0, type = 0, size = 0;

    memcpy(&header, data, sizeof(u32));

    type = (header >> 29) & 0x3;
    size = (header >> 0) & 0x3ff;

    switch (type)
    {
    case TLP_3_NO_DATA:

        return sizeof(u32) * 3;

    case TLP_4_NO_DATA:
 
        return sizeof(u32) * 4;

    case TLP_3_DATA:

        return sizeof(u32) * (3 + size);

    case TLP_4_DATA:

        return sizeof(u32) * (4 + size);
    }

    return 0;
}

void tlp_handle_init(void)
{
    buff_init();

    ring_buff_init();
}

int tlp_handle(u8 *data, u32 size)
{
    u32 *header_rx = (u32 *)data; 
    u32 type = (header_rx[0] >> 24) & 0xff;    

    // check for the memory read TLP
    if (type == TLP_TYPE_MRd32 || type == TLP_TYPE_MRd64)
    {        
        u32 addr_hi = 0, addr_lo = 0;
        u32 addr_rom = 0, completer = 0;

        // check if device is initialized
        if ((completer = axis_pcie_status_bus_id()) == 0)
        {            
            return 0;
        }

        // check for available option ROM
        if ((addr_rom = aixs_pcie_rom_address()) == 0)
        {
            return 0;
        }
            
        if (type == TLP_TYPE_MRd64)
        {
            // 64-bit request
            addr_hi = header_rx[2];
            addr_lo = header_rx[3] & 0xfffffffc;
        }
        else if (type == TLP_TYPE_MRd32)
        {
            // 32-bit request
            addr_lo = header_rx[2] & 0xfffffffc;
        }

        // check for option ROM read request
        if (addr_hi == 0 &&
            addr_lo >= addr_rom && addr_lo < addr_rom + OPTION_ROM_MAX_SIZE)
        {            
            u32 offset = addr_lo - addr_rom, i = 0;

            // data length
            u32 length = (header_rx[0] >> 0) & 0x3ff;

            // transaction ID
            u8 tag = (header_rx[1] >> 8) & 0xff;
            u32 requester = (header_rx[1] >> 16) & 0xffff;

            // byte enable flags
            u8 last_dw_be = (header_rx[1] >> 4) & 0xf;
            u8 first_dw_be = (header_rx[1] >> 0) & 0xf;            

            u32 byte_count = length * 4;
            u32 lower_address = addr_lo & 0x7c;            

            BUFF *buff = NULL;
            u32 size_tx = (3 + length) * sizeof(u32);

            if (size_tx > PROT_MAX_PACKET_SIZE)
            {
                xil_printf("tlp_handle() ERROR: Request is too large\n");
                return 0;
            }   

            // calculate completion lower address
            if ((first_dw_be & 0x3) == 0x2) lower_address |= 0x1;
            if ((first_dw_be & 0x7) == 0x4) lower_address |= 0x2;
            if ((first_dw_be & 0xf) == 0x8) lower_address |= 0x3;

            // calculate completion byte count
            if ((first_dw_be & (1 << 0)) == 0) byte_count -= 1;
            if ((first_dw_be & (1 << 1)) == 0) byte_count -= 1;
            if ((first_dw_be & (1 << 2)) == 0) byte_count -= 1;
            if ((first_dw_be & (1 << 3)) == 0) byte_count -= 1;                   

            if (length > 1)
            {
                if ((last_dw_be & (1 << 0)) == 0) byte_count -= 1;
                if ((last_dw_be & (1 << 1)) == 0) byte_count -= 1;
                if ((last_dw_be & (1 << 2)) == 0) byte_count -= 1;
                if ((last_dw_be & (1 << 3)) == 0) byte_count -= 1;
            }

            if (m_verbose)
            {
                u32 real_offset = (offset & 0xffffff80) + lower_address;

                xil_printf("ROM read: size = %d, offset = 0x%x\n", byte_count, real_offset);
            }

            // allocate memory for CplD TLP
            if ((buff = buff_get()) != NULL)
            {
                u32 *header_tx = (u32 *)&buff->data;

                // construct reply header
                header_tx[0] = (TLP_TYPE_CplD << 24) | length;
                header_tx[1] = (completer << 16) | byte_count;
                header_tx[2] = (requester << 16) | (tag << 8) | lower_address;

                for (i = 0; i < length; i += 1)
                {
                    u8 *val = m_option_rom + offset + (i * sizeof(u32));

                    // construct reply data
                    header_tx[3 + i] = (u32)((val[3] << 0) | (val[2] << 8) | (val[1] << 16) | (val[0] << 24));
                }

                // send reply
                if (!ring_buff_push(buff))
                {
                    buff_free(buff);
                }
            }
            else
            {
                xil_printf("tlp_handle() ERROR: Out of memory\n");
            }            

            return TLP_IGNORE;
        }
    }

    return 0;
}

void tlp_handle_pending(void)
{
    BUFF *buff = NULL;

    // check if we have any data to send
    if (!axi_dma_queued_tx() && ring_buff_pop(&buff))
    {
        u32 size = tlp_size(buff->data);

#ifdef VERBOSE

        xil_printf("tlp_handle_pending(): Sending %d bytes\n", size);
#endif
        // send TLP to the PCI-E device
        dma_tx_queue(buff->data, size);

        buff_free(buff);
    }
}
