# AGENTS.md

## Project
- PlatformIO C++ project for Arduino on `nanoatmega328`.
- The project is a cooperative bomb-defusal style game inspired by "Keep Talking and Nobody Explodes".
- One player interacts with the Arduino device and solves riddles. Other players have the instructions/manual and must communicate what to do.
- Main firmware source lives in `src/`; shared headers belong in `include/`; PlatformIO tests belong in `test/`.

## Hardware
- Board: Arduino Nano / ATmega328.
- Display: SSD1306 OLED connected via I2C.
- Display libraries: `Adafruit_SSD1306` with `Adafruit_GFX`.
- LEDs: 3 outputs on `A0`, `A1`, and `A2`.
- Buttons: 4 inputs on `D7`, `D8`, `D9`, and `D10`.
- Sound: beeper or sound module on `D2`.
- Cable inputs: cuttable/restorable cable-style inputs on `D3` through `D6`.

## Game Design
- Show a countdown on the SSD1306 display.
- Play beeping sounds during the countdown.
- Track lives; incorrect player actions remove one life.
- Keep the starting number of lives in the same constants/config file as pin assignments.
- Model the game as a sequence of riddle steps.
- Each step should define:
  - An identification/output pattern, such as LED colors, display shapes, text, or other future signals.
  - A required player action, such as cutting a cable, pressing buttons, or restoring a cable.
- Keep step definitions separate from the main runtime so different games can be swapped in.

## File Organization
- Keep pin assignments and simple game constants, including starting lives, in a dedicated constants/config file.
- Keep reusable hardware helpers separate from game step definitions.
- Keep alternate game/riddle sequences in separate files where practical.

## Agent Guidance
- Update this file when the user explicitly asks.
- If the user shares information that seems important for future development, ask whether to add it here.
- If a user request conflicts with this file, ask for clarification before changing code.

## Commands
- Build: `pio run`
- Test: `pio test`
- Upload: `pio run -t upload`
- Clean: `pio run -t clean`

## Coding Guidelines
- Keep changes small and focused on the requested behavior.
- Follow the existing C++/Arduino style in nearby files.
- Prefer clear names and simple control flow over new abstractions.
- Keep hardware-specific constants near the code that uses them unless they are shared.
- Do not commit generated PlatformIO output such as `.pio/`.

## Validation
- Run `pio run` after firmware changes when PlatformIO is available.
- Add or update tests under `test/` when changing behavior that can be covered without hardware.
