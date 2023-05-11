#include "TFC.h"


void print_file_inline(file_type *a);
void initArray(Array *a, size_t initialSize);
void insertArray(Array *a, char element);
void freeArray(Array *a);
void init_filetype(file_type *a, Array name, int size, uint32_t adress);
void initArray_files(Array_files *a);
void insertArray_files(Array_files *a, file_type element);

void sysConfig(void);
void enterLPM(unsigned char LPM_level);

void enterLPM(unsigned char LPM_level);
void setPIT1_timer(unsigned int a);
void setTPM2_C0V(int a);
void delay_function(unsigned int t);
void disable_Key_Board_Intrapt();
void enable_Key_Board_Intrapt();
int keyboard();


void PORTD_IRQHandler(void);
void PORTA_IRQHandler(void);
void PIT_IRQHandler();
void FTM0_IRQHandler();
void FTM2_IRQHandler();
void ADC0_IRQHandler();
void UART0_IRQHandler();
void DMA0_IRQHandler();



void initArray_files(Array_files *a) {
  a->array = malloc(1 * sizeof(file_type));
  a->how_many_files = 0;
  a->free_space = 8192;
}

void insertArray_files(Array_files *a, file_type element) {
  // a->used is the number of used entries, because a->array[a->used++] updates a->used only *after* the array has been accessed.
  // Therefore a->used can go up to a->size 
  a->how_many_files += 1;
  a->array = realloc(a->array, a->how_many_files * sizeof(file_type));
  a->array[a->how_many_files - 1] = element;
  a->free_space -= element.size; 
}

void init_filetype(file_type *a, Array name, int size, uint32_t adress) {
  initArray(&a->name, 1);
  int i;
  for(i=0; i<name.used; i++){
	  insertArray(&a->name, name.array[i]);
  }
  //a->name = malloc(1 * sizeof(name));
  a->size = size;
  a->address = adress;
  //a->LRU = 0;
}

void Array_files_delete_first_element(Array_files *a) {
  // a->used is the number of used entries, because a->array[a->used++] updates a->used only *after* the array has been accessed.
  // Therefore a->used can go up to a->size 
	int i;
	freeArray_for_file_type(&a->array[0].name);
	a->how_many_files -= 1;
	a->free_space += a->array[0].size;
	for(i = 0; i < a->how_many_files; i++){
		a->array[i] = a->array[i + 1];
	}
	a->array = realloc(a->array, a->how_many_files * sizeof(file_type));
	//a->array = realloc(a->array, a->how_many_files * sizeof(file_type));
}

void initArray(Array *a, size_t initialSize) {
  a->array = malloc(initialSize * sizeof(char));
  a->used = 0;
  a->size = initialSize;
}

void insertArray(Array *a, char element) {
  // a->used is the number of used entries, because a->array[a->used++] updates a->used only *after* the array has been accessed.
  // Therefore a->used can go up to a->size 
  if (a->used == a->size) {
    a->size += 2;   //<<<----------------------------------------------------------------- was here a->size *= 2
    a->array = realloc(a->array, a->size * sizeof(char));
  }
  a->array[a->used++] = element;
}

void freeArray(Array *a) {
  free(a->array);
  a->array = NULL;
  a->used = a->size = 0;
  initArray(a, 1);
}

void freeArray_for_file_type(Array *a) {
  free(a->array);
  a->array = NULL;
  a->used = a->size = 0;
}

void print_file_inline(file_type *a){
	int i = 0;
	char buffer[10];
	for(i = 0; i < a->name.used; i++){
		lcd_data(a->name.array[i]);
	}

	sprintf(buffer, " %dB", a->size);
	lcd_puts(buffer);
}



int hex2int(char *hex) {
    int val = 0;
    int counter_chars_convert = 0;
    
    while (*hex) {
        // get current character then increment
        char byte = *hex++; 
        if (how_many_chars_to_convert == counter_chars_convert){
            break;
        }
        counter_chars_convert++;
        // transform hex character to the 4bit equivalent number, using the ascii table indexes
        if (byte >= '0' && byte <= '9') byte = byte - '0';
        else if (byte >= 'a' && byte <='f') byte = byte - 'a' + 10;
        else if (byte >= 'A' && byte <='F') byte = byte - 'A' + 10;
        // shift 4 to make space for new digit, and add the 4 bits of the new digit 
        val = (val << 4) | (byte & 0xF);
    }
    return val;
}



