#pragma once

#include "../Core/Aliases.h"

#include <avr/io.h>

struct FSPI
{
    
public:
    static void Initialize()
    {
        DDRB |= (1 << PB3) | (1 << PB5) | (1 << PB2);
        SPCR = (1 << SPE) | (1 << MSTR);
        SPSR = (1 << SPI2X);
    }
    
    static void Send(const uint8 InData) { SPDR = InData; }
    static void Synchronize() { while(!(SPSR & (1 << SPIF))); }
    
    static void SendAndSynchronize(const uint8 InData) { Send(InData); Synchronize(); }
};
