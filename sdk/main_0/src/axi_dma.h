#ifndef __AXI_DMA_H_
#define __AXI_DMA_H_

typedef void (* AXI_DMA_HANDLER)(void);

int axi_dma_initialize(void);

int axi_dma_queue_tx(void *buff, u32 size, AXI_DMA_HANDLER handler);
int axi_dma_queue_rx(void *buff, u32 size, AXI_DMA_HANDLER handler);

bool axi_dma_queued_tx(void);
bool axi_dma_queued_rx(void);

void axi_dma_wait_tx(void);
void axi_dma_wait_rx(void);

#endif