//--------------------------------------------------------------------
//             System Configuration  
//--------------------------------------------------------------------
void sysConfig(void){ 
		InitGPIO();
		InitADCs();
		lcd_init();
		//InitDAC();
		InitPIT();
		ClockSetupTPM();
		//InitTPM(2);
		dma_init(0);
		dma_init(1);
		//dma_init(2);
		InitUARTs();
		GIE();
		disable_Key_SW_Intrapt();
		initArray(&massageRX, 1);
		initArray(&massageTX, 1);
		initArray_files(&files_in_memory);
		state = state0;
}


void enterLPM(unsigned char LPM_level){
	if (LPM_level == stop_mode) 
		stop();    /* Enter Low Power Mode 0 */
    else if(LPM_level == wait_mode) 
    	wait();      /* Enter Low Power Mode 1 */
}

void enable_Key_SW_Intrapt(){
	clear_sw_iturrpt_flag;
	enable_irq(INT_PORTA-16);           // Enable PORTA Interrupts 
}

void disable_Key_SW_Intrapt(){
	disable_irq(INT_PORTA-16);           // Enable PORTA Interrupts 
}

void enable_push_buttom_Intrapt(){
	PBsArrIntPendClear(PB0_LOC);
	PBsArrIntPendClear(PB1_LOC);
	PBsArrIntPendClear(PB2_LOC);
	PBsArrIntPendClear(PB3_LOC);
	enable_irq(INT_PORTD-16);           // Enable PORTA Interrupts 
}

void disable_push_buttom_Intrapt(){
	disable_irq(INT_PORTD-16);           // Enable PORTA Interrupts 
}

void setPIT1_timer(unsigned int a){
	disable_pit(); //disable PIT0 and its interrupt
	PIT_LDVAL1 = a; // setup timer a for a counting period
	PIT_TCTRL1 = PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK; //enable PIT0 and its interrupt
	
}

void setTPM2_C0V(int a){
	TPM2_C0V  = a; // setup timer a for a counting period
}

void delay_function(unsigned int t){  // t[msec]
	volatile unsigned int i;
	
	for(i=t; i>0; i--);
}

void TPM0_off(){
    TPM0_C3SC = off_set_zero;
    TPM0_SC = off_set_zero;
}

void TPM2_off(){
    TPM2_C0SC = off_set_zero;
    TPM2_SC = off_set_zero;
}

void disable_pit(){
	disable_pit_module; // disable PIT0 and its interrupt
}

void stop_motor(){
	disable_pit();
	Motor_reg = off;
}

void start_TPM0(){
	  TPM0_SC |= TOF_TOIE_CMOD01_PS4;  //Start the TPM0 counter
	  TPM0_C3SC = Cflag_IE_inputcupture; //Start channel 3 as inputcupture interrupt enable
}

void start_TPM2(){
    TPM2_SC = CMOD01_DMA; 
    TPM2_C0SC = Cflag_PWM_cflag;
}

unsigned int hertz_to_timer(int HZ){
	unsigned int i;
	i = 240000000/HZ;
	return i;
}


void insert_to_array_from_buffer(Array *a, char buffer[]){
	int i = 0;
	while(buffer[i] != '\n'){
		insertArray(a, buffer[i]);
		i++;
	}
	insertArray(a, buffer[i]);
}

void Turn_on_DMA1_for_simple_ADC(){
	DMA_DSR_BCR1 = DMA_DSR_BCR_BCR(ADC_number_of_simple*2);
	DMA_DCR1 |= 0xC0000000;
	DMAMUX0_CHCFG1 |= DMAMUX_CHCFG_ENBL_MASK | DMAMUX_CHCFG_SOURCE(40); // Enable DMA channel and set ADC0 resive as source
}

void Phise_conter_swich_FULL_clock_UNwise(){
	switch(T_priod){
		case 0:
			Motor_reg = phise_A;
			T_priod++;
			break;
		case 1:
			Motor_reg = phise_D;
			T_priod++;
			break;
		case 2:
			Motor_reg = phise_C;
			T_priod++;
			break;
		case 3:
			Motor_reg = phise_B;
			T_priod++;
			if((state != state3) && (state != state0)){
				T_priod = 0;
			}
			counter_step--;
			break;
		case 4:
			if(((state == state3) && (massageRX.array[1] == '3')) || (state == state0)){
				stop_motor();
			}
			T_priod = 0;
			break;
	}
}

