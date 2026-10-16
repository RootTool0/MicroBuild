# MicroBuild

**[[RU]](README.md)** | **[EN]**

Sector-based pseudo-3D engine in the spirit of **Build Engine** _(Duke Nukem 3D)_ for **8-bit AVR**

Honest portals, non-Euclidean geometry and 10-15 FPS rendering on **ATmega328P** without FPU

[![Habr article](https://img.shields.io/badge/Habr-80K%20views-orange)](https://habr.com/ru/articles/1094808/)
[![License](https://img.shields.io/badge/license-MIT-blue)](LICENSE)

---

## Features

- Portal-based rendering _(sectors, walls, portals)_
- Non-Euclidean geometry
- Fixed-point Q16.16 instead of float
- BAM - all 360° in a single `uint8_t`
- Sin LUT with 64 entries
- Render task queue instead of recursion
- ~10–15 FPS rendering on ATmega328P without display, ~4.8 FPS with ST7735

## Hardware

| Component | Model |
|---|---|
| MCU | ATmega328P (Arduino Nano / Uno) |
| Display | ST7735, 160x128, SPI |
| Clock | 16 MHz |
| Controls | Serial, 9600 baud |

Buttons, joysticks, encoders - up to you

---

## Cloning

The project uses git submodules _(`Advanced-AVR-Templates` and dependencies)_, so clone recursively:

```bash
git clone --recurse-submodules https://github.com/RootTool0/MicroBuild.git
```

If you already cloned without the flag:

```bash
git submodule update --init --recursive
```

---

## Building

### Compiler

You need **AVR-GCC**. Options:

1. Prebuilt: [avr-gcc-15.2.0-x64-windows.zip](https://github.com/ZakKemble/avr-gcc-build/releases/download/v15.2.0-1/avr-gcc-15.2.0-x64-windows.zip)
2. From Arduino IDE - look in `Arduino/hardware/tools/avr/bin/`.

The path is set in `CMakeLists.txt`:

```cmake
set(AVR_GCC_PATH "C:/Dev/avr-gcc-15.2.0-x64-windows")
```

### IDE version (PC)

For PC debugging, **[RGFW](https://github.com/ColleagueRiley/RGFW)** is used - a header-only library, bundled right in the project. No **vcpkg** or external dependencies needed

```bash
cmake -B Build
```

Builds an executable. Resolution is 160x128, the window is scaled via `WINDOW_SCALE` in `Platform.h`.

### AVR version

**CMake** generates an `Upload.bat` script that builds the firmware via `avr-gcc` and flashes it via `avrdude`

```bash
cmake -B Build
```

After that, `Upload.bat` appears in the root. It runs:

1. Compilation _(`-Os -flto -ffunction-sections -fdata-sections -Wl,--gc-sections`)_
2. Memory stats via `avr-size`
3. `.hex` generation via `avr-objcopy`
4. Flashing via `avrdude`
5. Cleanup of temporary files

Settings in `CMakeLists.txt`:

```cmake
set(MCU_NAME "atmega328p")
set(MCU_DUDE "m328p")
set(COM_PORT "COM4") # change this for your setup!
set(F_CPU "16000000UL")
```

> Check `COM_PORT` before the first flash!

---

## Controls

Controls go through the **Serial port** at **9600 baud**. Any terminal works: **PuTTY** or **Arduino Serial Monitor**

| Key | Action |
|---|---|
| `W` / `S` | Forward / backward |
| `A` / `D` | Strafe left / right |
| `Q` / `E` | Turn left / right |
| `R` / `F` | Move up / down along Z |

On the PC build, the same keys work from the keyboard.

---

## Maps

Maps live in `Source/Maps/`, selection is in `Source/Map.h` _(uncomment the one you need)_:

```cpp
// #include "Maps/DuoRooms.h"
// #include "Maps/ThreeRooms.h"
// #include "Maps/Stairs.h"
#include "Maps/SimplePortals!.h"
// #include "Maps/TwoCorridors.h"
// #include "Maps/TARDIS.h"
```

- `TARDIS` - non-Euclidean map _(small box outside, large hall inside)_
- `SimplePortals!` - basic portal demo
- `Stairs` - sectors with different floor and ceiling heights

---

## Debugging

Flags in `Source/Global.h`:

```cpp
#define DEBUG_SERIAL_PRINT_TICKS 1  // print ticks to serial
#define DEBUG_NO_ST7355          0  // disable display (for benchmarks)
#define DEBUG_NO_COLLISION       0  // disable collisions
```

FPS is measured via Timer1 with prescaler 64 _(`FPS = 250000 / Ticks`)_

---

## Project structure

```
Source/
├── main.cpp     - Entry point, game loop, controls
├── Renderer.h   - Renderer, task queue, palette
├── Structures.h - FFixedFloat, vectors, FWall, FSector, FPlayer
├── Math.h       - Sin/Cos, Lerp, Clamp, SinLUT
├── Global.h     - Resolution, debug flags, render constants
├── Platform.h   - ST7735, Serial, FPS timer, RGFW / AVR
├── Map.h        - Map selection
└── Maps/        - Maps
```

---

## License

MIT License

See [LICENSE](LICENSE)

---

## Feedback

If you have questions, suggestions or bugs, you can reach me:

* **Telegram (channel/blog)**: [RootTool Blog](https://t.me/RootToolBlog)
* **Telegram (DM)**: [@RootTool0](https://t.me/RootTool0)
* **Discord**: `.roottool` (or `RootTool#5776`)
* **YouTube**: [RootTool](https://www.youtube.com/@RTEdits0)

_(please don't bother me without a reason)_
