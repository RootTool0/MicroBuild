// MicroBuild

#pragma once

#include "AAT/Core/Aliases.h"

#include "Global.h"
#include "Math.h"
#include "RenderTask.h"
#include "Structures.h"

#if __IDE__
#include "QuickFixOrEmulatorForPC.h"
#else
#include "AAT/Delay.h"
#include "AAT/Display/ST7735.h"
#include "AAT/GPIO/Pin.h"
#include "AAT/Communication/Serial.h"

#if DEBUG_NO_ST7355
struct TEmptyST7735
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

using ST7735 = TEmptyST7735;
#else
using ST7735 = TST7735<EPin::D6, EPin::D2>;
#endif

#endif

    
#if !__IDE__ && DEBUG_SERIAL_PRINT_TICKS
struct FTimerFPS
{
    static void Initialize() { TCCR1A = 0; TCCR1B = (1 << CS11) | (1 << CS10); }
    static void Start() { TCNT1 = 0;  }
    static void End()
    {
        uint16 Timer = TCNT1;
            
        FSerial::Send(( Timer / 10000)      + '0'); 
        FSerial::Send(((Timer / 1000) % 10) + '0');
        FSerial::Send(((Timer / 100) % 10)  + '0');  
        FSerial::Send(((Timer / 10) % 10)   + '0');
        FSerial::Send(( Timer % 10)         + '0');
        FSerial::Send('\n');
        FSerial::Send('\r');
    }
};
#endif

struct FRenderer
{

public:
    static void Render()
    {
#if __IDE__
        if(!FWindow::IsRunning()) exit(0);
        FWindow::Render();
        memset(GScreen, 80, sizeof(GScreen));
#endif
        
#if !__IDE__ && DEBUG_SERIAL_PRINT_TICKS
        FTimerFPS::Start();
#endif
        
        memset(ClipTop   , 0                , sizeof(ClipTop   ));
        memset(ClipBottom, WINDOW_HEIGHT - 1, sizeof(ClipBottom));
        
        NonEuclideanPortalRecursion = 0;
        FRenderTaskQueue::Empty();
        FRenderTaskQueue::AddTask(FRenderTask { FPlayer::CurrentSector, 0, WINDOW_WIDTH - 1, FPlayer::LocationXY, FPlayer::Angle });
        
        while(true)
        {
            const int8 TaskIndex = FRenderTaskQueue::PopTaskIndex();
            if(TaskIndex == -1) break;
            RenderSector(
                FRenderTaskQueue::Queue[TaskIndex].InSectorIndex,
                FRenderTaskQueue::Queue[TaskIndex].InMinX,
                FRenderTaskQueue::Queue[TaskIndex].InMaxX,
                FRenderTaskQueue::Queue[TaskIndex].InCameraLocation,
                FRenderTaskQueue::Queue[TaskIndex].InCameraAngle
            );
            
            // RenderSector(FPlayer::CurrentSector, 0, WINDOW_WIDTH - 1, FPlayer::LocationXY, FPlayer::Angle);
        }
        
#if !__IDE__ && DEBUG_SERIAL_PRINT_TICKS
        FTimerFPS::End();
#endif
    }

private:
    static inline uint8 NonEuclideanPortalRecursion = 0;
    static inline uint8 ClipTop   [WINDOW_WIDTH];
    static inline uint8 ClipBottom[WINDOW_WIDTH];
    
    static inline FColor GPalette[] =
    {
        FColor(255, 0  , 0  ), //  0 - Красный
        FColor(255, 128, 0  ), //  1 - Оранжевый
        FColor(255, 255, 0  ), //  2 - Желтый
    
        FColor(0  , 255, 0  ), //  3 - Зеленый
        FColor(0  , 255, 128), //  4 - Салатовый / Мятный
        FColor(0  , 255, 255), //  5 - Голубой
    
        FColor(0  , 128, 255), //  6 - Лазурный
        FColor(0  , 0  , 255), //  7 - Синий
        FColor(128, 0  , 255), //  8 - Фиолетовый
    
        FColor(255, 0  , 255), //  9 - Розовый
        FColor(255, 0  , 128), // 10 - Малиновый
        FColor(128, 64 , 0  ), // 11 - Коричневый
    
        FColor(255, 255, 255), // 12 - Белый
        FColor(128, 128, 128), // 13 - Серый
        FColor(64 , 64 , 64 ), // 14 - Темно-серый
        FColor(0  , 0  , 0  ), // 15 - Черный
    };
    