void Phise_conter_swich_FULL_clock_wise(){
	switch(T_priod){
		case 0:
			Motor_reg = phise_D;
			T_priod++;
			break;
		case 1:
			Motor_reg = phise_A;
			T_priod++;
			break;
		case 2:
			Motor_reg = phise_B;
			T_priod++;
			break;
		case 3:
			Motor_reg = phise_C;
			T_priod++;
			if((state != state3) && (state != state0)){
				T_priod = 0;
			}
			counter_step++;
			break;
		case 4:
			if((state == state3) && (massageRX.array[1] == '2')){
				stop_motor();
			}
			else if(state == state0){
				stop_motor();
				clock_side = unwise;
				step = Full;
				setPIT1_timer(hertz_to_timer(test_HZ));
			}
			T_priod = 0;
			break;
	}
}

void Phise_conter_swich_Half_clock_UNwise(){
	switch(T_priod){
		case 0:
			Motor_reg = phise_D;
			T_priod++;
			break;
		case 1:
			Motor_reg = phise_D | phise_C;
			T_priod++;
			break;
		case 2:
			Motor_reg = phise_C;
			T_priod++;
			break;
		case 3:
			Motor_reg = phise_C | phise_B;
			T_priod++;
			counter_step -= 0.5;
			break;
		case 4:
			Motor_reg = phise_B;
			T_priod++;
			break;
		case 5:
			Motor_reg = phise_B | phise_A;
			T_priod++;
			break;
		case 6:
			Motor_reg = phise_A;
			T_priod++;
			break;
		case 7:
			Motor_reg = phise_A | phise_D;
			T_priod = 0;
			counter_step -= 0.5;
			break;
	}
}

void Phise_conter_swich_Half_clock_wise(){
	switch(T_priod){
		case 0:
			Motor_reg = phise_A;
			T_priod++;
			break;
		case 1:
			Motor_reg = phise_A | phise_B;
			T_priod++;
			break;
		case 2:
			Motor_reg = phise_B;
			T_priod++;
			break;
		case 3:
			Motor_reg = phise_B | phise_C;
			T_priod++;
			counter_step += 0.5;
			break;
		case 4:
			Motor_reg = phise_C;
			T_priod++;
			break;
		case 5:
			Motor_reg = phise_C | phise_D;
			T_priod++;
			break;
		case 6:
			Motor_reg = phise_D;
			T_priod++;
			break;
		case 7:
			Motor_reg = phise_D | phise_A;
			T_priod = 0;
			counter_step += 0.5;
			break;
	}
}

void RGB_blink(unsigned int amount_of_blink){   // OPC = 1
	count_down = amount_of_blink;
	setPIT1_timer(calculate_delay_for_pit());
}

void OPC_LCD(unsigned int x){	// OPC = 2,3
	x_count = x;
	if(OPC == '2'){
		count_down = 0;
	}
	else if(OPC == '3'){
		count_down = 10;
	}
	setPIT1_timer(calculate_delay_for_pit());
}

void set_delay(unsigned int d){	// OPC = 4
	delay = d;
}

void set_Full_cycle_stpes(float steps){
	Full_cycle_stpes = steps;
}

void clear_all(){		//OPC = 5
	LCD_clear_move_to_first_line();
	RGB_off;
}

void move_motor_to_degree(float p){		//OPC = 6    
	while(p >= 360.0){
		p -= 360.0;
	}
	while(p < 0.0){
		p += 360.0;
	}
	float number_of_half_stpes_for_degree;
	float my_degree;
	int diff;
	my_degree = counter_step/(Full_cycle_stpes/360);
	diff = p - my_degree;
	if((diff + 360) % 360 < 180){
		clock_side = wise;
	}
	else{
		clock_side = unwise;
	}
	number_of_half_stpes_for_degree = ((Full_cycle_stpes/360)*2)*p;
	target_steps_to_degree = (round(number_of_half_stpes_for_degree)/2);
	step = Half;
	setPIT1_timer(hertz_to_timer(test_HZ));

}

