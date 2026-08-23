#pragma once

#include "Pin.h"
#include "../Core/Aliases.h"

template<uint8 InPin>
struct TPinButton
{
    
private:
    enum EFlags : uint8
    {
        None     = 0,
        Current  = 1 << 0,
        Previous = 1 << 1
    };
    
    using Pin = TPin<InPin>;
    static inline uint8 Flags = EFlags::None;
    
public:
    FORCEINLINE static void Initialize()
    {
        Pin::SetupAsInput();
        Pin::EnablePullUp();
    }
    
    FORCEINLINE static void Update()
    {
        (Flags & EFlags::Current) ? Flags |=   EFlags::Previous : Flags &= ~(EFlags::Previous);
        Pin::Get()                ? Flags &= ~(EFlags::Current) : Flags |=   EFlags::Current  ;
    }
    
    FORCEINLINE static bool IsPressed()   { return  (Flags & EFlags::Current); }
    FORCEINLINE static bool IsReleased()  { return !(Flags & EFlags::Current); }
    
    FORCEINLINE static bool WasPressed()  { return  (Flags & EFlags::Current) && !(Flags & EFlags::Previous); }
    FORCEINLINE static bool WasReleased() { return !(Flags & EFlags::Current) &&  (Flags & EFlags::Previous); }
    
};
