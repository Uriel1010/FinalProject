#include <stdint.h>


#ifndef TFC_BOARDSUPPORT_H_
#define TFC_BOARDSUPPORT_H_


#define TFC_HBRIDGE_EN_LOC			(uint32_t)(1<<21)
#define TFC_HBRIDGE_FAULT_LOC     	(uint32_t)(1<<20)

#define TFC_HBRIDGE_ENABLE			GPIOE_PSOR = TFC_HBRIDGE_EN_LOC	
#define TFC_HBRIDGE_DISABLE			GPIOE_PCOR = TFC_HBRIDGE_EN_LOC	

#define TFC_DIP_SWITCH0_LOC			((uint32_t)(1<<2))
#define TFC_DIP_SWITCH1_LOC			((uint32_t)(1<<3))
#define TFC_DIP_SWITCH2_LOC			((uint32_t)(1<<4))
#define TFC_DIP_SWITCH3_LOC			((uint32_t)(1<<5))

#define TFC_PUSH_BUTT0N0_LOC		((uint32_t)(1<<13))
#define TFC_PUSH_BUTT0N1_LOC		((uint32_t)(1<<17))	

#define TFC_BAT_LED0_LOC			((uint32_t)(1<<8))
#define TFC_BAT_LED1_LOC			((uint32_t)(1<<9))
#define TFC_BAT_LED2_LOC			((uint32_t)(1<<10))
#define TFC_BAT_LED3_LOC			((uint32_t)(1<<11))
//-------------  RGB LEDs ---------------------------------------------
#define RED_LED_LOC	         		((uint32_t)(1<<18))
#define GREEN_LED_LOC	         	((uint32_t)(1<<19))
#define BLUE_LED_LOC	         	((uint32_t)(1<<1))
#define PORT_LOC(x)        	        ((uint32_t)(1<<x))

#define RED_LED_OFF          		GPIOB_PSOR |= RED_LED_LOC
#define GREEN_LED_OFF	         	GPIOB_PSOR |= GREEN_LED_LOC
#define BLUE_LED_OFF	            GPIOD_PSOR |= BLUE_LED_LOC
#define RGB_LED_OFF	                RED_LED_OFF,GREEN_LED_OFF,BLUE_LED_OFF

#define RED_LED_TOGGLE          	GPIOB_PTOR |= RED_LED_LOC
#define GREEN_LED_TOGGLE	        GPIOB_PTOR |= GREEN_LED_LOC
#define BLUE_LED_TOGGLE	            GPIOD_PTOR |= BLUE_LED_LOC
#define RGB_LED_TOGGLE	            RED_LED_TOGGLE,GREEN_LED_TOGGLE,BLUE_LED_TOGGLE

#define RED_LED_ON          		GPIOB_PCOR |= RED_LED_LOC
#define GREEN_LED_ON	         	GPIOB_PCOR |= GREEN_LED_LOC
#define BLUE_LED_ON	                GPIOD_PCOR |= BLUE_LED_LOC
#define RGB_LED_ON	                RED_LED_ON,GREEN_LED_ON,BLUE_LED_ON

#define SW_POS 0x80  //PTD7
//---------------------------------------------------------------------
#define TFC_BAT_LED0_ON				GPIOB_PSOR = TFC_BAT_LED0_LOC
#define TFC_BAT_LED1_ON				GPIOB_PSOR = TFC_BAT_LED1_LOC
#define TFC_BAT_LED2_ON				GPIOB_PSOR = TFC_BAT_LED2_LOC
#define TFC_BAT_LED3_ON				GPIOB_PSOR = TFC_BAT_LED3_LOC

#define TFC_BAT_LED0_OFF			GPIOB_PCOR = TFC_BAT_LED0_LOC
#define TFC_BAT_LED1_OFF			GPIOB_PCOR = TFC_BAT_LED1_LOC
#define TFC_BAT_LED2_OFF			GPIOB_PCOR = TFC_BAT_LED2_LOC
#define TFC_BAT_LED3_OFF			GPIOB_PCOR = TFC_BAT_LED3_LOC

