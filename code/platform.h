#if !defined(PLATFORM_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Mohamed Shazan $
   $Notice: All Rights Reserved. $
   ======================================================================== */


#define UI_INCLUDE_COMMAND_USERDATA
#define UI_INCLUDE_FIXED_TYPES
#define UI_INCLUDE_STANDARD_IO
#define UI_INCLUDE_STANDARD_VARARGS
#define UI_INCLUDE_DEFAULT_ALLOCATOR
#define UI_IMPLEMENTATION

#include "ui.h"
#include "ui_style.h"


#include <assert.h>
#include <stdio.h>
//
// NOTE: Types
//
#include <stdint.h>
#include <stddef.h>
#include <limits.h>
#include <float.h>

#include <stdint.h>
#include <stddef.h>
#include <limits.h>
#include <float.h>
    
typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef int32 bool32;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef intptr_t intptr;
typedef uintptr_t uintptr;

typedef size_t memory_index;
    
typedef float real32;
typedef double real64;
    
typedef int8 s8;
typedef int8 s08;
typedef int16 s16;
typedef int32 s32;
typedef int64 s64;
typedef bool32 b32;
typedef b32 b32x;

typedef uint8 u8;
typedef uint8 u08;
typedef uint16 u16;
typedef uint32 u32;
typedef uint64 u64;

typedef real32 r32;
typedef real64 r64;
typedef real32 f32;
typedef real64 f64;
    
typedef uintptr_t umm;
typedef intptr_t smm;
    
#define U16Maximum 65535
#define U32Maximum ((u32)-1)
#define Real32Maximum FLT_MAX
#define Real32Minimum -FLT_MAX
#define R32Maximum FLT_MAX
#define R32Minimum -FLT_MAX

    
#define U32FromPointer(Pointer) ((u32)(memory_index)(Pointer))
#define PointerFromU32(type, Value) (type *)((memory_index)Value)

#define OffsetOf(type, Member) (umm)&(((type *)0)->Member)

#if !defined(internal)
#define internal static
#endif
#define global_variable static
#define local_persist static

#define Pi32 3.14159265359f
#define Tau32 6.2831853071795864692f


#if HANDMADE_SLOW
#define Assert(Expression) assert(Expression)
#else
#define Assert(Expression)
#endif

#define Kilobytes(Value) ((Value) * 1024LL)
#define Megabytes(Value) (Kilobytes(Value) * 1024LL)
#define Gigabytes(Value) (Megabytes(Value) * 1024LL)
#define Terabytes(Value) (Gigabytes(Value) * 1024LL)

#define ArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))

// TODO: swap, min, max ... macros ???

#define AlignPow2(Value, Alignment) ((Value + (Alignment - 1)) & ~(Alignment - 1))
#define Align4(Value) (((Value) + 3) & ~3)
#define Align8(Value) (((Value) + 7) & ~7)
#define Align16(Value) (((Value) + 15) & ~15)



inline uint32
SafeTruncateUInt64(uint64 Value) {
    // TODO: Defines for maximum values UInt32Max
    Assert(Value <= 0xFFFFFFFF);
    uint32 Result = (uint32) Value;
    return Result;
}

inline u16
SafeTruncateToUInt16(s32 Value) {
    // TODO: Defines for maximum values UInt32Max
    Assert(Value <= 65535);
    Assert(Value >= 0);
    u16 Result = (u16)Value;
    return Result;
}

inline s16
SafeTruncateToInt16(s32 Value) {
    // TODO: Defines for maximum values UInt32Max
    Assert(Value <= 32767);
    Assert(Value >= -32768);
    u16 Result = (s16)Value;
    return Result;
}
/*
 * NOTE: Services that the platform layer provides to the app.
 */
/* IMPORTANT:

   These are NOT for doing anything in the shipping app - they are
   blocking and the write doesn't protect against lost data!
 */
typedef struct debug_read_file_result {
    uint32 ContentsSize;
    void *Contents;
} debug_read_file_result;

#define DEBUG_PLATFORM_FREE_FILE_MEMORY(name) void name(void *Memory)
typedef DEBUG_PLATFORM_FREE_FILE_MEMORY(debug_platform_free_file_memory);

#define DEBUG_PLATFORM_READ_ENTIRE_FILE(name) debug_read_file_result name(const char *Filename)
typedef DEBUG_PLATFORM_READ_ENTIRE_FILE(debug_platform_read_entire_file);

#define DEBUG_PLATFORM_WRITE_ENTIRE_FILE(name) bool32 name(const char *Filename, uint32 MemorySize, void *Memory)
typedef DEBUG_PLATFORM_WRITE_ENTIRE_FILE(debug_platform_write_entire_file);

// TODO: Actually stoping using this
extern struct app_memory *DebugGlobalMemory;


/*
 * NOTE: Services that the app provides to the platform layer.
 */


typedef struct app_offscreen_buffer {
    void *Memory;
    real32 Width;
    real32 Height;
    int Pitch;
} app_offscreen_buffer;

typedef struct app_sound_output_buffer {
    int SamplesPerSecond;
    int SampleCount;

    // IMPORTANT: Samples must be padded to a multiple of 4 samples!
    int16 *Samples;
} app_sound_output_buffer;

typedef struct app_button_state {
    int HalfTransitionCount;
    bool32 EndedDown;
} app_button_state;

