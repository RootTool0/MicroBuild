#pragma once

#include "../Core/Aliases.h"

#if __IDE__
struct FPROGMEM
{
    
public:
    FORCEINLINE static uint8  ReadInt8 (const void* InAddr) { return *static_cast<const uint8 *>(InAddr); }
    FORCEINLINE static uint16 ReadInt16(const void* InAddr) { return *static_cast<const uint16*>(InAddr); }
    FORCEINLINE static uint32 ReadInt32(const void* InAddr) { return *static_cast<const uint32*>(InAddr); }
    FORCEINLINE static uint32 ReadFloat(const void* InAddr) { return *static_cast<const uint32*>(InAddr); }
    
    FORCEINLINE static const void* ReadPtr(const void* InAddr) { return *static_cast<const void* const*>(InAddr);  }
    template<typename T>
    FORCEINLINE static T ReadPtrAndCast(const void* InAddr) { return reinterpret_cast<T>(ReadPtr(InAddr)); }
    
    FORCEINLINE static void Memcpy(void* OutDest, const void* InFlashAddr, const uint16 InSize) { memcpy(OutDest, InFlashAddr, InSize); }
};
#else
#include "avr/pgmspace.h"

struct FPROGMEM
{
    
public:
    FORCEINLINE static uint8  ReadInt8 (const void* InAddr) { return pgm_read_byte (InAddr); }
    FORCEINLINE static uint16 ReadInt16(const void* InAddr) { return pgm_read_word (InAddr); }
    FORCEINLINE static uint32 ReadInt32(const void* InAddr) { return pgm_read_dword(InAddr); }
    FORCEINLINE static uint32 ReadFloat(const void* InAddr) { return pgm_read_dword(InAddr); }
    
    FORCEINLINE static const void* ReadPtr(const void* InAddr) { return reinterpret_cast<const void*>(pgm_read_ptr(InAddr)); }
    template<typename T>
    FORCEINLINE static T ReadPtrAndCast(const void* InAddr) { return reinterpret_cast<T>(ReadPtr(InAddr)); }
    
    FORCEINLINE static void Memcpy(void* OutDest, const void* InFlashAddr, const uint16 InSize) { memcpy_P(OutDest, InFlashAddr, InSize); }
};
#endif
