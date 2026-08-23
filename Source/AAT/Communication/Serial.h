#pragma once

#include "../Core/Aliases.h"

#include <avr/io.h>

struct FSerial
{

public:
    template<uint32 InBaud>
    FORCEINLINE static void Initialize()
    {
        // Set Baud Rate registers
        constexpr uint16 UBBR = (F_CPU / 16UL / InBaud) - 1;
        UBRR0H = (uint8)(UBBR >> 8);
        UBRR0L = (uint8)(UBBR);
        
        UCSR0B = (1 << RXEN0) | (1 << TXEN0);   // UCSR0B: Enable Receiver (RX) and Transmitter (TX)
        UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // UCSR0C: Set Frame Format - 8 Data bits, No parity, 1 Stop bit (8N1)
    }
    
    FORCEINLINE static uint8 Receive() 
    {
        while(!(UCSR0A & (1 << RXC0))) {} 
        return UDR0; 
    }
    
    FORCEINLINE static void Send(uint8 InData) 
    {
        while(!(UCSR0A & (1 << UDRE0))) {}
        UDR0 = InData;
    }
};
