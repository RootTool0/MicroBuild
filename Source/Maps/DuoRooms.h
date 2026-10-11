// MicroBuild

#pragma once

inline constexpr FWall GWalls[] PROGMEM =
{
    // 0
    { FIntVector2D(-20, -20), FIntVector2D(-20, +20), 0, -1, -1, 0 },
    { FIntVector2D(-20, +20), FIntVector2D(+20, +20), 1, +1, -1, 0 },
    { FIntVector2D(+20, +20), FIntVector2D(+20, -20), 2, -1, -1, 0 },
    { FIntVector2D(+20, -20), FIntVector2D(-20, -20), 3, -1, -1, 0 },
    
    // 1
    { FIntVector2D(-20, +20), FIntVector2D(-20, +60), 4, -1, -1, 0 },
    { FIntVector2D(-20, +60), FIntVector2D(+20, +60), 5, -1, -1, 0 },
    { FIntVector2D(+20, +60), FIntVector2D(+20, +20), 6, -1, -1, 0 },
    { FIntVector2D(+20, +20), FIntVector2D(-20, +20), 7, +0, -1, 0 }
};

inline constexpr FSector GSectors[] PROGMEM =
{
    { 0, 20, 0, 4, 13, 14 },
    { 5, 15, 4, 4, 13, 14 }
};

inline void MapSetup() { FPlayer::LocationXY = FFloatVector2D(0, 0); FPlayer::Angle = 0; }