    static void DrawNoTexture(const uint8 InX, const FColor InColor = FColor(255, 0, 255))
    {
        ST7735::SetWindow(InX, ClipTop[InX], InX, ClipBottom[InX]);
        ST7735::BeginDraw();
        {
            bool bBlack = (InX & 1) == 0;
            for(int16 _ = ClipTop[InX]; _ <= ClipBottom[InX]; ++_) { ST7735::DrawFast(bBlack ? FColor(0, 0, 0).Data : InColor.Data); bBlack = !bBlack; }
        }
        ST7735::EndDraw();
    }
    
    static void DrawWall(const uint8 InX, int16 InTopY, int16 InBottomY, const FColor InColor)
    {
        InTopY    = FMath::Max<int16>(ClipTop   [InX], InTopY   );
        InBottomY = FMath::Min<int16>(ClipBottom[InX], InBottomY);
    
        ST7735::SetWindow(InX, ClipTop[InX], InX, ClipBottom[InX]);
        ST7735::BeginDraw();
        {
            for(int16 _ = ClipTop[InX]; _ <= InTopY - 1; ++_) ST7735::DrawFast(FColor(255, 0, 0).Data);
            ST7735::DrawFast(FColor(0, 0, 0).Data);
            for(int16 _ = InTopY + 1; _ <= InBottomY - 1; ++_) ST7735::DrawFast(InColor.Data);
            ST7735::DrawFast(FColor(0, 0, 0).Data);
            for(int16 _ = InBottomY + 1; _ <= ClipBottom[InX]; ++_) ST7735::DrawFast(FColor(0, 0, 255).Data);
        }
        ST7735::EndDraw();
    }
    
