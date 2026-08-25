// MicroBuild

#pragma once

#include "AAT/Core/Aliases.h"
#include "AAT/Memory/PROGMEM.h"

struct FColor
{
    
public:
    uint16 Data = 0;
    
    constexpr FColor() = default;
    constexpr FColor(const uint16 InData) : Data(InData) {}
    constexpr FColor(const uint8 InR, const uint8 InG, const uint8 InB) : Data(((uint16)(InR & 0xF8) << 8) | ((uint16)(InG & 0xFC) << 3) | ((uint16)(InB & 0xF8) >> 3)) {}
    
    static const FColor Black;
    static const FColor Ceil;
    static const FColor Floor;
};

constexpr FColor FColor::Black = FColor(0, 0, 0);
constexpr FColor FColor::Ceil  = FColor(255, 0, 0);
constexpr FColor FColor::Floor = FColor(0, 0, 255);

struct FFixedFloat
{
    using FFixedFloatInt       = int32;
    using FFixedFloatIntDouble = int64;
    static constexpr int FRACTIONAL_BITS = 16;
    static constexpr FFixedFloatInt SCALE = static_cast<FFixedFloatInt>(1) << FRACTIONAL_BITS;

public:
    FFixedFloatInt Raw;
    
    constexpr FFixedFloat() : Raw(0) {}
    
    constexpr /*    */ FFixedFloat(const int    InVal) : Raw(static_cast<FFixedFloatInt>(InVal) * SCALE) {}
    constexpr explicit FFixedFloat(const float  InVal) : Raw(static_cast<FFixedFloatInt>(InVal * SCALE)) {}
    constexpr explicit FFixedFloat(const double InVal) : Raw(static_cast<FFixedFloatInt>(InVal * SCALE)) {}
    
    static constexpr FFixedFloat FromRaw(const FFixedFloatInt InRawValue) { FFixedFloat FixedFloat; FixedFloat.Raw = InRawValue; return FixedFloat; }
    
    constexpr float ToFloat() const { return static_cast<float>(Raw) / (1UL << FRACTIONAL_BITS); }
    constexpr FFixedFloatInt ToInt() const { return Raw >> FRACTIONAL_BITS; }
    
    constexpr FFixedFloat DivByTwo() const { return FFixedFloat::FromRaw(Raw >> 1); }
    constexpr FFixedFloat MulByTwo() const { return FFixedFloat::FromRaw(Raw << 1); }
    
    constexpr FFixedFloat operator+(const FFixedFloat& Other) const { return FromRaw(Raw + Other.Raw); }
    constexpr FFixedFloat operator-(const FFixedFloat& Other) const { return FromRaw(Raw - Other.Raw); }
    constexpr FFixedFloat operator*(const FFixedFloat& Other) const { return FromRaw(static_cast<FFixedFloatInt>((static_cast<FFixedFloatIntDouble>(Raw) * Other.Raw) >> FRACTIONAL_BITS)); }
    constexpr FFixedFloat operator/(const FFixedFloat& Other) const { return FromRaw(static_cast<FFixedFloatInt>((static_cast<FFixedFloatIntDouble>(Raw) << FRACTIONAL_BITS) / Other.Raw)); }
    constexpr FFixedFloat operator*(const FFixedFloatInt Scalar) const { return FromRaw(Raw * Scalar); }
    constexpr FFixedFloat operator/(const FFixedFloatInt Scalar) const { return FromRaw(Raw / Scalar); }
    constexpr FFixedFloat operator+() const { FFixedFloat FixedFloat; FixedFloat.Raw = +Raw; return FixedFloat; }
    constexpr FFixedFloat operator-() const { FFixedFloat FixedFloat; FixedFloat.Raw = -Raw; return FixedFloat; }
    
    constexpr FFixedFloat& operator+=(const FFixedFloat& Other) { *this = *this + Other; return *this; }
    constexpr FFixedFloat& operator-=(const FFixedFloat& Other) { *this = *this - Other; return *this; }
    constexpr FFixedFloat& operator*=(const FFixedFloat& Other) { *this = *this * Other; return *this; }
    constexpr FFixedFloat& operator/=(const FFixedFloat& Other) { *this = *this / Other; return *this; }
    
