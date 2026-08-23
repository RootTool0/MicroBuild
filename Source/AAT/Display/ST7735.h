#pragma once

#include "../Delay.h"
#include "../Communication/SPI.h"
#include "../Core/Aliases.h"
#include "../GPIO/Pin.h"

template<EPin InCS, EPin InDC>
struct TST7735
{

public:
    using CS = TPin<InCS>;
    using DC = TPin<InDC>;
    
    static constexpr uint8 Width  = 160;
    static constexpr uint8 Height = 128;
    
    static void Initialize()
    {
        FSPI::Initialize();
        
        CS::InitializeAsOutput();
        DC::InitializeAsOutput();
        CS::High();
        
        FDelay::Milliseconds<120>(); WriteCMD(0x01);                  // Software Reset
        FDelay::Milliseconds<120>(); WriteCMD(0x11);                  // Sleep Out
        FDelay::Milliseconds<120>(); WriteCMD(0x3A); WriteData(0x05); // Color Mode = 16-bit
        // FDelay::Milliseconds<120>(); WriteCMD(0x36); WriteData(0xC8); // MADCTL = BLACKTAB
        FDelay::Milliseconds<120>(); WriteCMD(0x36); WriteData(0xA0); // MADCTL = BLACKTAB
        FDelay::Milliseconds<120>(); WriteCMD(0x29);                  // Display On
        FDelay::Milliseconds<20>();
    }
    
    static void SetWindow(const uint8 InX0, const uint8 InY0, const uint8 InX1, const uint8 InY1)
    {
        WriteCMD(0x2A); WriteData(0x00); WriteData(InX0); WriteData(0x00); WriteData(InX1);
        WriteCMD(0x2B); WriteData(0x00); WriteData(InY0); WriteData(0x00); WriteData(InY1);
    }
    
    static void Synchronize() { FSPI::Synchronize(); }
    
    static void BeginDraw() { WriteCMD(0x2C); DC::High(); CS::Low(); }
    static void Draw(const uint16 InColor) { FSPI::SendAndSynchronize(InColor >> 8); FSPI::SendAndSynchronize(InColor & 0xFF); }
    static void DrawFast(const uint16 InColor) { FSPI::Synchronize(); FSPI::Send(InColor >> 8); FSPI::Synchronize(); FSPI::Send(InColor & 0xFF); }
    static void EndDraw() { FSPI::Synchronize(); CS::High(); }
    
    static void FillScreen(const uint16 InColor)
    {
        SetWindow(0, 0, Width - 1, Height - 1);
        BeginDraw();
        for(uint16 i = 0; i < Width * Height; ++i) Draw(InColor);
        EndDraw();
    }
    
    static void DrawPoint(const uint8 InX, const uint8 InY, const uint16 InColor)
    {
        SetWindow(InX, InY, InX, InY);
        BeginDraw();
        Draw(InColor);
        EndDraw();
    }
    
    static void DrawVLine(const uint8 InX, const uint8 InY0, const uint8 InY1, const uint16 InColor)
    {
        SetWindow(InX, InY0, InX, InY1);
        BeginDraw();
        for(uint8 _ = InY0; _ <= InY1; ++_) Draw(InColor);
        EndDraw();
    }

private:
    static void WriteCMD(const uint8 InCMD)
    {
        DC::Low();
        CS::Low();
        FSPI::SendAndSynchronize(InCMD);
        CS::High();
    }
    
    static void WriteData(const uint8 InData)
    {
        DC::High();
        CS::Low();
        FSPI::SendAndSynchronize(InData);
        CS::High();
    }
};
