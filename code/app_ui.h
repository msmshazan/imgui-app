
#if !defined(APP_UI_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Mohamed Shazan $
   $Notice: All Rights Reserved. $
   ======================================================================== */


#include "appcode.h"

#define UNUSED(a) (void)a
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) < (b) ? (b) : (a))
#define LEN(a) (sizeof(a)/sizeof(a)[0])

#define UIPARAM ui_context* Context,float X,float Y,float Width,float Height,app_data* Appdata

internal int CreateSubWindow(UIPARAM,const char* Title){
/* window flags */
    static int show_menu = true;
    static int titlebar = true;
    static int border = true;
    static int resize = true;
    static int movable = true;
    static int no_scrollbar = false;
    static int minimizable = true;
    static int closeable = true;
    static int background = false;
    static int scalable = true;
    static int autohide_scrollbar = false;

    /* popups */
    static enum ui_style_header_align header_align = UI_HEADER_RIGHT;
    
    /* window flags */
    ui_flags WindowFlags = 0;
    Context->style.window.header.align = header_align;
    if(border) WindowFlags |=UI_WINDOW_BORDER;            /* Draws a border around the window to visually separate the window from the background */
    if(movable) WindowFlags |=UI_WINDOW_MOVABLE;            /* The movable flag indicates that a window can be moved by user input or by dragging the window header */
    if(scalable) WindowFlags |=UI_WINDOW_SCALABLE;          /* The scalable flag indicates that a window can be scaled by user input by dragging a scaler icon at the button of the window */
    if(closeable) WindowFlags |=UI_WINDOW_CLOSABLE;         /* adds a closable icon into the header */
    if(minimizable) WindowFlags |=UI_WINDOW_MINIMIZABLE;        /* adds a minimize icon into the header */
    if(no_scrollbar) WindowFlags |=UI_WINDOW_NO_SCROLLBAR;       /* Removes the scrollbar from the window */
    if(titlebar) WindowFlags |=UI_WINDOW_TITLE;              /* Forces a header at the top at the window showing the title */
    if(autohide_scrollbar) WindowFlags |=UI_WINDOW_SCROLL_AUTO_HIDE;   /* Automatically hides the window scrollbar if no user interaction: also requires delta time in `context` to be set each frame */
    if(background) WindowFlags |=UI_WINDOW_BACKGROUND;        /* Always keep window in the background */
    
    if(ui_begin(Context,Title,ui_rect(X,Y,Width,Height),WindowFlags))
    {
        if (show_menu)
        {
            /* menubar */
            enum menu_states {MENU_DEFAULT, MENU_WINDOWS};
            static int mslider = 10;
            static int mcheck = ui_true;

            ui_menubar_begin(Context);
            ui_layout_row_dynamic(Context, 25, 2);
            if (ui_menu_begin_label(Context, "MENU", UI_TEXT_LEFT, ui_vec2(120, 200)))
            {
                static size_t prog = 40;
                static int slider = 10;
                static int check = ui_true;
                ui_layout_row_dynamic(Context, 25, 1);
                ui_progress(Context, &prog, 100, UI_MODIFIABLE);
                ui_slider_int(Context, 0, &slider, 16, 1);
                ui_checkbox_label(Context, "check", &check);
                ui_menu_end(Context);
            }
            if (ui_menu_begin_label(Context, "EDIT", UI_TEXT_LEFT, ui_vec2(120, 200)))
            {
                static size_t prog = 40;
                static int slider = 10;
                static int check = ui_true;
                ui_layout_row_dynamic(Context, 25, 1);
                ui_progress(Context, &prog, 100, UI_MODIFIABLE);
                ui_slider_int(Context, 0, &slider, 16, 1);
                ui_checkbox_label(Context, "check", &check);
                ui_menu_end(Context);
            }
        }
        ui_menubar_end(Context);
        
    }
    ui_end(Context);
    return !ui_window_is_closed(Context,Title);    
    
}

internal void LoginWindow(UIPARAM,const char* Title = "#Login"){

/* window flags */
    static int titlebar = false;
    static int border = false;
    static int resize = false;
    static int movable = false;
    static int no_scrollbar = true;
    static int minimizable = false;
    static int closeable = false;
    static int background = false;
    static int scalable = false;
    static int autohide_scrollbar = false;

    /* popups */
    static enum ui_style_header_align header_align = UI_HEADER_RIGHT;
    
    /* window flags */
    ui_flags WindowFlags = 0;
    Context->style.window.header.align = header_align;
    if(border) WindowFlags |=UI_WINDOW_BORDER;            /* Draws a border around the window to visually separate the window from the background */
    if(movable) WindowFlags |=UI_WINDOW_MOVABLE;            /* The movable flag indicates that a window can be moved by user input or by dragging the window header */
    if(scalable) WindowFlags |=UI_WINDOW_SCALABLE;          /* The scalable flag indicates that a window can be scaled by user input by dragging a scaler icon at the button of the window */
    if(closeable) WindowFlags |=UI_WINDOW_CLOSABLE;         /* adds a closable icon into the header */
    if(minimizable) WindowFlags |=UI_WINDOW_MINIMIZABLE;        /* adds a minimize icon into the header */
    if(no_scrollbar) WindowFlags |=UI_WINDOW_NO_SCROLLBAR;       /* Removes the scrollbar from the window */
    if(titlebar) WindowFlags |=UI_WINDOW_TITLE;              /* Forces a header at the top at the window showing the title */
    if(autohide_scrollbar) WindowFlags |=UI_WINDOW_SCROLL_AUTO_HIDE;   /* Automatically hides the window scrollbar if no user interaction: also requires delta time in `context` to be set each frame */
    if(background) WindowFlags |=UI_WINDOW_BACKGROUND;        /* Always keep window in the background */

    b32 loggedin = 0;
    if(ui_begin(Context,Title,ui_rect(X,Y,Width,Height),WindowFlags))
    {
        ui_layout_row_dynamic(Context,25,2);
        if(ui_button_label(Context,"Login")){
            loggedin = true;
        }
        if(ui_button_label(Context,"Quit")) CloseApp(Context);
    }
    ui_end(Context);
    if(loggedin) Appdata->Loggedin = true;
            
}

