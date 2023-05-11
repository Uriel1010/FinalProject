#include "TFC.h"
#include "mcg.h"



#define MUDULO_REGISTER  0x2EE0

// set I/O for switches and LEDs
void InitGPIO()
{
	//enable Clocks to all ports - page 206, enable clock to Ports
	SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK;

	//Setup Pins as Timer Output PWM
	//PORTD_PCR1 = PORT_PCR_MUX(4);  //PTD1 pin TPM0_CH1- ALT4, Blue = Edge Aligned PWM
	//PORTB_PCR18 = PORT_PCR_MUX(3);  //PTB18 pin TPM2_CH0- ALT3, Red = Edge Aligned PWM
	//PORTB_PCR19 = PORT_PCR_MUX(3);  //PTB19 pin TPM2_CH1- ALT3, Green = Edge Aligned PWM
	
	//--------------------------LCD-------------------------------------//
	//--------------------RS,RW,E-----------------------//
	PORTD_PCR4 = PORT_PCR_MUX(1); // assign PTD4 as GPIO
	PORTD_PCR5 = PORT_PCR_MUX(1); // assign PTD5 as GPIO
	PORTD_PCR6 = PORT_PCR_MUX(1); // assign PTD6 as GPIO
	PORTD_PCR7 = PORT_PCR_MUX(1); // assign PTD7 as GPIO
	GPIOD_PDDR = 0xF0;            // set port D4-D7 as OUTPUT
	GPIOD_PDOR = 0x00;
	
	//-------------------LCD DATA 1-4-------------------------//
	//-----------------------AND------------------------------//
	//---------------------JOYSTICK--------------------------//
	PORTE_PCR3 = PORT_PCR_MUX(1); // assign PTE3 as GPIO
	PORTE_PCR4 = PORT_PCR_MUX(1); // assign PTE4 as GPIO
	PORTE_PCR5 = PORT_PCR_MUX(1); // assign PTE5 as GPIO
	PORTE_PCR20 = PORT_PCR_MUX(0); // VRX -> ADC0_SE0/TSI0_CH14 
	PORTE_PCR22 = PORT_PCR_MUX(0); // VRY -> ADC0_SE3/TSI0_CH13
	GPIOE_PDDR = 0x000038;        // set ports E3,E4,E5 as OUTPUT
	GPIOE_PDOR = 0x000000;
	

	//---------------------JOYSTICK(SW)--------------------------//
	//PORTA_PCR1 = PORT_PCR_MUX(1); // BLUE
	//PORTA_PCR2 = PORT_PCR_MUX(1); // GREEN
	//PORTA_PCR4 = PORT_PCR_MUX(1); // RED
	PORTA_PCR5 = PBsArrPortSel+PBsArrIntEdgeSel(PULL_UP); //PORT_PCR_MUX(1); // SW
	GPIOA_PDDR = 0x000000;        // set all input
	GPIOA_PDOR = 0x000000;
	//enable_irq(INT_PORTA-16);           // Enable PORTA Interrupts 
	set_irq_priority (INT_PORTA-16,0);  // PORTA Interrupt priority = 0 = max
	
	//---------------------LED RGB--------------------------//
	//-----------------------AND------------------------------//
	//-------------------MOTOR STEPPER------------------------//
	PORTC_PCR0 = PORT_PCR_MUX(1); // BLUE
	PORTC_PCR1 = PORT_PCR_MUX(1); // GREEN
	PORTC_PCR2 = PORT_PCR_MUX(1); // RED
	PORTC_PCR10 = PORT_PCR_MUX(1);// assign PTC11 as GPIO IN1
	PORTC_PCR6 = PORT_PCR_MUX(1);// assign PTC10 as GPIO IN2
	PORTC_PCR5 = PORT_PCR_MUX(1);// assign PTC6 as GPIO IN3
	PORTC_PCR3 = PORT_PCR_MUX(1);// assign PTC5 as GPIO IN4
	GPIOC_PDDR = 0x00046F;        // set ports C0,C1,C2,C3,C5,C6,C10 as OUTPUT
	GPIOC_PDOR = 0x000000;
	
//	GPIOD_PDDR &= PORT_LOC(4);  // PTD4 is OUTPUT
//	GPIOD_PDDR &= PORT_LOC(5);  // PTD5 is OUTPUT
//	GPIOD_PDDR &= PORT_LOC(6);  // PTD6 is OUTPUT
//	GPIOD_PDDR &= PORT_LOC(7);  // PTD7 is OUTPUT
//	PORTD_PCR7 |= PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_PFE_MASK | PORT_PCR_IRQC(0x0a);

	
	// PushButtons Setup
		//PORTD_PCR0 = PBsArrPortSel+PBsArrIntEdgeSel(PULL_UP); 	// PTD.0=PB0 + Interrupt request enable
		//PORTD_PCR1 = PBsArrPortSel+PBsArrIntEdgeSel(PULL_UP); 	// PTD.1=PB1 + Interrupt request enable
		//PORTD_PCR2 = PBsArrPortSel+PBsArrIntEdgeSel(PULL_DOWN); // PTB.2=PB2 + Interrupt request enable
		//PORTD_PCR3 = PBsArrPortSel+PBsArrIntEdgeSel(PULL_DOWN); // set PTD3 as input cupture
		//PBsArrPortDir &= ~PBsArr_LOC; // port PTD 0-2 Input
		//PBsArrIntPendClear(PBsArr_LOC);
		
		//enable_irq(INT_PORTD-16);           // Enable PORTD Interrupts 
		//set_irq_priority (INT_PORTD-16,0);  // PORTD Interrupt priority = 0 = max
		
		
		
	// Key PAD
		//PORTA_PCR16 =  PBsArrPortSel+PBsArrIntEdgeSel(PULL_UP);	// keypad interrupt
		//GPIOA_PDDR = 0x00; // set all channel A to input
		//GPIOA_PDOR = 0x00;
		
//		enable_irq(INT_PORTA-16);           // Enable PORTA Interrupts 
//		set_irq_priority (INT_PORTA-16,1);  // PORTA Interrupt priority = 0 = max
		
		//PORTB_PCR0 = PORT_PCR_MUX(1); // keypad row (F)
		//PORTB_PCR1 = PORT_PCR_MUX(1); // keypad row (7)
		//PORTB_PCR2 = PORT_PCR_MUX(1); // keypad row (4)
		//PORTB_PCR3 = PORT_PCR_MUX(1); // keypad row (1)
		//PORTB_PCR8 = PORT_PCR_MUX(1); // keypad column (A)
		//PORTB_PCR9 = PORT_PCR_MUX(1); // keypad column (3)
//		PORTB_PCR10 = PORT_PCR_MUX(1); // keypad column (2)
//		PORTB_PCR11 = PORT_PCR_MUX(1); // keypad column (1)
//		GPIOB_PDDR = 0x00F;			  //  set port B3 as OUTPUT
//		GPIOB_PDOR = 0x00;
	
}