    constexpr bool operator< (const FFixedFloat& Other) const { return Raw <  Other.Raw; }
    constexpr bool operator> (const FFixedFloat& Other) const { return Raw >  Other.Raw; }
    constexpr bool operator<=(const FFixedFloat& Other) const { return Raw <= Other.Raw; }
    constexpr bool operator>=(const FFixedFloat& Other) const { return Raw >= Other.Raw; }
    constexpr bool operator==(const FFixedFloat& Other) const { return Raw == Other.Raw; }
    constexpr bool operator!=(const FFixedFloat& Other) const { return Raw != Other.Raw; }
    
    static const FFixedFloat Zero;
    static const FFixedFloat ZeroPointFive;
    static const FFixedFloat One;
    static const FFixedFloat OnePointFive;
    static const FFixedFloat ThreePointFive;
};

inline constexpr FFixedFloat FFixedFloat::Zero           = FFixedFloat(0.0f);
inline constexpr FFixedFloat FFixedFloat::ZeroPointFive  = FFixedFloat(0.5f);
inline constexpr FFixedFloat FFixedFloat::One            = FFixedFloat(1.0f);
inline constexpr FFixedFloat FFixedFloat::OnePointFive   = FFixedFloat(1.5f); 
inline constexpr FFixedFloat FFixedFloat::ThreePointFive = FFixedFloat(3.5f); 

struct FVector2D
{
    
public:
    FFixedFloat X;
    FFixedFloat Y;
    
    constexpr FVector2D() : X(0), Y(0) {}
    constexpr FVector2D(const FFixedFloat InF) : X(InF), Y(InF) {}
    constexpr FVector2D(const FFixedFloat InX, const FFixedFloat InY) : X(InX), Y(InY) {}
    
    constexpr FVector2D DivByTwo() const { return { X.DivByTwo(), Y.DivByTwo() }; }
    constexpr FVector2D MulByTwo() const { return { X.MulByTwo(), Y.MulByTwo() }; }
    
    constexpr FVector2D operator-() const { return { -X, -Y }; }
    constexpr FVector2D operator+(const FVector2D& Other) const { return { X + Other.X, Y + Other.Y }; }
    constexpr FVector2D operator-(const FVector2D& Other) const { return { X - Other.X, Y - Other.Y }; }
    constexpr FVector2D operator*(const FVector2D& Other) const { return { X * Other.X, Y * Other.Y }; }
    constexpr FVector2D operator*(const FFixedFloat Scalar) const { return { X * Scalar, Y * Scalar }; }
    constexpr FVector2D operator/(const FVector2D& Other) const { return { X / Other.X, Y / Other.Y }; }
    constexpr FVector2D operator/(const FFixedFloat Scalar) const { return { X / Scalar, Y / Scalar }; }
    
    constexpr FVector2D& operator+=(const FVector2D& Other) { *this = *this + Other; return *this; }
    constexpr FVector2D& operator-=(const FVector2D& Other) { *this = *this - Other; return *this; }
    constexpr FVector2D& operator*=(const FVector2D& Other) { *this = *this * Other; return *this; }
    constexpr FVector2D& operator*=(const FFixedFloat Scalar) { *this = *this * Scalar; return *this; }
    constexpr FVector2D& operator/=(const FVector2D& Other) { *this = *this / Other; return *this; }
    constexpr FVector2D& operator/=(const FFixedFloat Scalar) { *this = *this / Scalar; return *this; }
};

using FAngle = uint8;

struct FPlayer
{
    static inline FVector2D LocationXY;
    static inline FAngle Angle = 32;
    static inline int8 CurrentSector = 0;
    static inline int8 LocationZ = 10;
};

struct FWall
{
    FVector2D Point1, Point2;
    int8 WallPaletteIndex;
    int8 PortalNextSector;
    int8 PortalNextWall;
    FAngle PortalAngleDelta;
    
