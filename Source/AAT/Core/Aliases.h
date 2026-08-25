#pragma once

#ifdef __IDE__

// Dirty hack to stop the IDE from complaining about AVR-specific constructs
#define F_CPU 0
#define __extension__
#define __volatile__
#define __DOXYGEN__
#define __AVR_ATmega328__
#define PROGMEM

#endif

#include <stdint.h> 

using int8 = int8_t;
using int16 = int16_t;
using int32 = int32_t;
using int64 = int64_t;
using uint8 = uint8_t;
using uint16 = uint16_t;
using uint32 = uint32_t;
using uint64 = uint64_t;

static_assert(sizeof(int8)    == 1, "Invalid int8  size");
static_assert(sizeof(int16)   == 2, "Invalid int16 size");
static_assert(sizeof(int32)   == 4, "Invalid int32 size");
static_assert(sizeof(int64)   == 8, "Invalid int64 size");
static_assert(sizeof(uint8)   == 1, "Invalid int8  size");
static_assert(sizeof(uint16)  == 2, "Invalid int16 size");
static_assert(sizeof(uint32)  == 4, "Invalid int32 size");
static_assert(sizeof(uint64)  == 8, "Invalid int64 size");

#define FORCEINLINE [[gnu::always_inline]] inline
#define NOINLINE __attribute__ ((noinline))
