# Control System of Motor-Based Machine

## Project Description
The goal of this project is to design and implement a control system for a motor-based machine. The system supports both manual control using an analog joystick and remote control via a PC through a serial communication channel. The emphasis in the system design is on high-performance execution under real-time constraints with high precision.

The project involves the following key aspects:
1. Designing and implementing a real-time system based on interrupts using C++/C programming language. The system interacts with the hardware components and reads distance measurements.
2. Using a PC as an interface for user interaction and visualization using PySimpleGUI. The MCU (Microcontroller Unit) is connected to the PC through an asynchronous serial communication channel based on RS-232 standard.
3. The PC interface allows parameter configuration, file transmission, and high-level commands to the MCU. It also displays a radar image on the PC. The PC interface is developed using the Python programming language and supports serial communication between the controller and the PC.
4. The interface enables file transfer, including high-level encoded commands, for execution on the controller. All files on the controller are stored in RAM only.

## Project Flow
The project supports several types of operations:

* **Manual Control of the Motor using Joystick**: The joystick is used to control the motor movement manually. The joystick's movement in different directions corresponds to motor movement. The joystick's button is used to stop the motor.

* **Drawing on the PC Screen using Joystick**: The joystick is used to move a pointer (a white dot) on the PC screen. By default, the pointer draws on the canvas. The user can switch between drawing, moving the pointer, and erasing modes.

* **Motor Calibration Mode**: This mode is used to calibrate the motor. The goal is to align the motor when the white pointer is on the red line. Additionally, the mode calculates the number of steps in a full rotation to determine the nominal angle.

* **Script Mode**: This mode allows loading and executing scripts on the controller. A file can be sent to the controller, and it will execute the commands defined in the file. The PC displays the controller's output during script execution.

## Hardware Connections

The following hardware connections are used in the project:

* Buttons:
  * PB2 → P1.2 → A5
* LEDs:
  * RR2 → P2.3 → C2
  * GG7 → P2.2 → C1
  * BBB6 → P2.1 → C0
* Motor Phases:
  * IN1 → C11
  * IN2 → C10
  * IN3 → C6
  * IN4 → C5
* Joystick:
  * VRy → E22
  * VRx → E20
  
## Conclusion and Proposed Improvements

During the project, we faced challenges related to understanding the calibration process of the ADC module and the possibility of sampling from two legs. Further exploration and experimentation are needed to fully understand and utilize these features.

To improve the project, we recommend considering the following:

* Explore advanced motor control algorithms, such as PID control, to enhance motor movement precision.
* Implement error handling and recovery mechanisms in case of communication or execution failures.
* Optimize the memory usage on the controller to accommodate larger files or increase storage capacity if necessary.
* Enhance the user interface with additional features and visualizations.