void motor_move(){
	switch(clock_side){
		case wise:
			if((counter_step >= Full_cycle_stpes) && (state != state3)){
				counter_step = 0 + (counter_step - Full_cycle_stpes);
			}
			switch(step){
				case Full:
					Phise_conter_swich_FULL_clock_wise();
					break;
				case Half:
					Phise_conter_swich_Half_clock_wise();
					break;
			}
			break;
		case unwise:
			if((counter_step <= 0.0) && (state != state3)){
				counter_step = Full_cycle_stpes + counter_step;
			}
			switch(step){
				case Full:
					Phise_conter_swich_FULL_clock_UNwise();
					break;
				case Half:
					Phise_conter_swich_Half_clock_UNwise();
					break;
			}
			break;
	}
}

int calculate_delay_for_pit(){
	return delay * Ten_ms;
}

void Blincker(){
	switch(color){
		case Red_light:
			color = Green_light;
			RGB_reg = Red;
			break;
		case Green_light:
			color = Blue_light;
			RGB_reg = Green;
			break;
		case Blue_light:
			color = Red_light;
			RGB_reg = Blue;
			count_down--;
			if(count_down == 0){
				disable_pit();
				command_is_finish = 1;
			}
			break;
	}
}

void count_on_screen(char up_down){
	char i[2];
	sprintf(i, "%d", count_down);
	if(up_down  == down){
		LCD_clear_move_to_first_line();
		lcd_puts(i);
		count_down--;
		if(count_down == -1){
			count_down = 10;
			x_count--;
			if(x_count == 0){
				disable_pit();
				command_is_finish = 1;
			}
		}
	}
	else if(up_down  == up){
		LCD_clear_move_to_first_line();
		lcd_puts(i);
		count_down++;
		if(count_down == 11){
			count_down = 0;
			x_count--;
			if(x_count == 0){
				disable_pit();
				command_is_finish = 1;
			}
		}
	}
}


void change_state(){
	Stop_uart_send;                      // Disable Transmit interrupt
	freeArray(&massageTX);
	switch(massageRX.array[1]){
		case '0':
			DMAMUX0_CHCFG1 = off;
			disable_pit();
			state = state0;
			disable_Key_SW_Intrapt();
		case '1':
			DMAMUX0_CHCFG1 = off;
			disable_pit();
			reset_DMA1_Dest;
			reset_leg_ADC;
			state = state1;
			disable_Key_SW_Intrapt();
			Start_uart_receive;                      // Enable recive interrupt
			Turn_on_DMA1_for_simple_ADC();
			break;
		case '2':
			DMAMUX0_CHCFG1 = off;
			disable_pit();
			reset_DMA1_Dest;
			reset_leg_ADC;
			state = state2;
			enable_Key_SW_Intrapt();
			Start_uart_receive;                      // Enable recive interrupt
			Turn_on_DMA1_for_simple_ADC();
			break;
		case '3':
			DMAMUX0_CHCFG1 = off;
			disable_pit();
			state = state3;
			disable_Key_SW_Intrapt();
			break;
		case '4':
			DMAMUX0_CHCFG1 = off;
			disable_pit();
			state = state4;
			disable_Key_SW_Intrapt();
			break;
		case '5':
			DMAMUX0_CHCFG1 = off;
			disable_pit();
			state = state5;
			disable_Key_SW_Intrapt();
			break;
	}
}

void clabrition_move_motor_step1(){
	if(massageRX.array[1] == '0'){
		clock_side = wise;
		step = Full;
		setPIT1_timer(hertz_to_timer(test_HZ));
	}
	else if(massageRX.array[1] == '1'){
		stop_motor();
	}
	else if(massageRX.array[1] == '2'){    /// One step clock wise
		clock_side = wise;
		step = Full;
		setPIT1_timer(hertz_to_timer(test_HZ));
	}
	else if(massageRX.array[1] == '3'){    /// One step clock unwise
		clock_side = unwise;
		step = Full;
		setPIT1_timer(hertz_to_timer(test_HZ));
	}
	else if(massageRX.array[1] == '4'){   
		counter_step = 0;
	}
	else if(massageRX.array[1] == '5'){    
		set_Full_cycle_stpes(counter_step);
	}
}

