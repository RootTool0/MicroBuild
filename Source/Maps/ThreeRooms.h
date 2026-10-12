// MicroBuild

#pragma once

inline constexpr FWall GWalls[] PROGMEM =
{
    // 0
    { FIntVector2D(-20, -20 ), FIntVector2D(-20, +20 ), 0 , -1, -1, 0 },
    { FIntVector2D(-20, +20 ), FIntVector2D(+20, +20 ), 1 , +1, -1, 0 },
    { FIntVector2D(+20, +20 ), FIntVector2D(+20, -20 ), 2 , -1, -1, 0 },
    { FIntVector2D(+20, -20 ), FIntVector2D(-20, -20 ), 3 , -1, -1, 0 },
    
    // 1
    { FIntVector2D(-20, +20 ), FIntVector2D(-20, +60 ), 4 , -1, -1, 0 },
    { FIntVector2D(-20, +60 ), FIntVector2D(+20, +60 ), 5 , +2, -1, 0 },
    { FIntVector2D(+20, +60 ), FIntVector2D(+20, +20 ), 6 , -1, -1, 0 },
    { FIntVector2D(+20, +20 ), FIntVector2D(-20, +20 ), 7 , +0, -1, 0 },
    
    // 2
    { FIntVector2D(-20, +60 ), FIntVector2D(-20, +100), 8 , -1, -1, 0 },
    { FIntVector2D(-20, +100), FIntVector2D(+60, +100), 9 , -1, -1, 0 },
    { FIntVector2D(+60, +100), FIntVector2D(+60, +60 ), 10, -1, -1, 0 },
    { FIntVector2D(+60, +60 ), FIntVector2D(+20, +60 ), 11, -1, -1, 0 },
    { FIntVector2D(+20, +60 ), FIntVector2D(-20, +60 ), 12, +1, -1, 0 }
};

inline constexpr FSector GSectors[] PROGMEM =
{
    { +0 , +20, 1, 4, 13, 14 },
    { +5 , +15, 4, 4, 13, 14 },
    { -10, +30, 8, 5, 13, 14 }
};

inline void MapSetup() { FPlayer::LocationXY = FFloatVector2D(0, 0); FPlayer::Angle = 0; }