#define TFC_BAT_LED0_TOGGLE			GPIOB_PTOR = TFC_BAT_LED0_LOC
#define TFC_BAT_LED1_TOGGLE			GPIOB_PTOR = TFC_BAT_LED1_LOC
#define TFC_BAT_LED2_TOGGLE			GPIOB_PTOR = TFC_BAT_LED2_LOC
#define TFC_BAT_LED3_TOGGLE			GPIOB_PTOR = TFC_BAT_LED3_LOC

#define TFC_PUSH_BUTTON_0_PRESSED	((GPIOC_PDIR&TFC_PUSH_BUTT0N0_LOC)>0)
#define TFC_PUSH_BUTTON_1_PRESSED	((GPIOC_PDIR&TFC_PUSH_BUTT0N1_LOC)>0)


//------------------------------------------------------------------------
//					 PushButtons abstraction
//-------------------------------------------------------------------------
#define PB0_LOC		PORT_LOC(0)
#define PB1_LOC		PORT_LOC(1)	
#define PB2_LOC		PORT_LOC(2)
#define PB3_LOC		PORT_LOC(3)
#define PBsArr_LOC  PB0_LOC|PB1_LOC|PB2_LOC|PB3_LOC


#define PBsArrPort	       		GPIOD_PDOR 
#define PBsArrIntPend	    	PORTD_ISFR   
#define PBsArrIntPendClear(x)	PORTD_ISFR |= x  // using clear with PBi_LOC 
// #define PBsArrIntEn	       		
#define PBsArrIntEdgeSel(x) 	PORT_PCR_IRQC(x)
#define PULL_UP            		0x0a
#define PULL_DOWN           	0x09
#define PBsArrPortSel      		PORT_PCR_MUX(1)| PORT_PCR_PFE_MASK 
#define PBsArrPortDir      		GPIOD_PDDR 

extern enum FSMstate state; 
extern enum SYSmode lpm_mode; 
#define debounceVal      	  250
#define stop_mode			  0x00
#define wait_mode			  0x01
#define interupt_flag   	  0x80
#define off_set_zero     	  0x00
#define disable_pit_module 	  PIT_TCTRL1 &= ~0x00000003
#define CMOD01_DMA            0x108 // TOF(flag to clear)=0, TOIE(inttrupt enable)=0, CMOD=01, PS=0, DMA=1
#define TOF_TOIE_CMOD01       0xC8 // TOF(flag to clear)=1, TOIE(inttrupt enable)=1, CMOD=01, PS=0
#define TOF_TOIE_CMOD01_PS4   0xCA // TOF(flag to clear)=1, TOIE(inttrupt enable)=1, CMOD=01, PS=4
#define Cflag_PWM_cflag       0xA9 // CHF(flag for clear)=1, CHIE(inturrpt enable)=0, MSA=1, DMA=1, EPWM=1,     
#define Cflag_IE_outputcomper 0xD4 // CHF(flag for clear)=1, CHIE(inturrpt enable)=1, MSA=1      0xD4
#define Cflag_IE_inputcupture 0xC4 // CHF(flag for clear)=1, CHIE(inturrpt enable)=1, ELSA=1 
#define Leg_4				  0x04
#define keyboard_out_off      GPIOB_PDOR = 0x00 
#define keyboard_out_(x)   	  GPIOB_PDOR = x
#define row1				  0x07
#define row2				  0x0B
#define row3				  0x0D
#define row4				  0x0E
#define keyboard_out_check_column_(x)         GPIOB_PDIR & x
#define column1				  0x800
#define column2				  0x400
#define column3				  0x200
#define column4				  0x100
#define clear_key_board_flag  PORTA_ISFR = 0x10000
#define clear_sw_iturrpt_flag PORTA_ISFR = 1<<5
#define clear_pit_flag        PIT_TFLG1 = 0x1
#define clear_DMA_BCR0_flag   DMA_DSR_BCR0 = 0x1000000
#define clear_DMA_BCR1_flag   DMA_DSR_BCR1 = 0x1000000
#define add_2_for_DMA_BCR0_transfer   DMA_DSR_BCR0 = 0x2
#define add_2_for_DMA_BCR1_transfer   DMA_DSR_BCR1 = 0x2

extern void  InitGPIO();
extern void InitTPM(char x);
uint8_t TFC_GetDIP_Switch();
extern void ClockSetupTPM();
extern void InitPIT();


#endif /* TFC_BOARDSUPPORT_H_ */
