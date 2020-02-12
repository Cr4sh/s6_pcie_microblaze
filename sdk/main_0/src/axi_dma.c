#include <stdio.h>
#include <stdbool.h>

#include <xaxidma.h>
#include <xintc.h>
#include <xil_exception.h>
#include <xil_cache.h>

#include "config.h"
#include "platform.h"
#include "axi_dma.h"

// DMA engine instance
XAxiDma m_AxiDma;

// receive data handler
AXI_DMA_HANDLER m_handler_rx = NULL;

// send complete handler
AXI_DMA_HANDLER m_handler_tx = NULL;

void axi_dma_isr_tx(void *Param)
{
    XAxiDma *AxiDma = (XAxiDma *)Param;

    // read pending interrupts
    u32 IrqStatus = XAxiDma_IntrGetIrq(AxiDma, XAXIDMA_DMA_TO_DEVICE);

    // acknowledge pending interrupts
    XAxiDma_IntrAckIrq(AxiDma, IrqStatus, XAXIDMA_DMA_TO_DEVICE);

    if (!(IrqStatus & XAXIDMA_IRQ_ALL_MASK))
    {
        // no interrupt is asserted
        return;
    }

    // check for the error
    if (IrqStatus & XAXIDMA_IRQ_ERROR_MASK)
    {
        xil_printf("isr_dma_tx(): IRQ error\n");

        // reset transmit channel
        XAxiDma_Reset(&m_AxiDma);

        while (XAxiDma_ResetIsDone(&m_AxiDma) == 0)
        {
            // wait for reset
        }

        m_handler_tx = NULL;
        return;
    }

    // transmit completed
    if (IrqStatus & (XAXIDMA_IRQ_DELAY_MASK | XAXIDMA_IRQ_IOC_MASK))
    {

#ifdef VERBOSE

        xil_printf("isr_dma_tx(): transmit completed\n");
#endif
        if (m_handler_tx)
        {
            AXI_DMA_HANDLER handler = m_handler_tx;

            m_handler_tx = NULL;
            
            handler();            
        }
    }
}

void axi_dma_isr_rx(void *Param)
{
    XAxiDma *AxiDma = (XAxiDma *)Param;

    // read pending interrupts
    u32 IrqStatus = XAxiDma_IntrGetIrq(AxiDma, XAXIDMA_DEVICE_TO_DMA);

    // acknowledge pending interrupts
    XAxiDma_IntrAckIrq(AxiDma, IrqStatus, XAXIDMA_DEVICE_TO_DMA);

    if (!(IrqStatus & XAXIDMA_IRQ_ALL_MASK))
    {
        // no interrupt is asserted
        return;
    }

    // check for the error
    if (IrqStatus & XAXIDMA_IRQ_ERROR_MASK)
    {
        xil_printf("isr_dma_rx(): IRQ error\n");

        // reset transmit channel
        XAxiDma_Reset(&m_AxiDma);

        while (XAxiDma_ResetIsDone(&m_AxiDma) == 0)
        {
            // wait for reset
        }

        m_handler_rx = NULL;
        return;
    }

    if (IrqStatus & (XAXIDMA_IRQ_DELAY_MASK | XAXIDMA_IRQ_IOC_MASK))
    {

#ifdef VERBOSE

        xil_printf("isr_dma_rx(): receive completed\n");
#endif
        if (m_handler_rx)
        {
            AXI_DMA_HANDLER handler = m_handler_rx;

            m_handler_rx = NULL;
            
            handler();            
        }
    }
}

