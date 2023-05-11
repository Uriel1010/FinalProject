# Terminal Project

The objective of this project is to implement bidirectional asynchronous serial communication using the RS-232 standard between the KL25Z microcontroller and a PC. To achieve this, support code needs to be developed for both the microcontroller and the PC.

Physical Layer: The project utilizes the RS-232 protocol with full-duplex communication (without modem support) using only three wires for connection.
Data Link Layer: The communication is facilitated through a UART peripheral component in the KL25Z microcontroller and a communication controller on the PC side.
The project aims to establish reliable and efficient communication between the microcontroller and the PC, enabling data exchange and control commands in a synchronous manner according to the RS-232 standard.

Note: The project description has been translated to English for clarity and understanding.

## Table of Contents
- [Technologies Used](#technologies-used)
- [Usage](#usage)
- [Examples](#examples)
- [Project Structure](#project-structure)


### Technologies Used

This project utilizes the following technologies and components:

- DMA (Direct Memory Access): Used for efficient data transfer between peripherals and memory.
- UART (Universal Asynchronous Receiver-Transmitter): Used for serial communication between the microcontroller (KL25Z) and the PC.
- ARM Cortex-M0: The microcontroller core architecture used in this project.
- LCD Display: Used for visual output and user interaction.
- MKL25Z4 Microcontroller: The microcontroller platform used in the project.
- GPIO (General Purpose Input/Output): Used for controlling digital input/output pins.

These technologies and components play a crucial role in the functionality and performance of the project, enabling features such as bidirectional synchronous serial communication, user interface display, and efficient data transfer.



### Usage
This project implements a bidirectional asynchronous serial communication between a KL25Z controller and a PC using the RS-232 standard. It allows for various modes of operation:

1. **Chat Mode:** In this mode, you can send messages between the PC and the controller. Enter your message in the text box and click "SEND" to send it to the controller. The message will be displayed on the LCD of the controller and in the large textbox with the prefix `{PC}`. To exit this mode, click "EXIT" on the PC side.

2. **File Transfer Mode:** This mode enables you to transfer files between the PC and the controller. You can choose to send a file from the PC to the controller or receive a file from the controller. When entering this mode, the PC will send a message to the controller to indicate the file transfer mode. In the sending mode, select a file and click "OK" to initiate the file transfer. The controller checks if the file is within its memory limits and if it exists in the specified location. In the receiving mode, select the file number on the controller to send it to the PC. Upon successful transfer, a confirmation message will be displayed.

3. **Terminal Configuration:** This mode allows you to configure the terminal settings. When entering this mode on the PC side, the current settings will be displayed automatically. You can change the COM port and baud rate, and clicking "Apply" will send the new settings to the controller to update its baud rate accordingly. There is no specific indication on the controller side when entering this mode.

Please note that the project utilizes DMA for both sending and receiving files, enabling continuous data transfer between the PC and the controller.

### Examples

- Chat Mode:
  1. Enter a message in the text box on the PC side.
  2. Click "SEND" to send the message to the controller.
  3. The message will be displayed on the controller's LCD and in the large textbox with the prefix `{MCU}`.
  4. To exit the Chat Mode, click "EXIT" on the PC side.

- File Transfer Mode (Sending):
  1. Enter the File Transfer Mode on the PC side.
  2. Choose a file to send.
  3. Click "OK" to initiate the file transfer.
  4. The PC will check if the file size is within the controller's memory limits and if the file exists.
  5. Upon successful transfer, a confirmation message will be displayed.

- File Transfer Mode (Receiving):
  1. Enter the File Transfer Mode on the PC side.
  2. Select the file number on the controller to send it to the PC.
  3. The file will be received and saved in the specified location on the PC.
  4. A confirmation message will be displayed.

Make sure to follow the instructions and interact with the GUI on both the PC and controller sides to utilize the different modes effectively.

### Examples

- **Chat Mode:**

  1. Enter a message in the text box on the PC side.
  2. Click "SEND" to send the message to the controller.
  3. The message will be displayed on the controller's LCD and in the large textbox with the prefix `{PC}`.
  4. To exit the Chat Mode, click "EXIT" on the PC side.

- **File Transfer Mode (Sending):**

  1. Enter the File Transfer Mode on the PC side.
  2. Choose a file to send.
  3. Click "OK" to initiate the file transfer.
  4. The PC will check if the file size is within the controller's memory limits and if the file exists.
  5. Upon successful transfer, a confirmation message will be displayed.

- **File Transfer Mode (Receiving):**

  1. Enter the File Transfer Mode on the PC side.
  2. Select the file number on the controller to send it to the PC.
  3. The file will be received and saved in the specified location on the PC.
  4. A confirmation message will be displayed.

Make sure to follow the instructions and interact with the GUI on both the PC and controller sides to utilize the different modes effectively.

### Project Structure

The project is organized into the following directories and files:

- `pre_terminal.pdf`: Document providing information about the pre-terminal phase.

- `Project_Headers/`: Contains header files used in the project.
  - `ADCandDAC.h`: Header file for ADC and DAC functions.
  - `app.h`: Header file for the application layer.
  - `arm_cm0.h`: Header file for the ARM Cortex-M0 core.
  - `bme.h`: Header file for the BME sensor.
  - `BoardSupport.h`: Header file for board-specific support.
  - `derivative.h`: Header file for derivative-specific definitions.
  - `dma.h`: Header file for DMA functions.
  - `halGPIO.h`: Header file for GPIO functions.
  - `LCD.h`: Header file for the LCD display.
  - `mcg.h`: Header file for the MCG module.
  - `MKL25Z4.h`: Header file for the MKL25Z4 microcontroller.
  - `TFC.h`: Header file for the TFC library.
  - `UART.h`: Header file for UART functions.

- `README.md`: This file, providing an overview and instructions for the project.

- `README.txt`: Alternative version of the README file in plain text format.

- `Sources/`: Contains the source code files for the project.
  - `ADCandDAC.c`: Implementation of ADC and DAC functions.
  - `arm_cm0.c`: Implementation of ARM Cortex-M0 functions.
  - `BoardSupport.c`: Implementation of board-specific support functions.
  - `dma.c`: Implementation of DMA functions.
  - `halGPIO.c`: Implementation of GPIO functions.
  - `LCD.c`: Implementation of LCD functions.
  - `main.c`: Main source file of the project.
  - `mcg.c`: Implementation of MCG functions.
  - `sa_mtb.c`: Implementation of SA MTB functions.
  - `UART.c`: Implementation of UART functions.

- `Terminal_project.py`: Python script for the terminal project.

- `Terminal_Project_CE_2022_v1.pdf`: Document providing information about the terminal project.

- `Terminal-project GUI PART .py`: Python script for the GUI part of the terminal project.