    static FWall LoadFromFlash(const void* InFlashAddr) { FWall Sector; FPROGMEM::Memcpy(&Sector, InFlashAddr, sizeof(FWall)); return Sector; }
};

struct FSector
{
    int8 FloorZ;
    int8 CeilZ;
    uint8 WallStartIndex;
    uint8 WallCount;
    
    int8 GetMiddleZ() const { return (FloorZ + CeilZ) / 2; }
    
    static FSector LoadFromFlash(const void* InFlashAddr) { FSector Sector; FPROGMEM::Memcpy(&Sector, InFlashAddr, sizeof(FSector)); return Sector; }
};

/*
static inline constexpr FWall GWalls[] PROGMEM =
{
    {FVector2D(-20, -20), FVector2D(-20, +20), 0, -1},
    {FVector2D(-20, +20), FVector2D(+20, +20), 1, +1},
    {FVector2D(+20, +20), FVector2D(+20, -20), 2, -1},
    {FVector2D(+20, -20), FVector2D(-20, -20), 3, -1},
    
    {FVector2D(-20, 20), FVector2D(-20, +60), 4, -1},
    {FVector2D(-20, 60), FVector2D(+20, +60), 5, -1},
    {FVector2D(+20, 60), FVector2D(+20, +20), 6, -1},
    {FVector2D(+20, 20), FVector2D(-20, +20), 7, +0}
};

static inline constexpr FSector GSectors[] PROGMEM =
{
    {0, 20, 0, 4},
    {5, 15, 4, 4}
};
*/

/*
static inline constexpr FWall GWalls[] PROGMEM =
{
    // === СЕКТОР 0 (Комната 40x40, портал на стене №1) ===
    {FVector2D(-20, -20), FVector2D(-20, +20), 0, -1}, // Слева (глухая)
    {FVector2D(-20, +20), FVector2D(+20, +20), 1, +1}, // Вперед -> ПОРТАЛ В СЕКТОР 1
    {FVector2D(+20, +20), FVector2D(+20, -20), 2, -1}, // Справа (глухая)
    {FVector2D(+20, -20), FVector2D(-20, -20), 3, -1}, // Сзади (глухая)
    
    // === СЕКТОР 1 (Проходная зона, смещена вперед на +40) ===
    {FVector2D(-20, +20), FVector2D(-20, +60), 4, -1}, // Слева (глухая)
    {FVector2D(-20, +60), FVector2D(+20, +60), 5, +2}, // Вперед -> ПОРТАЛ В СЕКТОР 2 (Заворот)
    {FVector2D(+20, +60), FVector2D(+20, +20), 6, -1}, // Справа (глухая)
    {FVector2D(+20, +20), FVector2D(-20, +20), 7, +0}, // Назад -> ПОРТАЛ В СЕКТОР 0
    
    // === СЕКТОР 2 (Коридор справа, уходит вбок от Сектора 1) ===
    {FVector2D(-20, +60), FVector2D(-20, +100), 8, -1}, // Дальняя стена тупика
    {FVector2D(-20, +100), FVector2D(+60, +100), 9, -1}, // Длинная стена слева
    {FVector2D(+60, +100), FVector2D(+60, +60), 10, -1}, // Стена тупика справа
    {FVector2D(+60, +60), FVector2D(+20, +60), 11, -1},  // Ближняя стена
    {FVector2D(+20, +60), FVector2D(-20, +60), 12, +1}   // Вход -> ПОРТАЛ В СЕКТОР 1
};

static inline constexpr FSector GSectors[] PROGMEM =
{
    // {FloorZ, CeilZ, WallStartIndex, WallCount}
    {0,   20, 0, 4}, // Сектор 0: Стандартная высота
    {5,   15, 4, 4}, // Сектор 1: Узкий высокий порог (пол выше, потолок ниже)
    {-10, 30, 8, 5}  // Сектор 2: Огромный глубокий зал за углом
};
*/

