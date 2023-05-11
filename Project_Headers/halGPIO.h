#ifndef _halGPIO_H_
#define _halGPIO_H_

#include "TFC.h"   		// private library - APP layer


#define test_HZ 6500
#define ADC_number_of_simple  512
#define number_of_shifts      9


#define how_many_chars_to_convert   2

#define ACK		0x06
#define wise     0
#define unwise   1
#define Full     1
#define Half     0

#define Motor_reg	GPIOC_PDOR
#define phise_A  	1<<10//0x100000
#define phise_B 	1<<6//0x200000
#define phise_C		1<<5//0x400000
#define phise_D 	1<<3//0x800000
#define off 		0x00

#define RGB_reg		GPIOC_PDOR
#define Red			1<<2
#define Blue		1<<0
#define Green		1<<1
#define	RGB_off		GPIOC_PDOR = 0x00
#define Red_light			0
#define Green_light			1
#define Blue_light			2

#define Ten_ms		0x3A980
#define new_line    0xA   // ==> /n
#define A_ascii     0x41  
#define zero_ascii  0x30
#define one_ascii   0x31
#define SOH_ascii	0x1
#define STX_ascii	0x2
#define ETX_ascii	0x3
#define EOT_ascii 	0x4

#define Start_uart_send 	UART0_C2 |= UART_C2_TIE_MASK
#define Stop_uart_send 		UART0_C2 &= ~UART_C2_TIE_MASK
#define Start_uart_receive	UART0_C2 |= UART_C2_RIE_MASK
#define Stop_uart_receive	UART0_C2 &= ~UART_C2_RIE_MASK

#define reset_leg_ADC		ADC0_SC1A &= ~0x1F;
#define reset_DMA1_Dest		DMA_DAR1 = (uint32_t)ADC_simple;

#define down		0
#define up			1


typedef struct {
  char *array;
  size_t used;
  size_t size;
} Array;

typedef struct {
  Array name;
  int size;
  uint32_t *address;
//  int LRU;
} file_type;

typedef struct {
  file_type *array;
  int how_many_files;
  int free_space;
} Array_files;


extern enum FSMstate state;   // global variable
extern enum SYSmode lpm_mode; // global variable

extern char *storge;

extern int count_down;
extern unsigned int x_count;

extern char command_is_finish;
extern unsigned int delay;
extern char OPC;
extern unsigned int argumentA;
extern unsigned int argumentB;
extern char color;
extern char ACK_file_already_send;

extern char clock_side;
extern char step;
extern char T_priod;

extern float counter_step;
extern float Full_cycle_stpes;
extern float target_steps_to_degree;

extern uint16_t ADC_simple[ADC_number_of_simple*2];
extern uint16_t ADC_axis[2];
extern Array_files files_in_memory;
extern Array massageRX;
extern Array massageTX;
extern int k;
extern char SW_is_pressed;


extern void print_file_inline(file_type *a);
extern void initArray(Array *a, size_t initialSize);
extern void insertArray(Array *a, char element);
extern void freeArray(Array *a);
extern void init_filetype(file_type *a, Array name, int size, uint32_t adress);
extern void initArray_files(Array_files *a);
extern void insertArray_files(Array_files *a, file_type element);





extern void sysConfig(void);
extern void enterLPM(unsigned char LPM_level);

extern void enterLPM(unsigned char LPM_level);
extern void setPIT1_timer(unsigned int a);
extern void setTPM2_C0V(int a);
extern void delay_function(unsigned int t);
extern void disable_Key_Board_Intrapt();
extern void enable_Key_Board_Intrapt();
extern void disable_push_buttom_Intrapt();
extern void enable_push_buttom_Intrapt();

extern int keyboard();


extern void PORTD_IRQHandler(void);
extern void PORTA_IRQHandler(void);
extern void PIT_IRQHandler();
extern void FTM0_IRQHandler();
extern void FTM2_IRQHandler();
extern void ADC0_IRQHandler();
extern void DMA0_IRQHandler();

extern void RGB_blink(unsigned int amount_of_blink);
extern void OPC_LCD(unsigned int x);
extern void move_motor_to_degree(float p);

#endif







