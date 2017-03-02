#if !defined(APPCODE_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Mohamed Shazan $
   $Notice: All Rights Reserved. $
   ======================================================================== */
#include "platform.h"


enum AppState{
    Login = (1 << 0),
    MainWindow = (1 << 1),
    SubWindow = (1 << 2),
    Quit = (1 << 3)
    
};

struct userid{
    int id;
    char* name;
};
struct app_data{
    userid User; 
#define MAXWINDOWS  100
    b32 Window[MAXWINDOWS];
#undef MAXWINDOWS
    b32 Loggedin;
};


static void CloseApp(ui_context* Context)
{
   b32* GlobalRunning = (b32 *)Context->userdata.ptr;
  *GlobalRunning = false;
}


#define APPCODE_H
#endif