/*
static inline constexpr FWall GWalls[] PROGMEM =
{
    // === СЕКТОР 0: Стартовая площадка перед лестницей (Ширина 40, Длина 20) ===
    {FVector2D(-20, -20), FVector2D(-20,   0), 0, -1}, // Слева (глухая)
    {FVector2D(-20,   0), FVector2D(+20,   0), 1, +1}, // Вперед -> ПОРТАЛ НА СТУПЕНЬ 1
    {FVector2D(+20,   0), FVector2D(+20, -20), 2, -1}, // Справа (глухая)
    {FVector2D(+20, -20), FVector2D(-20, -20), 3, -1}, // Сзади (глухая)

    // === СЕКТОР 1: Ступенька 1 (FloorZ = 4) ===
    {FVector2D(-20,   0), FVector2D(-20,  10), 4, -1}, // Слева (глухая)
    {FVector2D(-20,  10), FVector2D(+20,  10), 5, +2}, // Вперед -> ПОРТАЛ НА СТУПЕНЬ 2
    {FVector2D(+20,  10), FVector2D(+20,   0), 6, -1}, // Справа (глухая)
    {FVector2D(+20,   0), FVector2D(-20,   0), 7, +0}, // Назад -> ПОРТАЛ В СЕКТОР 0

    // === СЕКТОР 2: Ступенька 2 (FloorZ = 8) ===
    {FVector2D(-20,  10), FVector2D(-20,  20), 8, -1}, // Слева (глухая)
    {FVector2D(-20,  20), FVector2D(+20,  20), 9, +3}, // Вперед -> ПОРТАЛ НА СТУПЕНЬ 3
    {FVector2D(+20,  20), FVector2D(+20,  10), 10, -1},// Справа (глухая)
    {FVector2D(+20,  10), FVector2D(-20,  10), 11, +1},// Назад -> ПОРТАЛ В СЕКТОР 1

    // === СЕКТОР 3: Ступенька 3 (FloorZ = 12) ===
    {FVector2D(-20,  20), FVector2D(-20,  30), 12, -1},// Слева (глухая)
    {FVector2D(-20,  30), FVector2D(+20,  30), 13, +4},// Вперед -> ПОРТАЛ НА СТУПЕНЬ 4
    {FVector2D(+20,  30), FVector2D(+20,  20), 14, -1},// Справа (глухая)
    {FVector2D(+20,  20), FVector2D(-20,  20), 15, +2},// Назад -> ПОРТАЛ В СЕКТОР 2

    // === СЕКТОР 4: Ступенька 4 (FloorZ = 16) ===
    {FVector2D(-20,  30), FVector2D(-20,  40), 16, -1},// Слева (глухая)
    {FVector2D(-20,  40), FVector2D(+20,  40), 17, +5},// Вперед -> ПОРТАЛ НА ВЕРХНИЙ ЭТАЖ
    {FVector2D(+20,  40), FVector2D(+20,  30), 18, -1},// Справа (глухая)
    {FVector2D(+20,  30), FVector2D(-20,  30), 19, +3},// Назад -> ПОРТАЛ В СЕКТОР 3

    // === СЕКТОР 5: Верхний этаж (FloorZ = 20, просторная комната 40x40) ===
    {FVector2D(-20,  40), FVector2D(-20,  80), 20, -1},// Слева (глухая)
    {FVector2D(-20,  80), FVector2D(+20,  80), 21, -1},// Вперед в тупик (глухая)
    {FVector2D(+20,  80), FVector2D(+20,  40), 22, -1},// Справа (глухая)
    {FVector2D(+20,  40), FVector2D(-20,  40), 23, +4} // Назад -> ПОРТАЛ ВНИЗ НА СТУПЕНЬ 4
};

static inline constexpr FSector GSectors[] PROGMEM =
{
    // {FloorZ, CeilZ, WallStartIndex, WallCount}
    {0,   20,  0, 4},  // Сектор 0: Низина
    {4,   40,  4, 4},  // Сектор 1: Ступень 1
    {8,   40,  8, 4},  // Сектор 2: Ступень 2
    {12,  40, 12, 4},  // Сектор 3: Ступень 3
    {16,  40, 16, 4},  // Сектор 4: Ступень 4
    {20,  40, 20, 4}   // Сектор 5: Верхняя платформа
};
*/

