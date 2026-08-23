// MicroBuild

#pragma once

#include "AAT/Core/Aliases.h"

#include "Structures.h"

struct FRenderTask
{
    
public:
    int8 InSectorIndex;
    int16 InMinX;
    int16 InMaxX;
    FVector2D InCameraLocation;
    FAngle InCameraAngle;
};

struct FRenderTaskQueue
{
    
public:
    static inline FRenderTask Queue[RENDER_TASK_QUEUE_ELEMENTS] = {};
    
    static void Empty() { Head = 0; Tail = 0; }
    
    static bool AddTask(const FRenderTask&& InRenderTask)
    {
        if(Head >= RENDER_TASK_QUEUE_ELEMENTS) return false;
        Queue[Head++] = InRenderTask;
        return true;
    }
    
    static int8 PopTaskIndex()
    {
        return Tail >= Head || Tail >= RENDER_TASK_QUEUE_ELEMENTS ? -1 : Tail++;
    }

private:
    static inline int8 Head = 0;
    static inline int8 Tail = 0;
};
