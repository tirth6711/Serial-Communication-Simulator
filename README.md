# Serial Communication Simulator

## Overview
A C-based software simulation of serial communication between virtual devices. The project demonstrates packet construction, data transmission, error injection, sequence tracking, and acknowledgment-based error recovery.

## Project Stages

### Stage 1 — Basic Transmitter and Receiver
Implements the basic exchange of data between simulated devices.

### Stage 2 — Data Packet Structure
Organizes transmitted data into a structured packet containing fields such as device ID, data type, length, data, checksum, and start/end markers, as implemented.

### Stage 3 — Error Injection Engine
Introduces simulated transmission errors to evaluate the receiver's error-handling behavior.

### Stage 4 — Sequence Numbers
Adds sequence information to help identify and track packets.

### Stage 5 — ACK/NACK Protocol
Implements acknowledgment and negative acknowledgment handling to support packet confirmation and retransmission, where implemented.

## Technologies
- C programming language
- File-based communication simulation
- Packet framing and error-handling concepts

## Repository Structure
- `S1_Tx_Rx/` — Basic transmission and reception
- `S2_Data_structure/` — Packet structure
- `S3_Error_injection_engine/` — Error injection
- `S4_add_seq/` — Sequence numbers
- `S5_ACK_NACK/` — ACK/NACK handling

## How to Run
1. Open the folder for the stage you want to test.
2. Follow the compilation and execution instructions for that stage.
3. Send test data and observe the receiver output.
4. For the later stages, test the implemented error-handling and acknowledgment behavior.

Add the exact compiler commands and expected output after testing each stage.

## Learning Outcomes
- C programming and modular code organization
- Packet framing and data fields
- Sequence tracking
- Simulated transmission errors
- Reliable communication concepts

## Author
Tirth Patel
