## STM32 Traffic Light Control & UART Communication
A sophisticated embedded system project developed on the STM32F103 (or similar) platform using STM32CubeIDE. This project implements a traffic control simulation combined with a command-based UART communication protocol using Finite State Machines (FSM).

## Introduction
This project simulates a smart traffic system that not only manages timing via hardware timers but also communicates with a computer via UART. It features real-time ADC data acquisition and a robust command parser to respond to specific user instructions.

## System Logic & Architecture
The software is structured around three main components:

### 1. Hardware Configuration
ADC1 (Channel 0): Configured in continuous conversion mode to read sensor data (mimicking traffic density or manual overrides).

TIM2: Provides the base time clock. It triggers interrupts to handle timing flags (like one_sec_flag).

USART2: Operating at 9600 baud, used for full-duplex communication with a serial terminal.

### 2. Finite State Machines (FSM)
The core logic resides in two cooperative FSMs located in the main loop:

Command Parser FSM: * Monitors the UART buffer.

Detects specific command formats (e.g., commands starting with ! and ending with #).

Sets flags for the communication FSM once a valid command is recognized.

UART Communication FSM: * Handles the transmission of data (like ADC values) back to the user.

Manages timeouts and re-transmission logic.

### 3. Background Tasks
Heartbeat LED: The system toggles an LED on PA5 every second to indicate the system is "alive."

Buffer Processing: Incoming serial data is handled via interrupts and processed in the main loop to ensure no commands are missed during execution.

## File Structure
main.c: Contains hardware initialization, the main execution loop, and system clock configuration.

uart.h / uart.c: (External modules) Implements the logic for the Command Parser and Communication FSMs.

timer.h / timer.c: Handles timer-based flags and interrupt service routines.

## How to Run
### Hardware Requirements
STM32 Development Board (e.g., Nucleo-F103RB).

USB-to-UART Converter (if not using the built-in ST-Link debugger).

LEDs connected to GPIOs for traffic light simulation.

## Software Setup
Clone the repository:

Bash

git clone https://github.com/your-username/stm32-traffic-uart.git
Open Project: Import the project into STM32CubeIDE.

Build: Click the 🔨 (Build) icon to compile the code.

Flash: Connect your board and click the ▶️ (Resume/Run) icon.

## Testing the Communication
Use a serial terminal (like Hercules, Tera Term, or Putty) with the following settings:

Baudrate: 9600

Data bits: 8

Stop bits: 1

Parity: None

Example Commands:

!RST#: Reset/Start sending ADC data.

!OK#: Acknowledge data receipt and stop sending.

Author
Nguyen Hoang Anh - Ho Chi Minh city University of Technology (HCMUT).
