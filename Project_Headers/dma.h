
#ifndef DMA_H_
#define DMA_H_

#include "TFC.h"


#define  ADC_READS 8
extern uint16_t  value[ADC_READS];

void dma_init(char x);
void DMA0_IRQHandler(void);

#endif /* DMA_H_ */
