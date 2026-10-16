# MicroBuild

**[RU]** | **[[EN]](README_EN.md)**

Секторный псевдо-3D движок в духе **Build Engine** _(Duke Nukem 3D)_ для **8-битных AVR**

Честные порталы, неевклидова геометрия и 10-15 FPS рендера на **ATmega328P** без FPU

[![Статья на Хабре](https://img.shields.io/badge/Хабр-80K%20просмотров-orange)](https://habr.com/ru/articles/1094808/)
[![License](https://img.shields.io/badge/license-MIT-blue)](LICENSE)

---

## Возможности

- Portal-based rendering _(сектора, стены, порталы)_
- Неевклидова геометрия
- Fixed-point Q16.16 вместо float
- BAM - все 360° в одном `uint8_t`
- LUT синусов на 64 значения
- Очередь задач рендера вместо рекурсии
- ~10–15 FPS рендера на ATmega328P без дисплея, ~4.8 FPS с ST7735

## Железо

| Компонент | Модель |
|---|---|
| МК | ATmega328P (Arduino Nano / Uno) |
| Дисплей | ST7735, 160x128, SPI |
| Тактовая | 16 МГц |
| Управление | Serial, 9600 baud |

Кнопки, джойстики, энкодеры - на ваше усмотрение

---

## Клонирование

Проект использует git-сабмодули _(`Advanced-AVR-Templates` и зависимости)_, поэтому клонировать нужно рекурсивно:

```bash
git clone --recurse-submodules https://github.com/RootTool0/MicroBuild.git
```

Если уже склонировали без флага:

```bash
git submodule update --init --recursive
```

---

## Сборка

### Компилятор

Нужен **AVR-GCC**. Варианты:

1. Готовая сборка: [avr-gcc-15.2.0-x64-windows.zip](https://github.com/ZakKemble/avr-gcc-build/releases/download/v15.2.0-1/avr-gcc-15.2.0-x64-windows.zip)
2. Из Arduino IDE - ищите в `Arduino/hardware/tools/avr/bin/`.

Путь прописывается в `CMakeLists.txt`:

```cmake
set(AVR_GCC_PATH "C:/Dev/avr-gcc-15.2.0-x64-windows")
```

### IDE-версия (ПК)

Для отладки на ПК используется **[RGFW](https://github.com/ColleagueRiley/RGFW)** - header-only библиотека, лежит прямо в проекте. Никаких **vcpkg** и внешних зависимостей ставить не нужно

```bash
cmake -B Build
```

Собирает исполняемый файл. Разрешение 160x128, окно масштабируется через `WINDOW_SCALE` в `Platform.h`.

### AVR-версия

**CMake** генерирует скрипт `Upload.bat`, который собирает прошивку через `avr-gcc` и заливает через `avrdude`

```bash
cmake -B Build
```

После этого в корне появится `Upload.bat`. Запуск:

1. Компиляция _(`-Os -flto -ffunction-sections -fdata-sections -Wl,--gc-sections`)_
2. Статистика памяти через `avr-size`
3. Генерация `.hex` через `avr-objcopy`
4. Заливка через `avrdude`
5. Очистка временных файлов

Настройки в `CMakeLists.txt`:

```cmake
set(MCU_NAME "atmega328p")
set(MCU_DUDE "m328p")
set(COM_PORT "COM4") # поменяйте под себя!
set(F_CPU "16000000UL")
```

> Перед первой заливкой проверьте `COM_PORT`!

---

## Управление

Управление идет через **Serial-порт** на **9600 бод**. Подойдет любой терминал: **PuTTY** или **Arduino Serial Monitor**

| Клавиша | Действие |
|---|---|
| `W` / `S` | Вперед / назад |
| `A` / `D` | Стрейф влево / вправо |
| `Q` / `E` | Поворот влево / вправо |
| `R` / `F` | Подъем / спуск по Z |

На ПК-сборке работает то же самое с клавиатуры.

---

## Карты

Карты лежат в `Source/Maps/`, выбор - в `Source/Map.h` _(раскомментировать нужную)_:

```cpp
// #include "Maps/DuoRooms.h"
// #include "Maps/ThreeRooms.h"
// #include "Maps/Stairs.h"
#include "Maps/SimplePortals!.h"
// #include "Maps/TwoCorridors.h"
// #include "Maps/TARDIS.h"
```

- `TARDIS` - неевклидова карта _(снаружи будка, внутри зал)_
- `SimplePortals!` - базовая демонстрация порталов
- `Stairs` - сектора с разной высотой пола и потолка

---

## Отладка

Флаги в `Source/Global.h`:

```cpp
#define DEBUG_SERIAL_PRINT_TICKS 1  // вывод тиков в serial
#define DEBUG_NO_ST7355          0  // отключить дисплей (для бенчмарков)
#define DEBUG_NO_COLLISION       0  // отключить коллизии
```

FPS считается через Timer1 с предделителем 64 _(`FPS = 250000 / Ticks`)_

---

## Структура проекта

```
Source/
├── main.cpp     - Точка входа, игровой цикл, управление
├── Renderer.h   - Рендер, очередь задач, палитра
├── Structures.h - FFixedFloat, векторы, FWall, FSector, FPlayer
├── Math.h       - Sin/Cos, Lerp, Clamp, SinLUT
├── Global.h     - Разрешение, дебаг-флаги, константы рендера
├── Platform.h   - ST7735, Serial, FPS-таймер, RGFW / AVR
├── Map.h        - Выбор карты
└── Maps/        - Карты
```

---

## Лицензия

MIT License

См. файл [LICENSE](LICENSE)

---

## Обратная связь

Если у вас есть вопросы, предложения или баги, то можете связаться со мной:

* **Telegram (канал/блог)**: [RootTool Blog](https://t.me/RootToolBlog)
* **Telegram (ЛС)**: [@RootTool0](https://t.me/RootTool0)
* **Discord**: `.roottool` (или `RootTool#5776`)
* **YouTube**: [RootTool](https://www.youtube.com/@RTEdits0)

_(прошу не беспокоить меня без причины)_
