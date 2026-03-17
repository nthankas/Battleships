# BattleBoats

Two-player Battleship game on PIC32 over wired UART. NMEA-style message protocol with XOR checksums, cryptographic coin-flip for turn order, AI boat placement and targeting, OLED display with real-time hit/miss/sunk feedback.

## Architecture

- **Message** (`Message.c`): NMEA-like framing (`$PAYLOAD*CHECKSUM\n`). Encodes/decodes CHA, ACC, REV, SHO, RES message types. XOR checksum validation, character-at-a-time state machine decoder
- **Agent** (`Agent.c`): top-level FSM -- START, CHALLENGING, ACCEPTING, ATTACKING, DEFENDING, WAITING_TO_SEND, END_SCREEN. Drives negotiation handshake, move submission, and win/loss detection
- **Field** (`Field.c`): 6x10 grid management for both players. Random boat placement with collision avoidance, enemy attack registration with per-boat life tracking, AI guess generation. Four boat types (small/3, medium/4, large/5, huge/6)
- **Negotiation** (`Negotiation.c`): commitment scheme using modular hash. Each player commits hash of secret, reveals after, then coin-flip determines first move. Detects cheating via hash mismatch
- **FieldOled** (`FieldOled.c`): renders both fields side-by-side on 128x64 OLED. Shows own boats, hits, misses, and unknown squares on opponent field

## Communication

Two Uno32 boards connected via UART TX/RX crossover. Messages sent one character per timer tick (100Hz timer, transmit every 10 ticks). Includes Python test tools (`BattleBoatsPC.py`, `agent.py`) for host-side debugging.

## Target

PIC32MX795F512H (Uno32). Build with MPLAB X -- open `Battleships.X/`.
