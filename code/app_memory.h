#if !defined(APP_MEMORY_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Mohamed Shazan $
   $Notice: All Rights Reserved. $
   ======================================================================== */

/* NOTE: PROGRAMMING!
   RAII = bad :(
   ZII = good :)
*/

platform_api Platform;

struct memory_arena
{
    // TODO: If we see perf problems here, maybe move Used/Base/Size out?
    platform_memory_block *CurrentBlock;
    umm MinimumBlockSize;
    
    u64 AllocationFlags;
    s32 TempCount;
};

struct temporary_memory
{
    memory_arena *Arena;
    platform_memory_block *Block;
    umm Used;
};

#define ZeroStruct(Instance) ZeroSize(sizeof(Instance), &(Instance))
#define ZeroArray(Count, Pointer) ZeroSize(Count*sizeof((Pointer)[0]), Pointer)
inline void
ZeroSize(memory_index Size, void *Ptr)
{
    uint8 *Byte = (uint8 *)Ptr;
    while(Size--)
    {
        *Byte++ = 0;
    }
}

inline void
SetMinimumBlockSize(memory_arena *Arena, memory_index MinimumBlockSize)
{
    Arena->MinimumBlockSize = MinimumBlockSize;
}

inline memory_index
GetAlignmentOffset(memory_arena *Arena, memory_index Alignment)
{
    memory_index AlignmentOffset = 0;
    
    memory_index ResultPointer = (memory_index)Arena->CurrentBlock->Base + Arena->CurrentBlock->Used;
    memory_index AlignmentMask = Alignment - 1;
    if(ResultPointer & AlignmentMask)
    {
        AlignmentOffset = Alignment - (ResultPointer & AlignmentMask);
    }

    return(AlignmentOffset);
}

enum arena_push_flag
{
    ArenaFlag_ClearToZero = 0x1,
};
struct arena_push_params
{
    u32 Flags;
    u32 Alignment;
};

inline arena_push_params
DefaultArenaParams(void)
{
    arena_push_params Params;
    Params.Flags = ArenaFlag_ClearToZero;
    Params.Alignment = 4;
    return(Params);
}

inline arena_push_params
AlignNoClear(u32 Alignment)
{
    arena_push_params Params = DefaultArenaParams();
    Params.Flags &= ~ArenaFlag_ClearToZero;
    Params.Alignment = Alignment;
    return(Params);
}

inline arena_push_params
Align(u32 Alignment, b32 Clear)
{
    arena_push_params Params = DefaultArenaParams();
    if(Clear)
    {
        Params.Flags |= ArenaFlag_ClearToZero;
    }
    else
    {
        Params.Flags &= ~ArenaFlag_ClearToZero;
    }
    Params.Alignment = Alignment;
    return(Params);
}

inline arena_push_params
NoClear(void)
{
    arena_push_params Params = DefaultArenaParams();
    Params.Flags &= ~ArenaFlag_ClearToZero;
    return(Params);
}

struct arena_bootstrap_params
{
    u64 AllocationFlags;
    umm MinimumBlockSize;
};

inline arena_bootstrap_params
DefaultBootstrapParams(void)
{
    arena_bootstrap_params Params = {};
    return(Params);
}

inline arena_bootstrap_params
NonRestoredArena(void)
{
    arena_bootstrap_params Params = DefaultBootstrapParams();
    Params.AllocationFlags = PlatformMemory_NotRestored;
    return(Params);
}

// TODO: Optional "clear" parameter!!!!
#define PushStruct(Arena, type, ...) (type *)PushSize_(Arena, sizeof(type), ## __VA_ARGS__)
#define PushArray(Arena, Count, type, ...) (type *)PushSize_(Arena, (Count)*sizeof(type), ## __VA_ARGS__)
#define PushSize(Arena, Size, ...) PushSize_(Arena, Size, ## __VA_ARGS__)
#define PushCopy(Arena, Size, Source, ...) Copy(Size, Source, PushSize_(Arena, Size, ## __VA_ARGS__))
inline memory_index
GetEffectiveSizeFor(memory_arena *Arena, memory_index SizeInit, arena_push_params Params = DefaultArenaParams())
{
    memory_index Size = SizeInit;
        
    memory_index AlignmentOffset = GetAlignmentOffset(Arena, Params.Alignment);
    Size += AlignmentOffset;

    return(Size);
}