void file_recive_from_PC(){
	Array filename_temp;
	unsigned int i = 2;
	switch(massageRX.array[1]){
		case '1':
			initArray(&filename_temp ,1);
			while(massageRX.array[i] != 2){
				insertArray(&filename_temp,massageRX.array[i]);
				i++;
			}			
			i++;
			unsigned int j = 0;
			unsigned int file_size = 0;
			while(massageRX.array[i] != '\n'){
				file_size += ((massageRX.array[i] - 48) * (pow(10,j)));
				i++;
				j++;
			}
			///<<<-----------------------------------------------------------------------------------------------
			///                       place for memory manegment --> FIFO
			while((files_in_memory.free_space < file_size) || (files_in_memory.how_many_files >= 3)){
				Array_files_delete_first_element(&files_in_memory);
			}
			
			///<<<------------------------------------------------------------------------------------------------
			file_type file;
			init_filetype(&file ,filename_temp, file_size, DMA_DAR0);
			insertArray_files(&files_in_memory, file);
			freeArray(&massageRX);
			freeArray(&filename_temp);
			DMA_DSR_BCR0 = DMA_DSR_BCR_BCR(file_size);
			insertArray(&massageTX, SOH_ascii);
			insertArray(&massageTX, ACK);
			Start_uart_send;                      // Enable Transmit interrupt
			break;
			
		case '2':
  		  	freeArray(&massageRX);
			Stop_uart_send;                      // Disable Transmit interrupt
			Stop_uart_receive;                      // Disable Recive interrupt
			DMAMUX0_CHCFG0 |= DMAMUX_CHCFG_ENBL_MASK | DMAMUX_CHCFG_SOURCE(2); // Enable DMA to transfer automaticly from UART0 to RAM
			//enterLPM(wait_mode);
			break;	
		case '3':
			freeArray(&massageRX);
			freeArray(&massageTX);
			i = 0;
			j = 0;
			insertArray(&massageTX,EOT_ascii);
			for(i = 0; i < files_in_memory.how_many_files; i++){
				for(j = 0; j < files_in_memory.array[i].name.used; j++){
					insertArray(&massageTX, files_in_memory.array[i].name.array[j]);
				}
				insertArray(&massageTX,STX_ascii);
			}
			if(massageTX.used == 1){
				freeArray(&massageTX);
				insertArray(&massageTX,ETX_ascii);
				insertArray(&massageTX, new_line);
			}
			else{
				insertArray(&massageTX, new_line);
			}
			Start_uart_send;                      // Enable Transmit interrupt
			break;
		 case '4':
			break;
		 }
}


void Run_specific_file_from_RAM(char x){
	char *real_address;
	real_address = files_in_memory.array[x].address;
	while(1){
		OPC = *(real_address + 1);
		switch(OPC){
			case '1':				
				argumentA = hex2int(real_address + 2);
				RGB_blink(argumentA);
				break;
			case '2':
				argumentA = hex2int(real_address + 2);
				OPC_LCD(argumentA);
				break;
			case '3':
				argumentA = hex2int(real_address + 2);
				OPC_LCD(argumentA);
				break;
			case '4':
				argumentA = hex2int(real_address + 2);
				set_delay(argumentA);
				command_is_finish = 1;
				break;
			case '5':
				clear_all();
				command_is_finish = 1;
				break;
			case '6':
				argumentA = hex2int(real_address + 2);
				move_motor_to_degree(argumentA);
				break;
			case '7':
				argumentA = hex2int(real_address + 2);
				argumentB = hex2int(real_address + 4);
				move_motor_to_degree(argumentA);
				break;
			case '8':
				command_is_finish = 1;
				break;
			
			
		}
		while(command_is_finish == 0){
			
		}
		command_is_finish = 0;
		while( (*real_address != '\n') && (*real_address != 0x03)){
			real_address++;
		}
		if(*real_address == 0x03){
			int i;
			for(i = 0; i < files_in_memory.array[x].name.used; i++){
				insertArray(&massageTX, files_in_memory.array[x].name.array[i]);
			}
			char buffer[13] = {": completed\n"};
			insert_to_array_from_buffer(&massageTX, buffer);
			Start_uart_send;                       // Enable Transmit interrupt
			return;
		}
		real_address++;
	}
}


