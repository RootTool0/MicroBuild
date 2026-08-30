// MicroBuild

#include <string.h>

#include "Global.h"
#include "Math.h"
#include "Structures.h"
#include "Renderer.h"
#include "Map.h"

int main()
{
    MapSetup();
    
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
                {
                    const FSector Sector = FSector::LoadFromFlash(&GSectors[FPlayer::CurrentSector]);
                    
                    for(int16 WallIndex = 0; WallIndex < Sector.WallCount; ++WallIndex)
                    {
                        const FWall Wall = FWall::LoadFromFlash(&GWalls[Sector.WallStartIndex + WallIndex]);
                        
                        if(!FMath::IsLineSegmentsIntersect(OldPlayerLocation, FPlayer::LocationXY, Wall.Point1, Wall.Point2)) continue;
                        
                        #if __IDE__
                        printf("Intersect Section: %i, Wall: %i\n", FPlayer::CurrentSector, WallIndex);
                        #endif
                        
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
                                
                                const FFloatVector2D LocalLocation = FPlayer::LocationXY - FFloatVector2D(FFloatVector2D(Wall.Point1) + FFloatVector2D(Wall.Point2)).DivByTwo();
                                const FFixedFloat Cos = FMath::Cos(Wall.PortalAngleDelta);
                                const FFixedFloat Sin = FMath::Sin(Wall.PortalAngleDelta);
                                
                                const FFloatVector2D RotatedLocal(LocalLocation.X * Cos - LocalLocation.Y * Sin, LocalLocation.X * Sin + LocalLocation.Y * Cos);
                                FPlayer::LocationXY = FFloatVector2D(FFloatVector2D(TargetWall.Point1) + FFloatVector2D(TargetWall.Point2)).DivByTwo() + RotatedLocal;
                            }
                            
                            FPlayer::CurrentSector = Wall.PortalNextSector;
                            FPlayer::LocationZ = FSector::LoadFromFlash(&GSectors[FPlayer::CurrentSector]).FloorZ + FPlayer::Height;
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
