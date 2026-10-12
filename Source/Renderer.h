// MicroBuild

#pragma once

#include "Platform.h"

#include "Global.h"
#include "Math.h"
#include "Structures.h"
#include "Map.h"

struct FRenderTask
{
    
public:
    int8 SectorIndex;
    int16 MinX;
    int16 MaxX;
    FFloatVector2D CameraLocation;
    FAngle CameraAngle;
};

struct FRenderTaskQueue
{
    
public:
    static inline FRenderTask Queue[RENDERING_TASK_QUEUE_ELEMENTS] = {};
    
    static void Empty() { Head = 0; Tail = 0; }
    
    static bool AddTask(const FRenderTask&& InRenderTask)
    {
        if(Head >= RENDERING_TASK_QUEUE_ELEMENTS) return false;
        Queue[Head++] = InRenderTask;
        return true;
    }
    
    static int8 PopTaskIndex()
    {
        return Tail >= Head || Tail >= RENDERING_TASK_QUEUE_ELEMENTS ? -1 : Tail++;
    }

private:
    static inline int8 Head = 0;
    static inline int8 Tail = 0;
};

struct FRenderer
{
    
public:
    static void Render()
    {
#if DEBUG_SERIAL_PRINT_TICKS
        FTimerFPS::Start();
#endif
        
        memset(ClipTop   , 0                , sizeof(ClipTop   ));
        memset(ClipBottom, WINDOW_HEIGHT - 1, sizeof(ClipBottom));
        
        NonEuclideanPortalRecursion = 0;
        FRenderTaskQueue::Empty();
        FRenderTaskQueue::AddTask(FRenderTask { FPlayer::CurrentSector, 0, WINDOW_WIDTH - 1, FPlayer::LocationXY, FPlayer::Angle });
        
        int8 TaskIndex;
        while((TaskIndex = FRenderTaskQueue::PopTaskIndex()) != -1)
        {
            RenderSector(TaskIndex);
        }
        
#if DEBUG_SERIAL_PRINT_TICKS
        FTimerFPS::End();
#endif
        
        FDisplay::Render();
        
#if __IDE__
        if(!FWindow::IsRunning()) exit(0);
        FWindow::Render();
        memset(GScreen, 80, sizeof(GScreen));
#endif
    }

private:
    static inline uint8 NonEuclideanPortalRecursion = 0;
    static inline uint8 ClipTop   [WINDOW_WIDTH];
    static inline uint8 ClipBottom[WINDOW_WIDTH];
    
