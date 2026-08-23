// MicroBuild

#pragma once

#include "AAT/Core/Aliases.h"
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

struct FWindow
{
    
public:
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
        
#if __IDE__ && DEBUG_SELECT_SECTORS
        else if(KeyState[SDL_SCANCODE_Z]) { GKey = 'z'; SDL_Delay(100); printf("CurrentSector: %i\n", FPlayer::CurrentSector + 1); }
        else if(KeyState[SDL_SCANCODE_X]) { GKey = 'x'; SDL_Delay(100); printf("CurrentSector: %i\n", FPlayer::CurrentSector - 1); }
#endif
        
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

struct ST7735
{
    static void Initialize() {}
    static void SetWindow(const uint8 InX0, const uint8 InY0, const uint8 InX1, const uint8 InY1)
    {
        XS = InX0; XE = InX1;
        YS = InY0; YE = InY1;
        CurrentX = XS;
        CurrentY = YS;
        IsHighByte = true;
    }
    
    static void BeginDraw() {}
    static void EndDraw() {}
    
    /*
    static void Draw(const uint16 InByte)
    { 
        if (IsHighByte)
        {
            HighByteBuf = InByte;
            IsHighByte = false;
        }
        else
        {
            FColor RawColor;
            RawColor.Data = (HighByteBuf << 8) | InByte;
            
            if (CurrentX < WINDOW_WIDTH && CurrentY < WINDOW_HEIGHT)
            {
                GScreen[WINDOW_WIDTH * CurrentY + CurrentX] = RawColor;
            }
            
            CurrentX++;
            if (CurrentX > XE)
            {
                CurrentX = XS;
                CurrentY++;
                if (CurrentY > YE)
                {
                    CurrentY = YS;
                }
            }
            IsHighByte = true;
        }
    }
    */
    
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
        
        CurrentX++;
        if (CurrentX > XE)
        {
            CurrentX = XS;
            CurrentY++;
            if (CurrentY > YE)
            {
                CurrentY = YS;
            }
        }
        IsHighByte = true;
    }
    
    static void DrawFast(const uint16 InColor) { Draw(InColor); }

private:
    static inline uint8 XS = 0, XE = WINDOW_WIDTH - 1;
    static inline uint8 YS = 0, YE = WINDOW_HEIGHT - 1;
    
    static inline uint8 CurrentX = 0;
    static inline uint8 CurrentY = 0;
    
    static inline bool IsHighByte = true;
    static inline uint8 HighByteBuf = 0;
};

class FWindowGuard
{

public:
    FWindowGuard() { FWindow::Initialize(); }
    ~FWindowGuard() { FWindow::Deinitialize(); }
};
