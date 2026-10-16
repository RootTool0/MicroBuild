// MicroBuild

#pragma once

#include "Global.h"
#include "Structures.h"

#include "Advanced-AVR-Templates/Core/Aliases.h"
#include "Advanced-AVR-Templates/Memory/PROGMEM.h"

#include <string.h>

#if __IDE__
#define RGFW_IMPLEMENTATION
#include "Thirdparty/RGFW.h"

#include <cstdio>
#include <time.h>

struct FPixel
{
    
public:
    uint8 R = 0;
    uint8 G = 0;
    uint8 B = 0;
    uint8 _ = 0xFF;
    
    constexpr FPixel() = default;
    constexpr FPixel(const FColor InColor) : R(((InColor.Data >> 11) & 0x1F) * 255 / 31), G(((InColor.Data >> 5) & 0x3F) * 255 / 63), B((InColor.Data & 0x1F) * 255 / 31), _(0xFF) {}
};

struct FWindow
{
    
public:
    #define WINDOW_SCALE 9
    
    static inline uint8 Key = 0;
    static inline FPixel Screen[WINDOW_WIDTH * WINDOW_SCALE * WINDOW_HEIGHT * WINDOW_SCALE];
    
    static void Initialize()
    {
        if(Window = RGFW_createWindow("MicroBuild", 0, 0, WINDOW_WIDTH * WINDOW_SCALE, WINDOW_HEIGHT * WINDOW_SCALE, RGFW_windowCenter); !Window)
        {
            printf("RGFW Create Window Error!\n");
            Deinitialize();
            return;
        }
        
        if(Surface = RGFW_createSurface(reinterpret_cast<uint8*>(Screen), WINDOW_WIDTH * WINDOW_SCALE, WINDOW_HEIGHT * WINDOW_SCALE, RGFW_formatRGBA8); !Surface)
        {
            printf("RGFW Create Surface Error!\n");
            Deinitialize();
            return;
        }
    }
    
    static void Deinitialize()
    {
        if(Surface) RGFW_surface_free(Surface);
        if(Window) RGFW_window_close(Window);
    }
    
    static bool IsRunning()
    {
        RGFW_event Event;
        while(RGFW_window_checkEvent(Window, &Event))
            if(Event.type == RGFW_quit || (Event.type == RGFW_keyPressed && Event.key.value == RGFW_escape))
                return false;
        
        /*
        const uint8_t* KeyState = SDL_GetKeyboardState(nullptr);
        
        /*#1# if(KeyState[SDL_SCANCODE_Q]) GKey = 'q';
        else if(KeyState[SDL_SCANCODE_E]) GKey = 'e';
        
        else if(KeyState[SDL_SCANCODE_Z]) { GKey = 'z'; SDL_Delay(100); printf("CurrentSector: %i\n", FPlayer::CurrentSector + 1); }
        else if(KeyState[SDL_SCANCODE_X]) { GKey = 'x'; SDL_Delay(100); printf("CurrentSector: %i\n", FPlayer::CurrentSector - 1); }
        
        else if(KeyState[SDL_SCANCODE_R]) GKey = 'r';
        else if(KeyState[SDL_SCANCODE_F]) GKey = 'f';
        
        else if(KeyState[SDL_SCANCODE_W]) GKey = 'w';
        else if(KeyState[SDL_SCANCODE_S]) GKey = 's';
        else if(KeyState[SDL_SCANCODE_A]) GKey = 'a';
        else if(KeyState[SDL_SCANCODE_D]) GKey = 'd';
        else GKey = 0;
        */
        
        if(Event.type == RGFW_keyReleased) Key = 0;
        if(Event.type == RGFW_keyPressed) Key = Event.key.sym;
        
        return true;
    }
    
    static void Present()
    {
        RGFW_window_blitSurface(Window, Surface);
    }
    
    static void DrawPixel(const int32 InX, const int32 InY, const FPixel InColor)
    {
        if(InX < 0 || InX >= WINDOW_WIDTH || InY < 0 || InY >= WINDOW_HEIGHT) return;
        
        for(int32 i = 0; i < WINDOW_SCALE; ++i)
            for(int32 j = 0; j < WINDOW_SCALE; ++j) 
                Screen[((InY * WINDOW_SCALE) + i) * (WINDOW_WIDTH * WINDOW_SCALE) + (InX * WINDOW_SCALE) + j] = InColor;
    }

private:
    static inline RGFW_window* Window = nullptr;
    static inline RGFW_surface* Surface = nullptr;
};

