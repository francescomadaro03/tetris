# Tetris: fully working game on a Landtiger LPC17XX Board


> A fully playable bare-metal game developed for the NXP LPC1768 (LandTiger) development board

This project was developed as special project within **Computer Architectures** course at **Politecnico di Torino**.

## Features
- **Complete Gameplay Mechanics:** Includes score tracking, collision detection, and dynamic 90° tile rotation.
- **Hardware-Driven Audio:** Integrated background music utilizing Pulse Width Modulation (PWM).
- **Dynamic Entities:** Implementation of special power-ups that resemble the original game
- **Bare-metal Rendering:** Custom routines for smooth LCD drawing and state updates.

## Tech Stack & Hardware
- **Languages:** C, ARM Thumb-2 Assembly.
- **Target Hardware:** NXP LPC1768 (ARM Cortex-M3).
- **Hardware Abstraction:** Strategic use of hardware templates and low-level drivers to safely interface with the board's peripherals (Timers, GPIO, LCD).

## Engineering Approach
Bridging the gap between high-level Software Engineering and low-level Embedded Systems, this project was built with a dual focus on **architecture** and **bare-metal performance**:

- **Modular Design in C:** The software was developed to ensure the most component modularity and reusability, to make it easily debuggable.
- **Performance Optimization:** The main development goal has been to ensure the best performance rate possible.
- **Interrupt-Driven Architecture:** Utilizing Repetitive Interrupt Timers (RIT) and hardware interrupts to manage game state, audio polling, and input debouncing concurrently, avoiding blocking operations in the main execution loop.
