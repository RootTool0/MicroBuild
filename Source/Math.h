// MicroBuild

#pragma once

#include "Structures.h"

struct FMath
{
    
public:
    template<typename T> static constexpr T Min(const T InA, const T InB) { return InA < InB ? InA : InB; }
    template<typename T> static constexpr T Max(const T InA, const T InB) { return InA > InB ? InA : InB; }
    template<typename T> static constexpr T Min3(const T InA, const T InB, const T InC) { return Min(Min(InA, InB), InC); }
    template<typename T> static constexpr T Max3(const T InA, const T InB, const T InC) { return Max(Max(InA, InB), InC); }
    template<typename T> static constexpr T Clamp(const T InValue, const T InMin, const T InMax) { return Min(Max(InValue, InMin), InMax); }
    template<typename T> static constexpr T Abs(const T InF) { return InF < 0 ? -InF : InF; }
    template<typename T> static constexpr T Lerp(const T InA, const T InB, const FFixedFloat InAlpha) { return static_cast<T>((FFixedFloat(InA) + FFixedFloat(InB - InA) * InAlpha).ToInt()); }
    
    template<typename T>
    static void Swap(T& InA, T& InB)
    {
        T Temp = static_cast<T&&>(InA);
        InA = static_cast<T&&>(InB);
        InB = static_cast<T&&>(Temp);
    }
    
    static FFixedFloat::FFixedFloatInt Floor(const FFixedFloat InF) { return InF.ToInt(); }
    
    static FFixedFloat Abs(const FFixedFloat InF) { return InF.Raw < 0 ? FFixedFloat::FromRaw(-InF.Raw) : InF; }
    
    static FFixedFloat Sin(const FAngle InAngle)
    {
        static_assert(sizeof(FFixedFloat) == 4, "[FMath::Sin] FPROGMEM read FFixedFloat size mismatch!");
        
        const uint8 Quadrant = InAngle >> 6;
        const uint8 Index = InAngle & 0x3F;
        
        const FFixedFloat Value = FFixedFloat::FromRaw(FPROGMEM::ReadInt32(&SinLUT[(Quadrant == 0 || Quadrant == 2) ? Index : (63 - Index)]));
        return Quadrant >= 2 ? -Value : Value;
    }
    
    static FFixedFloat Cos(const FAngle InAngle) { return Sin(InAngle + 64); }
    
    static bool IsLineSegmentsIntersect(const FFloatVector2D A, const FFloatVector2D B, const FFloatVector2D C, const FFloatVector2D D)
    {
        const FFixedFloat Denominator = (B.X - A.X) * (D.Y - C.Y) - (B.Y - A.Y) * (D.X - C.X);
        if(Denominator == FFixedFloat::Zero) return false;
        
        const FFixedFloat U = ((C.X - A.X) * (D.Y - C.Y) - (C.Y - A.Y) * (D.X - C.X)) / Denominator;
        const FFixedFloat I = ((C.X - A.X) * (B.Y - A.Y) - (C.Y - A.Y) * (B.X - A.X)) / Denominator;
        
        return U >= FFixedFloat::Zero && U <= FFixedFloat::One && I >= FFixedFloat::Zero && I <= FFixedFloat::One;
    }

private:
    /*
    for (int i = 0; i < 64; i++)
    {
        double Angle = (static_cast<double>(i) / 256.0) * 2.0 * 3.141592653589793;
        FMath::SinLUT[i] = FFixedFloat(sinf(Angle));
        printf("FFixedFloat::FromRaw(%i), ", FMath::SinLUT[i].Raw);
    }
    */
    
    static constexpr FFixedFloat SinLUT[64] PROGMEM = 
    {
        FFixedFloat::FromRaw(0    ), FFixedFloat::FromRaw(1608 ), FFixedFloat::FromRaw(3215 ), FFixedFloat::FromRaw(4821 ),
        FFixedFloat::FromRaw(6423 ), FFixedFloat::FromRaw(8022 ), FFixedFloat::FromRaw(9616 ), FFixedFloat::FromRaw(11204),
        FFixedFloat::FromRaw(12785), FFixedFloat::FromRaw(14359), FFixedFloat::FromRaw(15923), FFixedFloat::FromRaw(17479),
        FFixedFloat::FromRaw(19024), FFixedFloat::FromRaw(20557), FFixedFloat::FromRaw(22078), FFixedFloat::FromRaw(23586),
        FFixedFloat::FromRaw(25079), FFixedFloat::FromRaw(26557), FFixedFloat::FromRaw(28020), FFixedFloat::FromRaw(29465),
        FFixedFloat::FromRaw(30893), FFixedFloat::FromRaw(32302), FFixedFloat::FromRaw(33692), FFixedFloat::FromRaw(35061),
        FFixedFloat::FromRaw(36409), FFixedFloat::FromRaw(37736), FFixedFloat::FromRaw(39039), FFixedFloat::FromRaw(40319),
        FFixedFloat::FromRaw(41575), FFixedFloat::FromRaw(42806), FFixedFloat::FromRaw(44011), FFixedFloat::FromRaw(45189),
        FFixedFloat::FromRaw(46340), FFixedFloat::FromRaw(47464), FFixedFloat::FromRaw(48558), FFixedFloat::FromRaw(49624),
        FFixedFloat::FromRaw(50660), FFixedFloat::FromRaw(51665), FFixedFloat::FromRaw(52639), FFixedFloat::FromRaw(53581),
        FFixedFloat::FromRaw(54491), FFixedFloat::FromRaw(55368), FFixedFloat::FromRaw(56212), FFixedFloat::FromRaw(57022),
        FFixedFloat::FromRaw(57797), FFixedFloat::FromRaw(58538), FFixedFloat::FromRaw(59243), FFixedFloat::FromRaw(59913),
        FFixedFloat::FromRaw(60547), FFixedFloat::FromRaw(61144), FFixedFloat::FromRaw(61705), FFixedFloat::FromRaw(62228), 
        FFixedFloat::FromRaw(62714), FFixedFloat::FromRaw(63162), FFixedFloat::FromRaw(63571), FFixedFloat::FromRaw(63943),
        FFixedFloat::FromRaw(64276), FFixedFloat::FromRaw(64571), FFixedFloat::FromRaw(64826), FFixedFloat::FromRaw(65043),
        FFixedFloat::FromRaw(65220), FFixedFloat::FromRaw(65358), FFixedFloat::FromRaw(65457), FFixedFloat::FromRaw(65516)
    };
};