inline void *
PushSize_(memory_arena *Arena, memory_index SizeInit, arena_push_params Params = DefaultArenaParams())
{
    void *Result = 0;
    
    memory_index Size = 0;
    if(Arena->CurrentBlock)
    {
        Size = GetEffectiveSizeFor(Arena, SizeInit, Params);
    }
/*    
    if(!Arena->CurrentBlock ||
       (Arena->CurrentBlock->Used + Size) > Arena->CurrentBlock->Size)
    {
        Size = SizeInit; // NOTE: The base will automatically be aligned now!
        if(Arena->AllocationFlags & (PlatformMemory_OverflowCheck|
                                     PlatformMemory_UnderflowCheck))
        {
            Arena->MinimumBlockSize = 0;
            Size = AlignPow2(Size, Params.Alignment);
        }
        else if(!Arena->MinimumBlockSize)
        {
            // TODO: Tune default block size eventually?
            Arena->MinimumBlockSize = 1024*1024;
        }
        
        memory_index BlockSize = Maximum(Size, Arena->MinimumBlockSize);
        
        platform_memory_block *NewBlock = 
            Platform.AllocateMemory(BlockSize, Arena->AllocationFlags);
        NewBlock->ArenaPrev = Arena->CurrentBlock;
        Arena->CurrentBlock = NewBlock;
    }    
*/    
    Assert((Arena->CurrentBlock->Used + Size) <= Arena->CurrentBlock->Size);
    
    memory_index AlignmentOffset = GetAlignmentOffset(Arena, Params.Alignment);
    Result = Arena->CurrentBlock->Base + Arena->CurrentBlock->Used + AlignmentOffset;
    Arena->CurrentBlock->Used += Size;
    
    Assert(Size >= SizeInit);

    if(Params.Flags & ArenaFlag_ClearToZero)
    {
        ZeroSize(SizeInit, Result);
    }
    
    return(Result);
}

// NOTE: This is generally not for production use, this is probably
// only really something we need during testing, but who knows
inline char *
PushString(memory_arena *Arena, char *Source)
{
    u32 Size = 1;
    for(char *At = Source;
        *At;
        ++At)
    {
        ++Size;
    }
    
    char *Dest = (char *)PushSize_(Arena, Size, NoClear());
    for(u32 CharIndex = 0;
        CharIndex < Size;
        ++CharIndex)
    {
        Dest[CharIndex] = Source[CharIndex];
    }

    return(Dest);
}

inline char *
PushAndNullTerminate(memory_arena *Arena, u32 Length, char *Source)
{
    char *Dest = (char *)PushSize_(Arena, Length + 1, NoClear());
    for(u32 CharIndex = 0;
        CharIndex < Length;
        ++CharIndex)
    {
        Dest[CharIndex] = Source[CharIndex];
    }
    Dest[Length] = 0;

    return(Dest);
}

inline temporary_memory
BeginTemporaryMemory(memory_arena *Arena)
{
    temporary_memory Result;

    Result.Arena = Arena;
    Result.Block = Arena->CurrentBlock;
    Result.Used = Arena->CurrentBlock ? Arena->CurrentBlock->Used : 0;

    ++Arena->TempCount;

    return(Result);
}

inline void
FreeLastBlock(memory_arena *Arena)
{
    platform_memory_block *Free = Arena->CurrentBlock;
    Arena->CurrentBlock = Free->ArenaPrev;
    Platform.DeallocateMemory(Free);
}

inline void
EndTemporaryMemory(temporary_memory TempMem)
{
    memory_arena *Arena = TempMem.Arena;
    while(Arena->CurrentBlock != TempMem.Block)
    {
        FreeLastBlock(Arena);
    }
    
    if(Arena->CurrentBlock)
    {
        Assert(Arena->CurrentBlock->Used >= TempMem.Used);
        Arena->CurrentBlock->Used = TempMem.Used;
        Assert(Arena->TempCount > 0);
    }
    --Arena->TempCount;
}

inline void
Clear(memory_arena *Arena)
{
    while(Arena->CurrentBlock)
    {
        FreeLastBlock(Arena);
    }
}

inline void
CheckArena(memory_arena *Arena)
{
    Assert(Arena->TempCount == 0);
}

inline void *
Copy(memory_index Size, void *SourceInit, void *DestInit)
{
    u8 *Source = (u8 *)SourceInit;
    u8 *Dest = (u8 *)DestInit;
    while(Size--) {*Dest++ = *Source++;}

    return(DestInit);
}

#define BootstrapPushStruct(type, Member, ...) (type *)BootstrapPushSize_(sizeof(type), OffsetOf(type, Member), ## __VA_ARGS__)
inline void *
BootstrapPushSize_(umm StructSize, umm OffsetToArena,
                   arena_bootstrap_params BootstrapParams = DefaultBootstrapParams(), 
                   arena_push_params Params = DefaultArenaParams())
{
    memory_arena Bootstrap = {};
    Bootstrap.AllocationFlags = BootstrapParams.AllocationFlags;
    Bootstrap.MinimumBlockSize = BootstrapParams.MinimumBlockSize;
    void *Struct = PushSize(&Bootstrap, StructSize, Params);
    *(memory_arena *)((u8 *)Struct + OffsetToArena) = Bootstrap;
    
    return(Struct);
}




#define APP_MEMORY_H
#endif