    static void RenderSector(const int8 InTaskIndex)
    {
        FRenderTask& RenderTask = FRenderTaskQueue::Queue[InTaskIndex];
        
        if(RenderTask.MinX > RenderTask.MaxX) return;
        
        const FSector Sector = LoadFromFlash(GSectors[RenderTask.SectorIndex]);
        
        const FFixedFloat CosA = FMath::Cos(RenderTask.CameraAngle);
        const FFixedFloat SinA = FMath::Sin(RenderTask.CameraAngle);
        constexpr FFixedFloat FovDistance = FFixedFloat(80.f); // (WINDOW_WIDTH / 2.0f) / tanf((PLAYER_FOV * 0.5f) * (M_PI / 180.0f));
        
        for(int16 WallIndex = 0; WallIndex < Sector.WallCount; ++WallIndex)
        {
            const FWall Wall = LoadFromFlash(GWalls[Sector.WallStartIndex + WallIndex]);
            
            const FFloatVector2D D1 = FFloatVector2D(Wall.Point1) - RenderTask.CameraLocation;
            const FFloatVector2D D2 = FFloatVector2D(Wall.Point2) - RenderTask.CameraLocation;
            
            FFloatVector2D T1 = FFloatVector2D(D1.X * CosA + D1.Y * SinA, D1.Y * CosA - D1.X * SinA);
            FFloatVector2D T2 = FFloatVector2D(D2.X * CosA + D2.Y * SinA, D2.Y * CosA - D2.X * SinA);
            
            if(T1.Y <= FFixedFloat::Zero && T2.Y <= FFixedFloat::Zero) continue;
            
            constexpr FFixedFloat NearZ = FFixedFloat::ZeroPointFive;
            if(T1.Y <= NearZ) { if(T2.Y == T1.Y) continue; T1.X = T1.X + (T2.X - T1.X) * (NearZ - T1.Y) / (T2.Y - T1.Y); T1.Y = NearZ; }
            if(T2.Y <= NearZ) { if(T2.Y == T1.Y) continue; T2.X = T2.X + (T1.X - T2.X) * (NearZ - T2.Y) / (T1.Y - T2.Y); T2.Y = NearZ; }
            
            const int16 X1 = WINDOW_HALF_WIDTH + (T1.X * FovDistance / T1.Y).ToInt();
            const int16 X2 = WINDOW_HALF_WIDTH + (T2.X * FovDistance / T2.Y).ToInt();
            
            if(X1 >= X2) continue;
            
            const int16 StartX = FMath::Max(X1, RenderTask.MinX);
            const int16 EndX   = FMath::Min(X2, RenderTask.MaxX);
            
            if(StartX > EndX) continue;
            
            const int16 Ceil1  = WINDOW_HALF_HEIGHT - (FFixedFloat(Sector.CeilZ  - FPlayer::LocationZ) * FovDistance / T1.Y).ToInt();
            const int16 Floor1 = WINDOW_HALF_HEIGHT - (FFixedFloat(Sector.FloorZ - FPlayer::LocationZ) * FovDistance / T1.Y).ToInt();
            const int16 Ceil2  = WINDOW_HALF_HEIGHT - (FFixedFloat(Sector.CeilZ  - FPlayer::LocationZ) * FovDistance / T2.Y).ToInt();
            const int16 Floor2 = WINDOW_HALF_HEIGHT - (FFixedFloat(Sector.FloorZ - FPlayer::LocationZ) * FovDistance / T2.Y).ToInt();
            
            const bool bIsPortal = (Wall.PortalNextSector != -1);
            
            const FSector NextSector = bIsPortal ? LoadFromFlash(GSectors[Wall.PortalNextSector]) : FSector();;
            
            const int16 NextCeil1  = bIsPortal ? ((WINDOW_HEIGHT / 2) - (FFixedFloat(NextSector.CeilZ  - FPlayer::LocationZ) * (FovDistance / T1.Y)).ToInt()) : 0;
            const int16 NextFloor1 = bIsPortal ? ((WINDOW_HEIGHT / 2) - (FFixedFloat(NextSector.FloorZ - FPlayer::LocationZ) * (FovDistance / T1.Y)).ToInt()) : 0;
            const int16 NextCeil2  = bIsPortal ? ((WINDOW_HEIGHT / 2) - (FFixedFloat(NextSector.CeilZ  - FPlayer::LocationZ) * (FovDistance / T2.Y)).ToInt()) : 0;
            const int16 NextFloor2 = bIsPortal ? ((WINDOW_HEIGHT / 2) - (FFixedFloat(NextSector.FloorZ - FPlayer::LocationZ) * (FovDistance / T2.Y)).ToInt()) : 0;
            
            for(int16 X = StartX; X <= EndX; ++X)
            {
                const FFixedFloat Delta = FFixedFloat(X - X1) / FFixedFloat(X2 - X1);
                
                const int16 Ceil  = FMath::Lerp(Ceil1 , Ceil2 , Delta);
                const int16 Floor = FMath::Lerp(Floor1, Floor2, Delta);
                
                const int16 NextCeil  = bIsPortal ? FMath::Lerp(NextCeil1 , NextCeil2 , Delta) : Ceil ;
                const int16 NextFloor = bIsPortal ? FMath::Lerp(NextFloor1, NextFloor2, Delta) : Floor;
                
                const FColor WallColor = X == X1 || X == X2 ? FColor::Outline : Wall.WallPaletteIndex;
                
                if(!bIsPortal) // Wall
                {
                    // DrawWall(X, Ceil, Floor, WallColor, Sector.CeilPaletteIndex, Sector.FloorPaletteIndex);
                    const int16 TopY    = FMath::Max<int16>(ClipTop   [X], Ceil );
                    const int16 BottomY = FMath::Min<int16>(ClipBottom[X], Floor);
                    
                    FDisplay::SetWindow(X, ClipTop[X], X, ClipBottom[X]);
                    FDisplay::BeginDraw();
                    {
                        for(int16 Y = ClipTop[X]; Y <= TopY - 1; ++Y) FDisplay::Draw(X, Y, Sector.CeilPaletteIndex);
                        FDisplay::Draw(X, TopY, FColor::Outline);
                        for(int16 Y = TopY + 1; Y <= BottomY - 1; ++Y) FDisplay::Draw(X, Y, WallColor);
                        FDisplay::Draw(X, BottomY, FColor::Outline);
                        for(int16 Y = BottomY + 1; Y <= ClipBottom[X]; ++Y) FDisplay::Draw(X, Y, Sector.FloorPaletteIndex);
                    }
                    FDisplay::EndDraw();
                }
                else // Portal
                {
                    const int16 TopEnd = (Sector.CeilZ > NextSector.CeilZ ? NextCeil : Ceil) - 1;
                    FDisplay::SetWindow(X, ClipTop[X], X, TopEnd);
                    FDisplay::BeginDraw();
                    {
                        for(int16 Y = ClipTop[X]; Y <= TopEnd; ++Y)
                        {
                            if(Y <= Ceil) FDisplay::Draw(X, Y, Sector.CeilPaletteIndex);
                            else FDisplay::Draw(X, Y, ((Y == Ceil + 1 || Y == TopEnd) && Sector.FloorZ != NextSector.FloorZ) ? FColor::Outline : WallColor);
                        }
                    }
                    FDisplay::EndDraw();
                    
                    const int16 BottomStart = (Sector.FloorZ < NextSector.FloorZ ? NextFloor : Floor) + 1;
                    FDisplay::SetWindow(X, BottomStart, X, ClipBottom[X]);
                    FDisplay::BeginDraw();
                    {
                        for(int16 Y = BottomStart; Y <= ClipBottom[X]; ++Y)
                        {
                            if(Y >= Floor) FDisplay::Draw(X, Y, Sector.FloorPaletteIndex);
                            else FDisplay::Draw(X, Y, ((Y == BottomStart || Y == Floor - 1) && Sector.FloorZ != NextSector.FloorZ) ? FColor::Outline : WallColor);
                        }
                    }
                    FDisplay::EndDraw();
                }
                
                ClipTop   [X] = FMath::Clamp<int16>(FMath::Max<int16>(Ceil , NextCeil ), ClipTop[X], ClipBottom[X]);
                ClipBottom[X] = FMath::Clamp<int16>(FMath::Min<int16>(Floor, NextFloor), ClipTop[X], ClipBottom[X]);
            }
            
            if(Wall.PortalNextWall == -1) // Just-Euclidean Portal
            {
                FRenderTaskQueue::AddTask(FRenderTask{ Wall.PortalNextSector, StartX, EndX, RenderTask.CameraLocation, RenderTask.CameraAngle });
            }
            else if(NonEuclideanPortalRecursion++ <= RENDERING_NON_EUCLIDEAN_PORTAL_MAX_RECURSION)  // Non-Euclidean Portal
            {
                const FWall TargetWall = LoadFromFlash(GWalls[Wall.PortalNextWall]);
                const FAngle VirtualAngle = RenderTask.CameraAngle + Wall.PortalAngleDelta;
                
                const FFloatVector2D PortalCenter = FFloatVector2D(FFloatVector2D(      Wall.Point1) + FFloatVector2D(      Wall.Point2)).DivByTwo();
                const FFloatVector2D TargetCenter = FFloatVector2D(FFloatVector2D(TargetWall.Point1) + FFloatVector2D(TargetWall.Point2)).DivByTwo();
                
                const FFloatVector2D LocalLocation = RenderTask.CameraLocation - PortalCenter;
                
                const FFixedFloat CosAngle = FMath::Cos(Wall.PortalAngleDelta);
                const FFixedFloat SinAngle = FMath::Sin(Wall.PortalAngleDelta);
                
                const FFloatVector2D RotatedLocation = FFloatVector2D(LocalLocation.X * CosAngle - LocalLocation.Y * SinAngle, LocalLocation.Y * CosAngle + LocalLocation.X * SinAngle); 
                const FFloatVector2D VirtualLocation = TargetCenter + RotatedLocation;
                
                FRenderTaskQueue::AddTask(FRenderTask{ Wall.PortalNextSector, StartX, EndX, VirtualLocation, VirtualAngle });
            }
            
            /*
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
            */
        }
    }
    
    static void DrawNoTexture(const uint8 InX, const FColor InColor = FColor(0b10101010))
    {
        FDisplay::SetWindow(InX, ClipTop[InX], InX, ClipBottom[InX]);
        FDisplay::BeginDraw();
        {
            bool bBlack = (InX & 1) == 0;
            for(int16 Y = ClipTop[InX]; Y <= ClipBottom[InX]; ++Y) { FDisplay::Draw(InX, Y, bBlack ? FColor::Outline : InColor); bBlack = !bBlack; }
        }
        FDisplay::EndDraw();
    }
};
