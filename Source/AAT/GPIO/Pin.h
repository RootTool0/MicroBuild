#pragma once

#include "../Core/Aliases.h"

#include <avr/io.h>

enum class EPin : uint8
{
    D0  = 0,
    D1  = 1,
    D2  = 2,
    D3  = 3,
    D4  = 4,
    D5  = 5,
    D6  = 6,
    D7  = 7,
    D8  = 8,
    D9  = 9,
    D10 = 10,
    D11 = 11,
    D12 = 12,
    D13 = 13,
    A0  = 14,
    A1  = 15,
    A2  = 16,
    A3  = 17,
    A4  = 18,
    A5  = 19,
    
    MAX
};

template<EPin InPin>
struct TPin
{
    static_assert(static_cast<uint8>(InPin) <= static_cast<uint8>(EPin::MAX), "TPin Error: Pin index out of range [0-19]");
    
public:
    FORCEINLINE static void InitializeAsOutput() { *GetDDR() |=  GetBitMask(); }
    FORCEINLINE static void InitializeAsInput()  { *GetDDR() &= ~GetBitMask(); }
    
    FORCEINLINE static void EnablePullUp() { High(); }
    
    FORCEINLINE static void High()   { *GetPORT() |=  GetBitMask(); }
    FORCEINLINE static void Low()    { *GetPORT() &= ~GetBitMask(); }
    FORCEINLINE static void Toggle() { *GetPORT() ^=  GetBitMask(); }
    
    FORCEINLINE static void Set(bool bState) { bState ? High() : Low(); }
    FORCEINLINE static bool Get() { return (*GetPIN() & GetBitMask()) != 0; }
    
private:
    static constexpr bool bIsD = (static_cast<uint8>(InPin) <=  7);
    static constexpr bool bIsB = (static_cast<uint8>(InPin) >=  8 && static_cast<uint8>(InPin) <= 13);
    static constexpr bool bIsC = (static_cast<uint8>(InPin) >= 14 && static_cast<uint8>(InPin) <= 19);
    
    FORCEINLINE static constexpr volatile uint8* GetDDR()
    {
        if constexpr (bIsD) return &DDRD;
        if constexpr (bIsB) return &DDRB;
        if constexpr (bIsC) return &DDRC;
        return nullptr;
    }
    
    FORCEINLINE static constexpr volatile uint8* GetPORT()
    {
        if constexpr (bIsD) return &PORTD;
        if constexpr (bIsB) return &PORTB;
        if constexpr (bIsC) return &PORTC;
        return nullptr;
    }
    
    FORCEINLINE static constexpr volatile uint8* GetPIN()
    {
        if constexpr (bIsD) return &PIND;
        if constexpr (bIsB) return &PINB;
        if constexpr (bIsC) return &PINC;
        return nullptr;
    }

    FORCEINLINE static constexpr uint8 GetBitMask()
    {
        if constexpr (bIsD) return (1 << (static_cast<uint8>(InPin)));
        if constexpr (bIsB) return (1 << (static_cast<uint8>(InPin) - 8 ));
        if constexpr (bIsC) return (1 << (static_cast<uint8>(InPin) - 14));
        return 0;
    }
};
