  BattleBoats: Embedded Two-Player Battleship Game (C, PIC32, UART)

Author: Nikhil Thankasala

Overview:  
BattleBoats is a real-time, two-player Battleship-style strategy game built entirely in C for PIC32 microcontrollers using UART-based serial communication. Designed for embedded systems, this project includes a complete communication protocol, display interface, cryptographic turn negotiation, and two coordinated AI-driven players capable of playing against each other using only direct wired serial interaction.

The game operates across two microcontrollers connected by jumper wires over UART. A custom message protocol is used to send and verify move data, handle cryptographic commitment schemes to determine turn order, and communicate game state in real time. The player interfaces with the game through a small OLED display and onboard buttons, making this a fully self-contained experience that replicates Battleship on hardware.

Modules and Architecture:  
The game system is built using four key software modules:

- **Message**: Encodes and decodes UART messages using a lightweight NMEA-like protocol with XOR checksums. Includes full validation and framing logic for reliable transmission.
- **Agent**: A central FSM that governs the overall game state including turn order, display updates, move submission, and win/loss detection.
- **Field**: Handles grid logic, boat placement, AI move generation, and tracking both the player’s own field and the enemy’s. Manages collision avoidance, damage tracking, and strategic targeting.
- **Negotiation**: Implements a simple cryptographic commitment scheme using modular hashing to fairly determine which player goes first in a game.

Each module was paired with its own test suite to independently verify edge cases, interaction behavior, error conditions, and round-trip message validity.

Approach and Development Process:  
I implemented all four modules and their corresponding test harnesses individually, simulating UART traffic, message integrity, and field interactions. I first focused on building a reliable, parsable message protocol with working checksum logic. Once communication was stable, I implemented a field-based AI with randomized legal placement and move generation, then expanded into negotiation protocols and the top-level FSM to unify all modules.

Test harnesses were written to simulate interactive games, validate protocol correctness under bad input, and track state progression across module boundaries. The system was designed with modularity, event-driven responsiveness, and strict memory scoping in mind. A full integration test validated the entire gameplay sequence from negotiation through to victory detection.

Results and Notes:  
The final system plays full games successfully over wired UART using two PIC32 boards. The game dynamically updates on a pixel-based OLED screen with player-visible feedback including hits, misses, and sunk ships. Message transmission is robust and handles corrupted or malformed input gracefully. Turn negotiation is handled fairly through commitment schemes, and the system can detect and report violations of protocol.

This project involved over 20 hours of development time and required deep coordination between modules, careful event modeling, and debugging across both communication and gameplay logic. Future enhancements could include a smarter AI, support for dynamic ship sizes, or a full GUI-based host emulator to play against a board via USB.