int axi_dma_initialize_interrupts(u32 BaseAddr, XAxiDma *AxiDma)
{   
    // register transmit ISR
    XIntc_RegisterHandler(
        BaseAddr,
        XPAR_INTC_0_AXIDMA_0_MM2S_INTROUT_VEC_ID, axi_dma_isr_tx, AxiDma
    );

    // register receive ISR
    XIntc_RegisterHandler(
        BaseAddr,
        XPAR_INTC_0_AXIDMA_0_S2MM_INTROUT_VEC_ID, axi_dma_isr_rx, AxiDma
    );

    u32 Mask = XIntc_In32(BaseAddr + XIN_IER_OFFSET);

    Mask |= (1 << XPAR_INTC_0_AXIDMA_0_MM2S_INTROUT_VEC_ID);
    Mask |= (1 << XPAR_INTC_0_AXIDMA_0_S2MM_INTROUT_VEC_ID);

    // enable interrupts
    XIntc_EnableIntr(BaseAddr, Mask);

    return XST_SUCCESS;
}

int axi_dma_initialize(void)
{
    m_handler_rx = m_handler_tx = NULL;

    xil_printf("Initializing DMA...\n");

    XAxiDma_Config *Config = XAxiDma_LookupConfig(XPAR_AXI_DMA_0_DEVICE_ID);
    if (Config == NULL)
    {
        xil_printf("ERROR: XAxiDma_LookupConfig() fails\n");
        return XST_FAILURE;
    }

    XAxiDma_CfgInitialize(&m_AxiDma, Config);

    // check for Scatter-Gather mode
    if (XAxiDma_HasSg(&m_AxiDma))
    {
        xil_printf("ERROR: Scatter-Gather DMA is configured\n");
        return XST_FAILURE;
    }

    xil_printf("Initializing interrupts...\n");

    // set up interrupt controller
    if (axi_dma_initialize_interrupts(XPAR_MICROBLAZE_0_INTC_BASEADDR, &m_AxiDma) != XST_SUCCESS)
    {
        return XST_FAILURE;
    }

    // disable all interrupts
    XAxiDma_IntrDisable(&m_AxiDma, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DMA_TO_DEVICE);
    XAxiDma_IntrDisable(&m_AxiDma, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DEVICE_TO_DMA);

    // enable all interrupts
    XAxiDma_IntrEnable(&m_AxiDma, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DMA_TO_DEVICE);
    XAxiDma_IntrEnable(&m_AxiDma, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DEVICE_TO_DMA);

    return XST_SUCCESS;
}

int axi_dma_queue_tx(void *buff, u32 size, AXI_DMA_HANDLER handler)
{
    if (m_handler_tx != NULL)
    {
        xil_printf("axi_dma_queue_tx() ERROR: Busy\n");
        return XST_FAILURE;
    }

    m_handler_tx = handler;

    // start transmit transfer
    if (XAxiDma_SimpleTransfer(&m_AxiDma, (u32)buff, size, XAXIDMA_DMA_TO_DEVICE) != XST_SUCCESS)
    {
        xil_printf("ERROR: XAxiDma_SimpleTransfer() fails\n");
        return XST_FAILURE;
    }

    return XST_SUCCESS;
}

int axi_dma_queue_rx(void *buff, u32 size, AXI_DMA_HANDLER handler)
{
    if (m_handler_rx != NULL)
    {
        xil_printf("axi_dma_queue_rx() ERROR: Busy\n");
        return XST_FAILURE;
    }

    m_handler_rx = handler;

    // start receive transfer
    if (XAxiDma_SimpleTransfer(&m_AxiDma, (u32)buff, size, XAXIDMA_DEVICE_TO_DMA) != XST_SUCCESS)
    {
        xil_printf("ERROR: XAxiDma_SimpleTransfer() fails\n");
        return XST_FAILURE;
    }

    return XST_SUCCESS;
}

bool axi_dma_queued_tx(void)
{
    return m_handler_tx != NULL;
}

bool axi_dma_queued_rx(void)
{
    return m_handler_rx != NULL;
}

void axi_dma_wait_tx(void)
{
    while (m_handler_tx != NULL) {}
}

void axi_dma_wait_rx(void)
{
    while (m_handler_rx != NULL) {}
}
