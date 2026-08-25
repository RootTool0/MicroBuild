// MicroBuild

#include <string.h>

#include "Global.h"
#include "Math.h"
#include "Structures.h"
#include "Renderer.h"

int main()
{
#if __IDE__
    FWindowGuard WindowGuard;
    memset(GScreen, 80, sizeof(GScreen));
#endif
    
    ST7735::Initialize();
    FSerial::Initialize<9600>();
    
#if !__IDE__ && DEBUG_SERIAL_PRINT_TICKS
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
            
        case 'q': FPlayer::Angle += 4; break;
        case 'e': FPlayer::Angle -= 4; break;
            
        case 'r': FPlayer::LocationZ += 1; break;
        case 'f': FPlayer::LocationZ -= 1; break;
            
        case 'w':
        case 's':
        case 'a':
        case 'd':
            {
                const FVector2D OldPlayerLocation = FPlayer::LocationXY;
                
                /**/ if(Char == 'w') FPlayer::LocationXY += FVector2D(-FMath::Sin(FPlayer::Angle), +FMath::Cos(FPlayer::Angle)).MulByTwo();
                else if(Char == 's') FPlayer::LocationXY += FVector2D(+FMath::Sin(FPlayer::Angle), -FMath::Cos(FPlayer::Angle)).MulByTwo();
                else if(Char == 'a') FPlayer::LocationXY += FVector2D(-FMath::Cos(FPlayer::Angle), -FMath::Sin(FPlayer::Angle)).MulByTwo();
                else if(Char == 'd') FPlayer::LocationXY += FVector2D(+FMath::Cos(FPlayer::Angle), +FMath::Sin(FPlayer::Angle)).MulByTwo();
                
                // Resolve....
                {
                    const FSector Sector = FSector::LoadFromFlash(&GSectors[FPlayer::CurrentSector]);
                    
                    for(int16 WallIndex = 0; WallIndex < Sector.WallCount; ++WallIndex)
                    {
                        const FWall Wall = FWall::LoadFromFlash(&GWalls[Sector.WallStartIndex + WallIndex]);
                        
                        if(!FMath::IsLineSegmentsIntersect(OldPlayerLocation, FPlayer::LocationXY, Wall.Point1, Wall.Point2)) continue;
                        
                        printf("Intersect Section: %i, Wall: %i (Absolute: %i)\n", FPlayer::CurrentSector, WallIndex, Sector.WallStartIndex + WallIndex);
                        
                        if(Wall.PortalNextSector == -1)
                        {
#if !DEBUG_NO_COLLISION
                            FPlayer::LocationXY = OldPlayerLocation;
#endif
                        }
                        else
                        {
                            if(Wall.PortalNextWall != -1) 
                            {
                                const FWall TargetWall = FWall::LoadFromFlash(&GWalls[Wall.PortalNextWall]);
                                
                                FPlayer::Angle = FPlayer::Angle + Wall.PortalAngleDelta;
                    
                                const FVector2D LocalLocation = FPlayer::LocationXY - (Wall.Point1 + Wall.Point2).DivByTwo();
                                const FFixedFloat Cos = FMath::Cos(Wall.PortalAngleDelta);
                                const FFixedFloat Sin = FMath::Sin(Wall.PortalAngleDelta);
                    
                                const FVector2D RotatedLocal(LocalLocation.X * Cos - LocalLocation.Y * Sin, LocalLocation.X * Sin + LocalLocation.Y * Cos);
                                FPlayer::LocationXY = (TargetWall.Point1 + TargetWall.Point2).DivByTwo() + RotatedLocal;
                            }
                            
                            FPlayer::CurrentSector = Wall.PortalNextSector;
                            FPlayer::LocationZ = FSector::LoadFromFlash(&GSectors[FPlayer::CurrentSector]).GetMiddleZ();
                        }
                        
                        break;
                    }
                }
            }
            break;
        }
    }
    
    return 0;
}
