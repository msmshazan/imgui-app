/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Mohamed Shazan $
   $Notice: All Rights Reserved. $
   ======================================================================== */


// WINDOWS CONSOLE vs GUI
#if 1
#define WINDOWS_ENTRY_POINT int main(void)
#else
#define WINDOWS_ENTRY_POINT int                 \
    CALLBACK WinMain(HINSTANCE Instance,        \
                     HINSTANCE hPrevInstance,   \
                     LPSTR     lpCmdLine,       \
                     int       nCmdShow)

#endif

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <limits.h>

#define WINDOW_WIDTH 1080
#define WINDOW_HEIGHT 720
#include "platform.h"
#include "win32layer.h"

#define UNUSED(a) (void)a
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) < (b) ? (b) : (a))
#define LEN(a) (sizeof(a)/sizeof(a)[0])


internal void
CatStrings(size_t SourceACount, const char *SourceA,
           size_t SourceBCount, const char *SourceB,
           size_t DestCount, char *Dest) {
    for (size_t Index = 0; Index < SourceACount && Index < DestCount; ++Index) {
        *Dest++ = *SourceA++;
    }

    for (size_t Index = 0; Index < SourceBCount && Index < DestCount; ++Index) {
        *Dest++ = *SourceB++;
    }

    *Dest++ = 0;
}
inline FILETIME
Win32GetLastWriteTime(const char *Filename) {
    FILETIME LastWriteTime = {};

    WIN32_FILE_ATTRIBUTE_DATA Data;
    if (GetFileAttributesEx(Filename, GetFileExInfoStandard, &Data)) {
        LastWriteTime = Data.ftLastWriteTime;
    }
    return LastWriteTime;
}

