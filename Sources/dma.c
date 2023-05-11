
#include "TFC.h"

void dma_init(char x){
	switch(x){
		case 0:
			// Enable clocks
			SIM_SCGC6 |= SIM_SCGC6_DMAMUX_MASK;
			SIM_SCGC7 |= SIM_SCGC7_DMA_MASK;
		
			// Disable DMA Mux channel first
			DMAMUX0_CHCFG0 = 0x00;
			
			// Configure DMA
			DMA_SAR0 = (uint32_t)&UART0_D;
			DMA_DAR0 = (uint32_t)storge;//0x20001000;  // set destintion to middle of RAM.
			// number of bytes yet to be transferred for a given block - 1 bytes(16 bits)
			DMA_DSR_BCR0 = DMA_DSR_BCR_BCR(1); 
			
			DMA_DCR0 |= (DMA_DCR_EINT_MASK|		// Enable interrupt
						 DMA_DCR_ERQ_MASK |		// Enable peripheral request
						 DMA_DCR_CS_MASK  |		// set DMA to cycle stiling
						 DMA_DCR_SSIZE(1) |		// Set source size to 8 bits
						 //DMA_DCR_SINC_MASK |		// Set increments to source address
						 DMA_DCR_DMOD(10)  |     // Destination address modulo of 8 kilo Bytes ( Destination circular buffer 8KB)
						 DMA_DCR_DINC_MASK |     // set increments to Destination address
						 DMA_DCR_EADREQ_MASK |
						 //DMA_DCR_D_REQ_MASK |
						 DMA_DCR_DSIZE(1));		// Set destination size of 8 bits 
						 
			
			//Config DMA Mux for ADC0 operation, Enable DMA channel and source
			//DMAMUX0_CHCFG0 |= DMAMUX_CHCFG_ENBL_MASK | DMAMUX_CHCFG_SOURCE(2); // Enable DMA channel and set UART0 resive as source
			
			// Enable interrupt
			set_irq_priority(INT_DMA0-16,0);
			enable_irq(INT_DMA0 - 16);
			break;
		case 1:
			// Enable clocks
			SIM_SCGC6 |= SIM_SCGC6_DMAMUX_MASK;
			SIM_SCGC7 |= SIM_SCGC7_DMA_MASK;
		
			// Disable DMA Mux channel first
			DMAMUX0_CHCFG1 = 0x00;
			
			// Configure DMA
			DMA_SAR1 = (uint32_t)&ADC0_RA;
			DMA_DAR1 = (uint32_t)ADC_simple;//0x20001000;  // set destintion to middle of RAM.
			// number of bytes yet to be transferred for a given block - 1 bytes(16 bits)
			DMA_DSR_BCR1 = DMA_DSR_BCR_BCR(ADC_number_of_simple*2); 
			
			DMA_DCR1 |= (DMA_DCR_EINT_MASK|		// Enable interrupt
						 DMA_DCR_ERQ_MASK |		// Enable peripheral request
						 DMA_DCR_CS_MASK  |		// set DMA to cycle stiling
						 DMA_DCR_SSIZE(2) |		// Set source size to 16 bits
						 //DMA_DCR_SINC_MASK |		// Set increments to source address
						 //DMA_DCR_SMOD(10)  |     // Destination address modulo of 8 kilo Bytes ( Destination circular buffer 8KB)
						 DMA_DCR_DINC_MASK |     // set increments to Destination address
						 DMA_DCR_EADREQ_MASK |
						 //DMA_DCR_D_REQ_MASK |
						 DMA_DCR_DSIZE(2));		// Set destination size of 16 bits 
						 
			
			//Config DMA Mux for ADC0 operation, Enable DMA channel and source
			//DMAMUX0_CHCFG1 |= DMAMUX_CHCFG_ENBL_MASK | DMAMUX_CHCFG_SOURCE(40); // Enable DMA channel and set ADC0 resive as source
			
			// Enable interrupt
			set_irq_priority(INT_DMA1-16,2);
			enable_irq(INT_DMA1 - 16);
			break;
		case 2:
			// Enable clocks
						SIM_SCGC6 |= SIM_SCGC6_DMAMUX_MASK;
						SIM_SCGC7 |= SIM_SCGC7_DMA_MASK;
					
						// Disable DMA Mux channel first
						DMAMUX0_CHCFG2 = 0x00;
						
						// Configure DMA
						//DMA_SAR2 = (uint32_t)ADC_pin_changer;
						DMA_DAR2 = (uint32_t)&ADC0_SC1A;//0x20001000;  // set destintion to middle of RAM.
						// number of bytes yet to be transferred for a given block - 1 bytes(16 bits)
						DMA_DSR_BCR2 = DMA_DSR_BCR_BCR(1); 
						
						DMA_DCR2 |= (DMA_DCR_EINT_MASK|		// Enable interrupt
									 DMA_DCR_ERQ_MASK |		// Enable peripheral request
									 DMA_DCR_CS_MASK  |		// set DMA to cycle stiling
									 DMA_DCR_SSIZE(1) |		// Set source size to 8 bits
									 DMA_DCR_SINC_MASK |		// Set increments to source address
									 DMA_DCR_SMOD(10)  |     // Destination address modulo of 8 kilo Bytes ( Destination circular buffer 8KB)
									 //DMA_DCR_DINC_MASK |     // set increments to Destination address
									 DMA_DCR_EADREQ_MASK |
									 //DMA_DCR_D_REQ_MASK |
									 DMA_DCR_DSIZE(1));		// Set destination size of 8 bits 
									 
						
						//Config DMA Mux for ADC0 operation, Enable DMA channel and source
						//DMAMUX0_CHCFG0 |= DMAMUX_CHCFG_ENBL_MASK | DMAMUX_CHCFG_SOURCE(2); // Enable DMA channel and set UART0 resive as source
						
						// Enable interrupt
						set_irq_priority(INT_DMA2-16,0);
						enable_irq(INT_DMA2 - 16);
		
	}
}


