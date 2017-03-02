/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Mohamed Shazan $
   $Notice: All Rights Reserved. $
   ======================================================================== */

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <math.h>

#include "appcode.h"
#include "app_ui.h"
#include "app_memory.h"



extern "C" APP_UPDATE_AND_RENDER(AppUpdateAndRender)
{
    app_data *Data = (app_data *)Memory->PermanentStorage;
    if(!Memory->IsInitialized){    
        Memory->IsInitialized = true;
        Data->Loggedin = false;
    }
        CreateMainWindow(Context,0,0,Buffer->Width,Buffer->Height,Data);
        if(Data->Window[0]) Data->Window[0] = CreateSubWindow(Context,200,300,500,500,Data,"New"); 
        if(Data->Window[1]) Data->Window[1] = CreateSubWindow(Context,200,300,500,500,Data,"Start");
        if(Data->Window[2]) LoginWindow(Context,Buffer->Width/2 - 200,Buffer->Height/2 - 200,400,400,Data);
                    
}

extern "C" APP_GET_SOUND_SAMPLES(AppGetSoundSamples)
{
    
}