class FWindowGuard
{

public:
    FWindowGuard() { FWindow::Initialize(); }
    ~FWindowGuard() { FWindow::Deinitialize(); }
};

struct FSerial
{
    template<uint16 InBaud>
    static void Initialize() {}
    static uint8 Receive() { return FWindow::Key; }
};

struct ST7735
{
    static void Initialize() {}
    static void SetWindow(const uint8 InX0, const uint8 InY0, const uint8 InX1, const uint8 InY1)
    {
        XS = InX0; XE = InX1;
        YS = InY0; YE = InY1;
        CurrentX = XS;
        CurrentY = YS;
    }
    
    static void BeginDraw() {}
    static void EndDraw() {}
    
    static void Draw(const uint16 InColor)
    {
        if(CurrentX < WINDOW_WIDTH && CurrentY < WINDOW_HEIGHT)
        {
            // FWindow::Screen[WINDOW_WIDTH * CurrentY + CurrentX] = RawColor;
            FWindow::DrawPixel(CurrentX, CurrentY, FPixel(InColor));
            // FWindow::Render();
            // SDL_Delay(2);
        }
        
        if(++CurrentX > XE)
        {
            CurrentX = XS;
            if(++CurrentY > YE) CurrentY = YS;
        }
    }
    
    static void DrawFast(const uint16 InColor) { Draw(InColor); }

private:
    static inline uint8 XS = 0, XE = WINDOW_WIDTH - 1;
    static inline uint8 YS = 0, YE = WINDOW_HEIGHT - 1;
    
    static inline uint8 CurrentX = 0;
    static inline uint8 CurrentY = 0;
};
#else
#include "Advanced-AVR-Templates/Misc/Delay.h"
#include "Advanced-AVR-Templates/GPIO/Pin.h"
#include "Advanced-AVR-Templates/Communication/Serial.h"
#include "Advanced-AVR-Templates/Memory/PROGMEM.h"

#if DEBUG_NO_DISPLAY
struct ST7735
{
    
public:
    static void Initialize() {}
    static void SetWindow(const uint8 InX0, const uint8 InY0, const uint8 InX1, const uint8 InY1) {}
    
    static void Synchronize() {}
    
    static void BeginDraw() {}
    static void Draw(const uint16 InColor) {}
    static void DrawFast(const uint16 InColor) {}
    static void EndDraw() {}
    
    static void FillScreen(const uint16 InColor) {}
};
#else
#include "Advanced-AVR-Templates/Display/ST7735.h"

using ST7735 = TST7735<EPin::D2, EPin::D3>;
#endif

#endif

struct FTimerFPS
{
    
public:
#if __IDE__
    inline static uint32_t StartTime = 0;
#endif
    
    static void Initialize()
    {
#if __IDE__
        // Nothing To Show
#else
        TCCR1A = 0; TCCR1B = (1 << CS11) | (1 << CS10);
#endif
    }
    
    static void Start()
    {
#if __IDE__
        StartTime = clock();
#else
        TCNT1 = 0;
#endif
    }
    
    static void End()
    {
#if __IDE__
        printf("FPS: %.2f\n", 1000.f / static_cast<float>(clock() - StartTime));
#else
        uint16 Timer = TCNT1;
        
        FSerial::Send(( Timer / 10000)      + '0'); 
        FSerial::Send(((Timer / 1000) % 10) + '0');
        FSerial::Send(((Timer / 100) % 10)  + '0');  
        FSerial::Send(((Timer / 10) % 10)   + '0');
        FSerial::Send(( Timer % 10)         + '0');
        FSerial::Send('\n');
        FSerial::Send('\r');
#endif
    }
};

inline FColor LoadFromFlash(const FColor& InColor) { FColor Color; FPROGMEM::Memcpy(&Color, &InColor, sizeof(FColor)); return Color; }
inline FWall LoadFromFlash(const FWall& InWall) { FWall Wall; FPROGMEM::Memcpy(&Wall, &InWall, sizeof(FWall)); return Wall; }
inline FSector LoadFromFlash(const FSector& InSector) { FSector Sector; FPROGMEM::Memcpy(&Sector, &InSector, sizeof(FSector)); return Sector; }