    static void RenderSector(const uint16 InSectorIndex, const int16 InMinX, const int16 InMaxX, const FVector2D InCameraLocation, const FAngle InCameraAngle)
    {
        if(InMinX > InMaxX) return;
        
        const FSector Sector = FPROGMEM::ReadObjectS<FSector>(&GSectors[InSectorIndex]);
        
        const FFixedFloat CosA = FMath::Cos(InCameraAngle);
        const FFixedFloat SinA = FMath::Sin(InCameraAngle);
        constexpr FFixedFloat FovDistance = FFixedFloat(80.f); // (WINDOW_WIDTH / 2.0f) / tanf((PLAYER_FOV * 0.5f) * (M_PI / 180.0f));
        
        for(int16 WallIndex = 0; WallIndex < Sector.WallCount; ++WallIndex)
        {
            const FWall Wall = FPROGMEM::ReadObjectW<FWall>(&GWalls[Sector.WallStartIndex + WallIndex]);
            
            const FVector2D D1 = Wall.Point1 - InCameraLocation;
            const FVector2D D2 = Wall.Point2 - InCameraLocation;
            
            FVector2D T1 = FVector2D(D1.X * CosA + D1.Y * SinA, D1.Y * CosA - D1.X * SinA);
            FVector2D T2 = FVector2D(D2.X * CosA + D2.Y * SinA, D2.Y * CosA - D2.X * SinA);
            
            if(T1.Y <= FFixedFloat::Zero && T2.Y <= FFixedFloat::Zero) continue;
            
            constexpr FFixedFloat NearZ = FFixedFloat::ZeroPointFive;
            if(T1.Y <= NearZ) { if(T2.Y == T1.Y) continue; T1.X = T1.X + (T2.X - T1.X) * (NearZ - T1.Y) / (T2.Y - T1.Y); T1.Y = NearZ; }
            if(T2.Y <= NearZ) { if(T2.Y == T1.Y) continue; T2.X = T2.X + (T1.X - T2.X) * (NearZ - T2.Y) / (T1.Y - T2.Y); T2.Y = NearZ; }
            
            const int16 X1 = WINDOW_HALF_WIDTH + (T1.X * FovDistance / T1.Y).ToInt();
            const int16 X2 = WINDOW_HALF_WIDTH + (T2.X * FovDistance / T2.Y).ToInt();
            
            if(X1 >= X2) continue;
            
            const int16 StartX = FMath::Max(X1, InMinX);
            const int16 EndX   = FMath::Min(X2, InMaxX);
            
            if(StartX > EndX) continue;
            
            const int16 Ceil1  = WINDOW_HALF_HEIGHT - (FFixedFloat(Sector.CeilZ  - FPlayer::LocationZ) * FovDistance / T1.Y).ToInt();
            const int16 Floor1 = WINDOW_HALF_HEIGHT - (FFixedFloat(Sector.FloorZ - FPlayer::LocationZ) * FovDistance / T1.Y).ToInt();
            const int16 Ceil2  = WINDOW_HALF_HEIGHT - (FFixedFloat(Sector.CeilZ  - FPlayer::LocationZ) * FovDistance / T2.Y).ToInt();
            const int16 Floor2 = WINDOW_HALF_HEIGHT - (FFixedFloat(Sector.FloorZ - FPlayer::LocationZ) * FovDistance / T2.Y).ToInt();
            
            if(Wall.PortalNextSector == -1) // Wall
            {
                for(int16 X = StartX; X <= EndX; ++X)
                {
                    const FFixedFloat Delta = FFixedFloat(X - X1) / FFixedFloat(X2 - X1);
                    
                    const int16 Ceil  = FMath::Lerp(Ceil1 , Ceil2 , Delta);
                    const int16 Floor = FMath::Lerp(Floor1, Floor2, Delta);
                    
                    const FColor WallColor = X == X1 || X == X2 ? FColor(0, 0, 0) : GPalette[Wall.WallPaletteIndex];
                    DrawWall(X, Ceil, Floor, WallColor);
                    
                    ClipTop   [X] = FMath::Clamp<int16>(Ceil , ClipTop[X], ClipBottom[X]);
                    ClipBottom[X] = FMath::Clamp<int16>(Floor, ClipTop[X], ClipBottom[X]);
                }
            }
            else // Portal
            {
                // const FSector& NextSector = GSectors[Wall.PortalNextSector];
                const FSector NextSector = FPROGMEM::ReadObjectS<FSector>(&GSectors[Wall.PortalNextSector]);
                
                const int16 NextCeil1  = ((WINDOW_HEIGHT / 2) - (FFixedFloat(NextSector.CeilZ  - FPlayer::LocationZ) * (FovDistance / T1.Y)).ToInt());
                const int16 NextFloor1 = ((WINDOW_HEIGHT / 2) - (FFixedFloat(NextSector.FloorZ - FPlayer::LocationZ) * (FovDistance / T1.Y)).ToInt());
                const int16 NextCeil2  = ((WINDOW_HEIGHT / 2) - (FFixedFloat(NextSector.CeilZ  - FPlayer::LocationZ) * (FovDistance / T2.Y)).ToInt());
                const int16 NextFloor2 = ((WINDOW_HEIGHT / 2) - (FFixedFloat(NextSector.FloorZ - FPlayer::LocationZ) * (FovDistance / T2.Y)).ToInt());
                
                for(int16 X = StartX; X <= EndX; X++)
                {
                    const FFixedFloat Delta = FFixedFloat(X - X1) / FFixedFloat(X2 - X1);
                    
                    const int16 Ceil  = FMath::Lerp(Ceil1 , Ceil2 , Delta);
                    const int16 Floor = FMath::Lerp(Floor1, Floor2, Delta);
                    
                    const int16 NextCeil  = FMath::Lerp(NextCeil1 , NextCeil2 , Delta);
                    const int16 NextFloor = FMath::Lerp(NextFloor1, NextFloor2, Delta);
                    
                    // DrawPortal(X, Ceil, Floor, NextCeil, NextFloor, Sector.CeilZ, Sector.FloorZ, NextSector.CeilZ, NextSector.FloorZ, WallColor);
                    {
                        const FColor WallColor = X == X1 || X == X2 ? FColor(0, 0, 0) : GPalette[Wall.WallPaletteIndex];
                        const int16 TopEnd = (Sector.CeilZ > NextSector.CeilZ ? NextCeil : Ceil) - 1;
                        ST7735::SetWindow(X, ClipTop[X], X, TopEnd);
                        ST7735::BeginDraw();
                        {
                            for(int16 Y = ClipTop[X]; Y <= TopEnd; ++Y)
                            {
                                if(Y <= Ceil) ST7735::DrawFast(FColor::Ceil.Data);
                                else
                                {
                                    const bool bBlack = (Y == Ceil + 1 || Y == TopEnd) && Sector.FloorZ != NextSector.FloorZ;
                                    ST7735::DrawFast(bBlack ? FColor::Black.Data : WallColor.Data);
                                }
                            }
                        }
                        ST7735::EndDraw();
                        
                        const int16 BottomStart = (Sector.FloorZ < NextSector.FloorZ ? NextFloor : Floor) + 1;
                        ST7735::SetWindow(X, BottomStart, X, ClipBottom[X]);
                        ST7735::BeginDraw();
                        {
                            for(int16 Y = BottomStart; Y <= ClipBottom[X]; ++Y)
                            {
                                if(Y >= Floor) ST7735::DrawFast(FColor::Floor.Data);
                                else
                                {
                                    const bool bBlack = (Y == BottomStart  || Y == Floor - 1) && Sector.FloorZ != NextSector.FloorZ;
                                    ST7735::DrawFast(bBlack ? FColor::Black.Data : WallColor.Data);
                                }
                            }
                        }
                        ST7735::EndDraw();
                    }
                    
                    ClipTop   [X] = FMath::Clamp<int16>(FMath::Max<int16>(Ceil , NextCeil ), ClipTop[X], ClipBottom[X]);
                    ClipBottom[X] = FMath::Clamp<int16>(FMath::Min<int16>(Floor, NextFloor), ClipTop[X], ClipBottom[X]);
                }
                
                if(Wall.PortalNextWall == -1) // Just-Euclidean Portal
                {
                    if(FRenderTaskQueue::AddTask({ Wall.PortalNextSector, StartX, EndX, InCameraLocation, InCameraAngle }))
                        continue;
                }
                else if(NonEuclideanPortalRecursion++ <= NON_EUCLIDEAN_PORTAL_MAX_RECURSION)  // Non-Euclidean Portal
                {
                    const FWall TargetWall = FPROGMEM::ReadObjectW<FWall>(&GWalls[Wall.PortalNextWall]);
                    const FAngle VirtualAngle = InCameraAngle + Wall.PortalAngleDelta;
                    
                    const FVector2D PortalCenter = (Wall.Point1 + Wall.Point2).DivByTwo();
                    const FVector2D TargetCenter = (TargetWall.Point1 + TargetWall.Point2).DivByTwo();
                    
                    const FVector2D LocalLocation = InCameraLocation - PortalCenter;
                    
                    const FFixedFloat CosAngle = FMath::Cos(Wall.PortalAngleDelta);
                    const FFixedFloat SinAngle = FMath::Sin(Wall.PortalAngleDelta);
                    
                    const FVector2D RotatedLocation = FVector2D(LocalLocation.X * CosAngle - LocalLocation.Y * SinAngle, LocalLocation.Y * CosAngle + LocalLocation.X * SinAngle); 
                    const FVector2D VirtualLocation = TargetCenter + RotatedLocation;
                    
                    FRenderTaskQueue::AddTask({ Wall.PortalNextSector, StartX, EndX, VirtualLocation, VirtualAngle });
                    continue;
                }
                
                // Portal failback
                for(int16 X = StartX; X <= EndX; ++X)
                {
                    const FFixedFloat Delta = FFixedFloat(X - X1) / FFixedFloat(X2 - X1);
                        
                    const int16 Ceil  = FMath::Lerp(Ceil1 , Ceil2 , Delta);
                    const int16 Floor = FMath::Lerp(Floor1, Floor2, Delta);
                        
                    ClipTop   [X] = FMath::Clamp<int16>(Ceil , ClipTop[X], ClipBottom[X]);
                    ClipBottom[X] = FMath::Clamp<int16>(Floor, ClipTop[X], ClipBottom[X]);
                    DrawNoTexture(X);
                }
            }
        }
    }
};
