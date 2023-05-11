/*
` * main implementation: use this 'C' sample to create your own application
 *
 */


//#include "derivative.h" /* include peripheral declarations */
#include "TFC.h"
#include  "..\Project_Headers\app.h"    		// private library - APP layer
#define MUDULO_REGISTER  0x2EE0

enum FSMstate state;
enum SYSmode lpm_mode;

char *storge = 0x20000000;

int count_down = 0; // use for count_on_screen, RGB_blink
unsigned int x_count = 0;   // use for count_on_screen


char command_is_finish = 0;
unsigned int delay = 50;
char OPC;
unsigned int argumentA;
unsigned int argumentB;
char color = Red_light; 
char ACK_file_already_send = 0;


char clock_side = wise;
char step = Full;
char T_priod = 0;		//use to now what Phise(of MOTOR) to turn on
float counter_step = 0;  // use for motor step counter
float Full_cycle_stpes = 515; 
float target_steps_to_degree = 0;

uint16_t ADC_simple[ADC_number_of_simple*2];
uint16_t ADC_axis[2];

Array_files files_in_memory;
Array massageRX;
Array massageTX;

int k = 0;
char SW_is_pressed = 0;

int main(void){
	sysConfig();
	//state = state2;
	//UART0_C2 &= ~UART_C2_TIE_MASK;                      // Disable Transmit interrupt
	//UART0_C2 &= ~UART_C2_RIE_MASK;                      // Disable Transmit interrupt

	while(1){
		switch(state){
		  case state0:
			 enterLPM(wait_mode);
			break;
			 
		  case state1:
			  enterLPM(wait_mode);
			break;
			 
		  case state2:
			  break;
			
		  case state3:
			  break;
			  
		  case state4:
			  if( (massageTX.array[0] == 0x2) && (massageTX.array[1] == ACK) && (massageTX.used >= 2) && (ACK_file_already_send == 1) ){
				freeArray(&massageTX);
				Run_specific_file_from_RAM(files_in_memory.how_many_files - 1);
				if(OPC = '8'){
					enterLPM(wait_mode);
				}
			  }
			  if(massageRX.used >= 3){
				  switch(massageRX.array[2]){
						case '1':
							 Run_specific_file_from_RAM(0);
							 if(OPC = '8'){
								 freeArray(&massageRX);
								 enterLPM(wait_mode);
							}
							break;
						case '2':
							Run_specific_file_from_RAM(1);
							if(OPC = '8'){
								freeArray(&massageRX);
								enterLPM(wait_mode);
							}
							break;
						case '3':
							Run_specific_file_from_RAM(2);
							if(OPC = '8'){
								freeArray(&massageRX);
								enterLPM(wait_mode);
							}
							break;
						freeArray(&massageRX);
				  }
			  }
			  break;
		   case state5:
			   return 0;
			   break;   
		}
	 }
}








