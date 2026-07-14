# 8-Bit Microprocessor Emulator (ESP32)



A hardware-based 8-bit microprocessor emulator built on ESP32 microcontrollers.  
This project focuses on core CPU fundamentals: registers, instruction execution, arithmetic, and logic operations, all exposed through direct user interaction.

The system trades complexity for clarity. Every instruction is visible, deliberate, and constrained to classic 8-bit behavior.

<img width="480" height="360" alt="494817266_673571051972172_4024366124376740731_n" src="https://github.com/user-attachments/assets/6dc21d79-1454-4da2-bd74-5045761d748e" />
<img width="480" height="360" alt="494691862_1793637937849260_3353310645954634374_n" src="https://github.com/user-attachments/assets/c894e90b-adcd-4a52-9034-2455f7c1ba9b" />


## What This Is

- A simplified 8-bit CPU emulator  
- Four general-purpose registers: **A, B, C, D**  
- User-driven instruction execution via keypad  
- Real-time register visualization on a TFT display  
- Wireless communication between processing and display units  


## Instruction Set

The emulator supports a compact instruction set designed to cover essential CPU behavior:

- `MOV`, `XCHG`  
- `ADD`, `SUB`, `NEG`  
- `AND`, `OR`  
- `INC`, `DEC`  

All operations are masked to 8-bit limits (0–255).

## System Architecture

**Processor Unit**
- Executes instructions  
- Manages registers  

**Display Unit**
- Renders register states  
- Shows execution status  

**Input**
- 4×4 matrix keypad  

**Output**
- 7-inch TFT display  
- LEDs  
- Buzzer  

**Communication**
- ESP-NOW between ESP32 boards  


## Why This Exists

Modern systems hide the machine.  
This project exposes it.

It is designed for learning, testing, and demonstrating how processors actually work at a low level, without abstractions, operating systems, or unnecessary layers.

## Scope and Limits

- No memory addressing or interrupts  
- No pipelining or parallel execution  
- Not a full CPU, intentionally  

This is a teaching tool and a thinking device.