/*
static inline constexpr FWall GWalls[] PROGMEM =
{
    {FVector2D(-20, -20), FVector2D(-20,  -10), 0, -1, -1},
    {FVector2D(-20,  -10), FVector2D(-30,  -5), 1, -1, -1},
    {FVector2D(-30,  -5), FVector2D(-30,   5 ), 2, +0, +7, 64},
    {FVector2D(-30,   5), FVector2D(-20,   10), 3, -1, -1},
    {FVector2D(-20,   10), FVector2D(-20,  20), 4, -1, -1},
    
    {FVector2D(-20,  20), FVector2D( -10,  20), 5, -1, -1},
    {FVector2D( -10,  20), FVector2D( -5,  30), 6, -1, -1},
    {FVector2D( -5,  30), FVector2D(  5,  30 ), 2, +0, +2, 192},
    {FVector2D(  5,  30), FVector2D(  10,  20), 3, -1, -1},
    {FVector2D(  10,  20), FVector2D( 20,  20), 4, -1, -1},
    
    {FVector2D( 20,  20), FVector2D( 20, -20), 5, -1, -1},
    
    {FVector2D( 20, -20), FVector2D(-20, -20), 6, -1, -1}
};

static inline constexpr FSector GSectors[] PROGMEM =
{
    {0, 20, 0, 12} 
};
*/

/*
static inline const FWall GWalls[] PROGMEM =
{
    // --- СЕКТОР 0: Центральная комната (9 стен: с индекса 0 по 8) ---
    {FVector2D(-20, -20), FVector2D(-20, -10), 0, -1, -1, 0},   // 0: Левая нижняя стена
    {FVector2D(-20, -10), FVector2D(-20,  10), 1,  1, -1, 0},   // 1: ЕВКЛИДОВ ПРОХОД в левый аппендикс (Сектор 1). PortalNextWall = 1 (простой проем)
    {FVector2D(-20,  10), FVector2D(-20,  20), 4, -1, -1, 0},   // 2: Левая верхняя стена
    {FVector2D(-20,  20), FVector2D(-10,  20), 5, -1, -1, 0},   // 3: Верхняя левая стена
    {FVector2D(-10,  20), FVector2D( 10,  20), 6,  2, -1, 0},   // 4: ЕВКЛИДОВ ПРОХОД в верхний аппендикс (Сектор 2). PortalNextWall = 1
    {FVector2D( 10,  20), FVector2D( 20,  20), 4, -1, -1, 0},   // 5: Верхняя правая стена
    {FVector2D( 20,  20), FVector2D( 20, -20), 5, -1, -1, 0},   // 6: Правая стена
    {FVector2D( 20, -20), FVector2D(-20, -20), 6, -1, -1, 0},   // 7: Нижняя стена
    
    // --- СЕКТОР 1: Левый аппендикс с порталом (4 стены: с индекса 8 по 11) ---
    {FVector2D(-20,  10), FVector2D(-20, -10), 1,  0, -1, 0},   // 8: Оборотная сторона прохода в Центр (Сектор 0)
    {FVector2D(-20, -10), FVector2D(-30,  -5), 1, -1, -1, 0},   // 9: Боковая грань аппендикса
    {FVector2D(-30,  -5), FVector2D(-30,   5), 2,  2, 14, 64},  // 10: НЕЕВКЛИДОВ ПОРТАЛ. Ведет в Сектор 2, на Стену 14 (бывшая 7)
    {FVector2D(-30,   5), FVector2D(-20,  10), 3, -1, -1, 0},   // 11: Вторая боковая грань
    
    // --- СЕКТОР 2: Верхний аппендикс с порталом (4 стены: с индекса 12 по 15) ---
    {FVector2D( 10,  20), FVector2D(-10,  20), 6,  0, -1, 0},   // 12: Оборотная сторона прохода в Центр (Сектор 0)
    {FVector2D(-10,  20), FVector2D( -5,  30), 6, -1, -1, 0},   // 13: Боковая грань аппендикса
    {FVector2D( -5,  30), FVector2D(  5,  30), 2,  1, 10, 192}, // 14: НЕЕВКЛИДОВ ПОРТАЛ. Ведет в Сектор 1, на Стену 10 (бывшая 2)
    {FVector2D(  5,  30), FVector2D( 10,  20), 3, -1, -1, 0}    // 15: Вторая боковая грань
};

static inline constexpr FSector GSectors[] /*PROGMEM#1# =
{
    // FloorZ, CeilZ, WallStartIndex, WallCount
    { 0, 20, 0 , 8 }, // Сектор 0: Центральная комната (8 стен)
    { 5, 15, 8 , 4 }, // Сектор 1: Левый аппендикс (4 стены)
    { 0, 20, 12, 4 }  // Сектор 2: Верхний аппендикс (4 стены)
};
*/

