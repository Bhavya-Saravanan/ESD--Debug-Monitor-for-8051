**ESD – Debug Monitor for 8051**

**📌 Project Overview**

This project implements a Debug Monitor for the AT89C51RC2 microcontroller, designed as part of Embedded Systems Development. The monitor provides real-time debugging features, allowing developers to step through code, inspect registers, and validate memory contents.

**Key highlights:**

Supports both Bluetooth (HC-05) and RS-232 interfaces for flexible communication.

Implements single-step execution using INT1 interrupts and stack pointer manipulation.

Provides memory read/write capabilities for XRAM, NVRAM, and code memory verification.

Compact firmware design with an efficient 3.9 KB footprint.

**⚙️ Features**

**Instruction-Level Debugging**

Single-step execution with register capture (ACC, PSW, SP, PC).

**Memory Inspection & Modification**

Read/write external memory and verify program code.

**Flexible Communication**

UART shared via SPDT switch for wired/wireless debugging.

**Reliable & Tested**

Verified using Tera Term for stable command execution and program dumping.

**🖥️ Hardware Used**

AT89C51RC2 Microcontroller

HC-05 Bluetooth Module

RS-232 (MAX232) for wired communication

NVRAM/XRAM (32KB)

LCD Display for visualization

**🛠️ Tools & Development**

Compiler: SDCC (Small Device C Compiler)

Debugger: Paulmon2 (reference for validation)

Terminal: Tera Term (command execution)

Testing Tools: Oscilloscope, Logic Analyzer


**🚀 Getting Started
Prerequisites**

Install SDCC

Flash HEX file to AT89C51RC2 using programmer

Connect via Bluetooth (HC-05) or RS-232

**Commands**

s → Single-step execution

r → Read memory

w → Write memory

? → Help menu
