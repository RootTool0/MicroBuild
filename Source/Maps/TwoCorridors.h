// MicroBuild

#pragma once

static inline constexpr FWall GWalls[] PROGMEM =
{
    // 0
    { FIntVector2D(-24, -32), FIntVector2D(-24, +32), 10, -1 , -1 , 0 },
    { FIntVector2D(-24, +32), FIntVector2D(+24, +8 ), 12, +12, -1 , 0 },
    { FIntVector2D(+24, +8 ), FIntVector2D(+24, -8 ), 6 , -1 , -1 , 0 },
    { FIntVector2D(+24, -8 ), FIntVector2D(-24, -32), 12, +7 , -1 , 0 },
    
    // 1
    { FIntVector2D(-40, -56), FIntVector2D(-40, -32), 10, +8 , -1 , 0 },
    { FIntVector2D(-40, -32), FIntVector2D(-24, -32), 10, +14, +56, 0 },
    { FIntVector2D(-24, -32), FIntVector2D(-24, -56), 10, +7 , -1 , 0 },
    { FIntVector2D(-24, -56), FIntVector2D(-40, -56), 10, -1 , -1 , 0 },
    
    // 2
    { FIntVector2D(+24, -56), FIntVector2D(+24, -8 ), 6 , +7 , -1 , 0 },
    { FIntVector2D(+24, -8 ), FIntVector2D(+40, -8 ), 6 , +13, +52, 0 },
    { FIntVector2D(+40, -8 ), FIntVector2D(+40, -56), 6 , +4 , -1 , 0 },
    { FIntVector2D(+40, -56), FIntVector2D(+24, -56), 6 , -1 , -1 , 0 },
    
    // 3
    { FIntVector2D(+24, +8 ), FIntVector2D(+24, +56), 6 , +12, -1 , 0 },
    { FIntVector2D(+24, +56), FIntVector2D(+40, +56), 6 , -1 , -1 , 0 },
    { FIntVector2D(+40, +56), FIntVector2D(+40, +8 ), 6 , +6 , -1 , 0 },
    { FIntVector2D(+40, +8 ), FIntVector2D(+24, +8 ), 6 , +13, +54, 0 },
    
    // 4
    { FIntVector2D(+40, -56), FIntVector2D(+40, -8 ), 6 , +2 , -1 , 0 },
    { FIntVector2D(+40, -8 ), FIntVector2D(+64, -8 ), 6 , +5 , -1 , 0 },
    { FIntVector2D(+64, -8 ), FIntVector2D(+64, -56), 6 , -1 , -1 , 0 },
    { FIntVector2D(+64, -56), FIntVector2D(+40, -56), 6 , -1 , -1 , 0 },
    
    // 5
    { FIntVector2D(+40, -8 ), FIntVector2D(+40, +8 ), 6 , -1 , -1 , 0 },
    { FIntVector2D(+40, +8 ), FIntVector2D(+64, +8 ), 6 , +6 , -1 , 0 },
    { FIntVector2D(+64, +8 ), FIntVector2D(+64, -8 ), 6 , -1 , -1 , 0 },
    { FIntVector2D(+64, -8 ), FIntVector2D(+40, -8 ), 6 , +4 , -1 , 0 },
    
    // 6
    { FIntVector2D(+40, +8 ), FIntVector2D(+40, +56), 6 , +3 , -1 , 0 },
    { FIntVector2D(+40, +56), FIntVector2D(+64, +56), 6 , -1 , -1 , 0 },
    { FIntVector2D(+64, +56), FIntVector2D(+64, +8 ), 6 , -1 , -1 , 0 },
    { FIntVector2D(+64, +8 ), FIntVector2D(+40, +8 ), 6 , +5 , -1 , 0 },
    
    // 7
    { FIntVector2D(-24, -56), FIntVector2D(-24, -32), 12, +1 , -1 , 0 },
    { FIntVector2D(-24, -32), FIntVector2D(+24, -8 ), 12, +0 , -1 , 0 },
    { FIntVector2D(+24, -8 ), FIntVector2D(+24, -56), 12, +2 , -1 , 0 },
    { FIntVector2D(+24, -56), FIntVector2D(-24, -56), 12, -1 , -1 , 0 },
    
    // 8
    { FIntVector2D(-64, -56), FIntVector2D(-64, -32), 10, -1 , -1 , 0 },
    { FIntVector2D(-64, -32), FIntVector2D(-40, -32), 10, +9 , -1 , 0 },
    { FIntVector2D(-40, -32), FIntVector2D(-40, -56), 10, +1 , -1 , 0 },
    { FIntVector2D(-40, -56), FIntVector2D(-64, -56), 10, -1 , -1 , 0 },
    
    // 9
    { FIntVector2D(-64, -32), FIntVector2D(-64, +32), 10, -1 , -1 , 0 },
    { FIntVector2D(-64, +32), FIntVector2D(-40, +32), 10, +10, -1 , 0 },
    { FIntVector2D(-40, +32), FIntVector2D(-40, -32), 10, -1 , -1 , 0 },
    { FIntVector2D(-40, -32), FIntVector2D(-64, -32), 10, +8 , -1 , 0 },
    
    // 10
    { FIntVector2D(-64, +32), FIntVector2D(-64, +56), 10, -1 , -1 , 0 },
    { FIntVector2D(-64, +56), FIntVector2D(-40, +56), 10, -1 , -1 , 0 },
    { FIntVector2D(-40, +56), FIntVector2D(-40, +32), 10, +11, -1 , 0 },
    { FIntVector2D(-40, +32), FIntVector2D(-64, +32), 10, +9 , -1 , 0 },
    
    // 11
    { FIntVector2D(-40, +32), FIntVector2D(-40, +56), 10, +10, -1 , 0 },
    { FIntVector2D(-40, +56), FIntVector2D(-24, +56), 10, -1 , -1 , 0 },
    { FIntVector2D(-24, +56), FIntVector2D(-24, +32), 10, +12, -1 , 0 },
    { FIntVector2D(-24, +32), FIntVector2D(-40, +32), 10, +14, +58, 0 },
    
    // 12
    { FIntVector2D(-24, +32), FIntVector2D(-24, +56), 12, +11, -1 , 0 },
    { FIntVector2D(-24, +56), FIntVector2D(+24, +56), 12, -1 , -1 , 0 },
    { FIntVector2D(+24, +56), FIntVector2D(+24, +8 ), 12, +3 , -1 , 0 },
    { FIntVector2D(+24, +8 ), FIntVector2D(-24, +32), 12, +0 , -1 , 0 },
    
    // 13
    { FIntVector2D(+16, +0 ), FIntVector2D(+0 , +0 ), 10, +2 , +9 , 0 },
    { FIntVector2D(+16, +64), FIntVector2D(+16, +0 ), 10, -1 , -1 , 0 },
    { FIntVector2D(+0 , +64), FIntVector2D(+16, +64), 10, +3 , +15, 0 },
    { FIntVector2D(+0 , +0 ), FIntVector2D(+0 , +64), 10, -1 , -1 , 0 },
    
    // 14
    { FIntVector2D(+16, +0 ), FIntVector2D(+0 , +0 ), 6 , +1 , +5 , 0 },
    { FIntVector2D(+16, +16), FIntVector2D(+16, +0 ), 6 , -1 , -1 , 0 },
    { FIntVector2D(+0 , +16), FIntVector2D(+16, +16), 6 , +11, +47, 0 },
    { FIntVector2D(+0 , +0 ), FIntVector2D(+0 , +16), 6 , -1 , -1 , 0 },
};

static inline constexpr FSector GSectors[] PROGMEM =
{
    { 0, 20, 0 , 4, 0, 3 },
    { 0, 20, 4 , 4, 1, 4 },
    { 0, 20, 8 , 4, 1, 4 },
    { 0, 20, 12, 4, 1, 4 },
    { 0, 20, 16, 4, 1, 4 },
    { 0, 20, 20, 4, 1, 4 },
    { 0, 20, 24, 4, 1, 4 },
    { 0, 20, 28, 4, 1, 4 },
    { 0, 20, 32, 4, 1, 4 },
    { 0, 20, 36, 4, 1, 4 },
    { 0, 20, 40, 4, 1, 4 },
    { 0, 20, 44, 4, 1, 4 },
    { 0, 20, 48, 4, 1, 4 },
    { 0, 20, 52, 4, 1, 4 },
    { 0, 20, 56, 4, 1, 4 },
};