/*
static inline constexpr FWall GWalls[] PROGMEM =
{
    /*
    { FVector2D(40.0, -40.0), FVector2D(40.0, 40.0), 0, -1, -1, 0 },
    { FVector2D(40.0, 40.0), FVector2D(-40.0, 40.0), 1, -1, -1, 0 },
    { FVector2D(-40.0, 40.0), FVector2D(-40.0, -40.0), 2, -1, -1, 0 },
    { FVector2D(-40.0, -40.0), FVector2D(40.0, -40.0), 3, -1, -1, 0 },
    #1#
    
    { FVector2D(+40.0, -40.0), FVector2D(-24.0, -40.0), 0, -1, -1, 0 },
    { FVector2D(-24.0, -40.0), FVector2D(-24.0, +40.0), 0, -1, -1, 0 },
    { FVector2D(-24.0, +40.0), FVector2D(+40.0, +40.0), 0, +1, -1, 0 },
    { FVector2D(+40.0, +40.0), FVector2D(+40.0, -40.0), 0, -1, -1, 0 },
    
    { FVector2D(-24.0, -40.0), FVector2D(-16.0, -40.0), 1, -1, -1, 0 },
    { FVector2D(-16.0, -40.0), FVector2D(-16.0, +40.0), 1, -1, -1, 0 },
    { FVector2D(-16.0, +40.0), FVector2D(-24.0, +40.0), 1, -1, -1, 0 },
    { FVector2D(-24.0, +40.0), FVector2D(-24.0, -40.0), 1, -1, -1, 0 },
    
    /*
    { FVector2D(40.0, -40.0), FVector2D(-40.0, -40.0), 0, -1, -1, 0 },
    { FVector2D(-40.0, -40.0), FVector2D(-40.0, 40.0), 0, -1, -1, 0 },
    { FVector2D(-40.0, 40.0), FVector2D(40.0, 40.0), 0, -1, -1, 0 },
    { FVector2D(40.0, 40.0), FVector2D(40.0, -40.0), 0, -1, -1, 0 },
    #1#
    
    /*
    { FVector2D(-24.0, -24.0), FVector2D(-24.0, 24.0), 4, -1, -1, 0 },
    { FVector2D(-24.0, 24.0), FVector2D(-16.0, 24.0), 5, -1, -1, 0 },
    { FVector2D(-16.0, 24.0), FVector2D(-16.0, -24.0), 6, -1, -1, 0 },
    { FVector2D(-16.0, -24.0), FVector2D(16.0, -8.0), 7, -1, -1, 0 },
    { FVector2D(16.0, -8.0), FVector2D(16.0, 8.0), 8, -1, -1, 0 },
    { FVector2D(16.0, 8.0), FVector2D(24.0, 8.0), 9, -1, -1, 0 },
    { FVector2D(24.0, 8.0), FVector2D(24.0, -8.0), 10, -1, -1, 0 },
    { FVector2D(24.0, -8.0), FVector2D(40.0, -40.0), 11, -1, -1, 0 },
    #1#
    
    { FVector2D(56.0, -24.0), FVector2D(56.0, 24.0), 0, -1, -1, 0 },
    { FVector2D(56.0, 24.0), FVector2D(48.0, 24.0), 1, -1, -1, 0 },
    { FVector2D(48.0, 24.0), FVector2D(48.0, -24.0), 2, -1, -1, 0 },
    { FVector2D(48.0, -24.0), FVector2D(56.0, -24.0), 3, -1, -1, 0 },

    { FVector2D(72.0, -8.0), FVector2D(72.0, 8.0), 4, -1, -1, 0 },
    { FVector2D(72.0, 8.0), FVector2D(64.0, 8.0), 5, -1, -1, 0 },
    { FVector2D(64.0, 8.0), FVector2D(64.0, -8.0), 6, -1, -1, 0 },
    { FVector2D(64.0, -8.0), FVector2D(72.0, -8.0), 7, -1, -1, 0 },
};

static inline constexpr FSector GSectors[] PROGMEM =
{
    { 0, 20, 0, 4 },
    { 0, 20, 4, 4 },
    { 0, 20, 12, 4 },
    { 0, 20, 16, 4 },
};
*/