inline b32
WasPressed(app_button_state State) {
    b32 Result = ((State.HalfTransitionCount > 1) ||
                  (State.HalfTransitionCount == 1 ) && State.EndedDown);
    return Result;
}


typedef struct platform_file_handle {
    b32 NoErrors;
    void *Platform;
} platform_file_handle;

typedef struct platform_file_group {
    u32 FileCount;
    void *Platform;
} platform_file_group;

typedef enum platform_file_type {
    PlatformFileType_AssetFile,
    PlatformFileType_SavedAppFile,

    PlatformFileType_Count,
} platform_file_type;
    
enum platform_memory_block_flags
{
    PlatformMemory_NotRestored = 0x1,
    PlatformMemory_OverflowCheck = 0x2,
    PlatformMemory_UnderflowCheck = 0x4,
};
struct platform_memory_block
{
    u64 Flags;
    u64 Size;
    u8 *Base;
    umm Used;
    platform_memory_block *ArenaPrev;
};

#define PLATFORM_GET_ALL_FILES_OF_TYPE_BEGIN(name) platform_file_group name(platform_file_type Type)
typedef PLATFORM_GET_ALL_FILES_OF_TYPE_BEGIN(platform_get_all_files_of_type_begin);

#define PLATFORM_GET_ALL_FILES_OF_TYPE_END(name) void name(platform_file_group *FileGroup)
typedef PLATFORM_GET_ALL_FILES_OF_TYPE_END(platform_get_all_files_of_type_end);

#define PLATFORM_OPEN_NEXT_FILE(name) platform_file_handle name(platform_file_group *FileGroup)
typedef PLATFORM_OPEN_NEXT_FILE(platform_open_next_file);

#define PLATFORM_READ_DATA_FROM_FILE(name) void name(platform_file_handle *Source, u64 Offset, u64 Size, void *Dest)
typedef PLATFORM_READ_DATA_FROM_FILE(platform_read_data_from_file);

#define PLATFORM_FILE_ERROR(name) void name(platform_file_handle *Handle, char *Message)
typedef PLATFORM_FILE_ERROR(platform_file_error);

#define PlatformNoFileErrors(Handle) ((Handle)->NoErrors)

struct platform_work_queue;

#define PLATFORM_WORK_QUEUE_CALLBACK(name) void name(platform_work_queue *Queue, void *Data)
typedef PLATFORM_WORK_QUEUE_CALLBACK(platform_work_queue_callback);

#define PLATFORM_ALLOCATE_MEMORY(name) void *name(memory_index Size)
typedef PLATFORM_ALLOCATE_MEMORY(platform_allocate_memory);

#define PLATFORM_DEALLOCATE_MEMORY(name) void name(void *Memory)
typedef PLATFORM_DEALLOCATE_MEMORY(platform_deallocate_memory);

typedef void platform_add_entry(platform_work_queue *Queue, platform_work_queue_callback *Callback, void *Data);
typedef void platform_complete_all_work(platform_work_queue *Queue);

typedef struct platform_api {
    platform_add_entry *AddEntry;
    platform_complete_all_work *CompleteAllWork;

    platform_get_all_files_of_type_begin *GetAllFilesOfTypeBegin;
    platform_get_all_files_of_type_end *GetAllFilesOfTypeEnd;
    platform_open_next_file *OpenNextFile;
    platform_read_data_from_file *ReadDataFromFile;
    platform_file_error *FileError;

    platform_allocate_memory *AllocateMemory;
    platform_deallocate_memory *DeallocateMemory;

        
#if HANDMADE_INTERNAL
    debug_platform_free_file_memory *DEBUGFreeFileMemory;    
    debug_platform_read_entire_file *DEBUGReadEntireFile;
    debug_platform_write_entire_file *DEBUGWriteEntireFile;
    debug_platform_execute_system_command *DEBUGExecuteSystemCommand;
    debug_platform_get_process_state *DEBUGGetProcessState;
    debug_platform_get_memory_stats *DEBUGGetMemoryStats;
#endif

}platform_api;

typedef struct app_memory{
    memory_index PermanentStorageSize;
    void *PermanentStorage; // NOTE: REQUIRED to be cleared to zero at startup

    memory_index TransientStorageSize;
    void *TransientStorage; // NOTE: REQUIRED to be cleared to zero at startup

    memory_index DebugStorageSize;
    void *DebugStorage; // NOTE: REQUIRED to be cleared to zero at startup

    platform_work_queue *HighPriorityQueue;
    platform_work_queue *LowPriorityQueue;

    bool32 IsInitialized;

    platform_api PlatformAPI;

}app_memory;
 

#define APP_UPDATE_AND_RENDER(name) void name(app_memory *Memory,ui_context *Context,app_offscreen_buffer *Buffer)
typedef APP_UPDATE_AND_RENDER(app_update_and_render);

// NOTE: At the moment, this has to be a very fast function, it cannot be
// more than a millisecond or so.
// TODO: Reduce the pressure on this function's performance by measuring it
// or asking about it, etc.
#define APP_GET_SOUND_SAMPLES(name) void name(app_memory *Memory, app_sound_output_buffer *SoundBuffer)
typedef APP_GET_SOUND_SAMPLES(app_get_sound_samples);

#define PLATFORM_H
#endif