//-----------------------------------------------------------------
// PORTD = Interrupt Service Routine
//-----------------------------------------------------------------
void PORTD_IRQHandler(void){
   
	delay_function(debounceVal);
	if(PBsArrIntPend & PB0_LOC){
		 PBsArrIntPendClear(PB0_LOC);
		 //LCD_clear_move_to_first_line();
		 //disable_Key_Board_Intrapt();		 
		 //Start_uart_send;                        // Enable Transmit interrupt
	}
    else if (PBsArrIntPend & PB1_LOC){
		  PBsArrIntPendClear(PB1_LOC);
    }
    else if (PBsArrIntPend & PB2_LOC){
		 state = state3;
		 PBsArrIntPendClear(PB2_LOC);
    }
    else if (PBsArrIntPend & PB3_LOC){	 
    		state = state4;
    		PBsArrIntPendClear(PB3_LOC);
    }
}

//-----------------------------------------------------------------
//  PORTA - ISR
//-----------------------------------------------------------------
void PORTA_IRQHandler(void){
	delay_function(debounceVal);
	//char_on_screen_for_massge(bouttom_press);
	//setPIT1_timer(STX_asciiDC6C00); // set pit timer to inturrupt every 2000ms
	clear_sw_iturrpt_flag;
	SW_is_pressed = 1;
//	insertArray(&massageTX,0x53);
//	insertArray(&massageTX,new_line);
//	Start_uart_send;                       // Enable Transmit interrupt
}


//-----------------------------------------------------------------
//  PIT - ISR
//-----------------------------------------------------------------
void PIT_IRQHandler(void){
	if(state == state3){
		motor_move();
	}
	else if(state == state4){
		switch(OPC){
			case '1':
				Blincker();
				break;
			case '2':
				count_on_screen(up);
				break;
			case '3':
				count_on_screen(down);
				break;
			case '6':
				motor_move();
				if(counter_step == target_steps_to_degree){
					disable_pit();
					char buffer[22];
					sprintf(&buffer, "Now the angle is: %d\n", argumentA);
					insert_to_array_from_buffer(&massageTX, buffer);
					Start_uart_send;                       // Enable Transmit interrupt
				}
				break;
			case '7':
				motor_move();
				if(counter_step == target_steps_to_degree){
					disable_pit();
					char buffer[22];
					sprintf(&buffer, "Now the angle is: %d\n", argumentA);
					insert_to_array_from_buffer(&massageTX, buffer);
					Start_uart_send;                       // Enable Transmit interrupt
				}
				break;
		}
	}
	else if(state == state1){
		motor_move();
		if(counter_step == target_steps_to_degree){
			disable_pit();
		}
	}
	else if(state == state0){
		motor_move();
	}
	
	clear_pit_flag;
}


