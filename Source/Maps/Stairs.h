// MicroBuild

#pragma once

inline constexpr FWall GWalls[] PROGMEM =
{
    // 0
    { FIntVector2D(-20, -20), FIntVector2D(-20, +0 ), 0 , -1, -1, 0 },
    { FIntVector2D(-20, +0 ), FIntVector2D(+20, +0 ), 1 , +1, -1, 0 },
    { FIntVector2D(+20, +0 ), FIntVector2D(+20, -20), 2 , -1, -1, 0 },
    { FIntVector2D(+20, -20), FIntVector2D(-20, -20), 3 , -1, -1, 0 },

    // 1
    { FIntVector2D(-20, +0 ), FIntVector2D(-20, +10), 4 , -1, -1, 0 },
    { FIntVector2D(-20, +10), FIntVector2D(+20, +10), 5 , +2, -1, 0 },
    { FIntVector2D(+20, +10), FIntVector2D(+20, +0 ), 6 , -1, -1, 0 },
    { FIntVector2D(+20, +0 ), FIntVector2D(-20, +0 ), 7 , +0, -1, 0 },

    // 2
    { FIntVector2D(-20, +10), FIntVector2D(-20, +20), 8 , -1, -1, 0 },
    { FIntVector2D(-20, +20), FIntVector2D(+20, +20), 9 , +3, -1, 0 },
    { FIntVector2D(+20, +20), FIntVector2D(+20, +10), 10, -1, -1, 0 },
    { FIntVector2D(+20, +10), FIntVector2D(-20, +10), 11, +1, -1, 0 },

    // 3
    { FIntVector2D(-20, +20), FIntVector2D(-20, +30), 12, -1, -1, 0 },
    { FIntVector2D(-20, +30), FIntVector2D(+20, +30), 13, +4, -1, 0 },
    { FIntVector2D(+20, +30), FIntVector2D(+20, +20), 0 , -1, -1, 0 },
    { FIntVector2D(+20, +20), FIntVector2D(-20, +20), 1 , +2, -1, 0 },

    // 4
    { FIntVector2D(-20, +30), FIntVector2D(-20, +40), 2 , -1, -1, 0 },
    { FIntVector2D(-20, +40), FIntVector2D(+20, +40), 3 , +5, -1, 0 },
    { FIntVector2D(+20, +40), FIntVector2D(+20, +30), 4 , -1, -1, 0 },
    { FIntVector2D(+20, +30), FIntVector2D(-20, +30), 5 , +3, -1, 0 },

    // 5
    { FIntVector2D(-20, +40), FIntVector2D(-20, +80), 6 , -1, -1, 0 },
    { FIntVector2D(-20, +80), FIntVector2D(+20, +80), 7 , -1, -1, 0 },
    { FIntVector2D(+20, +80), FIntVector2D(+20, +40), 8 , -1, -1, 0 },
    { FIntVector2D(+20, +40), FIntVector2D(-20, +40), 9 , +4, -1, 0 }
};

inline constexpr FSector GSectors[] PROGMEM =
{
    { 0 , 20, 0 , 4, 13, 14 },
    { 4 , 40, 4 , 4, 13, 14 },
    { 8 , 40, 8 , 4, 13, 14 },
    { 12, 40, 12, 4, 13, 14 },
    { 16, 40, 16, 4, 13, 14 },
    { 20, 40, 20, 4, 13, 14 }
};

inline void MapSetup() { FPlayer::LocationXY = FFloatVector2D(0, -10); FPlayer::Angle = 0; }
