#pragma once

#include "Core/Aliases.h"

struct FDelay
{
    template<uint32 InTicks>
    FORCEINLINE static void Ticks() 
    {
#ifndef __IDE__
        __builtin_avr_delay_cycles(InTicks);
#endif
    }
    
    template<uint32 InMicroseconds>
    FORCEINLINE static void Microseconds() 
    {
        Ticks<static_cast<uint32>(static_cast<double>(F_CPU) * InMicroseconds / 1000000.0)>();
    }
    
    template<uint32 InMilliseconds>
    FORCEINLINE static void Milliseconds() 
    {
        Microseconds<InMilliseconds * 1000>();
    }
};
