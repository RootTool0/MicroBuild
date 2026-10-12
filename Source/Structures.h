// MicroBuild

#pragma once

struct FColor
{
    
public:
    uint8 Data = 0;
    
    constexpr FColor() = default;
    constexpr FColor(const uint16 InData) : Data(InData) {}
//  constexpr FColor(const uint8 InR, const uint8 InG, const uint8 InB) : Data(((uint16)(InR & 0xF8) << 8) | ((uint16)(InG & 0xFC) << 3) | ((uint16)(InB & 0xF8) >> 3)) {}
    constexpr FColor(const uint8 InR, const uint8 InG, const uint8 InB) : Data(InR) {}
    
    static const FColor Black;
};

constexpr FColor FColor::Black = FColor(0, 0, 0);

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

template<typename T>
struct TVector2D
{
    
public:
    using ScalarType = T;
    
    T X;
    T Y;
    
    constexpr TVector2D() : X(0), Y(0) {}
    constexpr TVector2D(const T InF) : X(InF), Y(InF) {}
    constexpr TVector2D(const T InX, const T InY) : X(InX), Y(InY) {}
    
    constexpr TVector2D operator-() const { return { -X, -Y }; }
    constexpr TVector2D operator+(const TVector2D& Other) const { return TVector2D( X + Other.X, Y + Other.Y ); }
    constexpr TVector2D operator-(const TVector2D& Other) const { return TVector2D( X - Other.X, Y - Other.Y ); }
    constexpr TVector2D operator*(const TVector2D& Other) const { return TVector2D( X * Other.X, Y * Other.Y ); }
    constexpr TVector2D operator/(const TVector2D& Other) const { return TVector2D( X / Other.X, Y / Other.Y ); }
    constexpr TVector2D operator*(const T Scalar) const { return { X * Scalar, Y * Scalar }; }
    constexpr TVector2D operator/(const T Scalar) const { return { X / Scalar, Y / Scalar }; }
    
    constexpr TVector2D& operator+=(const TVector2D& Other) { *this = *this + Other; return *this; }
    constexpr TVector2D& operator-=(const TVector2D& Other) { *this = *this - Other; return *this; }
    constexpr TVector2D& operator*=(const TVector2D& Other) { *this = *this * Other; return *this; }
    constexpr TVector2D& operator/=(const TVector2D& Other) { *this = *this / Other; return *this; }
    constexpr TVector2D& operator*=(const T Scalar) { *this = *this * Scalar; return *this; }
    constexpr TVector2D& operator/=(const T Scalar) { *this = *this / Scalar; return *this; }
};

using FAngle = uint8;
// using FIntVector2D = TVector2D<int16>;

struct FIntVector2D : public TVector2D<int8>
{
    
public:
    using TVector2D<int8>::TVector2D;
    
    constexpr FIntVector2D DivByTwo() const { return { static_cast<int8>(X >> 1), static_cast<int8>(Y >> 1) }; }
    constexpr FIntVector2D MulByTwo() const { return { static_cast<int8>(X << 1), static_cast<int8>(Y << 1) }; }
};

struct FFloatVector2D : public TVector2D<FFixedFloat>
{
    
public:
    using TVector2D<FFixedFloat>::TVector2D;

    // constexpr FFloatVector2D(const FIntVector2D InIntVector2D) : X(InIntVector2D.X), Y(InIntVector2D.Y) {}
    constexpr FFloatVector2D(const FIntVector2D InIntVector2D) : TVector2D<FFixedFloat>(InIntVector2D.X, InIntVector2D.Y) {}
    constexpr FFloatVector2D(const TVector2D<FFixedFloat> InVector2D)  : TVector2D<FFixedFloat>(InVector2D.X, InVector2D.Y) {}
    
    constexpr FFloatVector2D DivByTwo() const { return { X.DivByTwo(), Y.DivByTwo() }; }
    constexpr FFloatVector2D MulByTwo() const { return { X.MulByTwo(), Y.MulByTwo() }; }
};

struct FPlayer
{
    
public:
    static constexpr int8 Height = 8;
    
    static inline FFloatVector2D LocationXY;
    static inline FAngle Angle = 32;
    static inline int8 CurrentSector = 0;
    static inline int8 LocationZ = 10;
};

struct FWall
{
    
public:
    FIntVector2D Point1, Point2;
    uint8 WallPaletteIndex; // Used as color
    int8 PortalNextSector;
    int8 PortalNextWall;
    FAngle PortalAngleDelta;
    
    FORCEINLINE static FIntVector2D GetPoint1           (const FWall& InFlashWall);
    FORCEINLINE static FIntVector2D GetPoint2           (const FWall& InFlashWall);
    FORCEINLINE static uint8        GetWallPaletteIndex (const FWall& InFlashWall);
    FORCEINLINE static int8         GetPortalNextSector (const FWall& InFlashWall);
    FORCEINLINE static int8         GetPortalNextWall   (const FWall& InFlashWall);
    FORCEINLINE static FAngle       GetPortalAngleDelta (const FWall& InFlashWall);
};

struct FSector
{
    
public:
    int8 FloorZ;
    int8 CeilZ;
    uint8 WallStartIndex;
    uint8 WallCount;
    uint8 CeilPaletteIndex;  // Used as color
    uint8 FloorPaletteIndex; // Used as color
    
    FORCEINLINE static int8  GetFloorZ            (const FSector& InFlashSector);
    FORCEINLINE static int8  GetCeilZ             (const FSector& InFlashSector);
    FORCEINLINE static uint8 GetWallStartIndex    (const FSector& InFlashSector);
    FORCEINLINE static uint8 GetWallCount         (const FSector& InFlashSector);
    FORCEINLINE static uint8 GetCeilPaletteIndex  (const FSector& InFlashSector);
    FORCEINLINE static uint8 GetFloorPaletteIndex (const FSector& InFlashSector);
};

