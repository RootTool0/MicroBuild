// MicroBuild

#pragma once

static inline constexpr FWall GWalls[] PROGMEM =
{
    // 0
    { FIntVector2D(-40, -40), FIntVector2D(-40, +40), 5 , -1 , -1 , 0 },
    { FIntVector2D(-40, +40), FIntVector2D(-10, +10), 5 , +1 , -1 , 0 },
    { FIntVector2D(-10, +10), FIntVector2D(-10, -10), 7 , +7 , +31, 0 },
    { FIntVector2D(-10, -10), FIntVector2D(-40, -40), 5 , +3 , -1 , 0 },
    
    // 1
    { FIntVector2D(-40, +40), FIntVector2D(+40, +40), 5 , -1 , -1 , 0 },
    { FIntVector2D(-10, +10), FIntVector2D(-40, +40), 5 , +0 , -1 , 0 },
    { FIntVector2D(+10, +10), FIntVector2D(-10, +10), 7 , +4 , +16, 0 },
    { FIntVector2D(+40, +40), FIntVector2D(+10, +10), 5 , +2 , -1 , 0 },
    
    // 2
    { FIntVector2D(+40, +40), FIntVector2D(+40, -40), 5 , -1 , -1 , 0 },
    { FIntVector2D(+40, -40), FIntVector2D(+10, -10), 5 , +3 , -1 , 0 },
    { FIntVector2D(+10, -10), FIntVector2D(+10, +10), 7 , +5 , +21, 0 },
    { FIntVector2D(+10, +10), FIntVector2D(+40, +40), 5 , +1 , -1 , 0 },
    
    // 3
    { FIntVector2D(+40, -40), FIntVector2D(-40, -40), 5 , -1 , -1 , 0 },
    { FIntVector2D(+10, -10), FIntVector2D(+40, -40), 5 , +2 , -1 , 0 },
    { FIntVector2D(-10, -10), FIntVector2D(+10, -10), 7 , +6 , +26 , 0 },
    { FIntVector2D(-40, -40), FIntVector2D(-10, -10), 5 , +0 , -1 , 0 },
    
    // 4
    { FIntVector2D(-10, +90), FIntVector2D(+10, +90), 12, +1 , +6 , 0 },
    { FIntVector2D(+10, +90), FIntVector2D(+0 , +20), 12, +5 , -1 , 0 },
    { FIntVector2D(+0 , +20), FIntVector2D(-20, +0 ), 14, +8 , -1 , 0 },
    { FIntVector2D(-20, +0 ), FIntVector2D(-90, +10), 12, +7 , -1 , 0 },
    { FIntVector2D(-90, +10), FIntVector2D(-10, +90), 12, -1 , -1 , 0 },
    
    // 5
    { FIntVector2D(+90, +10), FIntVector2D(+90, -10), 12, +2 , +10, 0 },
    { FIntVector2D(+90, -10), FIntVector2D(+20, +0 ), 12, +6 , -1 , 0 },
    { FIntVector2D(+20, +0 ), FIntVector2D(+0 , +20), 14, +8 , -1 , 0 },
    { FIntVector2D(+0 , +20), FIntVector2D(+10, +90), 12, +4 , -1 , 0 },
    { FIntVector2D(+10, +90), FIntVector2D(+90, +10), 12, -1 , -1 , 0 },
    
    // 6
    { FIntVector2D(+10, -90), FIntVector2D(-10, -90), 12, +3 , +14, 0 },
    { FIntVector2D(-10, -90), FIntVector2D(-0 , -20), 12, +7 , -1 , 0 },
    { FIntVector2D(-0 , -20), FIntVector2D(+20, -0 ), 14, +8 , -1 , 0 },
    { FIntVector2D(+20, -0 ), FIntVector2D(+90, -10), 12, +5 , -1 , 0 },
    { FIntVector2D(+90, -10), FIntVector2D(+10, -90), 12, -1 , -1 , 0 },
    
    // 7
    { FIntVector2D(-90, -10), FIntVector2D(-90, +10), 12, +0 , +2 , 0 },
    { FIntVector2D(-90, +10), FIntVector2D(-20, -0 ), 12, +4 , -1 , 0 },
    { FIntVector2D(-20, -0 ), FIntVector2D(-0 , -20), 14, +8 , -1 , 0 },
    { FIntVector2D(-0 , -20), FIntVector2D(-10, -90), 12, +6 , -1 , 0 },
    { FIntVector2D(-10, -90), FIntVector2D(-90, -10), 12, -1 , -1 , 0 },
    
    // 8
    { FIntVector2D(-20, +0 ), FIntVector2D(+0 , +20), 13, +4 , -1 , 0 },
    { FIntVector2D(+0 , +20), FIntVector2D(+20, +0 ), 13, +5 , -1 , 0 },
    { FIntVector2D(+20, +0 ), FIntVector2D(+0 , -20), 13, +6 , -1 , 0 },
    { FIntVector2D(+0 , -20), FIntVector2D(-20, +0 ), 13, +7 , -1 , 0 },
};

static inline constexpr FSector GSectors[] PROGMEM =
{
    { 0, 80, 0 , 4, 5 , 3  },
    { 0, 80, 4 , 4, 5 , 3  },
    { 0, 80, 8 , 4, 5 , 3  },
    { 0, 80, 12, 4, 5 , 3  },
    
    { 0, 18, 16, 5, 13, 12 },
    { 0, 18, 21, 5, 13, 12 },
    { 0, 18, 26, 5, 13, 12 },
    { 0, 18, 31, 5, 13, 12 },
    
    { 4, 14, 36, 4, 14, 14 },
};

static inline void MapSetup() { FPlayer::LocationXY = FFloatVector2D(-38, 0); FPlayer::Angle = 192; }
