Final Project
Uriel Manzur - 312611197
Bar Rosenzw - 315520700

Py files:
	Final_project GUI PART.py		the main file of the PC side hold the GUI
	Final_project.py				holds the uart functions that comunnicate with the MCU

Sources files:
	main.c - 						the state machine and global variables
	UART.c							used for calibrating and initializing the uart
	dma.c							used for calibrating and initializing the DMA
	LCD.c							used for calibrating and initializing the LCD
	ADCandDAC.c						used for calibrating and initializing the ADC & DAC
	BoardSupport.c					used for calibrating and initializing the pins on the MCU 
	halGPIO.c						contains all of the functions and handlers
	
Headers files:
	halGPIO.h						directing regular commands to this specific MCU commands