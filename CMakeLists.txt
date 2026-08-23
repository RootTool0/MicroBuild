cmake_minimum_required(VERSION 3.18)

# Settings
set(AVR_GCC_PATH "C:/Dev/avr-gcc-15.2.0-x64-windows")
set(MCU_NAME "atmega328p")
set(MCU_DUDE "m328p")
set(COM_PORT "COM5")
set(F_CPU "16000000UL")

# Setup
project(MicroBuild CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source (IDE)
file(GLOB_RECURSE SOURCE_FILES_IDE CONFIGURE_DEPENDS "Source/*")
add_executable(${PROJECT_NAME} ${SOURCE_FILES_IDE})

# Package (IDE)
find_package(SDL2 CONFIG REQUIRED)

target_link_libraries(MicroBuild PRIVATE SDL2::SDL2)
target_compile_definitions(MicroBuild PRIVATE SDL_MAIN_HANDLED)

# Source (AVR)
file(GLOB_RECURSE SOURCE_FILES_AVR "Source/*.cpp")
set(SOURCE_FILES_AVR_STRING "")

foreach(src ${SOURCE_FILES_AVR})
    set(SOURCE_FILES_AVR_STRING "${SOURCE_FILES_AVR_STRING} \"${src}\"")
endforeach()

# AVR
# target_include_directories(${PROJECT_NAME} PRIVATE "${AVR_GCC_PATH}/avr/include")
add_compile_definitions(__IDE__)

file(GENERATE OUTPUT "../Upload.bat" CONTENT
"@echo off
setlocal enabledelayedexpansion

echo [1/5] Compiling...
\"${AVR_GCC_PATH}/bin/avr-gcc.exe\" -std=c++17 -Os -flto ^
    -fno-exceptions -fno-rtti -fno-threadsafe-statics -ffunction-sections -fdata-sections -Wl,--gc-sections ^
    -mmcu=${MCU_NAME} -DF_CPU=${F_CPU} ^
    -I\"../Source\" ${SOURCE_FILES_AVR_STRING} ^
    -o \"${PROJECT_NAME}.elf\" ^
    -Wl,--gc-sections -Wl,-Map=${PROJECT_NAME}.map

if %errorlevel% neq 0 (
    echo ERROR: Compilation failed!
    pause
    exit /b %errorlevel%
)

echo [2/5] Memory Usage Statistics:
\"${AVR_GCC_PATH}/bin/avr-size.exe\" -B \"${PROJECT_NAME}.elf\"

echo [3/5] Generating HEX...
\"${AVR_GCC_PATH}/bin/avr-objcopy.exe\" -j .text -j .data -O ihex \"${PROJECT_NAME}.elf\" \"${PROJECT_NAME}.hex\"

echo [4/5] Uploading...
\"${AVR_GCC_PATH}/bin/avrdude.exe\" -v -c arduino -p ${MCU_DUDE} -P ${COM_PORT} -D -U flash:w:\"${PROJECT_NAME}.hex\":i

echo [5/5] Cleaning...
if exist \"${PROJECT_NAME}.elf\" del \"${PROJECT_NAME}.elf\"
if exist \"${PROJECT_NAME}.hex\" del \"${PROJECT_NAME}.hex\"
if exist \"${PROJECT_NAME}.map\" del \"${PROJECT_NAME}.map\"

echo DONE!
:: pause"
)

# "C:/Dev/avr-gcc-15.2.0-x64-windows/bin/avr-nm.exe" --size-sort --print-size -C MicroBuild.elf

# Call:
# cmake . -B Build
# cmake -B Build -DCMAKE_TOOLCHAIN_FILE="$env:VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake"
