// MicroBuild

#pragma once

#include "Advanced-AVR-Templates/Core/Aliases.h"
#include "Structures.h"

#include <string.h>

#if __IDE__
#include "Global.h"

#include <SDL.h>

struct FPixel
{
    
public:
    uint8 B = 0;
    uint8 G = 0;
    uint8 R = 0;
    uint8 _ = 0xFF;
    
    constexpr FPixel() = default;
    constexpr FPixel(const FColor InColor) : B((InColor.Data & 0x1F) * 255 / 31), G(((InColor.Data >> 5) & 0x3F) * 255 / 63), R(((InColor.Data >> 11) & 0x1F) * 255 / 31), _(0xFF) {}
};

static inline uint8 GKey = 0;
static inline FPixel GScreen[WINDOW_WIDTH * WINDOW_HEIGHT];

struct FSerial
{
    template<uint16 InBaud>
    static void Initialize() {}
    static uint8 Receive() { return GKey; }
};

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
        FColor RawColor;
        RawColor.Data = InColor;
        
        if(CurrentX < WINDOW_WIDTH && CurrentY < WINDOW_HEIGHT)
        {
            GScreen[WINDOW_WIDTH * CurrentY + CurrentX] = RawColor;
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

struct FWindow
{
    
public:
    #define WINDOW_SCALE 9
    
    static void Initialize()
    {
        if(SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            printf("SDL Initialization error: %s\n", SDL_GetError());
            return;
        }
        
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
        SDL_SetRelativeMouseMode(SDL_TRUE);
        
        WindowHandle = SDL_CreateWindow("Cool 3D", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH * WINDOW_SCALE, WINDOW_HEIGHT * WINDOW_SCALE, SDL_WINDOW_SHOWN);
        if(!WindowHandle)
        {
            printf("Window Initialization error: %s\n", SDL_GetError());
            SDL_Quit();
            return;
        }
        
        RendererHandle = SDL_CreateRenderer(WindowHandle, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if(!RendererHandle)
        {
            printf("Renderer Initialization error: %s\n", SDL_GetError());
            SDL_DestroyWindow(WindowHandle);
            SDL_Quit();
            return;
        }
        
        TextureHandle = SDL_CreateTexture(RendererHandle, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT);
        if(!TextureHandle)
        {
            printf("Texture Initialization error: %s\n", SDL_GetError());
            SDL_DestroyRenderer(RendererHandle);
            SDL_DestroyWindow(WindowHandle);
            SDL_Quit();
            return;
        }
    }

    static void Deinitialize()
    {
        if(TextureHandle) SDL_DestroyTexture(TextureHandle);
        if(RendererHandle) SDL_DestroyRenderer(RendererHandle);
        if(WindowHandle) SDL_DestroyWindow(WindowHandle);
        SDL_Quit();
    }
    
    static bool IsRunning()
    {
        SDL_Event Event;
        while(SDL_PollEvent(&Event))
            if(Event.type == SDL_QUIT || (Event.type == SDL_KEYDOWN && Event.key.keysym.sym == SDLK_ESCAPE))
                return false;
        
        const uint8_t* KeyState = SDL_GetKeyboardState(nullptr);
        
        /**/ if(KeyState[SDL_SCANCODE_Q]) GKey = 'q';
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
        
        return true;
    }
    
    static void Render()
    {
        SDL_RenderClear(RendererHandle);
        SDL_UpdateTexture(TextureHandle, nullptr, GScreen, WINDOW_WIDTH * 4);
        SDL_RenderCopy(RendererHandle, TextureHandle, nullptr, nullptr);
        SDL_RenderPresent(RendererHandle);
    }

private:
    static inline SDL_Window* WindowHandle = nullptr;
    static inline SDL_Renderer* RendererHandle = nullptr;
    static inline SDL_Texture* TextureHandle = nullptr;
};

class FWindowGuard
{

public:
    FWindowGuard() { FWindow::Initialize(); }
    ~FWindowGuard() { FWindow::Deinitialize(); }
};
#else
#include "Advanced-AVR-Templates/Misc/Delay.h"
#include "Advanced-AVR-Templates/GPIO/Pin.h"
#include "Advanced-AVR-Templates/Communication/Serial.h"
#include "Advanced-AVR-Templates/Memory/PROGMEM.h"

#if DEBUG_NO_ST7355
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
        StartTime = SDL_GetTicks();
#else
        TCNT1 = 0;
#endif
    }
    
    static void End()
    {
#if __IDE__
        printf("FPS: %.2f\n", 1000.f / static_cast<float>(SDL_GetTicks() - StartTime));
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