/*
static inline constexpr FWall GWalls[] PROGMEM =
{
    // 0
    { FVector2D(-24, -3 ), FVector2D(-24, +32 ), 10, -1 , -1 , 0 },
};
*/

static inline constexpr FWall GWalls[] PROGMEM =
{
    // 0
    { FVector2D(-24, -32), { -24, +32 }, 10, -1 , -1 , 0 },
    { FVector2D(-24, +32), { +24, +8  }, 12, +12, -1 , 0 },
    { FVector2D(+24, +8 ), { +24, -8  }, 6 , -1 , -1 , 0 },
    { FVector2D(+24, -8 ), { -24, -32 }, 12, +7 , -1 , 0 },
    
    // 1
    { FVector2D(-40, -56), { -40, -32 }, 10, +8 , -1 , 0 },
    { FVector2D(-40, -32), { -24, -32 }, 10, +14, +56, 0 },
    { FVector2D(-24, -32), { -24, -56 }, 10, +7 , -1 , 0 },
    { FVector2D(-24, -56), { -40, -56 }, 10, -1 , -1 , 0 },
    
    // 2
    { FVector2D(+24, -56), { +24, -8  }, 6 , +7 , -1 , 0 },
    { FVector2D(+24, -8 ), { +40, -8  }, 6 , +13, +52, 0 },
    { FVector2D(+40, -8 ), { +40, -56 }, 6 , +4 , -1 , 0 },
    { FVector2D(+40, -56), { +24, -56 }, 6 , -1 , -1 , 0 },
    
    // 3
    { FVector2D(+24, +8 ), { +24, +56 }, 6 , +12, -1 , 0 },
    { FVector2D(+24, +56), { +40, +56 }, 6 , -1 , -1 , 0 },
    { FVector2D(+40, +56), { +40, +8  }, 6 , +6 , -1 , 0 },
    { FVector2D(+40, +8 ), { +24, +8  }, 6 , +13, +54, 0 },
    
    // 4
    { FVector2D(+40, -56), { +40, -8  }, 6 , +2 , -1 , 0 },
    { FVector2D(+40, -8 ), { +64, -8  }, 6 , +5 , -1 , 0 },
    { FVector2D(+64, -8 ), { +64, -56 }, 6 , -1 , -1 , 0 },
    { FVector2D(+64, -56), { +40, -56 }, 6 , -1 , -1 , 0 },
    
    // 5
    { FVector2D(+40, -8 ), { +40, +8  }, 6 , -1 , -1 , 0 },
    { FVector2D(+40, +8 ), { +64, +8  }, 6 , +6 , -1 , 0 },
    { FVector2D(+64, +8 ), { +64, -8  }, 6 , -1 , -1 , 0 },
    { FVector2D(+64, -8 ), { +40, -8  }, 6 , +4 , -1 , 0 },
    
    // 6
    { FVector2D(+40, +8 ), { +40, +56 }, 6 , +3 , -1 , 0 },
    { FVector2D(+40, +56), { +64, +56 }, 6 , -1 , -1 , 0 },
    { FVector2D(+64, +56), { +64, +8  }, 6 , -1 , -1 , 0 },
    { FVector2D(+64, +8 ), { +40, +8  }, 6 , +5 , -1 , 0 },
    
    // 7
    { FVector2D(-24, -56), { -24, -32 }, 12, +1 , -1 , 0 },
    { FVector2D(-24, -32), { +24, -8  }, 12, +0 , -1 , 0 },
    { FVector2D(+24, -8 ), { +24, -56 }, 12, +2 , -1 , 0 },
    { FVector2D(+24, -56), { -24, -56 }, 12, -1 , -1 , 0 },
    
    // 8
    { FVector2D(-64, -56), { -64, -32 }, 10, -1 , -1 , 0 },
    { FVector2D(-64, -32), { -40, -32 }, 10, +9 , -1 , 0 },
    { FVector2D(-40, -32), { -40, -56 }, 10, +1 , -1 , 0 },
    { FVector2D(-40, -56), { -64, -56 }, 10, -1 , -1 , 0 },
    
    // 9
    { FVector2D(-64, -32), { -64, +32 }, 10, -1 , -1 , 0 },
    { FVector2D(-64, +32), { -40, +32 }, 10, +10, -1 , 0 },
    { FVector2D(-40, +32), { -40, -32 }, 10, -1 , -1 , 0 },
    { FVector2D(-40, -32), { -64, -32 }, 10, +8 , -1 , 0 },
    
    // 10
    { FVector2D(-64, +32), { -64, +56 }, 10, -1 , -1 , 0 },
    { FVector2D(-64, +56), { -40, +56 }, 10, -1 , -1 , 0 },
    { FVector2D(-40, +56), { -40, +32 }, 10, +11, -1 , 0 },
    { FVector2D(-40, +32), { -64, +32 }, 10, +9 , -1 , 0 },
    
    // 11
    { FVector2D(-40, +32), { -40, +56 }, 10, +10, -1 , 0 },
    { FVector2D(-40, +56), { -24, +56 }, 10, -1 , -1 , 0 },
    { FVector2D(-24, +56), { -24, +32 }, 10, +12, -1 , 0 },
    { FVector2D(-24, +32), { -40, +32 }, 10, +14, +58, 0 },
    
    // 12
    { FVector2D(-24, +32), { -24, +56 }, 12, +11, -1 , 0 },
    { FVector2D(-24, +56), { +24, +56 }, 12, -1 , -1 , 0 },
    { FVector2D(+24, +56), { +24, +8  }, 12, +3 , -1 , 0 },
    { FVector2D(+24, +8 ), { -24, +32 }, 12, +0 , -1 , 0 },
    
    // 13
    { FVector2D(+16, +0 ), { +0 , +0  }, 10, +2 , +9 , 0 },
    { FVector2D(+16, +64), { +16, +0  }, 10, -1 , -1 , 0 },
    { FVector2D(+0 , +64), { +16, +64 }, 10, +3 , +15, 0 },
    { FVector2D(+0 , +0 ), { +0 , +64 }, 10, -1 , -1 , 0 },
    
    // 14
    { FVector2D(+16, +0 ), { +0 , +0  }, 6 , +1 , +5 , 0 },
    { FVector2D(+16, +16), { +16, +0  }, 6 , -1 , -1 , 0 },
    { FVector2D(+0 , +16), { +16, +16 }, 6 , +11, +47, 0 },
    { FVector2D(+0 , +0 ), { +0 , +16 }, 6 , -1 , -1 , 0 },
};

static inline constexpr FSector GSectors[] PROGMEM =
{
    { 0, 20, 0 , 4 },
    { 0, 20, 4 , 4 },
    { 0, 20, 8 , 4 },
    { 0, 20, 12, 4 },
    { 0, 20, 16, 4 },
    { 0, 20, 20, 4 },
    { 0, 20, 24, 4 },
    { 0, 20, 28, 4 },
    { 0, 20, 32, 4 },
    { 0, 20, 36, 4 },
    { 0, 20, 40, 4 },
    { 0, 20, 44, 4 },
    { 0, 20, 48, 4 },
    { 0, 20, 52, 4 },
    { 0, 20, 56, 4 },
};