internal int CreateMainWindow(UIPARAM)
{
    /* window flags */
    static int show_menu = true;
    static int titlebar = true;
    static int border = false;
    static int resize = false;
    static int movable = true;
    static int no_scrollbar = false;
    static int minimizable = false;
    static int closeable = false;
    static int background = true;
    static int scalable = false;
    static int autohide_scrollbar = true;

    /* popups */
    static enum ui_style_header_align header_align = UI_HEADER_RIGHT;
    
    /* window flags */
    ui_flags WindowFlags = 0;
    Context->style.window.header.align = header_align;
    if(border) WindowFlags |=UI_WINDOW_BORDER;            /* Draws a border around the window to visually separate the window from the background */
    if(movable) WindowFlags |=UI_WINDOW_MOVABLE;            /* The movable flag indicates that a window can be moved by user input or by dragging the window header */
    if(scalable) WindowFlags |=UI_WINDOW_SCALABLE;          /* The scalable flag indicates that a window can be scaled by user input by dragging a scaler icon at the button of the window */
    if(closeable) WindowFlags |=UI_WINDOW_CLOSABLE;         /* adds a closable icon into the header */
    if(minimizable) WindowFlags |=UI_WINDOW_MINIMIZABLE;        /* adds a minimize icon into the header */
    if(no_scrollbar) WindowFlags |=UI_WINDOW_NO_SCROLLBAR;       /* Removes the scrollbar from the window */
    if(titlebar) WindowFlags |=UI_WINDOW_TITLE;              /* Forces a header at the top at the window showing the title */
    if(autohide_scrollbar) WindowFlags |=UI_WINDOW_SCROLL_AUTO_HIDE;   /* Automatically hides the window scrollbar if no user interaction: also requires delta time in `context` to be set each frame */
    if(background) WindowFlags |=UI_WINDOW_BACKGROUND;        /* Always keep window in the background */

    if(ui_begin_titled(Context,"#Main","Testing_App",ui_rect(X,Y,Width,Height),WindowFlags))
    {
        if (show_menu)
        {
            /* menubar */
            enum menu_states {MENU_DEFAULT, MENU_WINDOWS};
            static int mslider = 10;
            static int mcheck = ui_true;
            
            ui_menubar_begin(Context);
            ui_layout_row_dynamic(Context , 25,5);
            if (ui_menu_begin_label(Context, "MENU", UI_TEXT_CENTERED, ui_vec2(120, 200)))
            {
                static size_t prog = 40;
                static int slider = 10;
                static int check = ui_true;
                ui_layout_row_dynamic(Context, 25, 1);
                ui_progress(Context, &prog, 100, UI_MODIFIABLE);
                ui_slider_int(Context, 0, &slider, 16, 1);
                ui_checkbox_label(Context, "check", &check);
                if(ui_button_label(Context,"New")) Appdata->Window[0] = true;
                    
                ui_menu_end(Context);
            }
            if (ui_menu_begin_label(Context, "EDIT", UI_TEXT_CENTERED, ui_vec2(120, 200)))
            {
                static size_t prog = 40;
                static int slider = 10;
                static int check = ui_true;
                ui_layout_row_dynamic(Context, 25, 1);
                ui_progress(Context, &prog, 100, UI_MODIFIABLE);
                ui_slider_int(Context, 0, &slider, 16, 1);
                ui_checkbox_label(Context, "check", &check);
                ui_menu_end(Context);
            }
            if(ui_menu_begin_label(Context,"Login",UI_TEXT_CENTERED,ui_vec2(120, 200))) {
                if(!Appdata->Loggedin) Appdata->Window[2] = true;
                ui_menu_end(Context);
            }
        if(ui_menu_begin_label(Context,"EXIT",UI_TEXT_CENTERED,ui_vec2(120, 200))) {
                CloseApp(Context);
                ui_menu_end(Context);
            }
        }
        ui_menubar_end(Context);
            
    }
    ui_end(Context);
    
    return !ui_window_is_closed(Context,"#Main");    
}



#undef UIPARAM
#define APP_UI_H
#endif