//-----------------------------------------------------------------
// DipSwitch data reading
//-----------------------------------------------------------------
uint8_t TFC_GetDIP_Switch()
{
	uint8_t DIP_Val=0;
	
	DIP_Val = (GPIOC_PDIR>>4) & 0xF;

	return DIP_Val;
}
//-----------------------------------------------------------------
// TPMx - Initialization
//-----------------------------------------------------------------
void InitTPM(char x){  // x={0,1,2}
	switch(x){
	case 0:
		TPM0_SC = 0; // to ensure that the counter is not running
		//TPM0_SC = 0x7;
		//TPM0_SC =  TPM_SC_PS(3)+TPM_SC_TOIE_MASK; //Prescaler =128, up-mode, counter-disable
		//TPM0_MOD = MUDULO_REGISTER; // PWM frequency of 250Hz = 24MHz/(8x12,000)
		TPM0_MOD = 0xFFFF; // time for interrupt
		TPM0_C0SC = 0xC4;//TPM_CnSC_ELSA_MASK;//0x00000004;
		//TPM0_C1V = 0xFFFF;
		TPM0_CONF = 0;//0xC0; 
		enable_irq(INT_TPM0-16); //  //Enable TMP0 IRQ on the NVIC
		set_irq_priority(INT_TPM0-16,1);  // Interrupt priority = 1
		
		break;
	case 1:
		
		break;
	case 2: 
		TPM2_SC = 0; // to ensure that the counter is not running
		//TPM2_SC |= TPM_SC_PS(3)+TPM_SC_TOIE_MASK; //Prescaler =128, up-mode, counter-disable
		TPM2_MOD = 0x5DBF; // PWM frequency of 250Hz = 24MHz/(8x12,000)
		TPM2_C0SC = 0xA9;  //TPM_CnSC_MSB_MASK + TPM_CnSC_ELSB_MASK + TPM_CnSC_CHIE_MASK;
		TPM2_C0V = 0xFFFF; 
		//TPM2_C1SC |= TPM_CnSC_MSB_MASK + TPM_CnSC_ELSB_MASK + TPM_CnSC_CHIE_MASK;
		//TPM2_C1V = 0xFFFF;
		TPM2_CONF = 0;
		//enable_irq(INT_TPM2-16); //  //Enable TMP0 IRQ on the NVIC
		//set_irq_priority(INT_TPM2-16,1);  // Interrupt priority = 1
		break;
	}
}
//-----------------------------------------------------------------
// TPMx - Clock Setup
//-----------------------------------------------------------------
void ClockSetupTPM(){
	    
	    pll_init(8000000, LOW_POWER, CRYSTAL,4,24,MCGOUT); //Core Clock is now at 48MHz using the 8MHZ Crystal
		
	    //Clock Setup for the TPM requires a couple steps.
	    //1st,  set the clock mux
	    //See Page 124 of f the KL25 Sub-Family Reference Manual
	    SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;// We Want MCGPLLCLK/2=24MHz (See Page 196 of the KL25 Sub-Family Reference Manual
	    SIM_SOPT2 &= ~(SIM_SOPT2_TPMSRC_MASK);
	    SIM_SOPT2 |= SIM_SOPT2_TPMSRC(1); //We want the MCGPLLCLK/2 (See Page 196 of the KL25 Sub-Family Reference Manual
		//Enable the Clock to the TPM0 and PIT Modules
		//See Page 207 of f the KL25 Sub-Family Reference Manual
		SIM_SCGC6 |= SIM_SCGC6_TPM0_MASK + SIM_SCGC6_TPM2_MASK;
	    // TPM_clock = 24MHz , PIT_clock = 48MHz
	    
}
//-----------------------------------------------------------------
// PIT - Initialisation
//-----------------------------------------------------------------
void InitPIT(){
	SIM_SCGC6 |= SIM_SCGC6_PIT_MASK; //Enable the Clock to the PIT Modules
	PIT_MCR = 0x1;
	enable_irq(INT_PIT-16); //  //Enable PIT IRQ on the NVIC
	set_irq_priority(INT_PIT-16,1);  // Interrupt priority = 0 = max
}


//---------------------------------------------------------------------
//            selector of transition between states
//---------------------------------------------------------------------
	







