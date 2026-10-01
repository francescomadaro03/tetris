# Tetris on the LandTiger LPC1768 board

A playable bare-metal Tetris for the NXP LPC1768 (ARM Cortex-M3) on the LandTiger development board, written in C with the Keil µVision toolchain.

It was developed locally as a special project for the **Computer Architectures** course at Politecnico di Torino, and is published here as a snapshot of the finished project, so the repository has no development history.

## Gameplay

- 10×20 playfield, seven tetromino types, random piece generation (LFSR seeded with the time spent in the touch-panel calibration at boot).
- Moving, soft drop, hard drop, pause/resume, game over and restart. The record is kept in RAM only (it is lost on reset).
- Rotation toggles between two orientations per piece (it is not a full four-state rotation).
- Scoring: 10 points per placed piece, 100 per cleared line, 600 for four lines at once.
- **Power-ups:** every 5 cleared lines a special block appears inside the stack; it is collected when the line containing it is cleared.
  - `S`: slows the game down to the base speed for 15 seconds.
  - `L`: clears half of the occupied rows.
- **Malus:** every 10 cleared lines a garbage row (7 filled cells, 3 gaps) is pushed up from the bottom.
- Game speed is set with the board potentiometer (5 levels, read via ADC); the LEDs show the selected level.
- Audio: the Tetris theme and a short jingle on line clear, played through the DAC.

### Controls

| Input | Action |
|---|---|
| Joystick left / right | Move the piece |
| Joystick up | Rotate |
| Joystick down | Soft drop (double speed) |
| KEY1 | Start / pause / resume, restart after game over |
| KEY2 | Hard drop |
| Potentiometer | Base speed |

## How it works

`main()` only initialises the peripherals and then goes to sleep (`WFI` with `SLEEPONEXIT`). Everything else runs inside interrupt handlers:

| Peripheral | Role |
|---|---|
| Timer 0 | Gravity tick: moves the piece down, checks collisions, spawns the next piece |
| Repetitive Interrupt Timer (RIT) | Periodic polling of KEY1, KEY2 and the joystick; starts ADC conversions |
| ADC (AD0.5) | Reads the potentiometer and sets the Timer 0 period |
| Timer 1 | Outputs one sine-table sample at a time to the DAC (pin P0.26) |
| Timer 2 | Note durations for the music |
| Timer 3 | One-shot 15 s timer that ends the slow-down power-up |

Code layout:

- `Source/tetris/backend.c`: game rules (movement, line clearing, scoring, power-ups, malus).
- `Source/tetris/frontend.c`: LCD drawing of pieces and the game UI, LFSR random numbers.
- `Source/tetris/collision_library.c`: collision checks for each piece shape.
- `Source/music/`: melody tables and note playback.
- `Source/RIT`, `timer`, `ADC`, `joystick`, `led`, `button_EXINT`: peripheral setup and interrupt handlers.

## Code origin

The project started from the course lab template. The LCD (`GLCD`) and touch panel drivers, the startup file, system and CMSIS files, and the skeletons of the peripheral drivers come from that template (originally by AVRman, adapted by the course staff). The Tetris game logic, collision code, rendering of pieces, music, power-ups and malus, and the game-specific parts of the interrupt handlers are my own work.

## Build and run

- Tools: Keil µVision 5 with Arm Compiler 6 (V6.24) and the LPC1700 device pack (device LPC1768).
- Open `sample.uvprojx`. The `LandTiger_LPC1768 (release)` target is for the board; `SW_DEBUG` is for debugging.
- I have only built and run it with this toolchain, on a LandTiger board.

## Limitations

- Game logic and LCD drawing run inside interrupt handlers. This keeps `main` trivial, but a flag-based design with the work done outside the ISRs would be more robust.
- Buttons and the joystick are polled in the RIT handler with only basic edge detection and no real debouncing.
- Only two orientations per piece; the record is not persisted.
- No automated tests, and performance has not been measured.
