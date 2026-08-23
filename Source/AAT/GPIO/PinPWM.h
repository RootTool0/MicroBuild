#pragma once

#include "../Core/Aliases.h"
#include "Pin.h"

template<uint8 InPin, bool bInverted>
struct TPinPWM
{
    static_assert(
        InPin == 3 || InPin == 5 || InPin == 6 || InPin == 9 || InPin == 10 || InPin == 11,
        "TPinPWM Error: Pin does not support Hardware PWM! (Available: 3, 5, 6, 9, 10, 11)");
    
public:
    FORCEINLINE static void Initialize()
    {
        Pin::SetupAsOutput();
        
        // Timer 0 (8-bit): Pins 5, 6
        if constexpr (InPin == 5 || InPin == 6)
        {
            TCCR0A |= (1 << WGM00); // Phase Correct PWM
            TCCR0B |= (1 << CS00);  // No prescaling (Fastest)
            
            if constexpr (InPin == 6) TCCR0A |= bInverted ? ((1 << COM0A1) | (1 << COM0A0)) : (1 << COM0A1); // OC0A
            else /*                */ TCCR0A |= bInverted ? ((1 << COM0B1) | (1 << COM0B0)) : (1 << COM0B1); // OC0B (Pin 5)
        }
        
        // Timer 1 (16-bit): Pins 9, 10
        else if constexpr (InPin == 9 || InPin == 10)
        {
            TCCR1A |= (1 << WGM10); // PWM, Phase Correct, 8-bit
            TCCR1B |= (1 << CS10);  // No prescaling
            
            if constexpr (InPin == 9) TCCR1A |= bInverted ? ((1 << COM1A1) | (1 << COM1A0)) : (1 << COM1A1); // OC1A
            else /*                */ TCCR1A |= bInverted ? ((1 << COM1B1) | (1 << COM1B0)) : (1 << COM1B1); // OC1B (Pin 10)
        }
        
        // Timer 2 (8-bit): Pins 3, 11
        else if constexpr (InPin == 3 || InPin == 11)
        {
            TCCR2A |= (1 << WGM20); // Phase Correct PWM
            TCCR2B |= (1 << CS20);  // No prescaling
            
            if constexpr (InPin == 11) TCCR2A |= bInverted ? ((1 << COM2A1) | (1 << COM2A0)) : (1 << COM2A1); // OC2A
            else /*                 */ TCCR2A |= bInverted ? ((1 << COM2B1) | (1 << COM2B0)) : (1 << COM2B1); // OC2B (Pin 3)
        }
        
        SetDuty(0);
    }
    
    FORCEINLINE static void SetDuty(uint8 InDuty) { *GetOCR() = InDuty; }
    
private:
    using Pin = TPin<InPin>;
    
    FORCEINLINE static constexpr volatile uint8* GetOCR()
    {
        if constexpr (InPin == 6)  return &OCR0A;
        if constexpr (InPin == 5)  return &OCR0B;
        if constexpr (InPin == 9)  return &OCR1AL;
        if constexpr (InPin == 10) return &OCR1BL;
        if constexpr (InPin == 11) return &OCR2A;
        if constexpr (InPin == 3)  return &OCR2B;
        return nullptr;
    }
};
