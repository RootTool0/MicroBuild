// MicroBuild

#include "Platform.h"

#include "Global.h"
#include "Math.h"
#include "Structures.h"
#include "Map.h"
#include "Renderer.h"

FORCEINLINE FIntVector2D FWall::GetPoint1           (const FWall& InFlashWall) { return FIntVector2D(FPROGMEM::ReadInt8(&InFlashWall.Point1.X), FPROGMEM::ReadInt8(&InFlashWall.Point1.Y)); }
FORCEINLINE FIntVector2D FWall::GetPoint2           (const FWall& InFlashWall) { return FIntVector2D(FPROGMEM::ReadInt8(&InFlashWall.Point2.X), FPROGMEM::ReadInt8(&InFlashWall.Point2.Y)); }
FORCEINLINE uint8        FWall::GetWallPaletteIndex (const FWall& InFlashWall) { return FPROGMEM::ReadInt8(&InFlashWall.WallPaletteIndex); }
FORCEINLINE int8         FWall::GetPortalNextSector (const FWall& InFlashWall) { return FPROGMEM::ReadInt8(&InFlashWall.PortalNextSector); }
FORCEINLINE int8         FWall::GetPortalNextWall   (const FWall& InFlashWall) { return FPROGMEM::ReadInt8(&InFlashWall.PortalNextWall  ); }
FORCEINLINE FAngle       FWall::GetPortalAngleDelta (const FWall& InFlashWall) { return FPROGMEM::ReadInt8(&InFlashWall.PortalAngleDelta); }

FORCEINLINE int8  FSector::GetFloorZ            (const FSector& InFlashSector) { return FPROGMEM::ReadInt8(&InFlashSector.FloorZ           ); }
FORCEINLINE int8  FSector::GetCeilZ             (const FSector& InFlashSector) { return FPROGMEM::ReadInt8(&InFlashSector.CeilZ            ); }
FORCEINLINE uint8 FSector::GetWallStartIndex    (const FSector& InFlashSector) { return FPROGMEM::ReadInt8(&InFlashSector.WallStartIndex   ); }
FORCEINLINE uint8 FSector::GetWallCount         (const FSector& InFlashSector) { return FPROGMEM::ReadInt8(&InFlashSector.WallCount        ); }
FORCEINLINE uint8 FSector::GetCeilPaletteIndex  (const FSector& InFlashSector) { return FPROGMEM::ReadInt8(&InFlashSector.CeilPaletteIndex ); }
FORCEINLINE uint8 FSector::GetFloorPaletteIndex (const FSector& InFlashSector) { return FPROGMEM::ReadInt8(&InFlashSector.FloorPaletteIndex); }

FORCEINLINE bool Screentone(const uint8 InX, const uint8 InY, const FColor InColor)
{
    const uint8 Pattern = InColor.Data;
    uint8 OffsetX = ((InY >> 1) & 1) << 1; 

    // Рассчитываем вертикальный сдвиг внутри байта (четная/нечетная строка)
    uint8 ShiftY = (InY & 1) << 2; 

    // Рассчитываем финальный индекс бита с учетом горизонтального смещения по X
    uint8 BitIndex = ShiftY + ((InX + OffsetX) & 3);
    
    return (Pattern >> BitIndex) & 1;
}

int main()
{
    MapSetup();
    
    PinButtonF::InitializeAsInput();
    PinButtonB::InitializeAsInput();
    PinButtonL::InitializeAsInput();
    PinButtonR::InitializeAsInput();
    
    PinButtonF::EnablePullUp();
    PinButtonB::EnablePullUp();
    PinButtonL::EnablePullUp();
    PinButtonR::EnablePullUp();
    
#if __IDE__
    FWindowGuard WindowGuard;
    memset(GScreen, 80, sizeof(GScreen));
#endif
    
    FDisplay::Initialize();
    FSerial::Initialize<9600>();
    
#if DEBUG_SERIAL_PRINT_TICKS
    FTimerFPS::Initialize();
#endif
    
    while(true)
    {
        FRenderer::Render();
        
        const bool bPinButtonF = !PinButtonF::Get();
        const bool bPinButtonB = !PinButtonB::Get();
        const bool bPinButtonL = !PinButtonL::Get();
        const bool bPinButtonR = !PinButtonR::Get();
        
        if(bPinButtonL) FPlayer::Angle += 2;
        if(bPinButtonR) FPlayer::Angle -= 2;
        
        if(bPinButtonF || bPinButtonB)
        {
            const FFloatVector2D OldPlayerLocation = FPlayer::LocationXY;
            
            if(bPinButtonF) FPlayer::LocationXY += FFloatVector2D(-FMath::Sin(FPlayer::Angle), +FMath::Cos(FPlayer::Angle)).MulByTwo();
            if(bPinButtonB) FPlayer::LocationXY += FFloatVector2D(+FMath::Sin(FPlayer::Angle), -FMath::Cos(FPlayer::Angle)).MulByTwo();
            
            const FSector& SectorFlashRef = GSectors[FPlayer::CurrentSector];
            
            for(int16 WallIndex = 0; WallIndex < FSector::GetWallCount(SectorFlashRef); ++WallIndex)
            {
                const FWall& WallFlashRef = GWalls[FSector::GetWallStartIndex(SectorFlashRef) + WallIndex];
                
                if(!FMath::IsLineSegmentsIntersect(OldPlayerLocation, FPlayer::LocationXY, FWall::GetPoint1(WallFlashRef), FWall::GetPoint2(WallFlashRef))) continue;
                
#if __IDE__
                printf("Intersect Sector: %i, Wall: %i\n", FPlayer::CurrentSector, WallIndex);
#endif
                
                if(FWall::GetPortalNextSector(WallFlashRef) == -1)
                {
#if !DEBUG_NO_COLLISION
                    FPlayer::LocationXY = OldPlayerLocation;
#endif
                }
                else
                {
                    if(FWall::GetPortalNextWall(WallFlashRef) != -1) 
                    {
                        const FWall& TargetWallFlashRef = GWalls[FWall::GetPortalNextWall(WallFlashRef)];
                        
                        FPlayer::Angle = FPlayer::Angle + FWall::GetPortalAngleDelta(WallFlashRef);
                        
                        const FFloatVector2D LocalLocation = FPlayer::LocationXY - FFloatVector2D(FFloatVector2D(FWall::GetPoint1(WallFlashRef)) + FFloatVector2D(FWall::GetPoint2(WallFlashRef))).DivByTwo();
                        const FFixedFloat Cos = FMath::Cos(FWall::GetPortalAngleDelta(WallFlashRef));
                        const FFixedFloat Sin = FMath::Sin(FWall::GetPortalAngleDelta(WallFlashRef));
                        
                        const FFloatVector2D RotatedLocal(LocalLocation.X * Cos - LocalLocation.Y * Sin, LocalLocation.X * Sin + LocalLocation.Y * Cos);
                        FPlayer::LocationXY = FFloatVector2D(FFloatVector2D(FWall::GetPoint1(TargetWallFlashRef)) + FFloatVector2D(FWall::GetPoint2(TargetWallFlashRef))).DivByTwo() + RotatedLocal;
                    }
                    
                    FPlayer::CurrentSector = FWall::GetPortalNextSector(WallFlashRef);
                    FPlayer::LocationZ = FSector::GetFloorZ(GSectors[FPlayer::CurrentSector]) + FPlayer::Height;
                }
                
                break;
            }
        }
    }
    
    return 0;
}