internal void
ToggleFullscreen(HWND Window,WINDOWPLACEMENT GlobalWindowPosition)
{
    // NOTE: This follows Raymond Chen's prescription
    // for fullscreen toggling, see:
    // http://blogs.msdn.com/b/oldnewthing/archive/2010/04/12/9994016.aspx

    DWORD Style = GetWindowLong(Window, GWL_STYLE);
    if(Style & WS_OVERLAPPEDWINDOW)
    {
        MONITORINFO MonitorInfo = {sizeof(MonitorInfo)};
        if(GetWindowPlacement(Window, &GlobalWindowPosition) &&
           GetMonitorInfo(MonitorFromWindow(Window, MONITOR_DEFAULTTOPRIMARY), &MonitorInfo))
        {
            SetWindowLong(Window, GWL_STYLE, Style & ~WS_OVERLAPPEDWINDOW);
            SetWindowPos(Window, HWND_TOP,
                         MonitorInfo.rcMonitor.left, MonitorInfo.rcMonitor.top,
                         MonitorInfo.rcMonitor.right - MonitorInfo.rcMonitor.left,
                         MonitorInfo.rcMonitor.bottom - MonitorInfo.rcMonitor.top,
                         SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
        }
    }
    else
    {
        SetWindowLong(Window, GWL_STYLE, Style | WS_OVERLAPPEDWINDOW);
        SetWindowPlacement(Window, &GlobalWindowPosition);
        SetWindowPos(Window, 0, 0, 0, 0, 0,
                     SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
                     SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
    }
}

internal win32_app_code
Win32LoadAppCode(const char *SourceDLLName) {

    win32_app_code Result = {};
    Result.TempDLLName = "Appcode_temp.dll";
    Result.DLLLastWriteTime = Win32GetLastWriteTime(SourceDLLName);

    CopyFile(SourceDLLName, Result.TempDLLName, FALSE);
    Result.AppCodeDLL = LoadLibrary(Result.TempDLLName);

    if (Result.AppCodeDLL) {
        Result.UpdateAndRender = (app_update_and_render *)GetProcAddress(Result.AppCodeDLL, "AppUpdateAndRender");
        Result.GetSoundSamples = (app_get_sound_samples *)GetProcAddress(Result.AppCodeDLL, "AppGetSoundSamples");

        Result.IsValid = (Result.UpdateAndRender && Result.GetSoundSamples);
    }

    if (!Result.IsValid) {
        Result.UpdateAndRender = 0;
        Result.GetSoundSamples = 0;
    }

    return Result;
}
internal void
Win32UnloadAppCode(win32_app_code *AppCode) {
    if (AppCode->AppCodeDLL) {
        FreeLibrary(AppCode->AppCodeDLL);
    }

    DeleteFile(AppCode->TempDLLName);
    AppCode->IsValid = false;
    AppCode->UpdateAndRender = 0;
    AppCode->GetSoundSamples = 0;
}


internal LRESULT CALLBACK
WindowProc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    if (ui_gdi_handle_event(wnd, msg, wparam, lparam))
        return 0;

    return DefWindowProcW(wnd, msg, wparam, lparam);
}
internal win32_window_dimension
Win32GetWindowDimension(HWND Window)
{
    win32_window_dimension Result;

    RECT ClientRect;
    GetClientRect(Window, &ClientRect);
    Result.Width = ClientRect.right - ClientRect.left;
    Result.Height = ClientRect.bottom - ClientRect.top;

    return(Result);
}

inline LARGE_INTEGER
Win32GetWallClock(void)
{    
    LARGE_INTEGER Result;
    QueryPerformanceCounter(&Result);
    return(Result);
}

inline real32
Win32GetSecondsElapsed(LARGE_INTEGER Start, LARGE_INTEGER End , int64 PerfCountFrequency)
{
    real32 Result = ((real32)(End.QuadPart - Start.QuadPart) /
                     (real32)PerfCountFrequency);
    return(Result);
}


WINDOWS_ENTRY_POINT
{
    GdiFont* font;
    struct ui_context *Context;

    WNDCLASSW wc;
    ATOM atom;
    RECT rect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
    DWORD style = WS_OVERLAPPEDWINDOW;
    DWORD exstyle = WS_EX_APPWINDOW;
    HWND wnd;
    HDC dc;
    
    /* Win32 */
    memset(&wc, 0, sizeof(wc));
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandleW(0);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = L"WindowClass";
    atom = RegisterClassW(&wc);
    internal WINDOWPLACEMENT GlobalWindowPosition = {sizeof(GlobalWindowPosition)};

    AdjustWindowRectEx(&rect, style, FALSE, exstyle);
    wnd = CreateWindowExW(exstyle, wc.lpszClassName, L"Demo",
                          style | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT,
                          rect.right - rect.left, rect.bottom - rect.top,
                          NULL, NULL, wc.hInstance, NULL);
    dc = GetDC(wnd);

    LARGE_INTEGER PerfCountFrequencyResult;
    QueryPerformanceFrequency(&PerfCountFrequencyResult);
    int64 PerfCountFrequency = PerfCountFrequencyResult.QuadPart;

    /* GUI */
    font = ui_gdifont_create("LiberationMono", 20);
    ToggleFullscreen(wnd,GlobalWindowPosition);
    win32_window_dimension Dim = Win32GetWindowDimension(wnd);
    Context = ui_gdi_init(font, dc, Dim.Width, Dim.Height);
    
    /* style.h */
    //set_style(Context, THEME_WHITE);
    /*set_style(Context, THEME_RED);*/
    set_style(Context, THEME_BLUE);
    /*set_style(Context, THEME_DARK);*/
    //ui_style_default(Context);
    char* SourceAppCodeDLL = "appcode.dll";
    win32_app_code AppCode ={};
    app_memory Memory = {};
    app_offscreen_buffer Buffer= {};
    Buffer.Width=(float)Dim.Width;
    Buffer.Height=(float)Dim.Height;
    Memory.PermanentStorageSize =Megabytes(200);
    Memory.TransientStorageSize =Gigabytes(1);
    memory_index TotalSize = Memory.PermanentStorageSize + Memory.TransientStorageSize;
    LPVOID BaseAddress = (LPVOID) Terabytes(2);
    Memory.PermanentStorage = VirtualAlloc(BaseAddress,
                                           TotalSize,
                                           MEM_RESERVE|MEM_COMMIT,
                                           PAGE_READWRITE);
    Memory.TransientStorage = ((uint8 *)Memory.PermanentStorage + Memory.PermanentStorageSize);    

    Context->userdata.ptr = &GlobalRunning;
    LARGE_INTEGER LastCounter= Win32GetWallClock();
    while (GlobalRunning)
    {
        FILETIME NewDLLWriteTime = Win32GetLastWriteTime(SourceAppCodeDLL);
        if (CompareFileTime(&NewDLLWriteTime, &AppCode.DLLLastWriteTime) != 0) {
            Win32UnloadAppCode(&AppCode);
            AppCode = Win32LoadAppCode(SourceAppCodeDLL);
        }

        /* Input */
        MSG msg;
        ui_input_begin(Context);
            if (GetMessageW(&msg, NULL, 0, 0) <= 0)
                GlobalRunning = false;
            else {
                TranslateMessage(&msg);
                DispatchMessageW(&msg);
            }

        while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT)
                GlobalRunning = false;
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
        
        ui_input_end(Context);
        AppCode.UpdateAndRender(&Memory,Context,&Buffer);
        /* Draw */
        ui_gdi_render(ui_rgb(0,50,100));


    LARGE_INTEGER EndCounter= Win32GetWallClock();
    Context->delta_time_seconds =1000.0f*Win32GetSecondsElapsed(LastCounter,EndCounter,PerfCountFrequency);
    LastCounter = EndCounter;
    }

    Win32UnloadAppCode(&AppCode);            
    ui_gdifont_del(font);
    ReleaseDC(wnd, dc);
    UnregisterClassW(wc.lpszClassName, wc.hInstance);
    return 0;
}

