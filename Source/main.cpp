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

FORCEINLINE int8 FSector::GetFloorZ             (const FSector& InFlashSector) { return FPROGMEM::ReadInt8(&InFlashSector.FloorZ           ); }
FORCEINLINE int8  FSector::GetCeilZ             (const FSector& InFlashSector) { return FPROGMEM::ReadInt8(&InFlashSector.CeilZ            ); }
FORCEINLINE uint8 FSector::GetWallStartIndex    (const FSector& InFlashSector) { return FPROGMEM::ReadInt8(&InFlashSector.WallStartIndex   ); }
FORCEINLINE uint8 FSector::GetWallCount         (const FSector& InFlashSector) { return FPROGMEM::ReadInt8(&InFlashSector.WallCount        ); }
FORCEINLINE uint8 FSector::GetCeilPaletteIndex  (const FSector& InFlashSector) { return FPROGMEM::ReadInt8(&InFlashSector.CeilPaletteIndex ); }
FORCEINLINE uint8 FSector::GetFloorPaletteIndex (const FSector& InFlashSector) { return FPROGMEM::ReadInt8(&InFlashSector.FloorPaletteIndex); }

int main()
{
    MapSetup();
    
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
        
        switch(const uint8 Char = FSerial::Receive())
        {
        default: break;
            
        case 'z': ++FPlayer::CurrentSector; break;
        case 'x': --FPlayer::CurrentSector; break;
            
        case 'q': FPlayer::Angle += 1; break;
        case 'e': FPlayer::Angle -= 1; break;
            
        case 'r': FPlayer::LocationZ += 1; break;
        case 'f': FPlayer::LocationZ -= 1; break;
            
        case 'w':
        case 's':
        case 'a':
        case 'd':
            {
                const FFloatVector2D OldPlayerLocation = FPlayer::LocationXY;
                
                /**/ if(Char == 'w') FPlayer::LocationXY += FFloatVector2D(-FMath::Sin(FPlayer::Angle), +FMath::Cos(FPlayer::Angle)).MulByTwo(); // .DivByTwo().DivByTwo();
                else if(Char == 's') FPlayer::LocationXY += FFloatVector2D(+FMath::Sin(FPlayer::Angle), -FMath::Cos(FPlayer::Angle)).MulByTwo(); // .DivByTwo().DivByTwo();
                else if(Char == 'a') FPlayer::LocationXY += FFloatVector2D(-FMath::Cos(FPlayer::Angle), -FMath::Sin(FPlayer::Angle)).MulByTwo(); // .DivByTwo().DivByTwo();
                else if(Char == 'd') FPlayer::LocationXY += FFloatVector2D(+FMath::Cos(FPlayer::Angle), +FMath::Sin(FPlayer::Angle)).MulByTwo(); // .DivByTwo().DivByTwo();
                
                // Resolve....
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
            break;
        }
        
        // FPlayer::Angle += 1;
    }
    
    return 0;
}