//-----------------------------------------------------------------
//  UART0 - ISR
//-----------------------------------------------------------------
void UART0_IRQHandler(){
	uint8_t Temp;
	char buffer[10];
	int Temp_counter_step;
	int i = 0;
	int j = 0;
	float angle = 0;
	if( UART0_S1 & UART_S1_RDRF_MASK ){ // RX buffer is full and ready for reading
		Temp = UART0_D;
		insertArray(&massageRX, Temp);
		 if (massageRX.array[massageRX.used - 1] == '\n'){
			switch(massageRX.array[0]){
				case '0':
					freeArray(&massageRX);
					state = state0;
					clock_side = wise;
					step = Full;
					setPIT1_timer(hertz_to_timer(test_HZ));
					break;
				case '1':
					clabrition_move_motor_step1();
					freeArray(&massageRX);
					if(massageRX.array[1] == '5'){
						Temp_counter_step = counter_step;
						sprintf(&buffer, "%d\n", Temp_counter_step);
						while(buffer[i] != '\n'){
							insertArray(&massageTX, buffer[i]);
							i++;
						}
						insertArray(&massageTX, buffer[i]);
						freeArray(&massageRX);
						Start_uart_send;                       // Enable Transmit interrupt
					}
					break;
				case '2':
					i = 1;
					while(massageRX.array[i] != '\n'){
						angle += ((massageRX.array[i] - 48) * (pow(10,j)));
						i++;
						j++;
					}
					freeArray(&massageRX);
					move_motor_to_degree(angle);
					break;
				case '3':
					break;
				case '4':
					change_state();
					freeArray(&massageRX);
					break;
				case '5':
					file_recive_from_PC();
					break;
				case '6':
					ACK_file_already_send = 1;
					freeArray(&massageRX);
					break;
			}
		 }
	 }
	 else if((UART0_S1 & UART_S1_TDRE_MASK) && (UART0_C2 & UART_C2_TIE_MASK)){ // TX buffer is empty and ready for sending  
		 Temp = massageTX.array[k];
		 UART0_D = Temp;
		 switch(state){
		 	 case state2:
		 		if (massageTX.array[k] == '\n'){
		 			Stop_uart_send;                      // Disable Transmit interrupt
					k = 0;
					freeArray(&massageTX);
					Turn_on_DMA1_for_simple_ADC();
					return;
		 		}
		 		 break;
			 case state3:
				if (massageTX.array[k] == '\n'){
					 Stop_uart_send;                      // Disable Transmit interrupt
					 freeArray(&massageTX);
					 k = 0;
					 Start_uart_receive;                      // Enable recive interrupt
					 return;
				}
				break;
			 case state1:
				if (massageTX.array[k] == '\n'){ 
					Stop_uart_send;                      // Disable Transmit interrupt
					k = 0;
					freeArray(&massageTX);
					Turn_on_DMA1_for_simple_ADC();
					return;
				}
				break;
			 case state4:
				if (massageTX.array[k] == ACK){
					Stop_uart_send;                      // Disable Transmit interrupt
					Start_uart_receive;                      // Enable recive interrupt
					k = 0;
					if(massageTX.array[0] == STX_ascii){
						//ACK_file_already_send = 1;
						//freeArray(&massageTX);
						//Run_specific_file_from_RAM(files_in_memory.how_many_files - 1);
						return;
					}
					freeArray(&massageTX);
					return;
				}
				else if(massageTX.array[k] == '\n'){
					Stop_uart_send;                      // Disable Transmit interrupt
					Start_uart_receive;                      // Enable recive interrupt
					k = 0;
					freeArray(&massageTX);
					if(OPC == '7'){
						if(argumentA != argumentB){
							argumentA = argumentB;
							move_motor_to_degree(argumentA);
						}
						else{
							command_is_finish = 1;
						}
					}
					else if(OPC == '6'){
						command_is_finish = 1;
					}
					else if(OPC == '8'){
						
					}
					return;
				}
				 break;
				 
		 }
		k++;
	}
}


//-----------------------------------------------------------------
//  DMA0 - ISR
//-----------------------------------------------------------------
void DMA0_IRQHandler(){
	DMAMUX0_CHCFG0 = off; // disble DMA to transfer from UART0
	clear_DMA_BCR0_flag;
	ACK_file_already_send = 0;
	insertArray(&massageTX, STX_ascii);
	insertArray(&massageTX, ACK);
	Start_uart_send;                      // Enable Transmit interrupt

	
	//UART0_D = 0x6;       // reload to buffer of UART ACK
	//DMA_DSR_BCR0 = SOH_ascii;
	//DMA_DCR0 |= DMA_DCR_START_MASK;
}

void DMA1_IRQHandler(){
	char buffer[12];
	DMAMUX0_CHCFG1 = off; // disble DMA to transfer from UART0
	DMA_DCR1 &= ~0xC0000000;
	clear_DMA_BCR1_flag;
	ADC0_SC1A ^= ETX_ascii;
	unsigned int i;
	unsigned int x = 0;
	unsigned int y = 0;
	if((ADC0_SC1A & SOH_ascii) == 0){
		DMA_DAR1 = (uint32_t)ADC_simple;
		for(i = 0; i < ADC_number_of_simple; i++){
			y += ADC_simple[i];
			x += ADC_simple[i + ADC_number_of_simple];
		}
		ADC_axis[0] = x>>number_of_shifts;
		ADC_axis[1] = y>>number_of_shifts;
		sprintf(&buffer, "%05dA%05d\n", ADC_axis[0], ADC_axis[1]);
		i = 0;
		while(buffer[i] != '\n'){
			insertArray(&massageTX, buffer[i]);
			i++;
		}
		if(state == state2){
			insertArray(&massageTX, A_ascii);  // insert A
			if(SW_is_pressed == 0){
				insertArray(&massageTX, zero_ascii); // insert 0
			}
			else{
				insertArray(&massageTX, one_ascii);  // insert 1
				SW_is_pressed = 0;
			}
		}
		insertArray(&massageTX, buffer[i]); // insert /n
		Start_uart_send;                       // Enable Transmit interrupt
		return;
	}
	Turn_on_DMA1_for_simple_ADC();
}

