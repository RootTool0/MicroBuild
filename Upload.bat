@echo off
setlocal enabledelayedexpansion

echo [1/5] Compiling...
"C:/Dev/avr-gcc-15.2.0-x64-windows/bin/avr-gcc.exe" -std=c++17 -Os -flto ^
    -fno-exceptions -fno-rtti -fno-threadsafe-statics -ffunction-sections -fdata-sections -Wl,--gc-sections ^
    -mmcu=atmega328p -DF_CPU=16000000UL ^
    -I"../Source"  "C:/Users/RootTool/Documents/GitHub/Clones/MicroBuild/Source/main.cpp" ^
    -o "MicroBuild.elf" ^
    -Wl,--gc-sections -Wl,-Map=MicroBuild.map

if %errorlevel% neq 0 (
    echo ERROR: Compilation failed!
    pause
    exit /b %errorlevel%
)

echo [2/5] Memory Usage Statistics:
"C:/Dev/avr-gcc-15.2.0-x64-windows/bin/avr-size.exe" -B "MicroBuild.elf"

echo [3/5] Generating HEX...
"C:/Dev/avr-gcc-15.2.0-x64-windows/bin/avr-objcopy.exe" -j .text -j .data -O ihex "MicroBuild.elf" "MicroBuild.hex"

echo [4/5] Uploading...
"C:/Dev/avr-gcc-15.2.0-x64-windows/bin/avrdude.exe" -v -c arduino -p m328p -P COM12 -D -U flash:w:"MicroBuild.hex":i

echo [5/5] Cleaning...
if exist "MicroBuild.elf" del "MicroBuild.elf"
if exist "MicroBuild.hex" del "MicroBuild.hex"
if exist "MicroBuild.map" del "MicroBuild.map"

echo DONE!
:: pause