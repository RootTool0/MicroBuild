// MicroBuild

#pragma once

inline const FWall GWalls[] PROGMEM =
{
    // 0
    { FIntVector2D(-20, -20), FIntVector2D(-20, -10), 1, -1, -1, 0   },
    { FIntVector2D(-20, -10), FIntVector2D(-20,  10), 1, +1, -1, 0   },
    { FIntVector2D(-20, +10), FIntVector2D(-20,  20), 4, -1, -1, 0   },
    { FIntVector2D(-20, +20), FIntVector2D(-10,  20), 5, -1, -1, 0   },
    { FIntVector2D(-10, +20), FIntVector2D(+10,  20), 6, +2, -1, 0   },
    { FIntVector2D(+10, +20), FIntVector2D(+20,  20), 4, -1, -1, 0   },
    { FIntVector2D(+20, +20), FIntVector2D(+20, -20), 5, -1, -1, 0   },
    { FIntVector2D(+20, -20), FIntVector2D(-20, -20), 6, -1, -1, 0   },
    
    // 1
    { FIntVector2D(-20, +10), FIntVector2D(-20, -10), 1, +0, -1, 0   },
    { FIntVector2D(-20, -10), FIntVector2D(-30, -5 ), 1, -1, -1, 0   },
    { FIntVector2D(-30, -5 ), FIntVector2D(-30, +5 ), 2, +2, 14, 64  },
    { FIntVector2D(-30, +5 ), FIntVector2D(-20, +10), 3, -1, -1, 0   },
    
    // 2
    { FIntVector2D(+10, +20), FIntVector2D(-10,  20), 6, +0, -1, 0   },
    { FIntVector2D(-10, +20), FIntVector2D(-5 ,  30), 6, -1, -1, 0   },
    { FIntVector2D(-5 , +30), FIntVector2D(+5 ,  30), 2, +1, 10, 192 },
    { FIntVector2D(+5 , +30), FIntVector2D(+10,  20), 3, -1, -1, 0   }
};

inline constexpr FSector GSectors[] PROGMEM =
{
    { 0, 20, 0 , 8, 13, 14 },
    { 5, 15, 8 , 4, 14, 13 },
    { 0, 20, 12, 4, 13, 14 }
};

inline void MapSetup() { FPlayer::LocationXY = FFloatVector2D(0, 0); FPlayer::Angle = 0; }
