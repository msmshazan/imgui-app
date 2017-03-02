/*
 Nuklear - 1.33.0 - public domain
 no warrenty implied; use at your own risk.
 authored from 2015-2016 by Micha Mettke

ABOUT:
    This is a minimal state graphical user interface single header toolkit
    written in ANSI C and licensed under public domain.
    It was designed as a simple embeddable user interface for application and does
    not have any dependencies, a default renderbackend or OS window and input handling
    but instead provides a very modular library approach by using simple input state
    for input and draw commands describing primitive shapes as output.
    So instead of providing a layered library that tries to abstract over a number
    of platform and render backends it only focuses on the actual UI.

VALUES:
    - Graphical user interface toolkit
    - Single header library
    - Written in C89 (a.k.a. ANSI C or ISO C90)
    - Small codebase (~17kLOC)
    - Focus on portability, efficiency and simplicity
    - No dependencies (not even the standard library if not wanted)
    - Fully skinnable and customizable
    - Low memory footprint with total memory control if needed or wanted
    - UTF-8 support
    - No global or hidden state
    - Customizable library modules (you can compile and use only what you need)
    - Optional font baker and vertex buffer output

USAGE:
    This library is self contained in one single header file and can be used either
    in header only mode or in implementation mode. The header only mode is used
    by default when included and allows including this header in other headers
    and does not contain the actual implementation.

    The implementation mode requires to define  the preprocessor macro
    UI_IMPLEMENTATION in *one* .c/.cpp file before #includeing this file, e.g.:

        #define UI_IMPLEMENTATION
        #include "nuklear.h"

    Also optionally define the symbols listed in the section "OPTIONAL DEFINES"
    below in header and implementation mode if you want to use additional functionality
    or need more control over the library.
    IMPORTANT:  Every time you include "nuklear.h" you have to define the same flags.
                This is very important not doing it either leads to compiler errors
                or even worse stack corruptions.

FEATURES:
    - Absolutely no platform dependend code
    - Memory management control ranging from/to
        - Ease of use by allocating everything from standard library
        - Control every byte of memory inside the library
    - Font handling control ranging from/to
        - Use your own font implementation for everything
        - Use this libraries internal font baking and handling API
    - Drawing output control ranging from/to
        - Simple shapes for more high level APIs which already have drawing capabilities
        - Hardware accessible anti-aliased vertex buffer output
    - Customizable colors and properties ranging from/to
        - Simple changes to color by filling a simple color table
        - Complete control with ability to use skinning to decorate widgets
    - Bendable UI library with widget ranging from/to
        - Basic widgets like buttons, checkboxes, slider, ...
        - Advanced widget like abstract comboboxes, contextual menus,...
    - Compile time configuration to only compile what you need
        - Subset which can be used if you do not want to liui or use the standard library
    - Can be easily modified to only update on user input instead of frame updates

OPTIONAL DEFINES:
    UI_PRIVATE
        If defined declares all functions as static, so they can only be accessed
        inside the file that contains the implementation

    UI_INCLUDE_FIXED_TYPES
        If defined it will include header <stdint.h> for fixed sized types
        otherwise nuklear tries to select the correct type. If that fails it will
        throw a compiler error and you have to select the correct types yourself.
        <!> If used needs to be defined for implementation and header <!>

    UI_INCLUDE_DEFAULT_ALLOCATOR
        if defined it will include header <stdlib.h> and provide additional functions
        to use this library without caring for memory allocation control and therefore
        ease memory management.
        <!> Adds the standard library with malloc and free so don't define if you
            don't want to liui to the standard library <!>
        <!> If used needs to be defined for implementation and header <!>

    UI_INCLUDE_STANDARD_IO
        if defined it will include header <stdio.h> and provide
        additional functions depending on file loading.
        <!> Adds the standard library with fopen, fclose,... so don't define this
            if you don't want to liui to the standard library <!>
        <!> If used needs to be defined for implementation and header <!>

    UI_INCLUDE_STANDARD_VARARGS
        if defined it will include header <stdarg.h> and provide
        additional functions depending on variable arguments
        <!> Adds the standard library with va_list and  so don't define this if
            you don't want to liui to the standard library<!>
        <!> If used needs to be defined for implementation and header <!>

    UI_INCLUDE_VERTEX_BUFFER_OUTPUT
        Defining this adds a vertex draw command list backend to this
        library, which allows you to convert queue commands into vertex draw commands.
        This is mainly if you need a hardware accessible format for OpenGL, DirectX,
        Vulkan, Metal,...
        <!> If used needs to be defined for implementation and header <!>

    UI_INCLUDE_FONT_BAKING
        Defining this adds the `stb_truetype` and `stb_rect_pack` implementation
        to this library and provides font baking and rendering.
        If you already have font handling or do not want to use this font handler
        you don't have to define it.
        <!> If used needs to be defined for implementation and header <!>

    UI_INCLUDE_DEFAULT_FONT
        Defining this adds the default font: ProggyClean.ttf into this library
        which can be loaded into a font atlas and allows using this library without
        having a truetype font
        <!> Enabling this adds ~12kb to global stack memory <!>
        <!> If used needs to be defined for implementation and header <!>

    UI_INCLUDE_COMMAND_USERDATA
        Defining this adds a userdata pointer into each command. Can be useful for
        example if you want to provide custom shaders depending on the used widget.
        Can be combined with the style structures.
        <!> If used needs to be defined for implementation and header <!>

    UI_BUTTON_TRIGGER_ON_RELEASE
        Different platforms require button clicks occuring either on buttons being
        pressed (up to down) or released (down to up).
        By default this library will react on buttons being pressed, but if you
        define this it will only trigger if a button is released.
        <!> If used it is only required to be defined for the implementation part <!>

    UI_ZERO_COMMAND_MEMORY
        Defining this will zero out memory for each drawing command added to a
        drawing queue (inside ui_command_buffer_push). Zeroing command memory
        is very useful for fast checking (using memcmp) if command buffers are
        equal and avoid drawing frames when nothing on screen has changed since
        previous frame.

    UI_ASSERT
        If you don't define this, nuklear will use <assert.h> with assert().
        <!> Adds the standard library so define to nothing of not wanted <!>
        <!> If used needs to be defined for implementation and header <!>

    UI_BUFFER_DEFAULT_INITIAL_SIZE
        Initial buffer size allocated by all buffers while using the default allocator
        functions included by defining UI_INCLUDE_DEFAULT_ALLOCATOR. If you don't
        want to allocate the default 4k memory then redefine it.
        <!> If used needs to be defined for implementation and header <!>

    UI_MAX_NUMBER_BUFFER
        Maximum buffer size for the conversion buffer between float and string
        Under normal circumstances this should be more than sufficient.
        <!> If used needs to be defined for implementation and header <!>

    UI_INPUT_MAX
        Defines the max number of bytes which can be added as text input in one frame.
        Under normal circumstances this should be more than sufficient.
        <!> If used it is only required to be defined for the implementation part <!>

    UI_MEMSET
        You can define this to 'memset' or your own memset implementation
        replacement. If not nuklear will use its own version.
        <!> If used it is only required to be defined for the implementation part <!>

    UI_MEMCPY
        You can define this to 'memcpy' or your own memcpy implementation
        replacement. If not nuklear will use its own version.
        <!> If used it is only required to be defined for the implementation part <!>

    UI_SQRT
        You can define this to 'sqrt' or your own sqrt implementation
        replacement. If not nuklear will use its own slow and not highly
        accurate version.
        <!> If used it is only required to be defined for the implementation part <!>

    UI_SIN
        You can define this to 'sinf' or your own sine implementation
        replacement. If not nuklear will use its own approximation implementation.
        <!> If used it is only required to be defined for the implementation part <!>

    UI_COS
        You can define this to 'cosf' or your own cosine implementation
        replacement. If not nuklear will use its own approximation implementation.
        <!> If used it is only required to be defined for the implementation part <!>

    UI_STRTOD
        You can define this to `strtod` or your own string to double conversion
        implementation replacement. If not defined nuklear will use its own
        imprecise and possibly unsafe version (does not handle nan or infinity!).
        <!> If used it is only required to be defined for the implementation part <!>

    UI_DTOA
        You can define this to `dtoa` or your own double to string conversion
        implementation replacement. If not defined nuklear will use its own
        imprecise and possibly unsafe version (does not handle nan or infinity!).
        <!> If used it is only required to be defined for the implementation part <!>

    UI_VSNPRINTF
        If you define `UI_INCLUDE_STANDARD_VARARGS` as well as `UI_INCLUDE_STANDARD_IO`
        and want to be safe define this to `vsnprintf` on compilers supporting
        later versions of C or C++. By default nuklear will check for your stdlib version
        in C as well as compiler version in C++. if `vsnprintf` is available
        it will define it to `vsnprintf` directly. If not defined and if you have
        older versions of C or C++ it will be defined to `vsprintf` which is unsafe.
        <!> If used it is only required to be defined for the implementation part <!>

    UI_BYTE
    UI_INT16
    UI_UINT16
    UI_INT32
    UI_UINT32
    UI_SIZE_TYPE
    UI_POINTER_TYPE
        If you compile without UI_USE_FIXED_TYPE then a number of standard types
        will be selected and compile time validated. If they are incorrect you can
        define the correct types by overloading these type defines.

CREDITS:
    Developed by Micha Mettke and every direct or indirect contributor.

    Embeds stb_texedit, stb_truetype and stb_rectpack by Sean Barret (public domain)
    Embeds ProggyClean.ttf font by Tristan Grimmer (MIT license).

    Big thaui you to Omar Cornut (ocornut@github) for his imgui library and
    giving me the inspiration for this library, Casey Muratori for handmade hero
    and his original immediate mode graphical user interface idea and Sean
    Barret for his amazing single header libraries which restored my faith
    in libraries and brought me to create some of my own.

LICENSE:
    This software is dual-licensed to the public domain and under the following
    license: you are granted a perpetual, irrevocable license to copy, modify,
    publish and distribute this file as you see fit.
*/
#ifndef UI_NUKLEAR_H_
#define UI_NUKLEAR_H_

#ifdef __cplusplus
extern "C" {
#endif
/*
 * ==============================================================
 *
 *                          CONSTANTS
 *
 * ===============================================================
 */
#define UI_UNDEFINED (-1.0f)
#define UI_UTF_INVALID 0xFFFD /* internal invalid utf8 rune */
#define UI_UTF_SIZE 4 /* describes the number of bytes a glyph consists of*/
#ifndef UI_INPUT_MAX
#define UI_INPUT_MAX 16
#endif
#ifndef UI_MAX_NUMBER_BUFFER
#define UI_MAX_NUMBER_BUFFER 64
#endif
#ifndef UI_SCROLLBAR_HIDING_TIMEOUT
#define UI_SCROLLBAR_HIDING_TIMEOUT 4.0f
#endif

/*
 * ==============================================================
 *
 *                          HELPER
 *
 * ===============================================================
 */
#ifndef UI_API
#ifdef UI_PRIVATE
#define UI_API static
#else
#define UI_API extern
#endif
#endif

#define UI_INTERN static
#define UI_STORAGE static
#define UI_GLOBAL static

#define UI_FLAG(x) (1 << (x))
#define UI_STRINGIFY(x) #x
#define UI_MACRO_STRINGIFY(x) UI_STRINGIFY(x)
#define UI_STRING_JOIN_IMMEDIATE(arg1, arg2) arg1 ## arg2
#define UI_STRING_JOIN_DELAY(arg1, arg2) UI_STRING_JOIN_IMMEDIATE(arg1, arg2)
#define UI_STRING_JOIN(arg1, arg2) UI_STRING_JOIN_DELAY(arg1, arg2)

#ifdef _MSC_VER
#define UI_UNIQUE_NAME(name) UI_STRING_JOIN(name,__COUNTER__)
#else
#define UI_UNIQUE_NAME(name) UI_STRING_JOIN(name,__LINE__)
#endif

#ifndef UI_STATIC_ASSERT
#define UI_STATIC_ASSERT(exp) typedef char UI_UNIQUE_NAME(_dummy_array)[(exp)?1:-1]
#endif

#ifndef UI_FILE_LINE
#ifdef _MSC_VER
#define UI_FILE_LINE __FILE__ ":" UI_MACRO_STRINGIFY(__COUNTER__)
#else
#define UI_FILE_LINE __FILE__ ":" UI_MACRO_STRINGIFY(__LINE__)
#endif
#endif

#define UI_MIN(a,b) ((a) < (b) ? (a) : (b))
#define UI_MAX(a,b) ((a) < (b) ? (b) : (a))
#define UI_CLAMP(i,v,x) (UI_MAX(UI_MIN(v,x), i))

/*
 * ===============================================================
 *
 *                          BASIC
 *
 * ===============================================================
 */
#ifdef UI_INCLUDE_FIXED_TYPES
 #include <stdint.h>
 #define UI_INT8 int8_t
 #define UI_UINT8 uint8_t
 #define UI_INT16 int16_t
 #define UI_UINT16 uint16_t
 #define UI_INT32 int32_t
 #define UI_UINT32 uint32_t
 #define UI_SIZE_TYPE uintptr_t
 #define UI_POINTER_TYPE uintptr_t
#else
  #ifndef UI_INT8
    #define UI_INT8 char
  #endif
  #ifndef UI_UINT8
    #define UI_UINT8 unsigned char
  #endif
  #ifndef UI_INT16
    #define UI_INT16 signed short
  #endif
  #ifndef UI_UINT16
    #define UI_UINT16 unsigned short
  #endif
  #ifndef UI_INT32
    #if defined(_MSC_VER)
      #define UI_INT32 __int32
    #else
      #define UI_INT32 signed int
    #endif
  #endif
  #ifndef UI_UINT32
    #if defined(_MSC_VER)
      #define UI_UINT32 unsigned __int32
    #else
      #define UI_UINT32 unsigned int
    #endif
  #endif
  #ifndef UI_SIZE_TYPE
    #if defined(_WIN64) && defined(_MSC_VER)
      #define UI_SIZE_TYPE unsigned __int64
    #elif (defined(_WIN32) || defined(WIN32)) && defined(_MSC_VER)
      #define UI_SIZE_TYPE unsigned __int32
    #elif defined(__GNUC__) || defined(__clang__)
      #if defined(__x86_64__) || defined(__ppc64__)
        #define UI_SIZE_TYPE unsigned long
      #else
        #define UI_SIZE_TYPE unsigned int
      #endif
    #else
      #define UI_SIZE_TYPE unsigned long
    #endif
  #endif
  #ifndef UI_POINTER_TYPE
    #if defined(_WIN64) && defined(_MSC_VER)
      #define UI_POINTER_TYPE unsigned __int64
    #elif (defined(_WIN32) || defined(WIN32)) && defined(_MSC_VER)
      #define UI_POINTER_TYPE unsigned __int32
    #elif defined(__GNUC__) || defined(__clang__)
      #if defined(__x86_64__) || defined(__ppc64__)
        #define UI_POINTER_TYPE unsigned long
      #else
        #define UI_POINTER_TYPE unsigned int
      #endif
    #else
      #define UI_POINTER_TYPE unsigned long
    #endif
  #endif
#endif

typedef UI_INT8 ui_char;
typedef UI_UINT8 ui_uchar;
typedef UI_UINT8 ui_byte;
typedef UI_INT16 ui_short;
typedef UI_UINT16 ui_ushort;
typedef UI_INT32 ui_int;
typedef UI_UINT32 ui_uint;
typedef UI_SIZE_TYPE ui_size;
typedef UI_POINTER_TYPE ui_ptr;

typedef ui_uint ui_hash;
typedef ui_uint ui_flags;
typedef ui_uint ui_rune;

/* Make sure correct type size:
 * This will fire with a negative subscript error if the type sizes
 * are set incorrectly by the compiler, and compile out if not */
UI_STATIC_ASSERT(sizeof(ui_short) == 2);
UI_STATIC_ASSERT(sizeof(ui_ushort) == 2);
UI_STATIC_ASSERT(sizeof(ui_uint) == 4);
UI_STATIC_ASSERT(sizeof(ui_int) == 4);
UI_STATIC_ASSERT(sizeof(ui_byte) == 1);
UI_STATIC_ASSERT(sizeof(ui_flags) >= 4);
UI_STATIC_ASSERT(sizeof(ui_rune) >= 4);
UI_STATIC_ASSERT(sizeof(ui_size) >= sizeof(void*));
UI_STATIC_ASSERT(sizeof(ui_ptr) >= sizeof(void*));

/* ============================================================================
 *
 *                                  API
 *
 * =========================================================================== */
struct ui_buffer;
struct ui_allocator;
struct ui_command_buffer;
struct ui_draw_command;
struct ui_convert_config;
struct ui_style_item;
struct ui_text_edit;
struct ui_draw_list;
struct ui_user_font;
struct ui_panel;
struct ui_context;
struct ui_draw_vertex_layout_element;
struct ui_style_button;
struct ui_style_toggle;
struct ui_style_selectable;
struct ui_style_slide;
struct ui_style_progress;
struct ui_style_scrollbar;
struct ui_style_edit;
struct ui_style_property;
struct ui_style_chart;
struct ui_style_combo;
struct ui_style_tab;
struct ui_style_window_header;
struct ui_style_window;

enum {ui_false, ui_true};
struct ui_color {ui_byte r,g,b,a;};
struct ui_colorf {float r,g,b,a;};
struct ui_vec2 {float x,y;};
struct ui_vec2i {short x, y;};
struct ui_rect {float x,y,w,h;};
struct ui_recti {short x,y,w,h;};
typedef char ui_glyph[UI_UTF_SIZE];
typedef union {void *ptr; int id;} ui_handle;
struct ui_image {ui_handle handle;unsigned short w,h;unsigned short region[4];};
struct ui_cursor {struct ui_image img; struct ui_vec2 size, offset;};
struct ui_scroll {ui_uint x, y;};

enum ui_heading         {UI_UP, UI_RIGHT, UI_DOWN, UI_LEFT};
enum ui_button_behavior {UI_BUTTON_DEFAULT, UI_BUTTON_REPEATER};
enum ui_modify          {UI_FIXED = ui_false, UI_MODIFIABLE = ui_true};
enum ui_orientation     {UI_VERTICAL, UI_HORIZONTAL};
enum ui_collapse_states {UI_MINIMIZED = ui_false, UI_MAXIMIZED = ui_true};
enum ui_show_states     {UI_HIDDEN = ui_false, UI_SHOWN = ui_true};
enum ui_chart_type      {UI_CHART_LINES, UI_CHART_COLUMN, UI_CHART_MAX};
enum ui_chart_event     {UI_CHART_HOVERING = 0x01, UI_CHART_CLICKED = 0x02};
enum ui_color_format    {UI_RGB, UI_RGBA};
enum ui_popup_type      {UI_POPUP_STATIC, UI_POPUP_DYNAMIC};
enum ui_layout_format   {UI_DYNAMIC, UI_STATIC};
enum ui_tree_type       {UI_TREE_NODE, UI_TREE_TAB};
enum ui_anti_aliasing   {UI_ANTI_ALIASING_OFF, UI_ANTI_ALIASING_ON};

typedef void*(*ui_plugin_alloc)(ui_handle, void *old, ui_size);
typedef void (*ui_plugin_free)(ui_handle, void *old);
typedef int(*ui_plugin_filter)(const struct ui_text_edit*, ui_rune unicode);
typedef void(*ui_plugin_paste)(ui_handle, struct ui_text_edit*);
typedef void(*ui_plugin_copy)(ui_handle, const char*, int len);

struct ui_allocator {
    ui_handle userdata;
    ui_plugin_alloc alloc;
    ui_plugin_free free;
};

struct ui_draw_null_texture {
    ui_handle texture;/* texture handle to a texture with a white pixel */
    struct ui_vec2 uv; /* coordinates to a white pixel in the texture  */
};
struct ui_convert_config {
    float global_alpha; /* global alpha value */
    enum ui_anti_aliasing line_AA; /* line anti-aliasing flag can be turned off if you are tight on memory */
    enum ui_anti_aliasing shape_AA; /* shape anti-aliasing flag can be turned off if you are tight on memory */
    unsigned int circle_segment_count; /* number of segments used for circles: default to 22 */
    unsigned int arc_segment_count; /* number of segments used for arcs: default to 22 */
    unsigned int curve_segment_count; /* number of segments used for curves: default to 22 */
    struct ui_draw_null_texture null; /* handle to texture with a white pixel for shape drawing */
    const struct ui_draw_vertex_layout_element *vertex_layout; /* describes the vertex output format and packing */
    ui_size vertex_size; /* sizeof one vertex for vertex packing */
    ui_size vertex_alignment; /* vertex alignment: Can be optained by UI_ALIGNOF */
};

struct ui_list_view {
/* public: */
    int begin, end, count;
/* private: */
    int total_height;
    struct ui_context *ctx;
    ui_uint *scroll_pointer;
    ui_uint scroll_value;
};

enum ui_symbol_type {
    UI_SYMBOL_NONE,
    UI_SYMBOL_X,
    UI_SYMBOL_UNDERSCORE,
    UI_SYMBOL_CIRCLE_SOLID,
    UI_SYMBOL_CIRCLE_OUTLINE,
    UI_SYMBOL_RECT_SOLID,
    UI_SYMBOL_RECT_OUTLINE,
    UI_SYMBOL_TRIANGLE_UP,
    UI_SYMBOL_TRIANGLE_DOWN,
    UI_SYMBOL_TRIANGLE_LEFT,
    UI_SYMBOL_TRIANGLE_RIGHT,
    UI_SYMBOL_PLUS,
    UI_SYMBOL_MINUS,
    UI_SYMBOL_MAX
};

enum ui_keys {
    UI_KEY_NONE,
    UI_KEY_SHIFT,
    UI_KEY_CTRL,
    UI_KEY_DEL,
    UI_KEY_ENTER,
    UI_KEY_TAB,
    UI_KEY_BACKSPACE,
    UI_KEY_COPY,
    UI_KEY_CUT,
    UI_KEY_PASTE,
    UI_KEY_UP,
    UI_KEY_DOWN,
    UI_KEY_LEFT,
    UI_KEY_RIGHT,

    /* Shortcuts: text field */
    UI_KEY_TEXT_INSERT_MODE,
    UI_KEY_TEXT_REPLACE_MODE,
    UI_KEY_TEXT_RESET_MODE,
    UI_KEY_TEXT_LINE_START,
    UI_KEY_TEXT_LINE_END,
    UI_KEY_TEXT_START,
    UI_KEY_TEXT_END,
    UI_KEY_TEXT_UNDO,
    UI_KEY_TEXT_REDO,
    UI_KEY_TEXT_SELECT_ALL,
    UI_KEY_TEXT_WORD_LEFT,
    UI_KEY_TEXT_WORD_RIGHT,

    /* Shortcuts: scrollbar */
    UI_KEY_SCROLL_START,
    UI_KEY_SCROLL_END,
    UI_KEY_SCROLL_DOWN,
    UI_KEY_SCROLL_UP,

    UI_KEY_MAX
};

enum ui_buttons {
    UI_BUTTON_LEFT,
    UI_BUTTON_MIDDLE,
    UI_BUTTON_RIGHT,
    UI_BUTTON_MAX
};

enum ui_style_colors {
    UI_COLOR_TEXT,
    UI_COLOR_WINDOW,
    UI_COLOR_HEADER,
    UI_COLOR_BORDER,
    UI_COLOR_BUTTON,
    UI_COLOR_BUTTON_HOVER,
    UI_COLOR_BUTTON_ACTIVE,
    UI_COLOR_TOGGLE,
    UI_COLOR_TOGGLE_HOVER,
    UI_COLOR_TOGGLE_CURSOR,
    UI_COLOR_SELECT,
    UI_COLOR_SELECT_ACTIVE,
    UI_COLOR_SLIDER,
    UI_COLOR_SLIDER_CURSOR,
    UI_COLOR_SLIDER_CURSOR_HOVER,
    UI_COLOR_SLIDER_CURSOR_ACTIVE,
    UI_COLOR_PROPERTY,
    UI_COLOR_EDIT,
    UI_COLOR_EDIT_CURSOR,
    UI_COLOR_COMBO,
    UI_COLOR_CHART,
    UI_COLOR_CHART_COLOR,
    UI_COLOR_CHART_COLOR_HIGHLIGHT,
    UI_COLOR_SCROLLBAR,
    UI_COLOR_SCROLLBAR_CURSOR,
    UI_COLOR_SCROLLBAR_CURSOR_HOVER,
    UI_COLOR_SCROLLBAR_CURSOR_ACTIVE,
    UI_COLOR_TAB_HEADER,
    UI_COLOR_COUNT
};

enum ui_style_cursor {
    UI_CURSOR_ARROW,
    UI_CURSOR_TEXT,
    UI_CURSOR_MOVE,
    UI_CURSOR_RESIZE_VERTICAL,
    UI_CURSOR_RESIZE_HORIZONTAL,
    UI_CURSOR_RESIZE_TOP_LEFT_DOWN_RIGHT,
    UI_CURSOR_RESIZE_TOP_RIGHT_DOWN_LEFT,
    UI_CURSOR_COUNT
};

enum ui_widget_layout_states {
    UI_WIDGET_INVALID, /* The widget cannot be seen and is completely out of view */
    UI_WIDGET_VALID, /* The widget is completely inside the window and can be updated and drawn */
    UI_WIDGET_ROM /* The widget is partially visible and cannot be updated */
};

/* widget states */
enum ui_widget_states {
    UI_WIDGET_STATE_MODIFIED    = UI_FLAG(1),
    UI_WIDGET_STATE_INACTIVE    = UI_FLAG(2), /* widget is neither active nor hovered */
    UI_WIDGET_STATE_ENTERED     = UI_FLAG(3), /* widget has been hovered on the current frame */
    UI_WIDGET_STATE_HOVER       = UI_FLAG(4), /* widget is being hovered */
    UI_WIDGET_STATE_ACTIVED     = UI_FLAG(5),/* widget is currently activated */
    UI_WIDGET_STATE_LEFT        = UI_FLAG(6), /* widget is from this frame on not hovered anymore */
    UI_WIDGET_STATE_HOVERED     = UI_WIDGET_STATE_HOVER|UI_WIDGET_STATE_MODIFIED, /* widget is being hovered */
    UI_WIDGET_STATE_ACTIVE      = UI_WIDGET_STATE_ACTIVED|UI_WIDGET_STATE_MODIFIED /* widget is currently activated */
};

/* text alignment */
enum ui_text_align {
    UI_TEXT_ALIGN_LEFT        = 0x01,
    UI_TEXT_ALIGN_CENTERED    = 0x02,
    UI_TEXT_ALIGN_RIGHT       = 0x04,
    UI_TEXT_ALIGN_TOP         = 0x08,
    UI_TEXT_ALIGN_MIDDLE      = 0x10,
    UI_TEXT_ALIGN_BOTTOM      = 0x20
};
enum ui_text_alignment {
    UI_TEXT_LEFT        = UI_TEXT_ALIGN_MIDDLE|UI_TEXT_ALIGN_LEFT,
    UI_TEXT_CENTERED    = UI_TEXT_ALIGN_MIDDLE|UI_TEXT_ALIGN_CENTERED,
    UI_TEXT_RIGHT       = UI_TEXT_ALIGN_MIDDLE|UI_TEXT_ALIGN_RIGHT
};

/* edit flags */
enum ui_edit_flags {
    UI_EDIT_DEFAULT                 = 0,
    UI_EDIT_READ_ONLY               = UI_FLAG(0),
    UI_EDIT_AUTO_SELECT             = UI_FLAG(1),
    UI_EDIT_SIG_ENTER               = UI_FLAG(2),
    UI_EDIT_ALLOW_TAB               = UI_FLAG(3),
    UI_EDIT_NO_CURSOR               = UI_FLAG(4),
    UI_EDIT_SELECTABLE              = UI_FLAG(5),
    UI_EDIT_CLIPBOARD               = UI_FLAG(6),
    UI_EDIT_CTRL_ENTER_NEWLINE      = UI_FLAG(7),
    UI_EDIT_NO_HORIZONTAL_SCROLL    = UI_FLAG(8),
    UI_EDIT_ALWAYS_INSERT_MODE      = UI_FLAG(9),
    UI_EDIT_MULTILINE               = UI_FLAG(11),
    UI_EDIT_GOTO_END_ON_ACTIVATE    = UI_FLAG(12)
};
enum ui_edit_types {
    UI_EDIT_SIMPLE  = UI_EDIT_ALWAYS_INSERT_MODE,
    UI_EDIT_FIELD   = UI_EDIT_SIMPLE|UI_EDIT_SELECTABLE|UI_EDIT_CLIPBOARD,
    UI_EDIT_BOX     = UI_EDIT_ALWAYS_INSERT_MODE| UI_EDIT_SELECTABLE| UI_EDIT_MULTILINE|UI_EDIT_ALLOW_TAB|UI_EDIT_CLIPBOARD,
    UI_EDIT_EDITOR  = UI_EDIT_SELECTABLE|UI_EDIT_MULTILINE|UI_EDIT_ALLOW_TAB| UI_EDIT_CLIPBOARD
};
enum ui_edit_events {
    UI_EDIT_ACTIVE      = UI_FLAG(0), /* edit widget is currently being modified */
    UI_EDIT_INACTIVE    = UI_FLAG(1), /* edit widget is not active and is not being modified */
    UI_EDIT_ACTIVATED   = UI_FLAG(2), /* edit widget went from state inactive to state active */
    UI_EDIT_DEACTIVATED = UI_FLAG(3), /* edit widget went from state active to state inactive */
    UI_EDIT_COMMITED    = UI_FLAG(4) /* edit widget has received an enter and lost focus */
};
enum ui_panel_flags {
    UI_WINDOW_BORDER            = UI_FLAG(0), /* Draws a border around the window to visually separate the window from the background */
    UI_WINDOW_MOVABLE           = UI_FLAG(1), /* The movable flag indicates that a window can be moved by user input or by dragging the window header */
    UI_WINDOW_SCALABLE          = UI_FLAG(2), /* The scalable flag indicates that a window can be scaled by user input by dragging a scaler icon at the button of the window */
    UI_WINDOW_CLOSABLE          = UI_FLAG(3), /* adds a closable icon into the header */
    UI_WINDOW_MINIMIZABLE       = UI_FLAG(4), /* adds a minimize icon into the header */
    UI_WINDOW_NO_SCROLLBAR      = UI_FLAG(5), /* Removes the scrollbar from the window */
    UI_WINDOW_TITLE             = UI_FLAG(6), /* Forces a header at the top at the window showing the title */
    UI_WINDOW_SCROLL_AUTO_HIDE  = UI_FLAG(7), /* Automatically hides the window scrollbar if no user interaction: also requires delta time in `ui_context` to be set each frame */
    UI_WINDOW_BACKGROUND        = UI_FLAG(8), /* Always keep window in the background */
    UI_WINDOW_SCALE_LEFT        = UI_FLAG(9) /* Puts window scaler in the left-ottom corner instead right-bottom*/
};

/* context */
#ifdef UI_INCLUDE_DEFAULT_ALLOCATOR
UI_API int                      ui_init_default(struct ui_context*, const struct ui_user_font*);
#endif
UI_API int                      ui_init_fixed(struct ui_context*, void *memory, ui_size size, const struct ui_user_font*);
UI_API int                      ui_init(struct ui_context*, struct ui_allocator*, const struct ui_user_font*);
UI_API int                      ui_init_custom(struct ui_context*, struct ui_buffer *cmds, struct ui_buffer *pool, const struct ui_user_font*);
UI_API void                     ui_clear(struct ui_context*);
UI_API void                     ui_free(struct ui_context*);
#ifdef UI_INCLUDE_COMMAND_USERDATA
UI_API void                     ui_set_user_data(struct ui_context*, ui_handle handle);
#endif

/* window */
UI_API int                      ui_begin(struct ui_context*, const char *title, struct ui_rect bounds, ui_flags flags);
UI_API int                      ui_begin_titled(struct ui_context*, const char *name, const char *title, struct ui_rect bounds, ui_flags flags);
UI_API void                     ui_end(struct ui_context*);

UI_API struct ui_window*        ui_window_find(struct ui_context *ctx, const char *name);
UI_API struct ui_rect           ui_window_get_bounds(const struct ui_context*);
UI_API struct ui_vec2           ui_window_get_position(const struct ui_context*);
UI_API struct ui_vec2           ui_window_get_size(const struct ui_context*);
UI_API float                    ui_window_get_width(const struct ui_context*);
UI_API float                    ui_window_get_height(const struct ui_context*);
UI_API struct ui_panel*         ui_window_get_panel(struct ui_context*);
UI_API struct ui_rect           ui_window_get_content_region(struct ui_context*);
UI_API struct ui_vec2           ui_window_get_content_region_min(struct ui_context*);
UI_API struct ui_vec2           ui_window_get_content_region_max(struct ui_context*);
UI_API struct ui_vec2           ui_window_get_content_region_size(struct ui_context*);
UI_API struct ui_command_buffer* ui_window_get_canvas(struct ui_context*);

UI_API int                      ui_window_has_focus(const struct ui_context*);
UI_API int                      ui_window_is_collapsed(struct ui_context*, const char*);
UI_API int                      ui_window_is_closed(struct ui_context*, const char*);
UI_API int                      ui_window_is_hidden(struct ui_context*, const char*);
UI_API int                      ui_window_is_active(struct ui_context*, const char*);
UI_API int                      ui_window_is_hovered(struct ui_context*);
UI_API int                      ui_window_is_any_hovered(struct ui_context*);
UI_API int                      ui_item_is_any_active(struct ui_context*);

UI_API void                     ui_window_set_bounds(struct ui_context*, struct ui_rect);
UI_API void                     ui_window_set_position(struct ui_context*, struct ui_vec2);
UI_API void                     ui_window_set_size(struct ui_context*, struct ui_vec2);
UI_API void                     ui_window_set_focus(struct ui_context*, const char *name);

UI_API void                     ui_window_close(struct ui_context *ctx, const char *name);
UI_API void                     ui_window_collapse(struct ui_context*, const char *name, enum ui_collapse_states);
UI_API void                     ui_window_collapse_if(struct ui_context*, const char *name, enum ui_collapse_states, int cond);
UI_API void                     ui_window_show(struct ui_context*, const char *name, enum ui_show_states);
UI_API void                     ui_window_show_if(struct ui_context*, const char *name, enum ui_show_states, int cond);

/* Layout */
UI_API void                     ui_layout_row_dynamic(struct ui_context*, float height, int cols);
UI_API void                     ui_layout_row_static(struct ui_context*, float height, int item_width, int cols);

UI_API void                     ui_layout_row_begin(struct ui_context*, enum ui_layout_format, float row_height, int cols);
UI_API void                     ui_layout_row_push(struct ui_context*, float value);
UI_API void                     ui_layout_row_end(struct ui_context*);
UI_API void                     ui_layout_row(struct ui_context*, enum ui_layout_format, float height, int cols, const float *ratio);

UI_API void                     ui_layout_row_template_begin(struct ui_context*, float height);
UI_API void                     ui_layout_row_template_push_dynamic(struct ui_context*);
UI_API void                     ui_layout_row_template_push_variable(struct ui_context*, float min_width);
UI_API void                     ui_layout_row_template_push_static(struct ui_context*, float width);
UI_API void                     ui_layout_row_template_end(struct ui_context*);

UI_API void                     ui_layout_space_begin(struct ui_context*, enum ui_layout_format, float height, int widget_count);
UI_API void                     ui_layout_space_push(struct ui_context*, struct ui_rect);
UI_API void                     ui_layout_space_end(struct ui_context*);

/* Layout: Utility */
UI_API struct ui_rect           ui_layout_space_bounds(struct ui_context*);
UI_API struct ui_vec2           ui_layout_space_to_screen(struct ui_context*, struct ui_vec2);
UI_API struct ui_vec2           ui_layout_space_to_local(struct ui_context*, struct ui_vec2);
UI_API struct ui_rect           ui_layout_space_rect_to_screen(struct ui_context*, struct ui_rect);
UI_API struct ui_rect           ui_layout_space_rect_to_local(struct ui_context*, struct ui_rect);
UI_API float                    ui_layout_ratio_from_pixel(struct ui_context*, float pixel_width);

/* Layout: Group */
UI_API int                      ui_group_begin(struct ui_context*, const char *title, ui_flags);
UI_API int                      ui_group_scrolled_offset_begin(struct ui_context*, ui_uint *x_offset, ui_uint *y_offset, const char*, ui_flags);
UI_API int                      ui_group_scrolled_begin(struct ui_context*, struct ui_scroll*, const char *title, ui_flags);
UI_API void                     ui_group_scrolled_end(struct ui_context*);
UI_API void                     ui_group_end(struct ui_context*);

UI_API int                      ui_list_view_begin(struct ui_context*, struct ui_list_view *out, const char *id, ui_flags, int row_height, int row_count);
UI_API void                     ui_list_view_end(struct ui_list_view*);

/* Layout: Tree */
#define                         ui_tree_push(ctx, type, title, state) ui_tree_push_hashed(ctx, type, title, state, UI_FILE_LINE,ui_strlen(UI_FILE_LINE),__LINE__)
#define                         ui_tree_push_id(ctx, type, title, state, id) ui_tree_push_hashed(ctx, type, title, state, UI_FILE_LINE,ui_strlen(UI_FILE_LINE),id)
UI_API int                      ui_tree_push_hashed(struct ui_context*, enum ui_tree_type, const char *title, enum ui_collapse_states initial_state, const char *hash, int len,int seed);
#define                         ui_tree_image_push(ctx, type, img, title, state) ui_tree_image_push_hashed(ctx, type, img, title, state, UI_FILE_LINE,ui_strlen(UI_FILE_LINE),__LINE__)
#define                         ui_tree_image_push_id(ctx, type, img, title, state, id) ui_tree_image_push_hashed(ctx, type, img, title, state, UI_FILE_LINE,ui_strlen(UI_FILE_LINE),id)
UI_API int                      ui_tree_image_push_hashed(struct ui_context*, enum ui_tree_type, struct ui_image, const char *title, enum ui_collapse_states initial_state, const char *hash, int len,int seed);
UI_API void                     ui_tree_pop(struct ui_context*);

UI_API int                      ui_tree_state_push(struct ui_context*, enum ui_tree_type, const char *title, enum ui_collapse_states *state);
UI_API int                      ui_tree_state_image_push(struct ui_context*, enum ui_tree_type, struct ui_image, const char *title, enum ui_collapse_states *state);
UI_API void                     ui_tree_state_pop(struct ui_context*);

/* Widgets */
UI_API void                     ui_text(struct ui_context*, const char*, int, ui_flags);
UI_API void                     ui_text_colored(struct ui_context*, const char*, int, ui_flags, struct ui_color);
UI_API void                     ui_text_wrap(struct ui_context*, const char*, int);
UI_API void                     ui_text_wrap_colored(struct ui_context*, const char*, int, struct ui_color);

UI_API void                     ui_label(struct ui_context*, const char*, ui_flags align);
UI_API void                     ui_label_colored(struct ui_context*, const char*, ui_flags align, struct ui_color);
UI_API void                     ui_label_wrap(struct ui_context*, const char*);
UI_API void                     ui_label_colored_wrap(struct ui_context*, const char*, struct ui_color);
UI_API void                     ui_image(struct ui_context*, struct ui_image);
#ifdef UI_INCLUDE_STANDARD_VARARGS
UI_API void                     ui_labelf(struct ui_context*, ui_flags, const char*, ...);
UI_API void                     ui_labelf_colored(struct ui_context*, ui_flags align, struct ui_color, const char*,...);
UI_API void                     ui_labelf_wrap(struct ui_context*, const char*,...);
UI_API void                     ui_labelf_colored_wrap(struct ui_context*, struct ui_color, const char*,...);

UI_API void                     ui_value_bool(struct ui_context*, const char *prefix, int);
UI_API void                     ui_value_int(struct ui_context*, const char *prefix, int);
UI_API void                     ui_value_uint(struct ui_context*, const char *prefix, unsigned int);
UI_API void                     ui_value_float(struct ui_context*, const char *prefix, float);
UI_API void                     ui_value_color_byte(struct ui_context*, const char *prefix, struct ui_color);
UI_API void                     ui_value_color_float(struct ui_context*, const char *prefix, struct ui_color);
UI_API void                     ui_value_color_hex(struct ui_context*, const char *prefix, struct ui_color);
#endif

/* Widgets: Buttons */
UI_API void                     ui_button_set_behavior(struct ui_context*, enum ui_button_behavior);
UI_API int                      ui_button_push_behavior(struct ui_context*, enum ui_button_behavior);
UI_API int                      ui_button_pop_behavior(struct ui_context*);

UI_API int                      ui_button_text(struct ui_context*, const char *title, int len);
UI_API int                      ui_button_label(struct ui_context*, const char *title);
UI_API int                      ui_button_color(struct ui_context*, struct ui_color);
UI_API int                      ui_button_symbol(struct ui_context*, enum ui_symbol_type);
UI_API int                      ui_button_image(struct ui_context*, struct ui_image img);
UI_API int                      ui_button_symbol_label(struct ui_context*, enum ui_symbol_type, const char*, ui_flags text_alignment);
UI_API int                      ui_button_symbol_text(struct ui_context*, enum ui_symbol_type, const char*, int, ui_flags alignment);
UI_API int                      ui_button_image_label(struct ui_context*, struct ui_image img, const char*, ui_flags text_alignment);
UI_API int                      ui_button_image_text(struct ui_context*, struct ui_image img, const char*, int, ui_flags alignment);

UI_API int                      ui_button_text_styled(struct ui_context*, const struct ui_style_button*, const char *title, int len);
UI_API int                      ui_button_label_styled(struct ui_context*, const struct ui_style_button*, const char *title);
UI_API int                      ui_button_symbol_styled(struct ui_context*, const struct ui_style_button*, enum ui_symbol_type);
UI_API int                      ui_button_image_styled(struct ui_context*, const struct ui_style_button*, struct ui_image img);
UI_API int                      ui_button_symbol_label_styled(struct ui_context*,const struct ui_style_button*, enum ui_symbol_type, const char*, ui_flags text_alignment);
UI_API int                      ui_button_symbol_text_styled(struct ui_context*,const struct ui_style_button*, enum ui_symbol_type, const char*, int, ui_flags alignment);
UI_API int                      ui_button_image_label_styled(struct ui_context*,const struct ui_style_button*, struct ui_image img, const char*, ui_flags text_alignment);
UI_API int                      ui_button_image_text_styled(struct ui_context*,const struct ui_style_button*, struct ui_image img, const char*, int, ui_flags alignment);

/* Widgets: Checkbox */
UI_API int                      ui_check_label(struct ui_context*, const char*, int active);
UI_API int                      ui_check_text(struct ui_context*, const char*, int,int active);
UI_API unsigned                 ui_check_flags_label(struct ui_context*, const char*, unsigned int flags, unsigned int value);
UI_API unsigned                 ui_check_flags_text(struct ui_context*, const char*, int, unsigned int flags, unsigned int value);
UI_API int                      ui_checkbox_label(struct ui_context*, const char*, int *active);
UI_API int                      ui_checkbox_text(struct ui_context*, const char*, int, int *active);
UI_API int                      ui_checkbox_flags_label(struct ui_context*, const char*, unsigned int *flags, unsigned int value);
UI_API int                      ui_checkbox_flags_text(struct ui_context*, const char*, int, unsigned int *flags, unsigned int value);

/* Widgets: Radio */
UI_API int                      ui_radio_label(struct ui_context*, const char*, int *active);
UI_API int                      ui_radio_text(struct ui_context*, const char*, int, int *active);
UI_API int                      ui_option_label(struct ui_context*, const char*, int active);
UI_API int                      ui_option_text(struct ui_context*, const char*, int, int active);

/* Widgets: Selectable */
UI_API int                      ui_selectable_label(struct ui_context*, const char*, ui_flags align, int *value);
UI_API int                      ui_selectable_text(struct ui_context*, const char*, int, ui_flags align, int *value);
UI_API int                      ui_selectable_image_label(struct ui_context*,struct ui_image,  const char*, ui_flags align, int *value);
UI_API int                      ui_selectable_image_text(struct ui_context*,struct ui_image, const char*, int, ui_flags align, int *value);

UI_API int                      ui_select_label(struct ui_context*, const char*, ui_flags align, int value);
UI_API int                      ui_select_text(struct ui_context*, const char*, int, ui_flags align, int value);
UI_API int                      ui_select_image_label(struct ui_context*, struct ui_image,const char*, ui_flags align, int value);
UI_API int                      ui_select_image_text(struct ui_context*, struct ui_image,const char*, int, ui_flags align, int value);

/* Widgets: Slider */
UI_API float                    ui_slide_float(struct ui_context*, float min, float val, float max, float step);
UI_API int                      ui_slide_int(struct ui_context*, int min, int val, int max, int step);
UI_API int                      ui_slider_float(struct ui_context*, float min, float *val, float max, float step);
UI_API int                      ui_slider_int(struct ui_context*, int min, int *val, int max, int step);

/* Widgets: Progressbar */
UI_API int                      ui_progress(struct ui_context*, ui_size *cur, ui_size max, int modifyable);
UI_API ui_size                  ui_prog(struct ui_context*, ui_size cur, ui_size max, int modifyable);

/* Widgets: Color picker */
UI_API struct ui_color          ui_color_picker(struct ui_context*, struct ui_color, enum ui_color_format);
UI_API int                      ui_color_pick(struct ui_context*, struct ui_color*, enum ui_color_format);

/* Widgets: Property */
UI_API void                     ui_property_int(struct ui_context*, const char *name, int min, int *val, int max, int step, float inc_per_pixel);
UI_API void                     ui_property_float(struct ui_context*, const char *name, float min, float *val, float max, float step, float inc_per_pixel);
UI_API void                     ui_property_double(struct ui_context*, const char *name, double min, double *val, double max, double step, float inc_per_pixel);
UI_API int                      ui_propertyi(struct ui_context*, const char *name, int min, int val, int max, int step, float inc_per_pixel);
UI_API float                    ui_propertyf(struct ui_context*, const char *name, float min, float val, float max, float step, float inc_per_pixel);
UI_API double                   ui_propertyd(struct ui_context*, const char *name, double min, double val, double max, double step, float inc_per_pixel);

/* Widgets: TextEdit */
UI_API void                     ui_edit_focus(struct ui_context*, ui_flags flags);
UI_API void                     ui_edit_unfocus(struct ui_context*);
UI_API ui_flags                 ui_edit_string(struct ui_context*, ui_flags, char *buffer, int *len, int max, ui_plugin_filter);
UI_API ui_flags                 ui_edit_buffer(struct ui_context*, ui_flags, struct ui_text_edit*, ui_plugin_filter);
UI_API ui_flags                 ui_edit_string_zero_terminated(struct ui_context*, ui_flags, char *buffer, int max, ui_plugin_filter);

/* Chart */
UI_API int                      ui_chart_begin(struct ui_context*, enum ui_chart_type, int num, float min, float max);
UI_API int                      ui_chart_begin_colored(struct ui_context*, enum ui_chart_type, struct ui_color, struct ui_color active, int num, float min, float max);
UI_API void                     ui_chart_add_slot(struct ui_context *ctx, const enum ui_chart_type, int count, float min_value, float max_value);
UI_API void                     ui_chart_add_slot_colored(struct ui_context *ctx, const enum ui_chart_type, struct ui_color, struct ui_color active, int count, float min_value, float max_value);
UI_API ui_flags                 ui_chart_push(struct ui_context*, float);
UI_API ui_flags                 ui_chart_push_slot(struct ui_context*, float, int);
UI_API void                     ui_chart_end(struct ui_context*);
UI_API void                     ui_plot(struct ui_context*, enum ui_chart_type, const float *values, int count, int offset);
UI_API void                     ui_plot_function(struct ui_context*, enum ui_chart_type, void *userdata, float(*value_getter)(void* user, int index), int count, int offset);

/* Popups */
UI_API int                      ui_popup_begin(struct ui_context*, enum ui_popup_type, const char*, ui_flags, struct ui_rect bounds);
UI_API void                     ui_popup_close(struct ui_context*);
UI_API void                     ui_popup_end(struct ui_context*);

/* Combobox */
UI_API int                      ui_combo(struct ui_context*, const char **items, int count, int selected, int item_height, struct ui_vec2 size);
UI_API int                      ui_combo_separator(struct ui_context*, const char *items_separated_by_separator, int separator, int selected, int count, int item_height, struct ui_vec2 size);
UI_API int                      ui_combo_string(struct ui_context*, const char *items_separated_by_zeros, int selected, int count, int item_height, struct ui_vec2 size);
UI_API int                      ui_combo_callback(struct ui_context*, void(*item_getter)(void*, int, const char**), void *userdata, int selected, int count, int item_height, struct ui_vec2 size);
UI_API void                     ui_combobox(struct ui_context*, const char **items, int count, int *selected, int item_height, struct ui_vec2 size);
UI_API void                     ui_combobox_string(struct ui_context*, const char *items_separated_by_zeros, int *selected, int count, int item_height, struct ui_vec2 size);
UI_API void                     ui_combobox_separator(struct ui_context*, const char *items_separated_by_separator, int separator,int *selected, int count, int item_height, struct ui_vec2 size);
UI_API void                     ui_combobox_callback(struct ui_context*, void(*item_getter)(void*, int, const char**), void*, int *selected, int count, int item_height, struct ui_vec2 size);

/* Combobox: abstract */
UI_API int                      ui_combo_begin_text(struct ui_context*, const char *selected, int, struct ui_vec2 size);
UI_API int                      ui_combo_begin_label(struct ui_context*, const char *selected, struct ui_vec2 size);
UI_API int                      ui_combo_begin_color(struct ui_context*, struct ui_color color, struct ui_vec2 size);
UI_API int                      ui_combo_begin_symbol(struct ui_context*,  enum ui_symbol_type,  struct ui_vec2 size);
UI_API int                      ui_combo_begin_symbol_label(struct ui_context*, const char *selected, enum ui_symbol_type, struct ui_vec2 size);
UI_API int                      ui_combo_begin_symbol_text(struct ui_context*, const char *selected, int, enum ui_symbol_type, struct ui_vec2 size);
UI_API int                      ui_combo_begin_image(struct ui_context*, struct ui_image img,  struct ui_vec2 size);
UI_API int                      ui_combo_begin_image_label(struct ui_context*, const char *selected, struct ui_image, struct ui_vec2 size);
UI_API int                      ui_combo_begin_image_text(struct ui_context*,  const char *selected, int, struct ui_image, struct ui_vec2 size);
UI_API int                      ui_combo_item_label(struct ui_context*, const char*, ui_flags alignment);
UI_API int                      ui_combo_item_text(struct ui_context*, const char*,int, ui_flags alignment);
UI_API int                      ui_combo_item_image_label(struct ui_context*, struct ui_image, const char*, ui_flags alignment);
UI_API int                      ui_combo_item_image_text(struct ui_context*, struct ui_image, const char*, int,ui_flags alignment);
UI_API int                      ui_combo_item_symbol_label(struct ui_context*, enum ui_symbol_type, const char*, ui_flags alignment);
UI_API int                      ui_combo_item_symbol_text(struct ui_context*, enum ui_symbol_type, const char*, int, ui_flags alignment);
UI_API void                     ui_combo_close(struct ui_context*);
UI_API void                     ui_combo_end(struct ui_context*);

/* Contextual */
UI_API int                      ui_contextual_begin(struct ui_context*, ui_flags, struct ui_vec2, struct ui_rect trigger_bounds);
UI_API int                      ui_contextual_item_text(struct ui_context*, const char*, int,ui_flags align);
UI_API int                      ui_contextual_item_label(struct ui_context*, const char*, ui_flags align);
UI_API int                      ui_contextual_item_image_label(struct ui_context*, struct ui_image, const char*, ui_flags alignment);
UI_API int                      ui_contextual_item_image_text(struct ui_context*, struct ui_image, const char*, int len, ui_flags alignment);
UI_API int                      ui_contextual_item_symbol_label(struct ui_context*, enum ui_symbol_type, const char*, ui_flags alignment);
UI_API int                      ui_contextual_item_symbol_text(struct ui_context*, enum ui_symbol_type, const char*, int, ui_flags alignment);
UI_API void                     ui_contextual_close(struct ui_context*);
UI_API void                     ui_contextual_end(struct ui_context*);

/* Tooltip */
UI_API void                     ui_tooltip(struct ui_context*, const char*);
UI_API int                      ui_tooltip_begin(struct ui_context*, float width);
UI_API void                     ui_tooltip_end(struct ui_context*);

/* Menu */
UI_API void                     ui_menubar_begin(struct ui_context*);
UI_API void                     ui_menubar_end(struct ui_context*);

UI_API int                      ui_menu_begin_text(struct ui_context*, const char* title, int title_len, ui_flags align, struct ui_vec2 size);
UI_API int                      ui_menu_begin_label(struct ui_context*, const char*, ui_flags align, struct ui_vec2 size);
UI_API int                      ui_menu_begin_image(struct ui_context*, const char*, struct ui_image, struct ui_vec2 size);
UI_API int                      ui_menu_begin_image_text(struct ui_context*, const char*, int,ui_flags align,struct ui_image, struct ui_vec2 size);
UI_API int                      ui_menu_begin_image_label(struct ui_context*, const char*, ui_flags align,struct ui_image, struct ui_vec2 size);
UI_API int                      ui_menu_begin_symbol(struct ui_context*, const char*, enum ui_symbol_type, struct ui_vec2 size);
UI_API int                      ui_menu_begin_symbol_text(struct ui_context*, const char*, int,ui_flags align,enum ui_symbol_type, struct ui_vec2 size);
UI_API int                      ui_menu_begin_symbol_label(struct ui_context*, const char*, ui_flags align,enum ui_symbol_type, struct ui_vec2 size);
UI_API int                      ui_menu_item_text(struct ui_context*, const char*, int,ui_flags align);
UI_API int                      ui_menu_item_label(struct ui_context*, const char*, ui_flags alignment);
UI_API int                      ui_menu_item_image_label(struct ui_context*, struct ui_image, const char*, ui_flags alignment);
UI_API int                      ui_menu_item_image_text(struct ui_context*, struct ui_image, const char*, int len, ui_flags alignment);
UI_API int                      ui_menu_item_symbol_text(struct ui_context*, enum ui_symbol_type, const char*, int, ui_flags alignment);
UI_API int                      ui_menu_item_symbol_label(struct ui_context*, enum ui_symbol_type, const char*, ui_flags alignment);
UI_API void                     ui_menu_close(struct ui_context*);
UI_API void                     ui_menu_end(struct ui_context*);

/* Drawing*/
#define                                 ui_foreach(c, ctx) for((c)=ui__begin(ctx); (c)!=0; (c)=ui__next(ctx, c))
#ifdef UI_INCLUDE_VERTEX_BUFFER_OUTPUT
UI_API void                             ui_convert(struct ui_context*, struct ui_buffer *cmds, struct ui_buffer *vertices, struct ui_buffer *elements, const struct ui_convert_config*);
#define                                 ui_draw_foreach(cmd,ctx, b) for((cmd)=ui__draw_begin(ctx, b); (cmd)!=0; (cmd)=ui__draw_next(cmd, b, ctx))
#define                                 ui_draw_foreach_bounded(cmd,from,to) for((cmd)=(from); (cmd) && (to) && (cmd)>=to; --(cmd))
UI_API const struct ui_draw_command*    ui__draw_begin(const struct ui_context*, const struct ui_buffer*);
UI_API const struct ui_draw_command*    ui__draw_end(const struct ui_context*, const struct ui_buffer*);
UI_API const struct ui_draw_command*    ui__draw_next(const struct ui_draw_command*, const struct ui_buffer*, const struct ui_context*);
#endif

/* User Input */
UI_API void                     ui_input_begin(struct ui_context*);
UI_API void                     ui_input_motion(struct ui_context*, int x, int y);
UI_API void                     ui_input_key(struct ui_context*, enum ui_keys, int down);
UI_API void                     ui_input_button(struct ui_context*, enum ui_buttons, int x, int y, int down);
UI_API void                     ui_input_scroll(struct ui_context*, float y);
UI_API void                     ui_input_char(struct ui_context*, char);
UI_API void                     ui_input_glyph(struct ui_context*, const ui_glyph);
UI_API void                     ui_input_unicode(struct ui_context*, ui_rune);
UI_API void                     ui_input_end(struct ui_context*);

/* Style */
UI_API void                     ui_style_default(struct ui_context*);
UI_API void                     ui_style_from_table(struct ui_context*, const struct ui_color*);
UI_API void                     ui_style_load_cursor(struct ui_context*, enum ui_style_cursor, const struct ui_cursor*);
UI_API void                     ui_style_load_all_cursors(struct ui_context*, struct ui_cursor*);
UI_API const char*              ui_style_get_color_by_name(enum ui_style_colors);
UI_API void                     ui_style_set_font(struct ui_context*, const struct ui_user_font*);
UI_API int                      ui_style_set_cursor(struct ui_context*, enum ui_style_cursor);
UI_API void                     ui_style_show_cursor(struct ui_context*);
UI_API void                     ui_style_hide_cursor(struct ui_context*);

/* Style: stack */
UI_API int                      ui_style_push_font(struct ui_context*, struct ui_user_font*);
UI_API int                      ui_style_push_float(struct ui_context*, float*, float);
UI_API int                      ui_style_push_vec2(struct ui_context*, struct ui_vec2*, struct ui_vec2);
UI_API int                      ui_style_push_style_item(struct ui_context*, struct ui_style_item*, struct ui_style_item);
UI_API int                      ui_style_push_flags(struct ui_context*, ui_flags*, ui_flags);
UI_API int                      ui_style_push_color(struct ui_context*, struct ui_color*, struct ui_color);

UI_API int                      ui_style_pop_font(struct ui_context*);
UI_API int                      ui_style_pop_float(struct ui_context*);
UI_API int                      ui_style_pop_vec2(struct ui_context*);
UI_API int                      ui_style_pop_style_item(struct ui_context*);
UI_API int                      ui_style_pop_flags(struct ui_context*);
UI_API int                      ui_style_pop_color(struct ui_context*);

/* Utilities */
UI_API struct ui_rect           ui_widget_bounds(struct ui_context*);
UI_API struct ui_vec2           ui_widget_position(struct ui_context*);
UI_API struct ui_vec2           ui_widget_size(struct ui_context*);
UI_API float                    ui_widget_width(struct ui_context*);
UI_API float                    ui_widget_height(struct ui_context*);
UI_API int                      ui_widget_is_hovered(struct ui_context*);
UI_API int                      ui_widget_is_mouse_clicked(struct ui_context*, enum ui_buttons);
UI_API int                      ui_widget_has_mouse_click_down(struct ui_context*, enum ui_buttons, int down);
UI_API void                     ui_spacing(struct ui_context*, int cols);

/* base widget function  */
UI_API enum ui_widget_layout_states ui_widget(struct ui_rect*, const struct ui_context*);
UI_API enum ui_widget_layout_states ui_widget_fitting(struct ui_rect*, struct ui_context*, struct ui_vec2);

/* color (conversion user --> nuklear) */
UI_API struct ui_color          ui_rgb(int r, int g, int b);
UI_API struct ui_color          ui_rgb_iv(const int *rgb);
UI_API struct ui_color          ui_rgb_bv(const ui_byte* rgb);
UI_API struct ui_color          ui_rgb_f(float r, float g, float b);
UI_API struct ui_color          ui_rgb_fv(const float *rgb);
UI_API struct ui_color          ui_rgb_hex(const char *rgb);

UI_API struct ui_color          ui_rgba(int r, int g, int b, int a);
UI_API struct ui_color          ui_rgba_u32(ui_uint);
UI_API struct ui_color          ui_rgba_iv(const int *rgba);
UI_API struct ui_color          ui_rgba_bv(const ui_byte *rgba);
UI_API struct ui_color          ui_rgba_f(float r, float g, float b, float a);
UI_API struct ui_color          ui_rgba_fv(const float *rgba);
UI_API struct ui_color          ui_rgba_hex(const char *rgb);

UI_API struct ui_color          ui_hsv(int h, int s, int v);
UI_API struct ui_color          ui_hsv_iv(const int *hsv);
UI_API struct ui_color          ui_hsv_bv(const ui_byte *hsv);
UI_API struct ui_color          ui_hsv_f(float h, float s, float v);
UI_API struct ui_color          ui_hsv_fv(const float *hsv);

UI_API struct ui_color          ui_hsva(int h, int s, int v, int a);
UI_API struct ui_color          ui_hsva_iv(const int *hsva);
UI_API struct ui_color          ui_hsva_bv(const ui_byte *hsva);
UI_API struct ui_color          ui_hsva_f(float h, float s, float v, float a);
UI_API struct ui_color          ui_hsva_fv(const float *hsva);

/* color (conversion nuklear --> user) */
UI_API void                     ui_color_f(float *r, float *g, float *b, float *a, struct ui_color);
UI_API void                     ui_color_fv(float *rgba_out, struct ui_color);
UI_API void                     ui_color_d(double *r, double *g, double *b, double *a, struct ui_color);
UI_API void                     ui_color_dv(double *rgba_out, struct ui_color);

UI_API ui_uint                  ui_color_u32(struct ui_color);
UI_API void                     ui_color_hex_rgba(char *output, struct ui_color);
UI_API void                     ui_color_hex_rgb(char *output, struct ui_color);

UI_API void                     ui_color_hsv_i(int *out_h, int *out_s, int *out_v, struct ui_color);
UI_API void                     ui_color_hsv_b(ui_byte *out_h, ui_byte *out_s, ui_byte *out_v, struct ui_color);
UI_API void                     ui_color_hsv_iv(int *hsv_out, struct ui_color);
UI_API void                     ui_color_hsv_bv(ui_byte *hsv_out, struct ui_color);
UI_API void                     ui_color_hsv_f(float *out_h, float *out_s, float *out_v, struct ui_color);
UI_API void                     ui_color_hsv_fv(float *hsv_out, struct ui_color);

UI_API void                     ui_color_hsva_i(int *h, int *s, int *v, int *a, struct ui_color);
UI_API void                     ui_color_hsva_b(ui_byte *h, ui_byte *s, ui_byte *v, ui_byte *a, struct ui_color);
UI_API void                     ui_color_hsva_iv(int *hsva_out, struct ui_color);
UI_API void                     ui_color_hsva_bv(ui_byte *hsva_out, struct ui_color);
UI_API void                     ui_color_hsva_f(float *out_h, float *out_s, float *out_v, float *out_a, struct ui_color);
UI_API void                     ui_color_hsva_fv(float *hsva_out, struct ui_color);

/* image */
UI_API ui_handle                ui_handle_ptr(void*);
UI_API ui_handle                ui_handle_id(int);
UI_API struct ui_image          ui_image_handle(ui_handle);
UI_API struct ui_image          ui_image_ptr(void*);
UI_API struct ui_image          ui_image_id(int);
UI_API int                      ui_image_is_subimage(const struct ui_image* img);
UI_API struct ui_image          ui_subimage_ptr(void*, unsigned short w, unsigned short h, struct ui_rect sub_region);
UI_API struct ui_image          ui_subimage_id(int, unsigned short w, unsigned short h, struct ui_rect sub_region);
UI_API struct ui_image          ui_subimage_handle(ui_handle, unsigned short w, unsigned short h, struct ui_rect sub_region);

/* math */
UI_API ui_hash                  ui_murmur_hash(const void *key, int len, ui_hash seed);
UI_API void                     ui_triangle_from_direction(struct ui_vec2 *result, struct ui_rect r, float pad_x, float pad_y, enum ui_heading);

UI_API struct ui_vec2           ui_vec2(float x, float y);
UI_API struct ui_vec2           ui_vec2i(int x, int y);
UI_API struct ui_vec2           ui_vec2v(const float *xy);
UI_API struct ui_vec2           ui_vec2iv(const int *xy);

UI_API struct ui_rect           ui_get_null_rect(void);
UI_API struct ui_rect           ui_rect(float x, float y, float w, float h);
UI_API struct ui_rect           ui_recti(int x, int y, int w, int h);
UI_API struct ui_rect           ui_recta(struct ui_vec2 pos, struct ui_vec2 size);
UI_API struct ui_rect           ui_rectv(const float *xywh);
UI_API struct ui_rect           ui_rectiv(const int *xywh);
UI_API struct ui_vec2           ui_rect_pos(struct ui_rect);
UI_API struct ui_vec2           ui_rect_size(struct ui_rect);

/* string*/
UI_API int                      ui_strlen(const char *str);
UI_API int                      ui_stricmp(const char *s1, const char *s2);
UI_API int                      ui_stricmpn(const char *s1, const char *s2, int n);
UI_API int                      ui_strtoi(const char *str, char **endptr);
UI_API float                    ui_strtof(const char *str, char **endptr);
UI_API double                   ui_strtod(const char *str, char **endptr);
UI_API int                      ui_strfilter(const char *text, const char *regexp);
UI_API int                      ui_strmatch_fuzzy_string(char const *str, char const *pattern, int *out_score);
UI_API int                      ui_strmatch_fuzzy_text(const char *txt, int txt_len, const char *pattern, int *out_score);

/* UTF-8 */
UI_API int                      ui_utf_decode(const char*, ui_rune*, int);
UI_API int                      ui_utf_encode(ui_rune, char*, int);
UI_API int                      ui_utf_len(const char*, int byte_len);
UI_API const char*              ui_utf_at(const char *buffer, int length, int index, ui_rune *unicode, int *len);

/* ===============================================================
 *
 *                          FONT
 *
 * ===============================================================*/
/*  Font handling in this library was designed to be quite customizable and lets
    you decide what you want to use and what you want to provide. There are three
    different ways to use the font atlas. The first two will use your font
    handling scheme and only requires essential data to run nuklear. The next
    slightly more advanced features is font handling with vertex buffer output.
    Finally the most complex API wise is using nuklears font baking API.

    1.) Using your own implementation without vertex buffer output
    --------------------------------------------------------------
    So first up the easiest way to do font handling is by just providing a
    `ui_user_font` struct which only requires the height in pixel of the used
    font and a callback to calculate the width of a string. This way of handling
    fonts is best fitted for using the normal draw shape command API where you
    do all the text drawing yourself and the library does not require any kind
    of deeper knowledge about which font handling mechanism you use.
    IMPORTANT: the `ui_user_font` pointer provided to nuklear has to persist
    over the complete life time! I know this sucks but it is currently the only
    way to switch between fonts.

        float your_text_width_calculation(ui_handle handle, float height, const char *text, int len)
        {
            your_font_type *type = handle.ptr;
            float text_width = ...;
            return text_width;
        }

        struct ui_user_font font;
        font.userdata.ptr = &your_font_class_or_struct;
        font.height = your_font_height;
        font.width = your_text_width_calculation;

        struct ui_context ctx;
        ui_init_default(&ctx, &font);

    2.) Using your own implementation with vertex buffer output
    --------------------------------------------------------------
    While the first approach works fine if you don't want to use the optional
    vertex buffer output it is not enough if you do. To get font handling working
    for these cases you have to provide two additional parameters inside the
    `ui_user_font`. First a texture atlas handle used to draw text as subimages
    of a bigger font atlas texture and a callback to query a character's glyph
    information (offset, size, ...). So it is still possible to provide your own
    font and use the vertex buffer output.

        float your_text_width_calculation(ui_handle handle, float height, const char *text, int len)
        {
            your_font_type *type = handle.ptr;
            float text_width = ...;
            return text_width;
        }
        void query_your_font_glyph(ui_handle handle, float font_height, struct ui_user_font_glyph *glyph, ui_rune codepoint, ui_rune next_codepoint)
        {
            your_font_type *type = handle.ptr;
            glyph.width = ...;
            glyph.height = ...;
            glyph.xadvance = ...;
            glyph.uv[0].x = ...;
            glyph.uv[0].y = ...;
            glyph.uv[1].x = ...;
            glyph.uv[1].y = ...;
            glyph.offset.x = ...;
            glyph.offset.y = ...;
        }

        struct ui_user_font font;
        font.userdata.ptr = &your_font_class_or_struct;
        font.height = your_font_height;
        font.width = your_text_width_calculation;
        font.query = query_your_font_glyph;
        font.texture.id = your_font_texture;

        struct ui_context ctx;
        ui_init_default(&ctx, &font);

    3.) Nuklear font baker
    ------------------------------------
    The final approach if you do not have a font handling functionality or don't
    want to use it in this library is by using the optional font baker.
    The font baker API's can be used to create a font plus font atlas texture
    and can be used with or without the vertex buffer output.

    It still uses the `ui_user_font` struct and the two different approaches
    previously stated still work. The font baker is not located inside
    `ui_context` like all other systems since it can be understood as more of
    an extension to nuklear and does not really depend on any `ui_context` state.

    Font baker need to be initialized first by one of the ui_font_atlas_init_xxx
    functions. If you don't care about memory just call the default version
    `ui_font_atlas_init_default` which will allocate all memory from the standard library.
    If you want to control memory allocation but you don't care if the allocated
    memory is temporary and therefore can be freed directly after the baking process
    is over or permanent you can call `ui_font_atlas_init`.

    After successfull intializing the font baker you can add Truetype(.ttf) fonts from
    different sources like memory or from file by calling one of the `ui_font_atlas_add_xxx`.
    functions. Adding font will permanently store each font, font config and ttf memory block(!)
    inside the font atlas and allows to reuse the font atlas. If you don't want to reuse
    the font baker by for example adding additional fonts you can call
    `ui_font_atlas_cleanup` after the baking process is over (after calling ui_font_atlas_end).

    As soon as you added all fonts you wanted you can now start the baking process
    for every selected glyphes to image by calling `ui_font_atlas_bake`.
    The baking process returns image memory, width and height which can be used to
    either create your own image object or upload it to any graphics library.
    No matter which case you finally have to call `ui_font_atlas_end` which
    will free all temporary memory including the font atlas image so make sure
    you created our texture beforehand. `ui_font_atlas_end` requires a handle
    to your font texture or object and optionally fills a `struct ui_draw_null_texture`
    which can be used for the optional vertex output. If you don't want it just
    set the argument to `NULL`.

    At this point you are done and if you don't want to reuse the font atlas you
    can call `ui_font_atlas_cleanup` to free all truetype blobs and configuration
    memory. Finally if you don't use the font atlas and any of it's fonts anymore
    you need to call `ui_font_atlas_clear` to free all memory still being used.

        struct ui_font_atlas atlas;
        ui_font_atlas_init_default(&atlas);
        ui_font_atlas_begin(&atlas);
        ui_font *font = ui_font_atlas_add_from_file(&atlas, "Path/To/Your/TTF_Font.ttf", 13, 0);
        ui_font *font2 = ui_font_atlas_add_from_file(&atlas, "Path/To/Your/TTF_Font2.ttf", 16, 0);
        void* img = ui_font_atlas_bake(&atlas, &img_width, &img_height, UI_FONT_ATLAS_RGBA32, 0);
        ui_font_atlas_end(&atlas, ui_handle_id(texture), 0);

        struct ui_context ctx;
        ui_init_default(&ctx, &font->handle);
        while (1) {

        }
        ui_font_atlas_clear(&atlas);

    The font baker API is probably the most complex API inside this library and
    I would suggest reading some of my examples `example/` to get a grip on how
    to use the font atlas. There are a number of details I left out. For example
    how to merge fonts, configure a font with `ui_font_config` to use other languages,
    use another texture coodinate format and a lot more:

        struct ui_font_config cfg = ui_font_config(font_pixel_height);
        cfg.merge_mode = ui_false or ui_true;
        cfg.range = ui_font_korean_glyph_ranges();
        cfg.coord_type = UI_COORD_PIXEL;
        ui_font *font = ui_font_atlas_add_from_file(&atlas, "Path/To/Your/TTF_Font.ttf", 13, &cfg);

*/
struct ui_user_font_glyph;
typedef float(*ui_text_width_f)(ui_handle, float h, const char*, int len);
typedef void(*ui_query_font_glyph_f)(ui_handle handle, float font_height,
                                    struct ui_user_font_glyph *glyph,
                                    ui_rune codepoint, ui_rune next_codepoint);

#ifdef UI_INCLUDE_VERTEX_BUFFER_OUTPUT
struct ui_user_font_glyph {
    struct ui_vec2 uv[2];
    /* texture coordinates */
    struct ui_vec2 offset;
    /* offset between top left and glyph */
    float width, height;
    /* size of the glyph  */
    float xadvance;
    /* offset to the next glyph */
};
#endif

struct ui_user_font {
    ui_handle userdata;
    /* user provided font handle */
    float height;
    /* max height of the font */
    ui_text_width_f width;
    /* font string width in pixel callback */
#ifdef UI_INCLUDE_VERTEX_BUFFER_OUTPUT
    ui_query_font_glyph_f query;
    /* font glyph callback to query drawing info */
    ui_handle texture;
    /* texture handle to the used font atlas or texture */
#endif
};

#ifdef UI_INCLUDE_FONT_BAKING
enum ui_font_coord_type {
    UI_COORD_UV, /* texture coordinates inside font glyphs are clamped between 0-1 */
    UI_COORD_PIXEL /* texture coordinates inside font glyphs are in absolute pixel */
};

struct ui_baked_font {
    float height;
    /* height of the font  */
    float ascent, descent;
    /* font glyphs ascent and descent  */
    ui_rune glyph_offset;
    /* glyph array offset inside the font glyph baking output array  */
    ui_rune glyph_count;
    /* number of glyphs of this font inside the glyph baking array output */
    const ui_rune *ranges;
    /* font codepoint ranges as pairs of (from/to) and 0 as last element */
};

struct ui_font_config {
    struct ui_font_config *next;
    /* NOTE: only used internally */
    void *ttf_blob;
    /* pointer to loaded TTF file memory block.
     * NOTE: not needed for ui_font_atlas_add_from_memory and ui_font_atlas_add_from_file. */
    ui_size ttf_size;
    /* size of the loaded TTF file memory block
     * NOTE: not needed for ui_font_atlas_add_from_memory and ui_font_atlas_add_from_file. */

    unsigned char ttf_data_owned_by_atlas;
    /* used inside font atlas: default to: 0*/
    unsigned char merge_mode;
    /* merges this font into the last font */
    unsigned char pixel_snap;
    /* align every character to pixel boundary (if true set oversample (1,1)) */
    unsigned char oversample_v, oversample_h;
    /* rasterize at hight quality for sub-pixel position */
    unsigned char padding[3];

    float size;
    /* baked pixel height of the font */
    enum ui_font_coord_type coord_type;
    /* texture coordinate format with either pixel or UV coordinates */
    struct ui_vec2 spacing;
    /* extra pixel spacing between glyphs  */
    const ui_rune *range;
    /* list of unicode ranges (2 values per range, zero terminated) */
    struct ui_baked_font *font;
    /* font to setup in the baking process: NOTE: not needed for font atlas */
    ui_rune fallback_glyph;
    /* fallback glyph to use if a given rune is not found */
};

struct ui_font_glyph {
    ui_rune codepoint;
    float xadvance;
    float x0, y0, x1, y1, w, h;
    float u0, v0, u1, v1;
};

struct ui_font {
    struct ui_font *next;
    struct ui_user_font handle;
    struct ui_baked_font info;
    float scale;
    struct ui_font_glyph *glyphs;
    const struct ui_font_glyph *fallback;
    ui_rune fallback_codepoint;
    ui_handle texture;
    struct ui_font_config *config;
};

enum ui_font_atlas_format {
    UI_FONT_ATLAS_ALPHA8,
    UI_FONT_ATLAS_RGBA32
};

struct ui_font_atlas {
    void *pixel;
    int tex_width;
    int tex_height;

    struct ui_allocator permanent;
    struct ui_allocator temporary;

    struct ui_recti custom;
    struct ui_cursor cursors[UI_CURSOR_COUNT];

    int glyph_count;
    struct ui_font_glyph *glyphs;
    struct ui_font *default_font;
    struct ui_font *fonts;
    struct ui_font_config *config;
    int font_num;
};

/* some language glyph codepoint ranges */
UI_API const ui_rune *ui_font_default_glyph_ranges(void);
UI_API const ui_rune *ui_font_chinese_glyph_ranges(void);
UI_API const ui_rune *ui_font_cyrillic_glyph_ranges(void);
UI_API const ui_rune *ui_font_korean_glyph_ranges(void);

#ifdef UI_INCLUDE_DEFAULT_ALLOCATOR
UI_API void ui_font_atlas_init_default(struct ui_font_atlas*);
#endif
UI_API void ui_font_atlas_init(struct ui_font_atlas*, struct ui_allocator*);
UI_API void ui_font_atlas_init_custom(struct ui_font_atlas*, struct ui_allocator *persistent, struct ui_allocator *transient);
UI_API void ui_font_atlas_begin(struct ui_font_atlas*);
UI_API struct ui_font_config ui_font_config(float pixel_height);
UI_API struct ui_font *ui_font_atlas_add(struct ui_font_atlas*, const struct ui_font_config*);
#ifdef UI_INCLUDE_DEFAULT_FONT
UI_API struct ui_font* ui_font_atlas_add_default(struct ui_font_atlas*, float height, const struct ui_font_config*);
#endif
UI_API struct ui_font* ui_font_atlas_add_from_memory(struct ui_font_atlas *atlas, void *memory, ui_size size, float height, const struct ui_font_config *config);
#ifdef UI_INCLUDE_STANDARD_IO
UI_API struct ui_font* ui_font_atlas_add_from_file(struct ui_font_atlas *atlas, const char *file_path, float height, const struct ui_font_config*);
#endif
UI_API struct ui_font *ui_font_atlas_add_compressed(struct ui_font_atlas*, void *memory, ui_size size, float height, const struct ui_font_config*);
UI_API struct ui_font* ui_font_atlas_add_compressed_base85(struct ui_font_atlas*, const char *data, float height, const struct ui_font_config *config);
UI_API const void* ui_font_atlas_bake(struct ui_font_atlas*, int *width, int *height, enum ui_font_atlas_format);
UI_API void ui_font_atlas_end(struct ui_font_atlas*, ui_handle tex, struct ui_draw_null_texture*);
UI_API const struct ui_font_glyph* ui_font_find_glyph(struct ui_font*, ui_rune unicode);
UI_API void ui_font_atlas_cleanup(struct ui_font_atlas *atlas);
UI_API void ui_font_atlas_clear(struct ui_font_atlas*);

#endif

/* ==============================================================
 *
 *                          MEMORY BUFFER
 *
 * ===============================================================*/
/*  A basic (double)-buffer with linear allocation and resetting as only
    freeing policy. The buffer's main purpose is to control all memory management
    inside the GUI toolkit and still leave memory control as much as possible in
    the hand of the user while also making sure the library is easy to use if
    not as much control is needed.
    In general all memory inside this library can be provided from the user in
    three different ways.

    The first way and the one providing most control is by just passing a fixed
    size memory block. In this case all control lies in the hand of the user
    since he can exactly control where the memory comes from and how much memory
    the library should consume. Of course using the fixed size API removes the
    ability to automatically resize a buffer if not enough memory is provided so
    you have to take over the resizing. While being a fixed sized buffer sounds
    quite limiting, it is very effective in this library since the actual memory
    consumption is quite stable and has a fixed upper bound for a lot of cases.

    If you don't want to thiui about how much memory the library should allocate
    at all time or have a very dynamic UI with unpredictable memory consumption
    habits but still want control over memory allocation you can use the dynamic
    allocator based API. The allocator consists of two callbacks for allocating
    and freeing memory and optional userdata so you can plugin your own allocator.

    The final and easiest way can be used by defining
    UI_INCLUDE_DEFAULT_ALLOCATOR which uses the standard library memory
    allocation functions malloc and free and takes over complete control over
    memory in this library.
*/
struct ui_memory_status {
    void *memory;
    unsigned int type;
    ui_size size;
    ui_size allocated;
    ui_size needed;
    ui_size calls;
};

enum ui_allocation_type {
    UI_BUFFER_FIXED,
    UI_BUFFER_DYNAMIC
};

enum ui_buffer_allocation_type {
    UI_BUFFER_FRONT,
    UI_BUFFER_BACK,
    UI_BUFFER_MAX
};

struct ui_buffer_marker {
    int active;
    ui_size offset;
};

struct ui_memory {void *ptr;ui_size size;};
struct ui_buffer {
    struct ui_buffer_marker marker[UI_BUFFER_MAX];
    /* buffer marker to free a buffer to a certain offset */
    struct ui_allocator pool;
    /* allocator callback for dynamic buffers */
    enum ui_allocation_type type;
    /* memory management type */
    struct ui_memory memory;
    /* memory and size of the current memory block */
    float grow_factor;
    /* growing factor for dynamic memory management */
    ui_size allocated;
    /* total amount of memory allocated */
    ui_size needed;
    /* totally consumed memory given that enough memory is present */
    ui_size calls;
    /* number of allocation calls */
    ui_size size;
    /* current size of the buffer */
};

#ifdef UI_INCLUDE_DEFAULT_ALLOCATOR
UI_API void ui_buffer_init_default(struct ui_buffer*);
#endif
UI_API void ui_buffer_init(struct ui_buffer*, const struct ui_allocator*, ui_size size);
UI_API void ui_buffer_init_fixed(struct ui_buffer*, void *memory, ui_size size);
UI_API void ui_buffer_info(struct ui_memory_status*, struct ui_buffer*);
UI_API void ui_buffer_push(struct ui_buffer*, enum ui_buffer_allocation_type type, const void *memory, ui_size size, ui_size align);
UI_API void ui_buffer_mark(struct ui_buffer*, enum ui_buffer_allocation_type type);
UI_API void ui_buffer_reset(struct ui_buffer*, enum ui_buffer_allocation_type type);
UI_API void ui_buffer_clear(struct ui_buffer*);
UI_API void ui_buffer_free(struct ui_buffer*);
UI_API void *ui_buffer_memory(struct ui_buffer*);
UI_API const void *ui_buffer_memory_const(const struct ui_buffer*);
UI_API ui_size ui_buffer_total(struct ui_buffer*);

/* ==============================================================
 *
 *                          STRING
 *
 * ===============================================================*/
/*  Basic string buffer which is only used in context with the text editor
 *  to manage and manipulate dynamic or fixed size string content. This is _NOT_
 *  the default string handling method. The only instance you should have any contact
 *  with this API is if you interact with an `ui_text_edit` object inside one of the
 *  copy and paste functions and even there only for more advanced cases. */
struct ui_str {
    struct ui_buffer buffer;
    int len; /* in codepoints/runes/glyphs */
};

#ifdef UI_INCLUDE_DEFAULT_ALLOCATOR
UI_API void ui_str_init_default(struct ui_str*);
#endif
UI_API void ui_str_init(struct ui_str*, const struct ui_allocator*, ui_size size);
UI_API void ui_str_init_fixed(struct ui_str*, void *memory, ui_size size);
UI_API void ui_str_clear(struct ui_str*);
UI_API void ui_str_free(struct ui_str*);

UI_API int ui_str_append_text_char(struct ui_str*, const char*, int);
UI_API int ui_str_append_str_char(struct ui_str*, const char*);
UI_API int ui_str_append_text_utf8(struct ui_str*, const char*, int);
UI_API int ui_str_append_str_utf8(struct ui_str*, const char*);
UI_API int ui_str_append_text_runes(struct ui_str*, const ui_rune*, int);
UI_API int ui_str_append_str_runes(struct ui_str*, const ui_rune*);

UI_API int ui_str_insert_at_char(struct ui_str*, int pos, const char*, int);
UI_API int ui_str_insert_at_rune(struct ui_str*, int pos, const char*, int);

UI_API int ui_str_insert_text_char(struct ui_str*, int pos, const char*, int);
UI_API int ui_str_insert_str_char(struct ui_str*, int pos, const char*);
UI_API int ui_str_insert_text_utf8(struct ui_str*, int pos, const char*, int);
UI_API int ui_str_insert_str_utf8(struct ui_str*, int pos, const char*);
UI_API int ui_str_insert_text_runes(struct ui_str*, int pos, const ui_rune*, int);
UI_API int ui_str_insert_str_runes(struct ui_str*, int pos, const ui_rune*);

UI_API void ui_str_remove_chars(struct ui_str*, int len);
UI_API void ui_str_remove_runes(struct ui_str *str, int len);
UI_API void ui_str_delete_chars(struct ui_str*, int pos, int len);
UI_API void ui_str_delete_runes(struct ui_str*, int pos, int len);

UI_API char *ui_str_at_char(struct ui_str*, int pos);
UI_API char *ui_str_at_rune(struct ui_str*, int pos, ui_rune *unicode, int *len);
UI_API ui_rune ui_str_rune_at(const struct ui_str*, int pos);
UI_API const char *ui_str_at_char_const(const struct ui_str*, int pos);
UI_API const char *ui_str_at_const(const struct ui_str*, int pos, ui_rune *unicode, int *len);

UI_API char *ui_str_get(struct ui_str*);
UI_API const char *ui_str_get_const(const struct ui_str*);
UI_API int ui_str_len(struct ui_str*);
UI_API int ui_str_len_char(struct ui_str*);

/*===============================================================
 *
 *                      TEXT EDITOR
 *
 * ===============================================================*/
/* Editing text in this library is handled by either `ui_edit_string` or
 * `ui_edit_buffer`. But like almost everything in this library there are multiple
 * ways of doing it and a balance between control and ease of use with memory
 * as well as functionality controlled by flags.
 *
 * This library generally allows three different levels of memory control:
 * First of is the most basic way of just providing a simple char array with
 * string length. This method is probably the easiest way of handling simple
 * user text input. Main upside is complete control over memory while the biggest
 * downside in comparsion with the other two approaches is missing undo/redo.
 *
 * For UIs that require undo/redo the second way was created. It is based on
 * a fixed size ui_text_edit struct, which has an internal undo/redo stack.
 * This is mainly useful if you want something more like a text editor but don't want
 * to have a dynamically growing buffer.
 *
 * The final way is using a dynamically growing ui_text_edit struct, which
 * has both a default version if you don't care where memory comes from and an
 * allocator version if you do. While the text editor is quite powerful for its
 * complexity I would not recommend editing gigabytes of data with it.
 * It is rather designed for uses cases which make sense for a GUI library not for
 * an full blown text editor.
 */
#ifndef UI_TEXTEDIT_UNDOSTATECOUNT
#define UI_TEXTEDIT_UNDOSTATECOUNT     99
#endif

#ifndef UI_TEXTEDIT_UNDOCHARCOUNT
#define UI_TEXTEDIT_UNDOCHARCOUNT      999
#endif

struct ui_text_edit;
struct ui_clipboard {
    ui_handle userdata;
    ui_plugin_paste paste;
    ui_plugin_copy copy;
};

struct ui_text_undo_record {
   int where;
   short insert_length;
   short delete_length;
   short char_storage;
};

struct ui_text_undo_state {
   struct ui_text_undo_record undo_rec[UI_TEXTEDIT_UNDOSTATECOUNT];
   ui_rune undo_char[UI_TEXTEDIT_UNDOCHARCOUNT];
   short undo_point;
   short redo_point;
   short undo_char_point;
   short redo_char_point;
};

enum ui_text_edit_type {
    UI_TEXT_EDIT_SINGLE_LINE,
    UI_TEXT_EDIT_MULTI_LINE
};

enum ui_text_edit_mode {
    UI_TEXT_EDIT_MODE_VIEW,
    UI_TEXT_EDIT_MODE_INSERT,
    UI_TEXT_EDIT_MODE_REPLACE
};

struct ui_text_edit {
    struct ui_clipboard clip;
    struct ui_str string;
    ui_plugin_filter filter;
    struct ui_vec2 scrollbar;

    int cursor;
    int select_start;
    int select_end;
    unsigned char mode;
    unsigned char cursor_at_end_of_line;
    unsigned char initialized;
    unsigned char has_preferred_x;
    unsigned char single_line;
    unsigned char active;
    unsigned char padding1;
    float preferred_x;
    struct ui_text_undo_state undo;
};

/* filter function */
UI_API int ui_filter_default(const struct ui_text_edit*, ui_rune unicode);
UI_API int ui_filter_ascii(const struct ui_text_edit*, ui_rune unicode);
UI_API int ui_filter_float(const struct ui_text_edit*, ui_rune unicode);
UI_API int ui_filter_decimal(const struct ui_text_edit*, ui_rune unicode);
UI_API int ui_filter_hex(const struct ui_text_edit*, ui_rune unicode);
UI_API int ui_filter_oct(const struct ui_text_edit*, ui_rune unicode);
UI_API int ui_filter_binary(const struct ui_text_edit*, ui_rune unicode);

/* text editor */
#ifdef UI_INCLUDE_DEFAULT_ALLOCATOR
UI_API void ui_textedit_init_default(struct ui_text_edit*);
#endif
UI_API void ui_textedit_init(struct ui_text_edit*, struct ui_allocator*, ui_size size);
UI_API void ui_textedit_init_fixed(struct ui_text_edit*, void *memory, ui_size size);
UI_API void ui_textedit_free(struct ui_text_edit*);
UI_API void ui_textedit_text(struct ui_text_edit*, const char*, int total_len);
UI_API void ui_textedit_delete(struct ui_text_edit*, int where, int len);
UI_API void ui_textedit_delete_selection(struct ui_text_edit*);
UI_API void ui_textedit_select_all(struct ui_text_edit*);
UI_API int ui_textedit_cut(struct ui_text_edit*);
UI_API int ui_textedit_paste(struct ui_text_edit*, char const*, int len);
UI_API void ui_textedit_undo(struct ui_text_edit*);
UI_API void ui_textedit_redo(struct ui_text_edit*);

/* ===============================================================
 *
 *                          DRAWING
 *
 * ===============================================================*/
/*  This library was designed to be render backend agnostic so it does
    not draw anything to screen. Instead all drawn shapes, widgets
    are made of, are buffered into memory and make up a command queue.
    Each frame therefore fills the command buffer with draw commands
    that then need to be executed by the user and his own render backend.
    After that the command buffer needs to be cleared and a new frame can be
    started. It is probably important to note that the command buffer is the main
    drawing API and the optional vertex buffer API only takes this format and
    converts it into a hardware accessible format.

    To use the command queue to draw your own widgets you can access the
    command buffer of each window by calling `ui_window_get_canvas` after
    previously having called `ui_begin`:

        void draw_red_rectangle_widget(struct ui_context *ctx)
        {
            struct ui_command_buffer *canvas;
            struct ui_input *input = &ctx->input;
            canvas = ui_window_get_canvas(ctx);

            struct ui_rect space;
            enum ui_widget_layout_states state;
            state = ui_widget(&space, ctx);
            if (!state) return;

            if (state != UI_WIDGET_ROM)
                update_your_widget_by_user_input(...);
            ui_fill_rect(canvas, space, 0, ui_rgb(255,0,0));
        }

        if (ui_begin(...)) {
            ui_layout_row_dynamic(ctx, 25, 1);
            draw_red_rectangle_widget(ctx);
        }
        ui_end(..)

    Important to know if you want to create your own widgets is the `ui_widget`
    call. It allocates space on the panel reserved for this widget to be used,
    but also returns the state of the widget space. If your widget is not seen and does
    not have to be updated it is '0' and you can just return. If it only has
    to be drawn the state will be `UI_WIDGET_ROM` otherwise you can do both
    update and draw your widget. The reason for seperating is to only draw and
    update what is actually neccessary which is crucial for performance.
*/
enum ui_command_type {
    UI_COMMAND_NOP,
    UI_COMMAND_SCISSOR,
    UI_COMMAND_LINE,
    UI_COMMAND_CURVE,
    UI_COMMAND_RECT,
    UI_COMMAND_RECT_FILLED,
    UI_COMMAND_RECT_MULTI_COLOR,
    UI_COMMAND_CIRCLE,
    UI_COMMAND_CIRCLE_FILLED,
    UI_COMMAND_ARC,
    UI_COMMAND_ARC_FILLED,
    UI_COMMAND_TRIANGLE,
    UI_COMMAND_TRIANGLE_FILLED,
    UI_COMMAND_POLYGON,
    UI_COMMAND_POLYGON_FILLED,
    UI_COMMAND_POLYLINE,
    UI_COMMAND_TEXT,
    UI_COMMAND_IMAGE
};

/* command base and header of every command inside the buffer */
struct ui_command {
    enum ui_command_type type;
    ui_size next;
#ifdef UI_INCLUDE_COMMAND_USERDATA
    ui_handle userdata;
#endif
};

struct ui_command_scissor {
    struct ui_command header;
    short x, y;
    unsigned short w, h;
};

struct ui_command_line {
    struct ui_command header;
    unsigned short line_thickness;
    struct ui_vec2i begin;
    struct ui_vec2i end;
    struct ui_color color;
};

struct ui_command_curve {
    struct ui_command header;
    unsigned short line_thickness;
    struct ui_vec2i begin;
    struct ui_vec2i end;
    struct ui_vec2i ctrl[2];
    struct ui_color color;
};

struct ui_command_rect {
    struct ui_command header;
    unsigned short rounding;
    unsigned short line_thickness;
    short x, y;
    unsigned short w, h;
    struct ui_color color;
};

struct ui_command_rect_filled {
    struct ui_command header;
    unsigned short rounding;
    short x, y;
    unsigned short w, h;
    struct ui_color color;
};

struct ui_command_rect_multi_color {
    struct ui_command header;
    short x, y;
    unsigned short w, h;
    struct ui_color left;
    struct ui_color top;
    struct ui_color bottom;
    struct ui_color right;
};

struct ui_command_triangle {
    struct ui_command header;
    unsigned short line_thickness;
    struct ui_vec2i a;
    struct ui_vec2i b;
    struct ui_vec2i c;
    struct ui_color color;
};

struct ui_command_triangle_filled {
    struct ui_command header;
    struct ui_vec2i a;
    struct ui_vec2i b;
    struct ui_vec2i c;
    struct ui_color color;
};

struct ui_command_circle {
    struct ui_command header;
    short x, y;
    unsigned short line_thickness;
    unsigned short w, h;
    struct ui_color color;
};

struct ui_command_circle_filled {
    struct ui_command header;
    short x, y;
    unsigned short w, h;
    struct ui_color color;
};

struct ui_command_arc {
    struct ui_command header;
    short cx, cy;
    unsigned short r;
    unsigned short line_thickness;
    float a[2];
    struct ui_color color;
};

struct ui_command_arc_filled {
    struct ui_command header;
    short cx, cy;
    unsigned short r;
    float a[2];
    struct ui_color color;
};

struct ui_command_polygon {
    struct ui_command header;
    struct ui_color color;
    unsigned short line_thickness;
    unsigned short point_count;
    struct ui_vec2i points[1];
};

struct ui_command_polygon_filled {
    struct ui_command header;
    struct ui_color color;
    unsigned short point_count;
    struct ui_vec2i points[1];
};

struct ui_command_polyline {
    struct ui_command header;
    struct ui_color color;
    unsigned short line_thickness;
    unsigned short point_count;
    struct ui_vec2i points[1];
};

struct ui_command_image {
    struct ui_command header;
    short x, y;
    unsigned short w, h;
    struct ui_image img;
    struct ui_color col;
};

struct ui_command_text {
    struct ui_command header;
    const struct ui_user_font *font;
    struct ui_color background;
    struct ui_color foreground;
    short x, y;
    unsigned short w, h;
    float height;
    int length;
    char string[1];
};

enum ui_command_clipping {
    UI_CLIPPING_OFF = ui_false,
    UI_CLIPPING_ON = ui_true
};

struct ui_command_buffer {
    struct ui_buffer *base;
    struct ui_rect clip;
    int use_clipping;
    ui_handle userdata;
    ui_size begin, end, last;
};

/* shape outlines */
UI_API void ui_stroke_line(struct ui_command_buffer *b, float x0, float y0, float x1, float y1, float line_thickness, struct ui_color);
UI_API void ui_stroke_curve(struct ui_command_buffer*, float, float, float, float, float, float, float, float, float line_thickness, struct ui_color);
UI_API void ui_stroke_rect(struct ui_command_buffer*, struct ui_rect, float rounding, float line_thickness, struct ui_color);
UI_API void ui_stroke_circle(struct ui_command_buffer*, struct ui_rect, float line_thickness, struct ui_color);
UI_API void ui_stroke_arc(struct ui_command_buffer*, float cx, float cy, float radius, float a_min, float a_max, float line_thickness, struct ui_color);
UI_API void ui_stroke_triangle(struct ui_command_buffer*, float, float, float, float, float, float, float line_thichness, struct ui_color);
UI_API void ui_stroke_polyline(struct ui_command_buffer*, float *points, int point_count, float line_thickness, struct ui_color col);
UI_API void ui_stroke_polygon(struct ui_command_buffer*, float*, int point_count, float line_thickness, struct ui_color);

/* filled shades */
UI_API void ui_fill_rect(struct ui_command_buffer*, struct ui_rect, float rounding, struct ui_color);
UI_API void ui_fill_rect_multi_color(struct ui_command_buffer*, struct ui_rect, struct ui_color left, struct ui_color top, struct ui_color right, struct ui_color bottom);
UI_API void ui_fill_circle(struct ui_command_buffer*, struct ui_rect, struct ui_color);
UI_API void ui_fill_arc(struct ui_command_buffer*, float cx, float cy, float radius, float a_min, float a_max, struct ui_color);
UI_API void ui_fill_triangle(struct ui_command_buffer*, float x0, float y0, float x1, float y1, float x2, float y2, struct ui_color);
UI_API void ui_fill_polygon(struct ui_command_buffer*, float*, int point_count, struct ui_color);

/* misc */
UI_API void ui_push_scissor(struct ui_command_buffer*, struct ui_rect);
UI_API void ui_draw_image(struct ui_command_buffer*, struct ui_rect, const struct ui_image*, struct ui_color);
UI_API void ui_draw_text(struct ui_command_buffer*, struct ui_rect, const char *text, int len, const struct ui_user_font*, struct ui_color, struct ui_color);
UI_API const struct ui_command* ui__next(struct ui_context*, const struct ui_command*);
UI_API const struct ui_command* ui__begin(struct ui_context*);

/* ===============================================================
 *
 *                          INPUT
 *
 * ===============================================================*/
struct ui_mouse_button {
    int down;
    unsigned int clicked;
    struct ui_vec2 clicked_pos;
};
struct ui_mouse {
    struct ui_mouse_button buttons[UI_BUTTON_MAX];
    struct ui_vec2 pos;
    struct ui_vec2 prev;
    struct ui_vec2 delta;
    float scroll_delta;
    unsigned char grab;
    unsigned char grabbed;
    unsigned char ungrab;
};

struct ui_key {
    int down;
    unsigned int clicked;
};
struct ui_keyboard {
    struct ui_key keys[UI_KEY_MAX];
    char text[UI_INPUT_MAX];
    int text_len;
};

struct ui_input {
    struct ui_keyboard keyboard;
    struct ui_mouse mouse;
};

UI_API int ui_input_has_mouse_click(const struct ui_input*, enum ui_buttons);
UI_API int ui_input_has_mouse_click_in_rect(const struct ui_input*, enum ui_buttons, struct ui_rect);
UI_API int ui_input_has_mouse_click_down_in_rect(const struct ui_input*, enum ui_buttons, struct ui_rect, int down);
UI_API int ui_input_is_mouse_click_in_rect(const struct ui_input*, enum ui_buttons, struct ui_rect);
UI_API int ui_input_is_mouse_click_down_in_rect(const struct ui_input *i, enum ui_buttons id, struct ui_rect b, int down);
UI_API int ui_input_any_mouse_click_in_rect(const struct ui_input*, struct ui_rect);
UI_API int ui_input_is_mouse_prev_hovering_rect(const struct ui_input*, struct ui_rect);
UI_API int ui_input_is_mouse_hovering_rect(const struct ui_input*, struct ui_rect);
UI_API int ui_input_mouse_clicked(const struct ui_input*, enum ui_buttons, struct ui_rect);
UI_API int ui_input_is_mouse_down(const struct ui_input*, enum ui_buttons);
UI_API int ui_input_is_mouse_pressed(const struct ui_input*, enum ui_buttons);
UI_API int ui_input_is_mouse_released(const struct ui_input*, enum ui_buttons);
UI_API int ui_input_is_key_pressed(const struct ui_input*, enum ui_keys);
UI_API int ui_input_is_key_released(const struct ui_input*, enum ui_keys);
UI_API int ui_input_is_key_down(const struct ui_input*, enum ui_keys);

/* ===============================================================
 *
 *                          DRAW LIST
 *
 * ===============================================================*/
#ifdef UI_INCLUDE_VERTEX_BUFFER_OUTPUT
/*  The optional vertex buffer draw list provides a 2D drawing context
    with antialiasing functionality which takes basic filled or outlined shapes
    or a path and outputs vertexes, elements and draw commands.
    The actual draw list API is not required to be used directly while using this
    library since converting the default library draw command output is done by
    just calling `ui_convert` but I decided to still make this library accessible
    since it can be useful.

    The draw list is based on a path buffering and polygon and polyline
    rendering API which allows a lot of ways to draw 2D content to screen.
    In fact it is probably more powerful than needed but allows even more crazy
    things than this library provides by default.
*/
typedef ui_ushort ui_draw_index;
enum ui_draw_list_stroke {
    UI_STROKE_OPEN = ui_false,
    /* build up path has no connection back to the beginning */
    UI_STROKE_CLOSED = ui_true
    /* build up path has a connection back to the beginning */
};

enum ui_draw_vertex_layout_attribute {
    UI_VERTEX_POSITION,
    UI_VERTEX_COLOR,
    UI_VERTEX_TEXCOORD,
    UI_VERTEX_ATTRIBUTE_COUNT
};

enum ui_draw_vertex_layout_format {
    UI_FORMAT_SCHAR,
    UI_FORMAT_SSHORT,
    UI_FORMAT_SINT,
    UI_FORMAT_UCHAR,
    UI_FORMAT_USHORT,
    UI_FORMAT_UINT,
    UI_FORMAT_FLOAT,
    UI_FORMAT_DOUBLE,

UI_FORMAT_COLOR_BEGIN,
    UI_FORMAT_R8G8B8 = UI_FORMAT_COLOR_BEGIN,
    UI_FORMAT_R16G15B16,
    UI_FORMAT_R32G32B32,

    UI_FORMAT_R8G8B8A8,
    UI_FORMAT_R16G15B16A16,
    UI_FORMAT_R32G32B32A32,
    UI_FORMAT_R32G32B32A32_FLOAT,
    UI_FORMAT_R32G32B32A32_DOUBLE,

    UI_FORMAT_RGB32,
    UI_FORMAT_RGBA32,
UI_FORMAT_COLOR_END = UI_FORMAT_RGBA32,
    UI_FORMAT_COUNT
};

#define UI_VERTEX_LAYOUT_END UI_VERTEX_ATTRIBUTE_COUNT,UI_FORMAT_COUNT,0
struct ui_draw_vertex_layout_element {
    enum ui_draw_vertex_layout_attribute attribute;
    enum ui_draw_vertex_layout_format format;
    ui_size offset;
};

struct ui_draw_command {
    unsigned int elem_count;
    /* number of elements in the current draw batch */
    struct ui_rect clip_rect;
    /* current screen clipping rectangle */
    ui_handle texture;
    /* current texture to set */
#ifdef UI_INCLUDE_COMMAND_USERDATA
    ui_handle userdata;
#endif
};

struct ui_draw_list {
    struct ui_rect clip_rect;
    struct ui_vec2 circle_vtx[12];
    struct ui_convert_config config;

    struct ui_buffer *buffer;
    struct ui_buffer *vertices;
    struct ui_buffer *elements;

    unsigned int element_count;
    unsigned int vertex_count;
    unsigned int cmd_count;
    ui_size cmd_offset;

    unsigned int path_count;
    unsigned int path_offset;

#ifdef UI_INCLUDE_COMMAND_USERDATA
    ui_handle userdata;
#endif
};

/* draw list */
UI_API void ui_draw_list_init(struct ui_draw_list*);
UI_API void ui_draw_list_setup(struct ui_draw_list*, const struct ui_convert_config*, struct ui_buffer *cmds, struct ui_buffer *vertices, struct ui_buffer *elements);
UI_API void ui_draw_list_clear(struct ui_draw_list*);

/* drawing */
#define ui_draw_list_foreach(cmd, can, b) for((cmd)=ui__draw_list_begin(can, b); (cmd)!=0; (cmd)=ui__draw_list_next(cmd, b, can))
UI_API const struct ui_draw_command* ui__draw_list_begin(const struct ui_draw_list*, const struct ui_buffer*);
UI_API const struct ui_draw_command* ui__draw_list_next(const struct ui_draw_command*, const struct ui_buffer*, const struct ui_draw_list*);
UI_API const struct ui_draw_command* ui__draw_list_end(const struct ui_draw_list*, const struct ui_buffer*);
UI_API void ui_draw_list_clear(struct ui_draw_list *list);

/* path */
UI_API void ui_draw_list_path_clear(struct ui_draw_list*);
UI_API void ui_draw_list_path_line_to(struct ui_draw_list*, struct ui_vec2 pos);
UI_API void ui_draw_list_path_arc_to_fast(struct ui_draw_list*, struct ui_vec2 center, float radius, int a_min, int a_max);
UI_API void ui_draw_list_path_arc_to(struct ui_draw_list*, struct ui_vec2 center, float radius, float a_min, float a_max, unsigned int segments);
UI_API void ui_draw_list_path_rect_to(struct ui_draw_list*, struct ui_vec2 a, struct ui_vec2 b, float rounding);
UI_API void ui_draw_list_path_curve_to(struct ui_draw_list*, struct ui_vec2 p2, struct ui_vec2 p3, struct ui_vec2 p4, unsigned int num_segments);
UI_API void ui_draw_list_path_fill(struct ui_draw_list*, struct ui_color);
UI_API void ui_draw_list_path_stroke(struct ui_draw_list*, struct ui_color, enum ui_draw_list_stroke closed, float thickness);

/* stroke */
UI_API void ui_draw_list_stroke_line(struct ui_draw_list*, struct ui_vec2 a, struct ui_vec2 b, struct ui_color, float thickness);
UI_API void ui_draw_list_stroke_rect(struct ui_draw_list*, struct ui_rect rect, struct ui_color, float rounding, float thickness);
UI_API void ui_draw_list_stroke_triangle(struct ui_draw_list*, struct ui_vec2 a, struct ui_vec2 b, struct ui_vec2 c, struct ui_color, float thickness);
UI_API void ui_draw_list_stroke_circle(struct ui_draw_list*, struct ui_vec2 center, float radius, struct ui_color, unsigned int segs, float thickness);
UI_API void ui_draw_list_stroke_curve(struct ui_draw_list*, struct ui_vec2 p0, struct ui_vec2 cp0, struct ui_vec2 cp1, struct ui_vec2 p1, struct ui_color, unsigned int segments, float thickness);
UI_API void ui_draw_list_stroke_poly_line(struct ui_draw_list*, const struct ui_vec2 *pnts, const unsigned int cnt, struct ui_color, enum ui_draw_list_stroke, float thickness, enum ui_anti_aliasing);

/* fill */
UI_API void ui_draw_list_fill_rect(struct ui_draw_list*, struct ui_rect rect, struct ui_color, float rounding);
UI_API void ui_draw_list_fill_rect_multi_color(struct ui_draw_list*, struct ui_rect rect, struct ui_color left, struct ui_color top, struct ui_color right, struct ui_color bottom);
UI_API void ui_draw_list_fill_triangle(struct ui_draw_list*, struct ui_vec2 a, struct ui_vec2 b, struct ui_vec2 c, struct ui_color);
UI_API void ui_draw_list_fill_circle(struct ui_draw_list*, struct ui_vec2 center, float radius, struct ui_color col, unsigned int segs);
UI_API void ui_draw_list_fill_poly_convex(struct ui_draw_list*, const struct ui_vec2 *points, const unsigned int count, struct ui_color, enum ui_anti_aliasing);

/* misc */
UI_API void ui_draw_list_add_image(struct ui_draw_list*, struct ui_image texture, struct ui_rect rect, struct ui_color);
UI_API void ui_draw_list_add_text(struct ui_draw_list*, const struct ui_user_font*, struct ui_rect, const char *text, int len, float font_height, struct ui_color);
#ifdef UI_INCLUDE_COMMAND_USERDATA
UI_API void ui_draw_list_push_userdata(struct ui_draw_list*, ui_handle userdata);
#endif

#endif

/* ===============================================================
 *
 *                          GUI
 *
 * ===============================================================*/
enum ui_style_item_type {
    UI_STYLE_ITEM_COLOR,
    UI_STYLE_ITEM_IMAGE
};

union ui_style_item_data {
    struct ui_image image;
    struct ui_color color;
};

struct ui_style_item {
    enum ui_style_item_type type;
    union ui_style_item_data data;
};

struct ui_style_text {
    struct ui_color color;
    struct ui_vec2 padding;
};

struct ui_style_button {
    /* background */
    struct ui_style_item normal;
    struct ui_style_item hover;
    struct ui_style_item active;
    struct ui_color border_color;

    /* text */
    struct ui_color text_background;
    struct ui_color text_normal;
    struct ui_color text_hover;
    struct ui_color text_active;
    ui_flags text_alignment;

    /* properties */
    float border;
    float rounding;
    struct ui_vec2 padding;
    struct ui_vec2 image_padding;
    struct ui_vec2 touch_padding;

    /* optional user callbacks */
    ui_handle userdata;
    void(*draw_begin)(struct ui_command_buffer*, ui_handle userdata);
    void(*draw_end)(struct ui_command_buffer*, ui_handle userdata);
};

struct ui_style_toggle {
    /* background */
    struct ui_style_item normal;
    struct ui_style_item hover;
    struct ui_style_item active;
    struct ui_color border_color;

    /* cursor */
    struct ui_style_item cursor_normal;
    struct ui_style_item cursor_hover;

    /* text */
    struct ui_color text_normal;
    struct ui_color text_hover;
    struct ui_color text_active;
    struct ui_color text_background;
    ui_flags text_alignment;

    /* properties */
    struct ui_vec2 padding;
    struct ui_vec2 touch_padding;
    float spacing;
    float border;

    /* optional user callbacks */
    ui_handle userdata;
    void(*draw_begin)(struct ui_command_buffer*, ui_handle);
    void(*draw_end)(struct ui_command_buffer*, ui_handle);
};

struct ui_style_selectable {
    /* background (inactive) */
    struct ui_style_item normal;
    struct ui_style_item hover;
    struct ui_style_item pressed;

    /* background (active) */
    struct ui_style_item normal_active;
    struct ui_style_item hover_active;
    struct ui_style_item pressed_active;

    /* text color (inactive) */
    struct ui_color text_normal;
    struct ui_color text_hover;
    struct ui_color text_pressed;

    /* text color (active) */
    struct ui_color text_normal_active;
    struct ui_color text_hover_active;
    struct ui_color text_pressed_active;
    struct ui_color text_background;
    ui_flags text_alignment;

    /* properties */
    float rounding;
    struct ui_vec2 padding;
    struct ui_vec2 touch_padding;
    struct ui_vec2 image_padding;

    /* optional user callbacks */
    ui_handle userdata;
    void(*draw_begin)(struct ui_command_buffer*, ui_handle);
    void(*draw_end)(struct ui_command_buffer*, ui_handle);
};

struct ui_style_slider {
    /* background */
    struct ui_style_item normal;
    struct ui_style_item hover;
    struct ui_style_item active;
    struct ui_color border_color;

    /* background bar */
    struct ui_color bar_normal;
    struct ui_color bar_hover;
    struct ui_color bar_active;
    struct ui_color bar_filled;

    /* cursor */
    struct ui_style_item cursor_normal;
    struct ui_style_item cursor_hover;
    struct ui_style_item cursor_active;

    /* properties */
    float border;
    float rounding;
    float bar_height;
    struct ui_vec2 padding;
    struct ui_vec2 spacing;
    struct ui_vec2 cursor_size;

    /* optional buttons */
    int show_buttons;
    struct ui_style_button inc_button;
    struct ui_style_button dec_button;
    enum ui_symbol_type inc_symbol;
    enum ui_symbol_type dec_symbol;

    /* optional user callbacks */
    ui_handle userdata;
    void(*draw_begin)(struct ui_command_buffer*, ui_handle);
    void(*draw_end)(struct ui_command_buffer*, ui_handle);
};

struct ui_style_progress {
    /* background */
    struct ui_style_item normal;
    struct ui_style_item hover;
    struct ui_style_item active;
    struct ui_color border_color;

    /* cursor */
    struct ui_style_item cursor_normal;
    struct ui_style_item cursor_hover;
    struct ui_style_item cursor_active;
    struct ui_color cursor_border_color;

    /* properties */
    float rounding;
    float border;
    float cursor_border;
    float cursor_rounding;
    struct ui_vec2 padding;

    /* optional user callbacks */
    ui_handle userdata;
    void(*draw_begin)(struct ui_command_buffer*, ui_handle);
    void(*draw_end)(struct ui_command_buffer*, ui_handle);
};

struct ui_style_scrollbar {
    /* background */
    struct ui_style_item normal;
    struct ui_style_item hover;
    struct ui_style_item active;
    struct ui_color border_color;

    /* cursor */
    struct ui_style_item cursor_normal;
    struct ui_style_item cursor_hover;
    struct ui_style_item cursor_active;
    struct ui_color cursor_border_color;

    /* properties */
    float border;
    float rounding;
    float border_cursor;
    float rounding_cursor;
    struct ui_vec2 padding;

    /* optional buttons */
    int show_buttons;
    struct ui_style_button inc_button;
    struct ui_style_button dec_button;
    enum ui_symbol_type inc_symbol;
    enum ui_symbol_type dec_symbol;

    /* optional user callbacks */
    ui_handle userdata;
    void(*draw_begin)(struct ui_command_buffer*, ui_handle);
    void(*draw_end)(struct ui_command_buffer*, ui_handle);
};

struct ui_style_edit {
    /* background */
    struct ui_style_item normal;
    struct ui_style_item hover;
    struct ui_style_item active;
    struct ui_color border_color;
    struct ui_style_scrollbar scrollbar;

    /* cursor  */
    struct ui_color cursor_normal;
    struct ui_color cursor_hover;
    struct ui_color cursor_text_normal;
    struct ui_color cursor_text_hover;

    /* text (unselected) */
    struct ui_color text_normal;
    struct ui_color text_hover;
    struct ui_color text_active;

    /* text (selected) */
    struct ui_color selected_normal;
    struct ui_color selected_hover;
    struct ui_color selected_text_normal;
    struct ui_color selected_text_hover;

    /* properties */
    float border;
    float rounding;
    float cursor_size;
    struct ui_vec2 scrollbar_size;
    struct ui_vec2 padding;
    float row_padding;
};

struct ui_style_property {
    /* background */
    struct ui_style_item normal;
    struct ui_style_item hover;
    struct ui_style_item active;
    struct ui_color border_color;

    /* text */
    struct ui_color label_normal;
    struct ui_color label_hover;
    struct ui_color label_active;

    /* symbols */
    enum ui_symbol_type sym_left;
    enum ui_symbol_type sym_right;

    /* properties */
    float border;
    float rounding;
    struct ui_vec2 padding;

    struct ui_style_edit edit;
    struct ui_style_button inc_button;
    struct ui_style_button dec_button;

    /* optional user callbacks */
    ui_handle userdata;
    void(*draw_begin)(struct ui_command_buffer*, ui_handle);
    void(*draw_end)(struct ui_command_buffer*, ui_handle);
};

struct ui_style_chart {
    /* colors */
    struct ui_style_item background;
    struct ui_color border_color;
    struct ui_color selected_color;
    struct ui_color color;

    /* properties */
    float border;
    float rounding;
    struct ui_vec2 padding;
};

struct ui_style_combo {
    /* background */
    struct ui_style_item normal;
    struct ui_style_item hover;
    struct ui_style_item active;
    struct ui_color border_color;

    /* label */
    struct ui_color label_normal;
    struct ui_color label_hover;
    struct ui_color label_active;

    /* symbol */
    struct ui_color symbol_normal;
    struct ui_color symbol_hover;
    struct ui_color symbol_active;

    /* button */
    struct ui_style_button button;
    enum ui_symbol_type sym_normal;
    enum ui_symbol_type sym_hover;
    enum ui_symbol_type sym_active;

    /* properties */
    float border;
    float rounding;
    struct ui_vec2 content_padding;
    struct ui_vec2 button_padding;
    struct ui_vec2 spacing;
};

struct ui_style_tab {
    /* background */
    struct ui_style_item background;
    struct ui_color border_color;
    struct ui_color text;

    /* button */
    struct ui_style_button tab_maximize_button;
    struct ui_style_button tab_minimize_button;
    struct ui_style_button node_maximize_button;
    struct ui_style_button node_minimize_button;
    enum ui_symbol_type sym_minimize;
    enum ui_symbol_type sym_maximize;

    /* properties */
    float border;
    float rounding;
    float indent;
    struct ui_vec2 padding;
    struct ui_vec2 spacing;
};

enum ui_style_header_align {
    UI_HEADER_LEFT,
    UI_HEADER_RIGHT
};
struct ui_style_window_header {
    /* background */
    struct ui_style_item normal;
    struct ui_style_item hover;
    struct ui_style_item active;

    /* button */
    struct ui_style_button close_button;
    struct ui_style_button minimize_button;
    enum ui_symbol_type close_symbol;
    enum ui_symbol_type minimize_symbol;
    enum ui_symbol_type maximize_symbol;

    /* title */
    struct ui_color label_normal;
    struct ui_color label_hover;
    struct ui_color label_active;

    /* properties */
    enum ui_style_header_align align;
    struct ui_vec2 padding;
    struct ui_vec2 label_padding;
    struct ui_vec2 spacing;
};

struct ui_style_window {
    struct ui_style_window_header header;
    struct ui_style_item fixed_background;
    struct ui_color background;

    struct ui_color border_color;
    struct ui_color popup_border_color;
    struct ui_color combo_border_color;
    struct ui_color contextual_border_color;
    struct ui_color menu_border_color;
    struct ui_color group_border_color;
    struct ui_color tooltip_border_color;
    struct ui_style_item scaler;

    float border;
    float combo_border;
    float contextual_border;
    float menu_border;
    float group_border;
    float tooltip_border;
    float popup_border;

    float rounding;
    struct ui_vec2 spacing;
    struct ui_vec2 scrollbar_size;
    struct ui_vec2 min_size;

    struct ui_vec2 padding;
    struct ui_vec2 group_padding;
    struct ui_vec2 popup_padding;
    struct ui_vec2 combo_padding;
    struct ui_vec2 contextual_padding;
    struct ui_vec2 menu_padding;
    struct ui_vec2 tooltip_padding;
};

struct ui_style {
    const struct ui_user_font *font;
    const struct ui_cursor *cursors[UI_CURSOR_COUNT];
    const struct ui_cursor *cursor_active;
    struct ui_cursor *cursor_last;
    int cursor_visible;

    struct ui_style_text text;
    struct ui_style_button button;
    struct ui_style_button contextual_button;
    struct ui_style_button menu_button;
    struct ui_style_toggle option;
    struct ui_style_toggle checkbox;
    struct ui_style_selectable selectable;
    struct ui_style_slider slider;
    struct ui_style_progress progress;
    struct ui_style_property property;
    struct ui_style_edit edit;
    struct ui_style_chart chart;
    struct ui_style_scrollbar scrollh;
    struct ui_style_scrollbar scrollv;
    struct ui_style_tab tab;
    struct ui_style_combo combo;
    struct ui_style_window window;
};

UI_API struct ui_style_item ui_style_item_image(struct ui_image img);
UI_API struct ui_style_item ui_style_item_color(struct ui_color);
UI_API struct ui_style_item ui_style_item_hide(void);

/*==============================================================
 *                          PANEL
 * =============================================================*/
#ifndef UI_MAX_LAYOUT_ROW_TEMPLATE_COLUMNS
#define UI_MAX_LAYOUT_ROW_TEMPLATE_COLUMNS 16
#endif
#ifndef UI_CHART_MAX_SLOT
#define UI_CHART_MAX_SLOT 4
#endif

enum ui_panel_type {
    UI_PANEL_WINDOW     = UI_FLAG(0),
    UI_PANEL_GROUP      = UI_FLAG(1),
    UI_PANEL_POPUP      = UI_FLAG(2),
    UI_PANEL_CONTEXTUAL = UI_FLAG(4),
    UI_PANEL_COMBO      = UI_FLAG(5),
    UI_PANEL_MENU       = UI_FLAG(6),
    UI_PANEL_TOOLTIP    = UI_FLAG(7)
};
enum ui_panel_set {
    UI_PANEL_SET_NONBLOCK = UI_PANEL_CONTEXTUAL|UI_PANEL_COMBO|UI_PANEL_MENU|UI_PANEL_TOOLTIP,
    UI_PANEL_SET_POPUP = UI_PANEL_SET_NONBLOCK|UI_PANEL_POPUP,
    UI_PANEL_SET_SUB = UI_PANEL_SET_POPUP|UI_PANEL_GROUP
};

struct ui_chart_slot {
    enum ui_chart_type type;
    struct ui_color color;
    struct ui_color highlight;
    float min, max, range;
    int count;
    struct ui_vec2 last;
    int index;
};

struct ui_chart {
    int slot;
    float x, y, w, h;
    struct ui_chart_slot slots[UI_CHART_MAX_SLOT];
};

enum ui_panel_row_layout_type {
    UI_LAYOUT_DYNAMIC_FIXED = 0,
    UI_LAYOUT_DYNAMIC_ROW,
    UI_LAYOUT_DYNAMIC_FREE,
    UI_LAYOUT_DYNAMIC,
    UI_LAYOUT_STATIC_FIXED,
    UI_LAYOUT_STATIC_ROW,
    UI_LAYOUT_STATIC_FREE,
    UI_LAYOUT_STATIC,
    UI_LAYOUT_TEMPLATE,
    UI_LAYOUT_COUNT
};
struct ui_row_layout {
    enum ui_panel_row_layout_type type;
    int index;
    float height;
    int columns;
    const float *ratio;
    float item_width;
    float item_height;
    float item_offset;
    float filled;
    struct ui_rect item;
    int tree_depth;
    float templates[UI_MAX_LAYOUT_ROW_TEMPLATE_COLUMNS];
};

struct ui_popup_buffer {
    ui_size begin;
    ui_size parent;
    ui_size last;
    ui_size end;
    int active;
};

struct ui_menu_state {
    float x, y, w, h;
    struct ui_scroll offset;
};

struct ui_panel {
    enum ui_panel_type type;
    ui_flags flags;
    struct ui_rect bounds;
    ui_uint *offset_x;
    ui_uint *offset_y;
    float at_x, at_y, max_x;
    float footer_height;
    float header_height;
    float border;
    unsigned int has_scrolling;
    struct ui_rect clip;
    struct ui_menu_state menu;
    struct ui_row_layout row;
    struct ui_chart chart;
    struct ui_popup_buffer popup_buffer;
    struct ui_command_buffer *buffer;
    struct ui_panel *parent;
};

/*==============================================================
 *                          WINDOW
 * =============================================================*/
#ifndef UI_WINDOW_MAX_NAME
#define UI_WINDOW_MAX_NAME 64
#endif

struct ui_table;
enum ui_window_flags {
    UI_WINDOW_PRIVATE       = UI_FLAG(10),
    UI_WINDOW_DYNAMIC       = UI_WINDOW_PRIVATE,
    /* special window type growing up in height while being filled to a certain maximum height */
    UI_WINDOW_ROM           = UI_FLAG(11),
    /* sets the window into a read only mode and does not allow input changes */
    UI_WINDOW_HIDDEN        = UI_FLAG(12),
    /* Hides the window and stops any window interaction and drawing */
    UI_WINDOW_CLOSED        = UI_FLAG(13),
    /* Directly closes and frees the window at the end of the frame */
    UI_WINDOW_MINIMIZED     = UI_FLAG(14),
    /* marks the window as minimized */
    UI_WINDOW_REMOVE_ROM    = UI_FLAG(15)
    /* Removes the read only mode at the end of the window */
};

struct ui_popup_state {
    struct ui_window *win;
    enum ui_panel_type type;
    ui_hash name;
    int active;
    unsigned combo_count;
    unsigned con_count, con_old;
    unsigned active_con;
    struct ui_rect header;
};

struct ui_edit_state {
    ui_hash name;
    unsigned int seq;
    unsigned int old;
    int active, prev;
    int cursor;
    int sel_start;
    int sel_end;
    struct ui_scroll scrollbar;
    unsigned char mode;
    unsigned char single_line;
};

struct ui_property_state {
    int active, prev;
    char buffer[UI_MAX_NUMBER_BUFFER];
    int length;
    int cursor;
    ui_hash name;
    unsigned int seq;
    unsigned int old;
    int state;
};

struct ui_window {
    unsigned int seq;
    ui_hash name;
    char name_string[UI_WINDOW_MAX_NAME];
    ui_flags flags;

    struct ui_rect bounds;
    struct ui_scroll scrollbar;
    struct ui_command_buffer buffer;
    struct ui_panel *layout;
    float scrollbar_hiding_timer;

    /* persistent widget state */
    struct ui_property_state property;
    struct ui_popup_state popup;
    struct ui_edit_state edit;
    unsigned int scrolled;

    struct ui_table *tables;
    unsigned short table_count;
    unsigned short table_size;

    /* window list hooks */
    struct ui_window *next;
    struct ui_window *prev;
    struct ui_window *parent;
};

/*==============================================================
 *                          STACK
 * =============================================================*/
/* The style modifier stack can be used to temporarily change a
 * property inside `ui_style`. For example if you want a special
 * red button you can temporarily push the old button color onto a stack
 * draw the button with a red color and then you just pop the old color
 * back from the stack:
 *
 *      ui_style_push_style_item(ctx, &ctx->style.button.normal, ui_style_item_color(ui_rgb(255,0,0)));
 *      ui_style_push_style_item(ctx, &ctx->style.button.hover, ui_style_item_color(ui_rgb(255,0,0)));
 *      ui_style_push_style_item(ctx, &ctx->style.button.active, ui_style_item_color(ui_rgb(255,0,0)));
 *      ui_style_push_vec2(ctx, &cx->style.button.padding, ui_vec2(2,2));
 *
 *      ui_button(...);
 *
 *      ui_style_pop_style_item(ctx);
 *      ui_style_pop_style_item(ctx);
 *      ui_style_pop_style_item(ctx);
 *      ui_style_pop_vec2(ctx);
 *
 * Nuklear has a stack for style_items, float properties, vector properties, 
 * flags, colors, fonts and for button_behavior. Each has it's own fixed size stack
 * which can be changed at compile time.
 */
#ifndef UI_BUTTON_BEHAVIOR_STACK_SIZE
#define UI_BUTTON_BEHAVIOR_STACK_SIZE 8
#endif

#ifndef UI_FONT_STACK_SIZE
#define UI_FONT_STACK_SIZE 8
#endif

#ifndef UI_STYLE_ITEM_STACK_SIZE
#define UI_STYLE_ITEM_STACK_SIZE 16
#endif

#ifndef UI_FLOAT_STACK_SIZE
#define UI_FLOAT_STACK_SIZE 32
#endif

#ifndef UI_VECTOR_STACK_SIZE
#define UI_VECTOR_STACK_SIZE 16
#endif

#ifndef UI_FLAGS_STACK_SIZE
#define UI_FLAGS_STACK_SIZE 32
#endif

#ifndef UI_COLOR_STACK_SIZE
#define UI_COLOR_STACK_SIZE 32
#endif

#define UI_CONFIGURATION_STACK_TYPE(prefix, name, type)\
    struct ui_config_stack_##name##_element {\
        prefix##_##type *address;\
        prefix##_##type old_value;\
    }
#define UI_CONFIG_STACK(type,size)\
    struct ui_config_stack_##type {\
        int head;\
        struct ui_config_stack_##type##_element elements[size];\
    }

#define ui_float float
UI_CONFIGURATION_STACK_TYPE(struct ui, style_item, style_item);
UI_CONFIGURATION_STACK_TYPE(ui ,float, float);
UI_CONFIGURATION_STACK_TYPE(struct ui, vec2, vec2);
UI_CONFIGURATION_STACK_TYPE(ui ,flags, flags);
UI_CONFIGURATION_STACK_TYPE(struct ui, color, color);
UI_CONFIGURATION_STACK_TYPE(const struct ui, user_font, user_font*);
UI_CONFIGURATION_STACK_TYPE(enum ui, button_behavior, button_behavior);

UI_CONFIG_STACK(style_item, UI_STYLE_ITEM_STACK_SIZE);
UI_CONFIG_STACK(float, UI_FLOAT_STACK_SIZE);
UI_CONFIG_STACK(vec2, UI_VECTOR_STACK_SIZE);
UI_CONFIG_STACK(flags, UI_FLAGS_STACK_SIZE);
UI_CONFIG_STACK(color, UI_COLOR_STACK_SIZE);
UI_CONFIG_STACK(user_font, UI_FONT_STACK_SIZE);
UI_CONFIG_STACK(button_behavior, UI_BUTTON_BEHAVIOR_STACK_SIZE);

struct ui_configuration_stacks {
    struct ui_config_stack_style_item style_items;
    struct ui_config_stack_float floats;
    struct ui_config_stack_vec2 vectors;
    struct ui_config_stack_flags flags;
    struct ui_config_stack_color colors;
    struct ui_config_stack_user_font fonts;
    struct ui_config_stack_button_behavior button_behaviors;
};

/*==============================================================
 *                          CONTEXT
 * =============================================================*/
#define UI_VALUE_PAGE_CAPACITY \
    ((UI_MAX(sizeof(struct ui_window),sizeof(struct ui_panel)) / sizeof(ui_uint)) / 2)

struct ui_table {
    unsigned int seq;
    ui_hash keys[UI_VALUE_PAGE_CAPACITY];
    ui_uint values[UI_VALUE_PAGE_CAPACITY];
    struct ui_table *next, *prev;
};

union ui_page_data {
    struct ui_table tbl;
    struct ui_panel pan;
    struct ui_window win;
};

struct ui_page_element {
    union ui_page_data data;
    struct ui_page_element *next;
    struct ui_page_element *prev;
};

struct ui_page {
    unsigned int size;
    struct ui_page *next;
    struct ui_page_element win[1];
};

struct ui_pool {
    struct ui_allocator alloc;
    enum ui_allocation_type type;
    unsigned int page_count;
    struct ui_page *pages;
    struct ui_page_element *freelist;
    unsigned capacity;
    ui_size size;
    ui_size cap;
};

struct ui_context {
/* public: can be accessed freely */
    struct ui_input input;
    struct ui_style style;
    struct ui_buffer memory;
    struct ui_clipboard clip;
    ui_flags last_widget_state;
    enum ui_button_behavior button_behavior;
    struct ui_configuration_stacks stacks;
    float delta_time_seconds;

/* private:
    should only be accessed if you
    know what you are doing */
#ifdef UI_INCLUDE_VERTEX_BUFFER_OUTPUT
    struct ui_draw_list draw_list;
#endif
#ifdef UI_INCLUDE_COMMAND_USERDATA
    ui_handle userdata;
#endif
    /* text editor objects are quite big because of an internal
     * undo/redo stack. Therefore it does not make sense to have one for
     * each window for temporary use cases, so I only provide *one* instance
     * for all windows. This works because the content is cleared anyway */
    struct ui_text_edit text_edit;
    /* draw buffer used for overlay drawing operation like cursor */
    struct ui_command_buffer overlay;

    /* windows */
    int build;
    int use_pool;
    struct ui_pool pool;
    struct ui_window *begin;
    struct ui_window *end;
    struct ui_window *active;
    struct ui_window *current;
    struct ui_page_element *freelist;
    unsigned int count;
    unsigned int seq;
};

/* ==============================================================
 *                          MATH
 * =============================================================== */
#define UI_PI 3.141592654f
#define UI_UTF_INVALID 0xFFFD
#define UI_MAX_FLOAT_PRECISION 2

#define UI_UNUSED(x) ((void)(x))
#define UI_SATURATE(x) (UI_MAX(0, UI_MIN(1.0f, x)))
#define UI_LEN(a) (sizeof(a)/sizeof(a)[0])
#define UI_ABS(a) (((a) < 0) ? -(a) : (a))
#define UI_BETWEEN(x, a, b) ((a) <= (x) && (x) <= (b))
#define UI_INBOX(px, py, x, y, w, h)\
    (UI_BETWEEN(px,x,x+w) && UI_BETWEEN(py,y,y+h))
#define UI_INTERSECT(x0, y0, w0, h0, x1, y1, w1, h1) \
    (!(((x1 > (x0 + w0)) || ((x1 + w1) < x0) || (y1 > (y0 + h0)) || (y1 + h1) < y0)))
#define UI_CONTAINS(x, y, w, h, bx, by, bw, bh)\
    (UI_INBOX(x,y, bx, by, bw, bh) && UI_INBOX(x+w,y+h, bx, by, bw, bh))

#define ui_vec2_sub(a, b) ui_vec2((a).x - (b).x, (a).y - (b).y)
#define ui_vec2_add(a, b) ui_vec2((a).x + (b).x, (a).y + (b).y)
#define ui_vec2_len_sqr(a) ((a).x*(a).x+(a).y*(a).y)
#define ui_vec2_muls(a, t) ui_vec2((a).x * (t), (a).y * (t))

#define ui_ptr_add(t, p, i) ((t*)((void*)((ui_byte*)(p) + (i))))
#define ui_ptr_add_const(t, p, i) ((const t*)((const void*)((const ui_byte*)(p) + (i))))
#define ui_zero_struct(s) ui_zero(&s, sizeof(s))

/* ==============================================================
 *                          ALIGNMENT
 * =============================================================== */
/* Pointer to Integer type conversion for pointer alignment */
#if defined(__PTRDIFF_TYPE__) /* This case should work for GCC*/
# define UI_UINT_TO_PTR(x) ((void*)(__PTRDIFF_TYPE__)(x))
# define UI_PTR_TO_UINT(x) ((ui_size)(__PTRDIFF_TYPE__)(x))
#elif !defined(__GNUC__) /* works for compilers other than LLVM */
# define UI_UINT_TO_PTR(x) ((void*)&((char*)0)[x])
# define UI_PTR_TO_UINT(x) ((ui_size)(((char*)x)-(char*)0))
#elif defined(UI_USE_FIXED_TYPES) /* used if we have <stdint.h> */
# define UI_UINT_TO_PTR(x) ((void*)(uintptr_t)(x))
# define UI_PTR_TO_UINT(x) ((uintptr_t)(x))
#else /* generates warning but works */
# define UI_UINT_TO_PTR(x) ((void*)(x))
# define UI_PTR_TO_UINT(x) ((ui_size)(x))
#endif

#define UI_ALIGN_PTR(x, mask)\
    (UI_UINT_TO_PTR((UI_PTR_TO_UINT((ui_byte*)(x) + (mask-1)) & ~(mask-1))))
#define UI_ALIGN_PTR_BACK(x, mask)\
    (UI_UINT_TO_PTR((UI_PTR_TO_UINT((ui_byte*)(x)) & ~(mask-1))))

#define UI_OFFSETOF(st,m) ((ui_ptr)&(((st*)0)->m))
#define UI_CONTAINER_OF(ptr,type,member)\
    (type*)((void*)((char*)(1 ? (ptr): &((type*)0)->member) - UI_OFFSETOF(type, member)))

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
template<typename T> struct ui_alignof;
template<typename T, int size_diff> struct ui_helper{enum {value = size_diff};};
template<typename T> struct ui_helper<T,0>{enum {value = ui_alignof<T>::value};};
template<typename T> struct ui_alignof{struct Big {T x; char c;}; enum {
    diff = sizeof(Big) - sizeof(T), value = ui_helper<Big, diff>::value};};
#define UI_ALIGNOF(t) (ui_alignof<t>::value);
#elif defined(_MSC_VER)
#define UI_ALIGNOF(t) (__alignof(t))
#else
#define UI_ALIGNOF(t) ((char*)(&((struct {char c; t _h;}*)0)->_h) - (char*)0)
#endif

#endif /* UI_H_ */
/*
 * ==============================================================
 *
 *                          IMPLEMENTATION
 *
 * ===============================================================
 */
#ifdef UI_IMPLEMENTATION

#ifndef UI_POOL_DEFAULT_CAPACITY
#define UI_POOL_DEFAULT_CAPACITY 16
#endif

#ifndef UI_DEFAULT_COMMAND_BUFFER_SIZE
#define UI_DEFAULT_COMMAND_BUFFER_SIZE (4*1024)
#endif

#ifndef UI_BUFFER_DEFAULT_INITIAL_SIZE
#define UI_BUFFER_DEFAULT_INITIAL_SIZE (4*1024)
#endif

/* standard library headers */
#ifdef UI_INCLUDE_DEFAULT_ALLOCATOR
#include <stdlib.h> /* malloc, free */
#endif
#ifdef UI_INCLUDE_STANDARD_IO
#include <stdio.h> /* fopen, fclose,... */
#endif
#ifdef UI_INCLUDE_STANDARD_VARARGS
#include <stdarg.h> /* valist, va_start, va_end, ... */
#endif
#ifndef UI_ASSERT
#include <assert.h>
#define UI_ASSERT(expr) assert(expr)
#endif

#ifndef UI_MEMSET
#define UI_MEMSET ui_memset
#endif
#ifndef UI_MEMCPY
#define UI_MEMCPY ui_memcopy
#endif
#ifndef UI_SQRT
#define UI_SQRT ui_sqrt
#endif
#ifndef UI_SIN
#define UI_SIN ui_sin
#endif
#ifndef UI_COS
#define UI_COS ui_cos
#endif
#ifndef UI_STRTOD
#define UI_STRTOD ui_strtod
#endif
#ifndef UI_DTOA
#define UI_DTOA ui_dtoa
#endif

#define UI_DEFAULT (-1)

#ifndef UI_VSNPRINTF
/* If your compiler does support `vsnprintf` I would highly recommend
 * defining this to vsnprintf instead since `vsprintf` is basically
 * unbelievable unsafe and should *NEVER* be used. But I have to support
 * it since C89 only provides this unsafe version. */
  #if (defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)) ||\
      (defined(__cplusplus) && (__cplusplus >= 201103L)) || \
      (defined(_POSIX_C_SOURCE) && (_POSIX_C_SOURCE >= 200112L)) ||\
      (defined(_XOPEN_SOURCE) && (_XOPEN_SOURCE >= 500)) ||\
       defined(_ISOC99_SOURCE) || defined(_BSD_SOURCE)
      #define UI_VSNPRINTF(s,n,f,a) vsnprintf(s,n,f,a)
  #else
    #define UI_VSNPRINTF(s,n,f,a) vsprintf(s,f,a)
  #endif
#endif

#define UI_SCHAR_MIN (-127)
#define UI_SCHAR_MAX 127
#define UI_UCHAR_MIN 0
#define UI_UCHAR_MAX 256
#define UI_SSHORT_MIN (-32767)
#define UI_SSHORT_MAX 32767
#define UI_USHORT_MIN 0
#define UI_USHORT_MAX 65535
#define UI_SINT_MIN (-2147483647)
#define UI_SINT_MAX 2147483647
#define UI_UINT_MIN 0
#define UI_UINT_MAX 4294967295u

/* Make sure correct type size:
 * This will fire with a negative subscript error if the type sizes
 * are set incorrectly by the compiler, and compile out if not */
UI_STATIC_ASSERT(sizeof(ui_size) >= sizeof(void*));
UI_STATIC_ASSERT(sizeof(ui_ptr) == sizeof(void*));
UI_STATIC_ASSERT(sizeof(ui_flags) >= 4);
UI_STATIC_ASSERT(sizeof(ui_rune) >= 4);
UI_STATIC_ASSERT(sizeof(ui_ushort) == 2);
UI_STATIC_ASSERT(sizeof(ui_short) == 2);
UI_STATIC_ASSERT(sizeof(ui_uint) == 4);
UI_STATIC_ASSERT(sizeof(ui_int) == 4);
UI_STATIC_ASSERT(sizeof(ui_byte) == 1);

UI_GLOBAL const struct ui_rect ui_null_rect = {-8192.0f, -8192.0f, 16384, 16384};
#define UI_FLOAT_PRECISION 0.00000000000001

UI_GLOBAL const struct ui_color ui_red = {255,0,0,255};
UI_GLOBAL const struct ui_color ui_green = {0,255,0,255};
UI_GLOBAL const struct ui_color ui_blue = {0,0,255,255};
UI_GLOBAL const struct ui_color ui_white = {255,255,255,255};
UI_GLOBAL const struct ui_color ui_black = {0,0,0,255};
UI_GLOBAL const struct ui_color ui_yellow = {255,255,0,255};

/*
 * ==============================================================
 *
 *                          MATH
 *
 * ===============================================================
 */
/*  Since nuklear is supposed to work on all systems providing floating point
    math without any dependencies I also had to implement my own math functions
    for sqrt, sin and cos. Since the actual highly accurate implementations for
    the standard library functions are quite complex and I do not need high
    precision for my use cases I use approximations.

    Sqrt
    ----
    For square root nuklear uses the famous fast inverse square root:
    https://en.wikipedia.org/wiki/Fast_inverse_square_root with
    slightly tweaked magic constant. While on todays hardware it is
    probably not faster it is still fast and accurate enough for
    nuklear's use cases. IMPORTANT: this requires float format IEEE 754

    Sine/Cosine
    -----------
    All constants inside both function are generated Remez's minimax
    approximations for value range 0...2*PI. The reason why I decided to
    approximate exactly that range is that nuklear only needs sine and
    cosine to generate circles which only requires that exact range.
    In addition I used Remez instead of Taylor for additional precision:
    www.lolengine.net/blog/2011/12/21/better-function-approximatations.

    The tool I used to generate constants for both sine and cosine
    (it can actually approximate a lot more functions) can be
    found here: www.lolengine.net/wiki/oss/lolremez
*/
UI_INTERN float
ui_inv_sqrt(float number)
{
    float x2;
    const float threehalfs = 1.5f;
    union {ui_uint i; float f;} conv = {0};
    conv.f = number;
    x2 = number * 0.5f;
    conv.i = 0x5f375A84 - (conv.i >> 1);
    conv.f = conv.f * (threehalfs - (x2 * conv.f * conv.f));
    return conv.f;
}

UI_INTERN float
ui_sqrt(float x)
{
    return x * ui_inv_sqrt(x);
}

UI_INTERN float
ui_sin(float x)
{
    UI_STORAGE const float a0 = +1.91059300966915117e-31f;
    UI_STORAGE const float a1 = +1.00086760103908896f;
    UI_STORAGE const float a2 = -1.21276126894734565e-2f;
    UI_STORAGE const float a3 = -1.38078780785773762e-1f;
    UI_STORAGE const float a4 = -2.67353392911981221e-2f;
    UI_STORAGE const float a5 = +2.08026600266304389e-2f;
    UI_STORAGE const float a6 = -3.03996055049204407e-3f;
    UI_STORAGE const float a7 = +1.38235642404333740e-4f;
    return a0 + x*(a1 + x*(a2 + x*(a3 + x*(a4 + x*(a5 + x*(a6 + x*a7))))));
}

UI_INTERN float
ui_cos(float x)
{
    UI_STORAGE const float a0 = +1.00238601909309722f;
    UI_STORAGE const float a1 = -3.81919947353040024e-2f;
    UI_STORAGE const float a2 = -3.94382342128062756e-1f;
    UI_STORAGE const float a3 = -1.18134036025221444e-1f;
    UI_STORAGE const float a4 = +1.07123798512170878e-1f;
    UI_STORAGE const float a5 = -1.86637164165180873e-2f;
    UI_STORAGE const float a6 = +9.90140908664079833e-4f;
    UI_STORAGE const float a7 = -5.23022132118824778e-14f;
    return a0 + x*(a1 + x*(a2 + x*(a3 + x*(a4 + x*(a5 + x*(a6 + x*a7))))));
}

UI_INTERN ui_uint
ui_round_up_pow2(ui_uint v)
{
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;
    return v;
}

UI_API struct ui_rect
ui_get_null_rect(void)
{
    return ui_null_rect;
}

UI_API struct ui_rect
ui_rect(float x, float y, float w, float h)
{
    struct ui_rect r;
    r.x = x; r.y = y;
    r.w = w; r.h = h;
    return r;
}

UI_API struct ui_rect
ui_recti(int x, int y, int w, int h)
{
    struct ui_rect r;
    r.x = (float)x;
    r.y = (float)y;
    r.w = (float)w;
    r.h = (float)h;
    return r;
}

UI_API struct ui_rect
ui_recta(struct ui_vec2 pos, struct ui_vec2 size)
{
    return ui_rect(pos.x, pos.y, size.x, size.y);
}

UI_API struct ui_rect
ui_rectv(const float *r)
{
    return ui_rect(r[0], r[1], r[2], r[3]);
}

UI_API struct ui_rect
ui_rectiv(const int *r)
{
    return ui_recti(r[0], r[1], r[2], r[3]);
}

UI_API struct ui_vec2
ui_rect_pos(struct ui_rect r)
{
    struct ui_vec2 ret;
    ret.x = r.x; ret.y = r.y;
    return ret;
}

UI_API struct ui_vec2
ui_rect_size(struct ui_rect r)
{
    struct ui_vec2 ret;
    ret.x = r.w; ret.y = r.h;
    return ret;
}

UI_INTERN struct ui_rect
ui_shriui_rect(struct ui_rect r, float amount)
{
    struct ui_rect res;
    r.w = UI_MAX(r.w, 2 * amount);
    r.h = UI_MAX(r.h, 2 * amount);
    res.x = r.x + amount;
    res.y = r.y + amount;
    res.w = r.w - 2 * amount;
    res.h = r.h - 2 * amount;
    return res;
}

UI_INTERN struct ui_rect
ui_pad_rect(struct ui_rect r, struct ui_vec2 pad)
{
    r.w = UI_MAX(r.w, 2 * pad.x);
    r.h = UI_MAX(r.h, 2 * pad.y);
    r.x += pad.x; r.y += pad.y;
    r.w -= 2 * pad.x;
    r.h -= 2 * pad.y;
    return r;
}

UI_API struct ui_vec2
ui_vec2(float x, float y)
{
    struct ui_vec2 ret;
    ret.x = x; ret.y = y;
    return ret;
}

UI_API struct ui_vec2
ui_vec2i(int x, int y)
{
    struct ui_vec2 ret;
    ret.x = (float)x;
    ret.y = (float)y;
    return ret;
}

UI_API struct ui_vec2
ui_vec2v(const float *v)
{
    return ui_vec2(v[0], v[1]);
}

UI_API struct ui_vec2
ui_vec2iv(const int *v)
{
    return ui_vec2i(v[0], v[1]);
}

/*
 * ==============================================================
 *
 *                          UTIL
 *
 * ===============================================================
 */
UI_INTERN int ui_str_match_here(const char *regexp, const char *text);
UI_INTERN int ui_str_match_star(int c, const char *regexp, const char *text);
UI_INTERN int ui_is_lower(int c) {return (c >= 'a' && c <= 'z') || (c >= 0xE0 && c <= 0xFF);}
UI_INTERN int ui_is_upper(int c){return (c >= 'A' && c <= 'Z') || (c >= 0xC0 && c <= 0xDF);}
UI_INTERN int ui_to_upper(int c) {return (c >= 'a' && c <= 'z') ? (c - ('a' - 'A')) : c;}
UI_INTERN int ui_to_lower(int c) {return (c >= 'A' && c <= 'Z') ? (c - ('a' + 'A')) : c;}

UI_INTERN void*
ui_memcopy(void *dst0, const void *src0, ui_size length)
{
    ui_ptr t;
    char *dst = (char*)dst0;
    const char *src = (const char*)src0;
    if (length == 0 || dst == src)
        goto done;

    #define ui_word int
    #define ui_wsize sizeof(ui_word)
    #define ui_wmask (ui_wsize-1)
    #define UI_TLOOP(s) if (t) UI_TLOOP1(s)
    #define UI_TLOOP1(s) do { s; } while (--t)

    if (dst < src) {
        t = (ui_ptr)src; /* only need low bits */
        if ((t | (ui_ptr)dst) & ui_wmask) {
            if ((t ^ (ui_ptr)dst) & ui_wmask || length < ui_wsize)
                t = length;
            else
                t = ui_wsize - (t & ui_wmask);
            length -= t;
            UI_TLOOP1(*dst++ = *src++);
        }
        t = length / ui_wsize;
        UI_TLOOP(*(ui_word*)(void*)dst = *(const ui_word*)(const void*)src;
            src += ui_wsize; dst += ui_wsize);
        t = length & ui_wmask;
        UI_TLOOP(*dst++ = *src++);
    } else {
        src += length;
        dst += length;
        t = (ui_ptr)src;
        if ((t | (ui_ptr)dst) & ui_wmask) {
            if ((t ^ (ui_ptr)dst) & ui_wmask || length <= ui_wsize)
                t = length;
            else
                t &= ui_wmask;
            length -= t;
            UI_TLOOP1(*--dst = *--src);
        }
        t = length / ui_wsize;
        UI_TLOOP(src -= ui_wsize; dst -= ui_wsize;
            *(ui_word*)(void*)dst = *(const ui_word*)(const void*)src);
        t = length & ui_wmask;
        UI_TLOOP(*--dst = *--src);
    }
    #undef ui_word
    #undef ui_wsize
    #undef ui_wmask
    #undef UI_TLOOP
    #undef UI_TLOOP1
done:
    return (dst0);
}

UI_INTERN void
ui_memset(void *ptr, int c0, ui_size size)
{
    #define ui_word unsigned
    #define ui_wsize sizeof(ui_word)
    #define ui_wmask (ui_wsize - 1)
    ui_byte *dst = (ui_byte*)ptr;
    unsigned c = 0;
    ui_size t = 0;

    if ((c = (ui_byte)c0) != 0) {
        c = (c << 8) | c; /* at least 16-bits  */
        if (sizeof(unsigned int) > 2)
            c = (c << 16) | c; /* at least 32-bits*/
    }

    /* too small of a word count */
    dst = (ui_byte*)ptr;
    if (size < 3 * ui_wsize) {
        while (size--) *dst++ = (ui_byte)c0;
        return;
    }

    /* align destination */
    if ((t = UI_PTR_TO_UINT(dst) & ui_wmask) != 0) {
        t = ui_wsize -t;
        size -= t;
        do {
            *dst++ = (ui_byte)c0;
        } while (--t != 0);
    }

    /* fill word */
    t = size / ui_wsize;
    do {
        *(ui_word*)((void*)dst) = c;
        dst += ui_wsize;
    } while (--t != 0);

    /* fill trailing bytes */
    t = (size & ui_wmask);
    if (t != 0) {
        do {
            *dst++ = (ui_byte)c0;
        } while (--t != 0);
    }

    #undef ui_word
    #undef ui_wsize
    #undef ui_wmask
}

UI_INTERN void
ui_zero(void *ptr, ui_size size)
{
    UI_ASSERT(ptr);
    UI_MEMSET(ptr, 0, size);
}

UI_API int
ui_strlen(const char *str)
{
    int siz = 0;
    UI_ASSERT(str);
    while (str && *str++ != '\0') siz++;
    return siz;
}

UI_API int
ui_strtoi(const char *str, char **endptr)
{
    int neg = 1;
    const char *p = str;
    int value = 0;

    UI_ASSERT(str);
    if (!str) return 0;

    /* skip whitespace */
    while (*p == ' ') p++;
    if (*p == '-') {
        neg = -1;
        p++;
    }
    while (*p && *p >= '0' && *p <= '9') {
        value = value * 10 + (int) (*p - '0');
        p++;
    }
    if (endptr)
        *endptr = (char*)p;
    return neg*value;
}

UI_API double
ui_strtod(const char *str, char **endptr)
{
    double m;
    double neg = 1.0;
    const char *p = str;
    double value = 0;
    double number = 0;

    UI_ASSERT(str);
    if (!str) return 0;

    /* skip whitespace */
    while (*p == ' ') p++;
    if (*p == '-') {
        neg = -1.0;
        p++;
    }

    while (*p && *p != '.' && *p != 'e') {
        value = value * 10.0 + (double) (*p - '0');
        p++;
    }

    if (*p == '.') {
        p++;
        for(m = 0.1; *p && *p != 'e'; p++ ) {
            value = value + (double) (*p - '0') * m;
            m *= 0.1;
        }
    }
    if (*p == 'e') {
        int i, pow, div;
        p++;
        if (*p == '-') {
            div = ui_true;
            p++;
        } else if (*p == '+') {
            div = ui_false;
            p++;
        } else div = ui_false;

        for (pow = 0; *p; p++)
            pow = pow * 10 + (int) (*p - '0');

        for (m = 1.0, i = 0; i < pow; i++)
            m *= 10.0;

        if (div)
            value /= m;
        else value *= m;
    }
    number = value * neg;
    if (endptr)
        *endptr = (char*)p;
    return number;
}

UI_API float
ui_strtof(const char *str, char **endptr)
{
    float float_value;
    double double_value;
    double_value = UI_STRTOD(str, endptr);
    float_value = (float)double_value;
    return float_value;
}

UI_API int
ui_stricmp(const char *s1, const char *s2)
{
    ui_int c1,c2,d;
    do {
        c1 = *s1++;
        c2 = *s2++;
        d = c1 - c2;
        while (d) {
            if (c1 <= 'Z' && c1 >= 'A') {
                d += ('a' - 'A');
                if (!d) break;
            }
            if (c2 <= 'Z' && c2 >= 'A') {
                d -= ('a' - 'A');
                if (!d) break;
            }
            return ((d >= 0) << 1) - 1;
        }
    } while (c1);
    return 0;
}

UI_API int
ui_stricmpn(const char *s1, const char *s2, int n)
{
    int c1,c2,d;
    UI_ASSERT(n >= 0);
    do {
        c1 = *s1++;
        c2 = *s2++;
        if (!n--) return 0;

        d = c1 - c2;
        while (d) {
            if (c1 <= 'Z' && c1 >= 'A') {
                d += ('a' - 'A');
                if (!d) break;
            }
            if (c2 <= 'Z' && c2 >= 'A') {
                d -= ('a' - 'A');
                if (!d) break;
            }
            return ((d >= 0) << 1) - 1;
        }
    } while (c1);
    return 0;
}

UI_INTERN int
ui_str_match_here(const char *regexp, const char *text)
{
    if (regexp[0] == '\0')
        return 1;
    if (regexp[1] == '*')
        return ui_str_match_star(regexp[0], regexp+2, text);
    if (regexp[0] == '$' && regexp[1] == '\0')
        return *text == '\0';
    if (*text!='\0' && (regexp[0]=='.' || regexp[0]==*text))
        return ui_str_match_here(regexp+1, text+1);
    return 0;
}

UI_INTERN int
ui_str_match_star(int c, const char *regexp, const char *text)
{
    do {/* a '* matches zero or more instances */
        if (ui_str_match_here(regexp, text))
            return 1;
    } while (*text != '\0' && (*text++ == c || c == '.'));
    return 0;
}

UI_API int
ui_strfilter(const char *text, const char *regexp)
{
    /*
    c    matches any literal character c
    .    matches any single character
    ^    matches the beginning of the input string
    $    matches the end of the input string
    *    matches zero or more occurrences of the previous character*/
    if (regexp[0] == '^')
        return ui_str_match_here(regexp+1, text);
    do {    /* must look even if string is empty */
        if (ui_str_match_here(regexp, text))
            return 1;
    } while (*text++ != '\0');
    return 0;
}

UI_API int
ui_strmatch_fuzzy_text(const char *str, int str_len,
    const char *pattern, int *out_score)
{
    /* Returns true if each character in pattern is found sequentially within str
     * if found then outScore is also set. Score value has no intrinsic meaning.
     * Range varies with pattern. Can only compare scores with same search pattern. */

    /* ------- scores --------- */
    /* bonus for adjacent matches */
    #define UI_ADJACENCY_BONUS 5
    /* bonus if match occurs after a separator */
    #define UI_SEPARATOR_BONUS 10
    /* bonus if match is uppercase and prev is lower */
    #define UI_CAMEL_BONUS 10
    /* penalty applied for every letter in str before the first match */
    #define UI_LEADING_LETTER_PENALTY (-3)
    /* maximum penalty for leading letters */
    #define UI_MAX_LEADING_LETTER_PENALTY (-9)
    /* penalty for every letter that doesn't matter */
    #define UI_UNMATCHED_LETTER_PENALTY (-1)

    /* loop variables */
    int score = 0;
    char const * pattern_iter = pattern;
    int str_iter = 0;
    int prev_matched = ui_false;
    int prev_lower = ui_false;
    /* true so if first letter match gets separator bonus*/
    int prev_separator = ui_true;

    /* use "best" matched letter if multiple string letters match the pattern */
    char const * best_letter = 0;
    int best_letter_score = 0;

    /* loop over strings */
    UI_ASSERT(str);
    UI_ASSERT(pattern);
    if (!str || !str_len || !pattern) return 0;
    while (str_iter < str_len)
    {
        const char pattern_letter = *pattern_iter;
        const char str_letter = str[str_iter];

        int next_match = *pattern_iter != '\0' &&
            ui_to_lower(pattern_letter) == ui_to_lower(str_letter);
        int rematch = best_letter && ui_to_upper(*best_letter) == ui_to_upper(str_letter);

        int advanced = next_match && best_letter;
        int pattern_repeat = best_letter && *pattern_iter != '\0';
        pattern_repeat = pattern_repeat &&
            ui_to_lower(*best_letter) == ui_to_lower(pattern_letter);

        if (advanced || pattern_repeat) {
            score += best_letter_score;
            best_letter = 0;
            best_letter_score = 0;
        }

        if (next_match || rematch)
        {
            int new_score = 0;
            /* Apply penalty for each letter before the first pattern match */
            if (pattern_iter == pattern) {
                int count = (int)(&str[str_iter] - str);
                int penalty = UI_LEADING_LETTER_PENALTY * count;
                if (penalty < UI_MAX_LEADING_LETTER_PENALTY)
                    penalty = UI_MAX_LEADING_LETTER_PENALTY;

                score += penalty;
            }

            /* apply bonus for consecutive bonuses */
            if (prev_matched)
                new_score += UI_ADJACENCY_BONUS;

            /* apply bonus for matches after a separator */
            if (prev_separator)
                new_score += UI_SEPARATOR_BONUS;

            /* apply bonus across camel case boundaries */
            if (prev_lower && ui_is_upper(str_letter))
                new_score += UI_CAMEL_BONUS;

            /* update pattern iter IFF the next pattern letter was matched */
            if (next_match)
                ++pattern_iter;

            /* update best letter in str which may be for a "next" letter or a rematch */
            if (new_score >= best_letter_score) {
                /* apply penalty for now skipped letter */
                if (best_letter != 0)
                    score += UI_UNMATCHED_LETTER_PENALTY;

                best_letter = &str[str_iter];
                best_letter_score = new_score;
            }
            prev_matched = ui_true;
        } else {
            score += UI_UNMATCHED_LETTER_PENALTY;
            prev_matched = ui_false;
        }

        /* separators should be more easily defined */
        prev_lower = ui_is_lower(str_letter) != 0;
        prev_separator = str_letter == '_' || str_letter == ' ';

        ++str_iter;
    }

    /* apply score for last match */
    if (best_letter)
        score += best_letter_score;

    /* did not match full pattern */
    if (*pattern_iter != '\0')
        return ui_false;

    if (out_score)
        *out_score = score;
    return ui_true;
}

UI_API int
ui_strmatch_fuzzy_string(char const *str, char const *pattern, int *out_score)
{return ui_strmatch_fuzzy_text(str, ui_strlen(str), pattern, out_score);}

UI_INTERN int
ui_string_float_limit(char *string, int prec)
{
    int dot = 0;
    char *c = string;
    while (*c) {
        if (*c == '.') {
            dot = 1;
            c++;
            continue;
        }
        if (dot == (prec+1)) {
            *c = 0;
            break;
        }
        if (dot > 0) dot++;
        c++;
    }
    return (int)(c - string);
}

UI_INTERN double
ui_pow(double x, int n)
{
    /*  check the sign of n */
    double r = 1;
    int plus = n >= 0;
    n = (plus) ? n : -n;
    while (n > 0) {
        if ((n & 1) == 1)
            r *= x;
        n /= 2;
        x *= x;
    }
    return plus ? r : 1.0 / r;
}

UI_INTERN int
ui_ifloord(double x)
{
    x = (double)((int)x - ((x < 0.0) ? 1 : 0));
    return (int)x;
}

UI_INTERN int
ui_ifloorf(float x)
{
    x = (float)((int)x - ((x < 0.0f) ? 1 : 0));
    return (int)x;
}

UI_INTERN int
ui_iceilf(float x)
{
    if (x >= 0) {
        int i = (int)x;
        return i;
    } else {
        int t = (int)x;
        float r = x - (float)t;
        return (r > 0.0f) ? t+1: t;
    }
}

UI_INTERN int
ui_log10(double n)
{
    int neg;
    int ret;
    int exp = 0;

    neg = (n < 0) ? 1 : 0;
    ret = (neg) ? (int)-n : (int)n;
    while ((ret / 10) > 0) {
        ret /= 10;
        exp++;
    }
    if (neg) exp = -exp;
    return exp;
}

UI_INTERN void
ui_strrev_ascii(char *s)
{
    int len = ui_strlen(s);
    int end = len / 2;
    int i = 0;
    char t;
    for (; i < end; ++i) {
        t = s[i];
        s[i] = s[len - 1 - i];
        s[len -1 - i] = t;
    }
}

UI_INTERN char*
ui_itoa(char *s, long n)
{
    long i = 0;
    if (n == 0) {
        s[i++] = '0';
        s[i] = 0;
        return s;
    }
    if (n < 0) {
        s[i++] = '-';
        n = -n;
    }
    while (n > 0) {
        s[i++] = (char)('0' + (n % 10));
        n /= 10;
    }
    s[i] = 0;
    if (s[0] == '-')
        ++s;

    ui_strrev_ascii(s);
    return s;
}

UI_INTERN char*
ui_dtoa(char *s, double n)
{
    int useExp = 0;
    int digit = 0, m = 0, m1 = 0;
    char *c = s;
    int neg = 0;

    UI_ASSERT(s);
    if (!s) return 0;

    if (n == 0.0) {
        s[0] = '0'; s[1] = '\0';
        return s;
    }

    neg = (n < 0);
    if (neg) n = -n;

    /* calculate magnitude */
    m = ui_log10(n);
    useExp = (m >= 14 || (neg && m >= 9) || m <= -9);
    if (neg) *(c++) = '-';

    /* set up for scientific notation */
    if (useExp) {
        if (m < 0)
           m -= 1;
        n = n / (double)ui_pow(10.0, m);
        m1 = m;
        m = 0;
    }
    if (m < 1.0) {
        m = 0;
    }

    /* convert the number */
    while (n > UI_FLOAT_PRECISION || m >= 0) {
        double weight = ui_pow(10.0, m);
        if (weight > 0) {
            double t = (double)n / weight;
            digit = ui_ifloord(t);
            n -= ((double)digit * weight);
            *(c++) = (char)('0' + (char)digit);
        }
        if (m == 0 && n > 0)
            *(c++) = '.';
        m--;
    }

    if (useExp) {
        /* convert the exponent */
        int i, j;
        *(c++) = 'e';
        if (m1 > 0) {
            *(c++) = '+';
        } else {
            *(c++) = '-';
            m1 = -m1;
        }
        m = 0;
        while (m1 > 0) {
            *(c++) = (char)('0' + (char)(m1 % 10));
            m1 /= 10;
            m++;
        }
        c -= m;
        for (i = 0, j = m-1; i<j; i++, j--) {
            /* swap without temporary */
            c[i] ^= c[j];
            c[j] ^= c[i];
            c[i] ^= c[j];
        }
        c += m;
    }
    *(c) = '\0';
    return s;
}

#ifdef UI_INCLUDE_STANDARD_VARARGS
static int
ui_vsnprintf(char *buf, int buf_size, const char *fmt, va_list args)
{
    enum ui_arg_type {
        UI_ARG_TYPE_CHAR,
        UI_ARG_TYPE_SHORT,
        UI_ARG_TYPE_DEFAULT,
        UI_ARG_TYPE_LONG
    };
    enum ui_arg_flags {
        UI_ARG_FLAG_LEFT = 0x01,
        UI_ARG_FLAG_PLUS = 0x02,
        UI_ARG_FLAG_SPACE = 0x04,
        UI_ARG_FLAG_NUM = 0x10,
        UI_ARG_FLAG_ZERO = 0x20
    };

    char number_buffer[UI_MAX_NUMBER_BUFFER];
    enum ui_arg_type arg_type = UI_ARG_TYPE_DEFAULT;
    int precision = UI_DEFAULT;
    int width = UI_DEFAULT;
    ui_flags flag = 0;

    int len = 0;
    int result = -1;
    const char *iter = fmt;

    UI_ASSERT(buf);
    UI_ASSERT(buf_size);
    if (!buf || !buf_size || !fmt) return 0;
    for (iter = fmt; *iter && len < buf_size; iter++) {
        /* copy all non-format characters */
        while (*iter && (*iter != '%') && (len < buf_size))
            buf[len++] = *iter++;
        if (!(*iter) || len >= buf_size) break;
        iter++;

        /* flag arguments */
        while (*iter) {
            if (*iter == '-') flag |= UI_ARG_FLAG_LEFT;
            else if (*iter == '+') flag |= UI_ARG_FLAG_PLUS;
            else if (*iter == ' ') flag |= UI_ARG_FLAG_SPACE;
            else if (*iter == '#') flag |= UI_ARG_FLAG_NUM;
            else if (*iter == '0') flag |= UI_ARG_FLAG_ZERO;
            else break;
            iter++;
        }

        /* width argument */
        width = UI_DEFAULT;
        if (*iter >= '1' && *iter <= '9') {
            char *end;
            width = ui_strtoi(iter, &end);
            if (end == iter)
                width = -1;
            else iter = end;
        } else if (*iter == '*') {
            width = va_arg(args, int);
            iter++;
        }

        /* precision argument */
        precision = UI_DEFAULT;
        if (*iter == '.') {
            iter++;
            if (*iter == '*') {
                precision = va_arg(args, int);
                iter++;
            } else {
                char *end;
                precision = ui_strtoi(iter, &end);
                if (end == iter)
                    precision = -1;
                else iter = end;
            }
        }

        /* length modifier */
        if (*iter == 'h') {
            if (*(iter+1) == 'h') {
                arg_type = UI_ARG_TYPE_CHAR;
                iter++;
            } else arg_type = UI_ARG_TYPE_SHORT;
            iter++;
        } else if (*iter == 'l') {
            arg_type = UI_ARG_TYPE_LONG;
            iter++;
        } else arg_type = UI_ARG_TYPE_DEFAULT;

        /* specifier */
        if (*iter == '%') {
            UI_ASSERT(arg_type == UI_ARG_TYPE_DEFAULT);
            UI_ASSERT(precision == UI_DEFAULT);
            UI_ASSERT(width == UI_DEFAULT);
            if (len < buf_size)
                buf[len++] = '%';
        } else if (*iter == 's') {
            /* string  */
            const char *str = va_arg(args, const char*);
            UI_ASSERT(str != buf && "buffer and argument are not allowed to overlap!");
            UI_ASSERT(arg_type == UI_ARG_TYPE_DEFAULT);
            UI_ASSERT(precision == UI_DEFAULT);
            UI_ASSERT(width == UI_DEFAULT);
            if (str == buf) return -1;
            while (str && *str && len < buf_size)
                buf[len++] = *str++;
        } else if (*iter == 'n') {
            /* current length callback */
            signed int *n = va_arg(args, int*);
            UI_ASSERT(arg_type == UI_ARG_TYPE_DEFAULT);
            UI_ASSERT(precision == UI_DEFAULT);
            UI_ASSERT(width == UI_DEFAULT);
            if (n) *n = len;
        } else if (*iter == 'c' || *iter == 'i' || *iter == 'd') {
            /* signed integer */
            long value = 0;
            const char *num_iter;
            int num_len, num_print, padding;
            int cur_precision = UI_MAX(precision, 1);
            int cur_width = UI_MAX(width, 0);

            /* retrieve correct value type */
            if (arg_type == UI_ARG_TYPE_CHAR)
                value = (signed char)va_arg(args, int);
            else if (arg_type == UI_ARG_TYPE_SHORT)
                value = (signed short)va_arg(args, int);
            else if (arg_type == UI_ARG_TYPE_LONG)
                value = va_arg(args, signed long);
            else if (*iter == 'c')
                value = (unsigned char)va_arg(args, int);
            else value = va_arg(args, signed int);

            /* convert number to string */
            ui_itoa(number_buffer, value);
            num_len = ui_strlen(number_buffer);
            padding = UI_MAX(cur_width - UI_MAX(cur_precision, num_len), 0);
            if ((flag & UI_ARG_FLAG_PLUS) || (flag & UI_ARG_FLAG_SPACE))
                padding = UI_MAX(padding-1, 0);

            /* fill left padding up to a total of `width` characters */
            if (!(flag & UI_ARG_FLAG_LEFT)) {
                while (padding-- > 0 && (len < buf_size)) {
                    if ((flag & UI_ARG_FLAG_ZERO) && (precision == UI_DEFAULT))
                        buf[len++] = '0';
                    else buf[len++] = ' ';
                }
            }

            /* copy string value representation into buffer */
            if ((flag & UI_ARG_FLAG_PLUS) && value >= 0 && len < buf_size)
                buf[len++] = '+';
            else if ((flag & UI_ARG_FLAG_SPACE) && value >= 0 && len < buf_size)
                buf[len++] = ' ';

            /* fill up to precision number of digits with '0' */
            num_print = UI_MAX(cur_precision, num_len);
            while (precision && (num_print > num_len) && (len < buf_size)) {
                buf[len++] = '0';
                num_print--;
            }

            /* copy string value representation into buffer */
            num_iter = number_buffer;
            while (precision && *num_iter && len < buf_size)
                buf[len++] = *num_iter++;

            /* fill right padding up to width characters */
            if (flag & UI_ARG_FLAG_LEFT) {
                while ((padding-- > 0) && (len < buf_size))
                    buf[len++] = ' ';
            }
        } else if (*iter == 'o' || *iter == 'x' || *iter == 'X' || *iter == 'u') {
            /* unsigned integer */
            unsigned long value = 0;
            int num_len = 0, num_print, padding = 0;
            int cur_precision = UI_MAX(precision, 1);
            int cur_width = UI_MAX(width, 0);
            unsigned int base = (*iter == 'o') ? 8: (*iter == 'u')? 10: 16;

            /* print oct/hex/dec value */
            const char *upper_output_format = "0123456789ABCDEF";
            const char *lower_output_format = "0123456789abcdef";
            const char *output_format = (*iter == 'x') ?
                lower_output_format: upper_output_format;

            /* retrieve correct value type */
            if (arg_type == UI_ARG_TYPE_CHAR)
                value = (unsigned char)va_arg(args, int);
            else if (arg_type == UI_ARG_TYPE_SHORT)
                value = (unsigned short)va_arg(args, int);
            else if (arg_type == UI_ARG_TYPE_LONG)
                value = va_arg(args, unsigned long);
            else value = va_arg(args, unsigned int);

            do {
                /* convert decimal number into hex/oct number */
                int digit = output_format[value % base];
                if (num_len < UI_MAX_NUMBER_BUFFER)
                    number_buffer[num_len++] = (char)digit;
                value /= base;
            } while (value > 0);

            num_print = UI_MAX(cur_precision, num_len);
            padding = UI_MAX(cur_width - UI_MAX(cur_precision, num_len), 0);
            if (flag & UI_ARG_FLAG_NUM)
                padding = UI_MAX(padding-1, 0);

            /* fill left padding up to a total of `width` characters */
            if (!(flag & UI_ARG_FLAG_LEFT)) {
                while ((padding-- > 0) && (len < buf_size)) {
                    if ((flag & UI_ARG_FLAG_ZERO) && (precision == UI_DEFAULT))
                        buf[len++] = '0';
                    else buf[len++] = ' ';
                }
            }

            /* fill up to precision number of digits */
            if (num_print && (flag & UI_ARG_FLAG_NUM)) {
                if ((*iter == 'o') && (len < buf_size)) {
                    buf[len++] = '0';
                } else if ((*iter == 'x') && ((len+1) < buf_size)) {
                    buf[len++] = '0';
                    buf[len++] = 'x';
                } else if ((*iter == 'X') && ((len+1) < buf_size)) {
                    buf[len++] = '0';
                    buf[len++] = 'X';
                }
            }
            while (precision && (num_print > num_len) && (len < buf_size)) {
                buf[len++] = '0';
                num_print--;
            }

            /* reverse number direction */
            while (num_len > 0) {
                if (precision && (len < buf_size))
                    buf[len++] = number_buffer[num_len-1];
                num_len--;
            }

            /* fill right padding up to width characters */
            if (flag & UI_ARG_FLAG_LEFT) {
                while ((padding-- > 0) && (len < buf_size))
                    buf[len++] = ' ';
            }
        } else if (*iter == 'f') {
            /* floating point */
            const char *num_iter;
            int cur_precision = (precision < 0) ? 6: precision;
            int prefix, cur_width = UI_MAX(width, 0);
            double value = va_arg(args, double);
            int num_len = 0, frac_len = 0, dot = 0;
            int padding = 0;

            UI_ASSERT(arg_type == UI_ARG_TYPE_DEFAULT);
            UI_DTOA(number_buffer, value);
            num_len = ui_strlen(number_buffer);

            /* calculate padding */
            num_iter = number_buffer;
            while (*num_iter && *num_iter != '.')
                num_iter++;

            prefix = (*num_iter == '.')?(int)(num_iter - number_buffer)+1:0;
            padding = UI_MAX(cur_width - (prefix + UI_MIN(cur_precision, num_len - prefix)) , 0);
            if ((flag & UI_ARG_FLAG_PLUS) || (flag & UI_ARG_FLAG_SPACE))
                padding = UI_MAX(padding-1, 0);

            /* fill left padding up to a total of `width` characters */
            if (!(flag & UI_ARG_FLAG_LEFT)) {
                while (padding-- > 0 && (len < buf_size)) {
                    if (flag & UI_ARG_FLAG_ZERO)
                        buf[len++] = '0';
                    else buf[len++] = ' ';
                }
            }

            /* copy string value representation into buffer */
            num_iter = number_buffer;
            if ((flag & UI_ARG_FLAG_PLUS) && (value >= 0) && (len < buf_size))
                buf[len++] = '+';
            else if ((flag & UI_ARG_FLAG_SPACE) && (value >= 0) && (len < buf_size))
                buf[len++] = ' ';
            while (*num_iter) {
                if (dot) frac_len++;
                if (len < buf_size)
                    buf[len++] = *num_iter;
                if (*num_iter == '.') dot = 1;
                if (frac_len >= cur_precision) break;
                num_iter++;
            }

            /* fill number up to precision */
            while (frac_len < cur_precision) {
                if (!dot && len < buf_size) {
                    buf[len++] = '.';
                    dot = 1;
                }
                if (len < buf_size)
                    buf[len++] = '0';
                frac_len++;
            }

            /* fill right padding up to width characters */
            if (flag & UI_ARG_FLAG_LEFT) {
                while ((padding-- > 0) && (len < buf_size))
                    buf[len++] = ' ';
            }
        } else {
            /* Specifier not supported: g,G,e,E,p,z */
            UI_ASSERT(0 && "specifier is not supported!");
            return result;
        }
    }
    buf[(len >= buf_size)?(buf_size-1):len] = 0;
    result = (len >= buf_size)?-1:len;
    return result;
}

UI_INTERN int
ui_strfmt(char *buf, int buf_size, const char *fmt, va_list args)
{
    int result = -1;
    UI_ASSERT(buf);
    UI_ASSERT(buf_size);
    if (!buf || !buf_size || !fmt) return 0;
#ifdef UI_INCLUDE_STANDARD_IO
    result = UI_VSNPRINTF(buf, (ui_size)buf_size, fmt, args);
    result = (result >= buf_size) ? -1: result;
    buf[buf_size-1] = 0;
#else
    result = ui_vsnprintf(buf, buf_size, fmt, args);
#endif
    return result;
}
#endif

UI_API ui_hash
ui_murmur_hash(const void * key, int len, ui_hash seed)
{
    /* 32-Bit MurmurHash3: https://code.google.com/p/smhasher/wiki/MurmurHash3*/
    #define UI_ROTL(x,r) ((x) << (r) | ((x) >> (32 - r)))
    union {const ui_uint *i; const ui_byte *b;} conv = {0};
    const ui_byte *data = (const ui_byte*)key;
    const int nblocks = len/4;
    ui_uint h1 = seed;
    const ui_uint c1 = 0xcc9e2d51;
    const ui_uint c2 = 0x1b873593;
    const ui_byte *tail;
    const ui_uint *blocks;
    ui_uint k1;
    int i;

    /* body */
    if (!key) return 0;
    conv.b = (data + nblocks*4);
    blocks = (const ui_uint*)conv.i;
    for (i = -nblocks; i; ++i) {
        k1 = blocks[i];
        k1 *= c1;
        k1 = UI_ROTL(k1,15);
        k1 *= c2;

        h1 ^= k1;
        h1 = UI_ROTL(h1,13);
        h1 = h1*5+0xe6546b64;
    }

    /* tail */
    tail = (const ui_byte*)(data + nblocks*4);
    k1 = 0;
    switch (len & 3) {
    case 3: k1 ^= (ui_uint)(tail[2] << 16);
    case 2: k1 ^= (ui_uint)(tail[1] << 8u);
    case 1: k1 ^= tail[0];
            k1 *= c1;
            k1 = UI_ROTL(k1,15);
            k1 *= c2;
            h1 ^= k1;
    default: break;
    }

    /* finalization */
    h1 ^= (ui_uint)len;
    /* fmix32 */
    h1 ^= h1 >> 16;
    h1 *= 0x85ebca6b;
    h1 ^= h1 >> 13;
    h1 *= 0xc2b2ae35;
    h1 ^= h1 >> 16;

    #undef UI_ROTL
    return h1;
}

#ifdef UI_INCLUDE_STANDARD_IO
UI_INTERN char*
ui_file_load(const char* path, ui_size* siz, struct ui_allocator *alloc)
{
    char *buf;
    FILE *fd;
    long ret;

    UI_ASSERT(path);
    UI_ASSERT(siz);
    UI_ASSERT(alloc);
    if (!path || !siz || !alloc)
        return 0;

    fd = fopen(path, "rb");
    if (!fd) return 0;
    fseek(fd, 0, SEEK_END);
    ret = ftell(fd);
    if (ret < 0) {
        fclose(fd);
        return 0;
    }
    *siz = (ui_size)ret;
    fseek(fd, 0, SEEK_SET);
    buf = (char*)alloc->alloc(alloc->userdata,0, *siz);
    UI_ASSERT(buf);
    if (!buf) {
        fclose(fd);
        return 0;
    }
    *siz = (ui_size)fread(buf, *siz, 1, fd);
    fclose(fd);
    return buf;
}
#endif

/*
 * ==============================================================
 *
 *                          COLOR
 *
 * ===============================================================
 */
UI_INTERN int
ui_parse_hex(const char *p, int length)
{
    int i = 0;
    int len = 0;
    while (len < length) {
        i <<= 4;
        if (p[len] >= 'a' && p[len] <= 'f')
            i += ((p[len] - 'a') + 10);
        else if (p[len] >= 'A' && p[len] <= 'F')
            i += ((p[len] - 'A') + 10);
        else i += (p[len] - '0');
        len++;
    }
    return i;
}

UI_API struct ui_color
ui_rgba(int r, int g, int b, int a)
{
    struct ui_color ret;
    ret.r = (ui_byte)UI_CLAMP(0, r, 255);
    ret.g = (ui_byte)UI_CLAMP(0, g, 255);
    ret.b = (ui_byte)UI_CLAMP(0, b, 255);
    ret.a = (ui_byte)UI_CLAMP(0, a, 255);
    return ret;
}

UI_API struct ui_color
ui_rgb_hex(const char *rgb)
{
    struct ui_color col;
    const char *c = rgb;
    if (*c == '#') c++;
    col.r = (ui_byte)ui_parse_hex(c, 2);
    col.g = (ui_byte)ui_parse_hex(c+2, 2);
    col.b = (ui_byte)ui_parse_hex(c+4, 2);
    col.a = 255;
    return col;
}

UI_API struct ui_color
ui_rgba_hex(const char *rgb)
{
    struct ui_color col;
    const char *c = rgb;
    if (*c == '#') c++;
    col.r = (ui_byte)ui_parse_hex(c, 2);
    col.g = (ui_byte)ui_parse_hex(c+2, 2);
    col.b = (ui_byte)ui_parse_hex(c+4, 2);
    col.a = (ui_byte)ui_parse_hex(c+6, 2);
    return col;
}

UI_API void
ui_color_hex_rgba(char *output, struct ui_color col)
{
    #define UI_TO_HEX(i) ((i) <= 9 ? '0' + (i): 'A' - 10 + (i))
    output[0] = (char)UI_TO_HEX((col.r & 0xF0) >> 4);
    output[1] = (char)UI_TO_HEX((col.r & 0x0F));
    output[2] = (char)UI_TO_HEX((col.g & 0xF0) >> 4);
    output[3] = (char)UI_TO_HEX((col.g & 0x0F));
    output[4] = (char)UI_TO_HEX((col.b & 0xF0) >> 4);
    output[5] = (char)UI_TO_HEX((col.b & 0x0F));
    output[6] = (char)UI_TO_HEX((col.a & 0xF0) >> 4);
    output[7] = (char)UI_TO_HEX((col.a & 0x0F));
    output[8] = '\0';
    #undef UI_TO_HEX
}

UI_API void
ui_color_hex_rgb(char *output, struct ui_color col)
{
    #define UI_TO_HEX(i) ((i) <= 9 ? '0' + (i): 'A' - 10 + (i))
    output[0] = (char)UI_TO_HEX((col.r & 0xF0) >> 4);
    output[1] = (char)UI_TO_HEX((col.r & 0x0F));
    output[2] = (char)UI_TO_HEX((col.g & 0xF0) >> 4);
    output[3] = (char)UI_TO_HEX((col.g & 0x0F));
    output[4] = (char)UI_TO_HEX((col.b & 0xF0) >> 4);
    output[5] = (char)UI_TO_HEX((col.b & 0x0F));
    output[6] = '\0';
    #undef UI_TO_HEX
}

UI_API struct ui_color
ui_rgba_iv(const int *c)
{
    return ui_rgba(c[0], c[1], c[2], c[3]);
}

UI_API struct ui_color
ui_rgba_bv(const ui_byte *c)
{
    return ui_rgba(c[0], c[1], c[2], c[3]);
}

UI_API struct ui_color
ui_rgb(int r, int g, int b)
{
    struct ui_color ret;
    ret.r = (ui_byte)UI_CLAMP(0, r, 255);
    ret.g = (ui_byte)UI_CLAMP(0, g, 255);
    ret.b = (ui_byte)UI_CLAMP(0, b, 255);
    ret.a = (ui_byte)255;
    return ret;
}

UI_API struct ui_color
ui_rgb_iv(const int *c)
{
    return ui_rgb(c[0], c[1], c[2]);
}

UI_API struct ui_color
ui_rgb_bv(const ui_byte* c)
{
    return ui_rgb(c[0], c[1], c[2]);
}

UI_API struct ui_color
ui_rgba_u32(ui_uint in)
{
    struct ui_color ret;
    ret.r = (in & 0xFF);
    ret.g = ((in >> 8) & 0xFF);
    ret.b = ((in >> 16) & 0xFF);
    ret.a = (ui_byte)((in >> 24) & 0xFF);
    return ret;
}

UI_API struct ui_color
ui_rgba_f(float r, float g, float b, float a)
{
    struct ui_color ret;
    ret.r = (ui_byte)(UI_SATURATE(r) * 255.0f);
    ret.g = (ui_byte)(UI_SATURATE(g) * 255.0f);
    ret.b = (ui_byte)(UI_SATURATE(b) * 255.0f);
    ret.a = (ui_byte)(UI_SATURATE(a) * 255.0f);
    return ret;
}

UI_API struct ui_color
ui_rgba_fv(const float *c)
{
    return ui_rgba_f(c[0], c[1], c[2], c[3]);
}

UI_API struct ui_color
ui_rgb_f(float r, float g, float b)
{
    struct ui_color ret;
    ret.r = (ui_byte)(UI_SATURATE(r) * 255.0f);
    ret.g = (ui_byte)(UI_SATURATE(g) * 255.0f);
    ret.b = (ui_byte)(UI_SATURATE(b) * 255.0f);
    ret.a = 255;
    return ret;
}

UI_API struct ui_color
ui_rgb_fv(const float *c)
{
    return ui_rgb_f(c[0], c[1], c[2]);
}

UI_API struct ui_color
ui_hsv(int h, int s, int v)
{
    return ui_hsva(h, s, v, 255);
}

UI_API struct ui_color
ui_hsv_iv(const int *c)
{
    return ui_hsv(c[0], c[1], c[2]);
}

UI_API struct ui_color
ui_hsv_bv(const ui_byte *c)
{
    return ui_hsv(c[0], c[1], c[2]);
}

UI_API struct ui_color
ui_hsv_f(float h, float s, float v)
{
    return ui_hsva_f(h, s, v, 1.0f);
}

UI_API struct ui_color
ui_hsv_fv(const float *c)
{
    return ui_hsv_f(c[0], c[1], c[2]);
}

UI_API struct ui_color
ui_hsva(int h, int s, int v, int a)
{
    float hf = ((float)UI_CLAMP(0, h, 255)) / 255.0f;
    float sf = ((float)UI_CLAMP(0, s, 255)) / 255.0f;
    float vf = ((float)UI_CLAMP(0, v, 255)) / 255.0f;
    float af = ((float)UI_CLAMP(0, a, 255)) / 255.0f;
    return ui_hsva_f(hf, sf, vf, af);
}

UI_API struct ui_color
ui_hsva_iv(const int *c)
{
    return ui_hsva(c[0], c[1], c[2], c[3]);
}

UI_API struct ui_color
ui_hsva_bv(const ui_byte *c)
{
    return ui_hsva(c[0], c[1], c[2], c[3]);
}

UI_API struct ui_color
ui_hsva_f(float h, float s, float v, float a)
{
    struct ui_colorf out = {0,0,0,0};
    float p, q, t, f;
    int i;

    if (s <= 0.0f) {
        out.r = v; out.g = v; out.b = v;
        return ui_rgb_f(out.r, out.g, out.b);
    }

    h = h / (60.0f/360.0f);
    i = (int)h;
    f = h - (float)i;
    p = v * (1.0f - s);
    q = v * (1.0f - (s * f));
    t = v * (1.0f - s * (1.0f - f));

    switch (i) {
    case 0: default: out.r = v; out.g = t; out.b = p; break;
    case 1: out.r = q; out.g = v; out.b = p; break;
    case 2: out.r = p; out.g = v; out.b = t; break;
    case 3: out.r = p; out.g = q; out.b = v; break;
    case 4: out.r = t; out.g = p; out.b = v; break;
    case 5: out.r = v; out.g = p; out.b = q; break;
    }
    return ui_rgba_f(out.r, out.g, out.b, a);
}

UI_API struct ui_color
ui_hsva_fv(const float *c)
{
    return ui_hsva_f(c[0], c[1], c[2], c[3]);
}

UI_API ui_uint
ui_color_u32(struct ui_color in)
{
    ui_uint out = (ui_uint)in.r;
    out |= ((ui_uint)in.g << 8);
    out |= ((ui_uint)in.b << 16);
    out |= ((ui_uint)in.a << 24);
    return out;
}

UI_API void
ui_color_f(float *r, float *g, float *b, float *a, struct ui_color in)
{
    UI_STORAGE const float s = 1.0f/255.0f;
    *r = (float)in.r * s;
    *g = (float)in.g * s;
    *b = (float)in.b * s;
    *a = (float)in.a * s;
}

UI_API void
ui_color_fv(float *c, struct ui_color in)
{
    ui_color_f(&c[0], &c[1], &c[2], &c[3], in);
}

UI_API void
ui_color_d(double *r, double *g, double *b, double *a, struct ui_color in)
{
    UI_STORAGE const double s = 1.0/255.0;
    *r = (double)in.r * s;
    *g = (double)in.g * s;
    *b = (double)in.b * s;
    *a = (double)in.a * s;
}

UI_API void
ui_color_dv(double *c, struct ui_color in)
{
    ui_color_d(&c[0], &c[1], &c[2], &c[3], in);
}

UI_API void
ui_color_hsv_f(float *out_h, float *out_s, float *out_v, struct ui_color in)
{
    float a;
    ui_color_hsva_f(out_h, out_s, out_v, &a, in);
}

UI_API void
ui_color_hsv_fv(float *out, struct ui_color in)
{
    float a;
    ui_color_hsva_f(&out[0], &out[1], &out[2], &a, in);
}

UI_API void
ui_color_hsva_f(float *out_h, float *out_s,
    float *out_v, float *out_a, struct ui_color in)
{
    float chroma;
    float K = 0.0f;
    float r,g,b,a;

    ui_color_f(&r,&g,&b,&a, in);
    if (g < b) {
        const float t = g; g = b; b = t;
        K = -1.f;
    }
    if (r < g) {
        const float t = r; r = g; g = t;
        K = -2.f/6.0f - K;
    }
    chroma = r - ((g < b) ? g: b);
    *out_h = UI_ABS(K + (g - b)/(6.0f * chroma + 1e-20f));
    *out_s = chroma / (r + 1e-20f);
    *out_v = r;
    *out_a = (float)in.a / 255.0f;
}

UI_API void
ui_color_hsva_fv(float *out, struct ui_color in)
{
    ui_color_hsva_f(&out[0], &out[1], &out[2], &out[3], in);
}

UI_API void
ui_color_hsva_i(int *out_h, int *out_s, int *out_v,
                int *out_a, struct ui_color in)
{
    float h,s,v,a;
    ui_color_hsva_f(&h, &s, &v, &a, in);
    *out_h = (ui_byte)(h * 255.0f);
    *out_s = (ui_byte)(s * 255.0f);
    *out_v = (ui_byte)(v * 255.0f);
    *out_a = (ui_byte)(a * 255.0f);
}

UI_API void
ui_color_hsva_iv(int *out, struct ui_color in)
{
    ui_color_hsva_i(&out[0], &out[1], &out[2], &out[3], in);
}

UI_API void
ui_color_hsva_bv(ui_byte *out, struct ui_color in)
{
    int tmp[4];
    ui_color_hsva_i(&tmp[0], &tmp[1], &tmp[2], &tmp[3], in);
    out[0] = (ui_byte)tmp[0];
    out[1] = (ui_byte)tmp[1];
    out[2] = (ui_byte)tmp[2];
    out[3] = (ui_byte)tmp[3];
}

UI_API void
ui_color_hsva_b(ui_byte *h, ui_byte *s, ui_byte *v, ui_byte *a, struct ui_color in)
{
    int tmp[4];
    ui_color_hsva_i(&tmp[0], &tmp[1], &tmp[2], &tmp[3], in);
    *h = (ui_byte)tmp[0];
    *s = (ui_byte)tmp[1];
    *v = (ui_byte)tmp[2];
    *a = (ui_byte)tmp[3];
}

UI_API void
ui_color_hsv_i(int *out_h, int *out_s, int *out_v, struct ui_color in)
{
    int a;
    ui_color_hsva_i(out_h, out_s, out_v, &a, in);
}

UI_API void
ui_color_hsv_b(ui_byte *out_h, ui_byte *out_s, ui_byte *out_v, struct ui_color in)
{
    int tmp[4];
    ui_color_hsva_i(&tmp[0], &tmp[1], &tmp[2], &tmp[3], in);
    *out_h = (ui_byte)tmp[0];
    *out_s = (ui_byte)tmp[1];
    *out_v = (ui_byte)tmp[2];
}

UI_API void
ui_color_hsv_iv(int *out, struct ui_color in)
{
    ui_color_hsv_i(&out[0], &out[1], &out[2], in);
}

UI_API void
ui_color_hsv_bv(ui_byte *out, struct ui_color in)
{
    int tmp[4];
    ui_color_hsv_i(&tmp[0], &tmp[1], &tmp[2], in);
    out[0] = (ui_byte)tmp[0];
    out[1] = (ui_byte)tmp[1];
    out[2] = (ui_byte)tmp[2];
}
/*
 * ==============================================================
 *
 *                          IMAGE
 *
 * ===============================================================
 */
UI_API ui_handle
ui_handle_ptr(void *ptr)
{
    ui_handle handle = {0};
    handle.ptr = ptr;
    return handle;
}

UI_API ui_handle
ui_handle_id(int id)
{
    ui_handle handle;
    ui_zero_struct(handle);
    handle.id = id;
    return handle;
}

UI_API struct ui_image
ui_subimage_ptr(void *ptr, unsigned short w, unsigned short h, struct ui_rect r)
{
    struct ui_image s;
    ui_zero(&s, sizeof(s));
    s.handle.ptr = ptr;
    s.w = w; s.h = h;
    s.region[0] = (unsigned short)r.x;
    s.region[1] = (unsigned short)r.y;
    s.region[2] = (unsigned short)r.w;
    s.region[3] = (unsigned short)r.h;
    return s;
}

UI_API struct ui_image
ui_subimage_id(int id, unsigned short w, unsigned short h, struct ui_rect r)
{
    struct ui_image s;
    ui_zero(&s, sizeof(s));
    s.handle.id = id;
    s.w = w; s.h = h;
    s.region[0] = (unsigned short)r.x;
    s.region[1] = (unsigned short)r.y;
    s.region[2] = (unsigned short)r.w;
    s.region[3] = (unsigned short)r.h;
    return s;
}

UI_API struct ui_image
ui_subimage_handle(ui_handle handle, unsigned short w, unsigned short h,
    struct ui_rect r)
{
    struct ui_image s;
    ui_zero(&s, sizeof(s));
    s.handle = handle;
    s.w = w; s.h = h;
    s.region[0] = (unsigned short)r.x;
    s.region[1] = (unsigned short)r.y;
    s.region[2] = (unsigned short)r.w;
    s.region[3] = (unsigned short)r.h;
    return s;
}

UI_API struct ui_image
ui_image_handle(ui_handle handle)
{
    struct ui_image s;
    ui_zero(&s, sizeof(s));
    s.handle = handle;
    s.w = 0; s.h = 0;
    s.region[0] = 0;
    s.region[1] = 0;
    s.region[2] = 0;
    s.region[3] = 0;
    return s;
}

UI_API struct ui_image
ui_image_ptr(void *ptr)
{
    struct ui_image s;
    ui_zero(&s, sizeof(s));
    UI_ASSERT(ptr);
    s.handle.ptr = ptr;
    s.w = 0; s.h = 0;
    s.region[0] = 0;
    s.region[1] = 0;
    s.region[2] = 0;
    s.region[3] = 0;
    return s;
}

UI_API struct ui_image
ui_image_id(int id)
{
    struct ui_image s;
    ui_zero(&s, sizeof(s));
    s.handle.id = id;
    s.w = 0; s.h = 0;
    s.region[0] = 0;
    s.region[1] = 0;
    s.region[2] = 0;
    s.region[3] = 0;
    return s;
}

UI_API int
ui_image_is_subimage(const struct ui_image* img)
{
    UI_ASSERT(img);
    return !(img->w == 0 && img->h == 0);
}

UI_INTERN void
ui_unify(struct ui_rect *clip, const struct ui_rect *a, float x0, float y0,
    float x1, float y1)
{
    UI_ASSERT(a);
    UI_ASSERT(clip);
    clip->x = UI_MAX(a->x, x0);
    clip->y = UI_MAX(a->y, y0);
    clip->w = UI_MIN(a->x + a->w, x1) - clip->x;
    clip->h = UI_MIN(a->y + a->h, y1) - clip->y;
    clip->w = UI_MAX(0, clip->w);
    clip->h = UI_MAX(0, clip->h);
}

UI_API void
ui_triangle_from_direction(struct ui_vec2 *result, struct ui_rect r,
    float pad_x, float pad_y, enum ui_heading direction)
{
    float w_half, h_half;
    UI_ASSERT(result);

    r.w = UI_MAX(2 * pad_x, r.w);
    r.h = UI_MAX(2 * pad_y, r.h);
    r.w = r.w - 2 * pad_x;
    r.h = r.h - 2 * pad_y;

    r.x = r.x + pad_x;
    r.y = r.y + pad_y;

    w_half = r.w / 2.0f;
    h_half = r.h / 2.0f;

    if (direction == UI_UP) {
        result[0] = ui_vec2(r.x + w_half, r.y);
        result[1] = ui_vec2(r.x + r.w, r.y + r.h);
        result[2] = ui_vec2(r.x, r.y + r.h);
    } else if (direction == UI_RIGHT) {
        result[0] = ui_vec2(r.x, r.y);
        result[1] = ui_vec2(r.x + r.w, r.y + h_half);
        result[2] = ui_vec2(r.x, r.y + r.h);
    } else if (direction == UI_DOWN) {
        result[0] = ui_vec2(r.x, r.y);
        result[1] = ui_vec2(r.x + r.w, r.y);
        result[2] = ui_vec2(r.x + w_half, r.y + r.h);
    } else {
        result[0] = ui_vec2(r.x, r.y + h_half);
        result[1] = ui_vec2(r.x + r.w, r.y);
        result[2] = ui_vec2(r.x + r.w, r.y + r.h);
    }
}

UI_INTERN int
ui_text_clamp(const struct ui_user_font *font, const char *text,
    int text_len, float space, int *glyphs, float *text_width,
    ui_rune *sep_list, int sep_count)
{
    int i = 0;
    int glyph_len = 0;
    float last_width = 0;
    ui_rune unicode = 0;
    float width = 0;
    int len = 0;
    int g = 0;
    float s;

    int sep_len = 0;
    int sep_g = 0;
    float sep_width = 0;
    sep_count = UI_MAX(sep_count,0);

    glyph_len = ui_utf_decode(text, &unicode, text_len);
    while (glyph_len && (width < space) && (len < text_len)) {
        len += glyph_len;
        s = font->width(font->userdata, font->height, text, len);
        for (i = 0; i < sep_count; ++i) {
            if (unicode != sep_list[i]) continue;
            sep_width = last_width = width;
            sep_g = g+1;
            sep_len = len;
            break;
        }
        if (i == UI_MAX(sep_count,0)){
            last_width = sep_width = width;
            sep_g = g+1;
        }
        width = s;
        glyph_len = ui_utf_decode(&text[len], &unicode, text_len - len);
        g++;
    }
    if (len >= text_len) {
        *glyphs = g;
        *text_width = last_width;
        return len;
    } else {
        *glyphs = sep_g;
        *text_width = sep_width;
        return (!sep_len) ? len: sep_len;
    }
}

enum {UI_DO_NOT_STOP_ON_NEW_LINE, UI_STOP_ON_NEW_LINE};
UI_INTERN struct ui_vec2
ui_text_calculate_text_bounds(const struct ui_user_font *font,
    const char *begin, int byte_len, float row_height, const char **remaining,
    struct ui_vec2 *out_offset, int *glyphs, int op)
{
    float line_height = row_height;
    struct ui_vec2 text_size = ui_vec2(0,0);
    float line_width = 0.0f;

    float glyph_width;
    int glyph_len = 0;
    ui_rune unicode = 0;
    int text_len = 0;
    if (!begin || byte_len <= 0 || !font)
        return ui_vec2(0,row_height);

    glyph_len = ui_utf_decode(begin, &unicode, byte_len);
    if (!glyph_len) return text_size;
    glyph_width = font->width(font->userdata, font->height, begin, glyph_len);

    *glyphs = 0;
    while ((text_len < byte_len) && glyph_len) {
        if (unicode == '\n') {
            text_size.x = UI_MAX(text_size.x, line_width);
            text_size.y += line_height;
            line_width = 0;
            *glyphs+=1;
            if (op == UI_STOP_ON_NEW_LINE)
                break;

            text_len++;
            glyph_len = ui_utf_decode(begin + text_len, &unicode, byte_len-text_len);
            continue;
        }

        if (unicode == '\r') {
            text_len++;
            *glyphs+=1;
            glyph_len = ui_utf_decode(begin + text_len, &unicode, byte_len-text_len);
            continue;
        }

        *glyphs = *glyphs + 1;
        text_len += glyph_len;
        line_width += (float)glyph_width;
        glyph_len = ui_utf_decode(begin + text_len, &unicode, byte_len-text_len);
        glyph_width = font->width(font->userdata, font->height, begin+text_len, glyph_len);
        continue;
    }

    if (text_size.x < line_width)
        text_size.x = line_width;
    if (out_offset)
        *out_offset = ui_vec2(line_width, text_size.y + line_height);
    if (line_width > 0 || text_size.y == 0.0f)
        text_size.y += line_height;
    if (remaining)
        *remaining = begin+text_len;
    return text_size;
}

/* ==============================================================
 *
 *                          UTF-8
 *
 * ===============================================================*/
UI_GLOBAL const ui_byte ui_utfbyte[UI_UTF_SIZE+1] = {0x80, 0, 0xC0, 0xE0, 0xF0};
UI_GLOBAL const ui_byte ui_utfmask[UI_UTF_SIZE+1] = {0xC0, 0x80, 0xE0, 0xF0, 0xF8};
UI_GLOBAL const ui_uint ui_utfmin[UI_UTF_SIZE+1] = {0, 0, 0x80, 0x800, 0x10000};
UI_GLOBAL const ui_uint ui_utfmax[UI_UTF_SIZE+1] = {0x10FFFF, 0x7F, 0x7FF, 0xFFFF, 0x10FFFF};

UI_INTERN int
ui_utf_validate(ui_rune *u, int i)
{
    UI_ASSERT(u);
    if (!u) return 0;
    if (!UI_BETWEEN(*u, ui_utfmin[i], ui_utfmax[i]) ||
         UI_BETWEEN(*u, 0xD800, 0xDFFF))
            *u = UI_UTF_INVALID;
    for (i = 1; *u > ui_utfmax[i]; ++i);
    return i;
}

UI_INTERN ui_rune
ui_utf_decode_byte(char c, int *i)
{
    UI_ASSERT(i);
    if (!i) return 0;
    for(*i = 0; *i < (int)UI_LEN(ui_utfmask); ++(*i)) {
        if (((ui_byte)c & ui_utfmask[*i]) == ui_utfbyte[*i])
            return (ui_byte)(c & ~ui_utfmask[*i]);
    }
    return 0;
}

UI_API int
ui_utf_decode(const char *c, ui_rune *u, int clen)
{
    int i, j, len, type=0;
    ui_rune udecoded;

    UI_ASSERT(c);
    UI_ASSERT(u);

    if (!c || !u) return 0;
    if (!clen) return 0;
    *u = UI_UTF_INVALID;

    udecoded = ui_utf_decode_byte(c[0], &len);
    if (!UI_BETWEEN(len, 1, UI_UTF_SIZE))
        return 1;

    for (i = 1, j = 1; i < clen && j < len; ++i, ++j) {
        udecoded = (udecoded << 6) | ui_utf_decode_byte(c[i], &type);
        if (type != 0)
            return j;
    }
    if (j < len)
        return 0;
    *u = udecoded;
    ui_utf_validate(u, len);
    return len;
}

UI_INTERN char
ui_utf_encode_byte(ui_rune u, int i)
{
    return (char)((ui_utfbyte[i]) | ((ui_byte)u & ~ui_utfmask[i]));
}

UI_API int
ui_utf_encode(ui_rune u, char *c, int clen)
{
    int len, i;
    len = ui_utf_validate(&u, 0);
    if (clen < len || !len || len > UI_UTF_SIZE)
        return 0;

    for (i = len - 1; i != 0; --i) {
        c[i] = ui_utf_encode_byte(u, 0);
        u >>= 6;
    }
    c[0] = ui_utf_encode_byte(u, len);
    return len;
}

UI_API int
ui_utf_len(const char *str, int len)
{
    const char *text;
    int glyphs = 0;
    int text_len;
    int glyph_len;
    int src_len = 0;
    ui_rune unicode;

    UI_ASSERT(str);
    if (!str || !len) return 0;

    text = str;
    text_len = len;
    glyph_len = ui_utf_decode(text, &unicode, text_len);
    while (glyph_len && src_len < len) {
        glyphs++;
        src_len = src_len + glyph_len;
        glyph_len = ui_utf_decode(text + src_len, &unicode, text_len - src_len);
    }
    return glyphs;
}

UI_API const char*
ui_utf_at(const char *buffer, int length, int index,
    ui_rune *unicode, int *len)
{
    int i = 0;
    int src_len = 0;
    int glyph_len = 0;
    const char *text;
    int text_len;

    UI_ASSERT(buffer);
    UI_ASSERT(unicode);
    UI_ASSERT(len);

    if (!buffer || !unicode || !len) return 0;
    if (index < 0) {
        *unicode = UI_UTF_INVALID;
        *len = 0;
        return 0;
    }

    text = buffer;
    text_len = length;
    glyph_len = ui_utf_decode(text, unicode, text_len);
    while (glyph_len) {
        if (i == index) {
            *len = glyph_len;
            break;
        }

        i++;
        src_len = src_len + glyph_len;
        glyph_len = ui_utf_decode(text + src_len, unicode, text_len - src_len);
    }
    if (i != index) return 0;
    return buffer + src_len;
}

/* ==============================================================
 *
 *                          BUFFER
 *
 * ===============================================================*/
#ifdef UI_INCLUDE_DEFAULT_ALLOCATOR
UI_INTERN void* ui_malloc(ui_handle unused, void *old,ui_size size)
{UI_UNUSED(unused); UI_UNUSED(old); return malloc(size);}
UI_INTERN void ui_mfree(ui_handle unused, void *ptr)
{UI_UNUSED(unused); free(ptr);}

UI_API void
ui_buffer_init_default(struct ui_buffer *buffer)
{
    struct ui_allocator alloc;
    alloc.userdata.ptr = 0;
    alloc.alloc = ui_malloc;
    alloc.free = ui_mfree;
    ui_buffer_init(buffer, &alloc, UI_BUFFER_DEFAULT_INITIAL_SIZE);
}
#endif

UI_API void
ui_buffer_init(struct ui_buffer *b, const struct ui_allocator *a,
    ui_size initial_size)
{
    UI_ASSERT(b);
    UI_ASSERT(a);
    UI_ASSERT(initial_size);
    if (!b || !a || !initial_size) return;

    ui_zero(b, sizeof(*b));
    b->type = UI_BUFFER_DYNAMIC;
    b->memory.ptr = a->alloc(a->userdata,0, initial_size);
    b->memory.size = initial_size;
    b->size = initial_size;
    b->grow_factor = 2.0f;
    b->pool = *a;
}

UI_API void
ui_buffer_init_fixed(struct ui_buffer *b, void *m, ui_size size)
{
    UI_ASSERT(b);
    UI_ASSERT(m);
    UI_ASSERT(size);
    if (!b || !m || !size) return;

    ui_zero(b, sizeof(*b));
    b->type = UI_BUFFER_FIXED;
    b->memory.ptr = m;
    b->memory.size = size;
    b->size = size;
}

UI_INTERN void*
ui_buffer_align(void *unaligned, ui_size align, ui_size *alignment,
    enum ui_buffer_allocation_type type)
{
    void *memory = 0;
    switch (type) {
    default:
    case UI_BUFFER_MAX:
    case UI_BUFFER_FRONT:
        if (align) {
            memory = UI_ALIGN_PTR(unaligned, align);
            *alignment = (ui_size)((ui_byte*)memory - (ui_byte*)unaligned);
        } else {
            memory = unaligned;
            *alignment = 0;
        }
        break;
    case UI_BUFFER_BACK:
        if (align) {
            memory = UI_ALIGN_PTR_BACK(unaligned, align);
            *alignment = (ui_size)((ui_byte*)unaligned - (ui_byte*)memory);
        } else {
            memory = unaligned;
            *alignment = 0;
        }
        break;
    }
    return memory;
}

UI_INTERN void*
ui_buffer_realloc(struct ui_buffer *b, ui_size capacity, ui_size *size)
{
    void *temp;
    ui_size buffer_size;

    UI_ASSERT(b);
    UI_ASSERT(size);
    if (!b || !size || !b->pool.alloc || !b->pool.free)
        return 0;

    buffer_size = b->memory.size;
    temp = b->pool.alloc(b->pool.userdata, b->memory.ptr, capacity);
    UI_ASSERT(temp);
    if (!temp) return 0;

    *size = capacity;
    if (temp != b->memory.ptr) {
        UI_MEMCPY(temp, b->memory.ptr, buffer_size);
        b->pool.free(b->pool.userdata, b->memory.ptr);
    }

    if (b->size == buffer_size) {
        /* no back buffer so just set correct size */
        b->size = capacity;
        return temp;
    } else {
        /* copy back buffer to the end of the new buffer */
        void *dst, *src;
        ui_size back_size;
        back_size = buffer_size - b->size;
        dst = ui_ptr_add(void, temp, capacity - back_size);
        src = ui_ptr_add(void, temp, b->size);
        UI_MEMCPY(dst, src, back_size);
        b->size = capacity - back_size;
    }
    return temp;
}

UI_INTERN void*
ui_buffer_alloc(struct ui_buffer *b, enum ui_buffer_allocation_type type,
    ui_size size, ui_size align)
{
    int full;
    ui_size alignment;
    void *unaligned;
    void *memory;

    UI_ASSERT(b);
    UI_ASSERT(size);
    if (!b || !size) return 0;
    b->needed += size;

    /* calculate total size with needed alignment + size */
    if (type == UI_BUFFER_FRONT)
        unaligned = ui_ptr_add(void, b->memory.ptr, b->allocated);
    else unaligned = ui_ptr_add(void, b->memory.ptr, b->size - size);
    memory = ui_buffer_align(unaligned, align, &alignment, type);

    /* check if buffer has enough memory*/
    if (type == UI_BUFFER_FRONT)
        full = ((b->allocated + size + alignment) > b->size);
    else full = ((b->size - (size + alignment)) <= b->allocated);

    if (full) {
        ui_size capacity;
        UI_ASSERT(b->type == UI_BUFFER_DYNAMIC);
        UI_ASSERT(b->pool.alloc && b->pool.free);
        if (b->type != UI_BUFFER_DYNAMIC || !b->pool.alloc || !b->pool.free)
            return 0;

        /* buffer is full so allocate bigger buffer if dynamic */
        capacity = (ui_size)((float)b->memory.size * b->grow_factor);
        capacity = UI_MAX(capacity, ui_round_up_pow2((ui_uint)(b->allocated + size)));
        b->memory.ptr = ui_buffer_realloc(b, capacity, &b->memory.size);
        if (!b->memory.ptr) return 0;

        /* align newly allocated pointer */
        if (type == UI_BUFFER_FRONT)
            unaligned = ui_ptr_add(void, b->memory.ptr, b->allocated);
        else unaligned = ui_ptr_add(void, b->memory.ptr, b->size - size);
        memory = ui_buffer_align(unaligned, align, &alignment, type);
    }

    if (type == UI_BUFFER_FRONT)
        b->allocated += size + alignment;
    else b->size -= (size + alignment);
    b->needed += alignment;
    b->calls++;
    return memory;
}

UI_API void
ui_buffer_push(struct ui_buffer *b, enum ui_buffer_allocation_type type,
    const void *memory, ui_size size, ui_size align)
{
    void *mem = ui_buffer_alloc(b, type, size, align);
    if (!mem) return;
    UI_MEMCPY(mem, memory, size);
}

UI_API void
ui_buffer_mark(struct ui_buffer *buffer, enum ui_buffer_allocation_type type)
{
    UI_ASSERT(buffer);
    if (!buffer) return;
    buffer->marker[type].active = ui_true;
    if (type == UI_BUFFER_BACK)
        buffer->marker[type].offset = buffer->size;
    else buffer->marker[type].offset = buffer->allocated;
}

UI_API void
ui_buffer_reset(struct ui_buffer *buffer, enum ui_buffer_allocation_type type)
{
    UI_ASSERT(buffer);
    if (!buffer) return;
    if (type == UI_BUFFER_BACK) {
        /* reset back buffer either back to marker or empty */
        buffer->needed -= (buffer->memory.size - buffer->marker[type].offset);
        if (buffer->marker[type].active)
            buffer->size = buffer->marker[type].offset;
        else buffer->size = buffer->memory.size;
        buffer->marker[type].active = ui_false;
    } else {
        /* reset front buffer either back to back marker or empty */
        buffer->needed -= (buffer->allocated - buffer->marker[type].offset);
        if (buffer->marker[type].active)
            buffer->allocated = buffer->marker[type].offset;
        else buffer->allocated = 0;
        buffer->marker[type].active = ui_false;
    }
}

UI_API void
ui_buffer_clear(struct ui_buffer *b)
{
    UI_ASSERT(b);
    if (!b) return;
    b->allocated = 0;
    b->size = b->memory.size;
    b->calls = 0;
    b->needed = 0;
}

UI_API void
ui_buffer_free(struct ui_buffer *b)
{
    UI_ASSERT(b);
    if (!b || !b->memory.ptr) return;
    if (b->type == UI_BUFFER_FIXED) return;
    if (!b->pool.free) return;
    UI_ASSERT(b->pool.free);
    b->pool.free(b->pool.userdata, b->memory.ptr);
}

UI_API void
ui_buffer_info(struct ui_memory_status *s, struct ui_buffer *b)
{
    UI_ASSERT(b);
    UI_ASSERT(s);
    if (!s || !b) return;
    s->allocated = b->allocated;
    s->size =  b->memory.size;
    s->needed = b->needed;
    s->memory = b->memory.ptr;
    s->calls = b->calls;
}

UI_API void*
ui_buffer_memory(struct ui_buffer *buffer)
{
    UI_ASSERT(buffer);
    if (!buffer) return 0;
    return buffer->memory.ptr;
}

UI_API const void*
ui_buffer_memory_const(const struct ui_buffer *buffer)
{
    UI_ASSERT(buffer);
    if (!buffer) return 0;
    return buffer->memory.ptr;
}

UI_API ui_size
ui_buffer_total(struct ui_buffer *buffer)
{
    UI_ASSERT(buffer);
    if (!buffer) return 0;
    return buffer->memory.size;
}

/*
 * ==============================================================
 *
 *                          STRING
 *
 * ===============================================================
 */
#ifdef UI_INCLUDE_DEFAULT_ALLOCATOR
UI_API void
ui_str_init_default(struct ui_str *str)
{
    struct ui_allocator alloc;
    alloc.userdata.ptr = 0;
    alloc.alloc = ui_malloc;
    alloc.free = ui_mfree;
    ui_buffer_init(&str->buffer, &alloc, 32);
    str->len = 0;
}
#endif

UI_API void
ui_str_init(struct ui_str *str, const struct ui_allocator *alloc, ui_size size)
{
    ui_buffer_init(&str->buffer, alloc, size);
    str->len = 0;
}

UI_API void
ui_str_init_fixed(struct ui_str *str, void *memory, ui_size size)
{
    ui_buffer_init_fixed(&str->buffer, memory, size);
    str->len = 0;
}

UI_API int
ui_str_append_text_char(struct ui_str *s, const char *str, int len)
{
    char *mem;
    UI_ASSERT(s);
    UI_ASSERT(str);
    if (!s || !str || !len) return 0;
    mem = (char*)ui_buffer_alloc(&s->buffer, UI_BUFFER_FRONT, (ui_size)len * sizeof(char), 0);
    if (!mem) return 0;
    UI_MEMCPY(mem, str, (ui_size)len * sizeof(char));
    s->len += ui_utf_len(str, len);
    return len;
}

UI_API int
ui_str_append_str_char(struct ui_str *s, const char *str)
{
    return ui_str_append_text_char(s, str, ui_strlen(str));
}

UI_API int
ui_str_append_text_utf8(struct ui_str *str, const char *text, int len)
{
    int i = 0;
    int byte_len = 0;
    ui_rune unicode;
    if (!str || !text || !len) return 0;
    for (i = 0; i < len; ++i)
        byte_len += ui_utf_decode(text+byte_len, &unicode, 4);
    ui_str_append_text_char(str, text, byte_len);
    return len;
}

UI_API int
ui_str_append_str_utf8(struct ui_str *str, const char *text)
{
    int runes = 0;
    int byte_len = 0;
    int num_runes = 0;
    int glyph_len = 0;
    ui_rune unicode;
    if (!str || !text) return 0;

    glyph_len = byte_len = ui_utf_decode(text+byte_len, &unicode, 4);
    while (unicode != '\0' && glyph_len) {
        glyph_len = ui_utf_decode(text+byte_len, &unicode, 4);
        byte_len += glyph_len;
        num_runes++;
    }
    ui_str_append_text_char(str, text, byte_len);
    return runes;
}

UI_API int
ui_str_append_text_runes(struct ui_str *str, const ui_rune *text, int len)
{
    int i = 0;
    int byte_len = 0;
    ui_glyph glyph;

    UI_ASSERT(str);
    if (!str || !text || !len) return 0;
    for (i = 0; i < len; ++i) {
        byte_len = ui_utf_encode(text[i], glyph, UI_UTF_SIZE);
        if (!byte_len) break;
        ui_str_append_text_char(str, glyph, byte_len);
    }
    return len;
}

UI_API int
ui_str_append_str_runes(struct ui_str *str, const ui_rune *runes)
{
    int i = 0;
    ui_glyph glyph;
    int byte_len;
    UI_ASSERT(str);
    if (!str || !runes) return 0;
    while (runes[i] != '\0') {
        byte_len = ui_utf_encode(runes[i], glyph, UI_UTF_SIZE);
        ui_str_append_text_char(str, glyph, byte_len);
        i++;
    }
    return i;
}

UI_API int
ui_str_insert_at_char(struct ui_str *s, int pos, const char *str, int len)
{
    int i;
    void *mem;
    char *src;
    char *dst;

    int copylen;
    UI_ASSERT(s);
    UI_ASSERT(str);
    UI_ASSERT(len >= 0);
    if (!s || !str || !len || (ui_size)pos > s->buffer.allocated) return 0;
    if ((s->buffer.allocated + (ui_size)len >= s->buffer.memory.size) &&
        (s->buffer.type == UI_BUFFER_FIXED)) return 0;

    copylen = (int)s->buffer.allocated - pos;
    if (!copylen) {
        ui_str_append_text_char(s, str, len);
        return 1;
    }
    mem = ui_buffer_alloc(&s->buffer, UI_BUFFER_FRONT, (ui_size)len * sizeof(char), 0);
    if (!mem) return 0;

    /* memmove */
    UI_ASSERT(((int)pos + (int)len + ((int)copylen - 1)) >= 0);
    UI_ASSERT(((int)pos + ((int)copylen - 1)) >= 0);
    dst = ui_ptr_add(char, s->buffer.memory.ptr, pos + len + (copylen - 1));
    src = ui_ptr_add(char, s->buffer.memory.ptr, pos + (copylen-1));
    for (i = 0; i < copylen; ++i) *dst-- = *src--;
    mem = ui_ptr_add(void, s->buffer.memory.ptr, pos);
    UI_MEMCPY(mem, str, (ui_size)len * sizeof(char));
    s->len = ui_utf_len((char *)s->buffer.memory.ptr, (int)s->buffer.allocated);
    return 1;
}

UI_API int
ui_str_insert_at_rune(struct ui_str *str, int pos, const char *cstr, int len)
{
    int glyph_len;
    ui_rune unicode;
    const char *begin;
    const char *buffer;

    UI_ASSERT(str);
    UI_ASSERT(cstr);
    UI_ASSERT(len);
    if (!str || !cstr || !len) return 0;
    begin = ui_str_at_rune(str, pos, &unicode, &glyph_len);
    if (!str->len)
        return ui_str_append_text_char(str, cstr, len);
    buffer = ui_str_get_const(str);
    if (!begin) return 0;
    return ui_str_insert_at_char(str, (int)(begin - buffer), cstr, len);
}

UI_API int
ui_str_insert_text_char(struct ui_str *str, int pos, const char *text, int len)
{
    return ui_str_insert_text_utf8(str, pos, text, len);
}

UI_API int
ui_str_insert_str_char(struct ui_str *str, int pos, const char *text)
{
    return ui_str_insert_text_utf8(str, pos, text, ui_strlen(text));
}

UI_API int
ui_str_insert_text_utf8(struct ui_str *str, int pos, const char *text, int len)
{
    int i = 0;
    int byte_len = 0;
    ui_rune unicode;

    UI_ASSERT(str);
    UI_ASSERT(text);
    if (!str || !text || !len) return 0;
    for (i = 0; i < len; ++i)
        byte_len += ui_utf_decode(text+byte_len, &unicode, 4);
    ui_str_insert_at_rune(str, pos, text, byte_len);
    return len;
}

UI_API int
ui_str_insert_str_utf8(struct ui_str *str, int pos, const char *text)
{
    int runes = 0;
    int byte_len = 0;
    int num_runes = 0;
    int glyph_len = 0;
    ui_rune unicode;
    if (!str || !text) return 0;

    glyph_len = byte_len = ui_utf_decode(text+byte_len, &unicode, 4);
    while (unicode != '\0' && glyph_len) {
        glyph_len = ui_utf_decode(text+byte_len, &unicode, 4);
        byte_len += glyph_len;
        num_runes++;
    }
    ui_str_insert_at_rune(str, pos, text, byte_len);
    return runes;
}

UI_API int
ui_str_insert_text_runes(struct ui_str *str, int pos, const ui_rune *runes, int len)
{
    int i = 0;
    int byte_len = 0;
    ui_glyph glyph;

    UI_ASSERT(str);
    if (!str || !runes || !len) return 0;
    for (i = 0; i < len; ++i) {
        byte_len = ui_utf_encode(runes[i], glyph, UI_UTF_SIZE);
        if (!byte_len) break;
        ui_str_insert_at_rune(str, pos+i, glyph, byte_len);
    }
    return len;
}

UI_API int
ui_str_insert_str_runes(struct ui_str *str, int pos, const ui_rune *runes)
{
    int i = 0;
    ui_glyph glyph;
    int byte_len;
    UI_ASSERT(str);
    if (!str || !runes) return 0;
    while (runes[i] != '\0') {
        byte_len = ui_utf_encode(runes[i], glyph, UI_UTF_SIZE);
        ui_str_insert_at_rune(str, pos+i, glyph, byte_len);
        i++;
    }
    return i;
}

UI_API void
ui_str_remove_chars(struct ui_str *s, int len)
{
    UI_ASSERT(s);
    UI_ASSERT(len >= 0);
    if (!s || len < 0 || (ui_size)len > s->buffer.allocated) return;
    UI_ASSERT(((int)s->buffer.allocated - (int)len) >= 0);
    s->buffer.allocated -= (ui_size)len;
    s->len = ui_utf_len((char *)s->buffer.memory.ptr, (int)s->buffer.allocated);
}

UI_API void
ui_str_remove_runes(struct ui_str *str, int len)
{
    int index;
    const char *begin;
    const char *end;
    ui_rune unicode;

    UI_ASSERT(str);
    UI_ASSERT(len >= 0);
    if (!str || len < 0) return;
    if (len >= str->len) {
        str->len = 0;
        return;
    }

    index = str->len - len;
    begin = ui_str_at_rune(str, index, &unicode, &len);
    end = (const char*)str->buffer.memory.ptr + str->buffer.allocated;
    ui_str_remove_chars(str, (int)(end-begin)+1);
}

UI_API void
ui_str_delete_chars(struct ui_str *s, int pos, int len)
{
    UI_ASSERT(s);
    if (!s || !len || (ui_size)pos > s->buffer.allocated ||
        (ui_size)(pos + len) > s->buffer.allocated) return;

    if ((ui_size)(pos + len) < s->buffer.allocated) {
        /* memmove */
        char *dst = ui_ptr_add(char, s->buffer.memory.ptr, pos);
        char *src = ui_ptr_add(char, s->buffer.memory.ptr, pos + len);
        UI_MEMCPY(dst, src, s->buffer.allocated - (ui_size)(pos + len));
        UI_ASSERT(((int)s->buffer.allocated - (int)len) >= 0);
        s->buffer.allocated -= (ui_size)len;
    } else ui_str_remove_chars(s, len);
    s->len = ui_utf_len((char *)s->buffer.memory.ptr, (int)s->buffer.allocated);
}

UI_API void
ui_str_delete_runes(struct ui_str *s, int pos, int len)
{
    char *temp;
    ui_rune unicode;
    char *begin;
    char *end;
    int unused;

    UI_ASSERT(s);
    UI_ASSERT(s->len >= pos + len);
    if (s->len < pos + len)
        len = UI_CLAMP(0, (s->len - pos), s->len);
    if (!len) return;

    temp = (char *)s->buffer.memory.ptr;
    begin = ui_str_at_rune(s, pos, &unicode, &unused);
    if (!begin) return;
    s->buffer.memory.ptr = begin;
    end = ui_str_at_rune(s, len, &unicode, &unused);
    s->buffer.memory.ptr = temp;
    if (!end) return;
    ui_str_delete_chars(s, (int)(begin - temp), (int)(end - begin));
}

UI_API char*
ui_str_at_char(struct ui_str *s, int pos)
{
    UI_ASSERT(s);
    if (!s || pos > (int)s->buffer.allocated) return 0;
    return ui_ptr_add(char, s->buffer.memory.ptr, pos);
}

UI_API char*
ui_str_at_rune(struct ui_str *str, int pos, ui_rune *unicode, int *len)
{
    int i = 0;
    int src_len = 0;
    int glyph_len = 0;
    char *text;
    int text_len;

    UI_ASSERT(str);
    UI_ASSERT(unicode);
    UI_ASSERT(len);

    if (!str || !unicode || !len) return 0;
    if (pos < 0) {
        *unicode = 0;
        *len = 0;
        return 0;
    }

    text = (char*)str->buffer.memory.ptr;
    text_len = (int)str->buffer.allocated;
    glyph_len = ui_utf_decode(text, unicode, text_len);
    while (glyph_len) {
        if (i == pos) {
            *len = glyph_len;
            break;
        }

        i++;
        src_len = src_len + glyph_len;
        glyph_len = ui_utf_decode(text + src_len, unicode, text_len - src_len);
    }
    if (i != pos) return 0;
    return text + src_len;
}

UI_API const char*
ui_str_at_char_const(const struct ui_str *s, int pos)
{
    UI_ASSERT(s);
    if (!s || pos > (int)s->buffer.allocated) return 0;
    return ui_ptr_add(char, s->buffer.memory.ptr, pos);
}

UI_API const char*
ui_str_at_const(const struct ui_str *str, int pos, ui_rune *unicode, int *len)
{
    int i = 0;
    int src_len = 0;
    int glyph_len = 0;
    char *text;
    int text_len;

    UI_ASSERT(str);
    UI_ASSERT(unicode);
    UI_ASSERT(len);

    if (!str || !unicode || !len) return 0;
    if (pos < 0) {
        *unicode = 0;
        *len = 0;
        return 0;
    }

    text = (char*)str->buffer.memory.ptr;
    text_len = (int)str->buffer.allocated;
    glyph_len = ui_utf_decode(text, unicode, text_len);
    while (glyph_len) {
        if (i == pos) {
            *len = glyph_len;
            break;
        }

        i++;
        src_len = src_len + glyph_len;
        glyph_len = ui_utf_decode(text + src_len, unicode, text_len - src_len);
    }
    if (i != pos) return 0;
    return text + src_len;
}

UI_API ui_rune
ui_str_rune_at(const struct ui_str *str, int pos)
{
    int len;
    ui_rune unicode = 0;
    ui_str_at_const(str, pos, &unicode, &len);
    return unicode;
}

UI_API char*
ui_str_get(struct ui_str *s)
{
    UI_ASSERT(s);
    if (!s || !s->len || !s->buffer.allocated) return 0;
    return (char*)s->buffer.memory.ptr;
}

UI_API const char*
ui_str_get_const(const struct ui_str *s)
{
    UI_ASSERT(s);
    if (!s || !s->len || !s->buffer.allocated) return 0;
    return (const char*)s->buffer.memory.ptr;
}

UI_API int
ui_str_len(struct ui_str *s)
{
    UI_ASSERT(s);
    if (!s || !s->len || !s->buffer.allocated) return 0;
    return s->len;
}

UI_API int
ui_str_len_char(struct ui_str *s)
{
    UI_ASSERT(s);
    if (!s || !s->len || !s->buffer.allocated) return 0;
    return (int)s->buffer.allocated;
}

UI_API void
ui_str_clear(struct ui_str *str)
{
    UI_ASSERT(str);
    ui_buffer_clear(&str->buffer);
    str->len = 0;
}

UI_API void
ui_str_free(struct ui_str *str)
{
    UI_ASSERT(str);
    ui_buffer_free(&str->buffer);
    str->len = 0;
}

/*
 * ==============================================================
 *
 *                      Command buffer
 *
 * ===============================================================
*/
UI_INTERN void
ui_command_buffer_init(struct ui_command_buffer *cmdbuf,
    struct ui_buffer *buffer, enum ui_command_clipping clip)
{
    UI_ASSERT(cmdbuf);
    UI_ASSERT(buffer);
    if (!cmdbuf || !buffer) return;
    cmdbuf->base = buffer;
    cmdbuf->use_clipping = clip;
    cmdbuf->begin = buffer->allocated;
    cmdbuf->end = buffer->allocated;
    cmdbuf->last = buffer->allocated;
}

UI_INTERN void
ui_command_buffer_reset(struct ui_command_buffer *buffer)
{
    UI_ASSERT(buffer);
    if (!buffer) return;
    buffer->begin = 0;
    buffer->end = 0;
    buffer->last = 0;
    buffer->clip = ui_null_rect;
#ifdef UI_INCLUDE_COMMAND_USERDATA
    buffer->userdata.ptr = 0;
#endif
}

UI_INTERN void*
ui_command_buffer_push(struct ui_command_buffer* b,
    enum ui_command_type t, ui_size size)
{
    UI_STORAGE const ui_size align = UI_ALIGNOF(struct ui_command);
    struct ui_command *cmd;
    ui_size alignment;
    void *unaligned;
    void *memory;

    UI_ASSERT(b);
    UI_ASSERT(b->base);
    if (!b) return 0;
    cmd = (struct ui_command*)ui_buffer_alloc(b->base,UI_BUFFER_FRONT,size,align);
    if (!cmd) return 0;

    /* make sure the offset to the next command is aligned */
    b->last = (ui_size)((ui_byte*)cmd - (ui_byte*)b->base->memory.ptr);
    unaligned = (ui_byte*)cmd + size;
    memory = UI_ALIGN_PTR(unaligned, align);
    alignment = (ui_size)((ui_byte*)memory - (ui_byte*)unaligned);
#ifdef UI_ZERO_COMMAND_MEMORY
    UI_MEMSET(cmd, 0, size + alignment);
#endif

    cmd->type = t;
    cmd->next = b->base->allocated + alignment;
#ifdef UI_INCLUDE_COMMAND_USERDATA
    cmd->userdata = b->userdata;
#endif
    b->end = cmd->next;
    return cmd;
}

UI_API void
ui_push_scissor(struct ui_command_buffer *b, struct ui_rect r)
{
    struct ui_command_scissor *cmd;
    UI_ASSERT(b);
    if (!b) return;

    b->clip.x = r.x;
    b->clip.y = r.y;
    b->clip.w = r.w;
    b->clip.h = r.h;
    cmd = (struct ui_command_scissor*)
        ui_command_buffer_push(b, UI_COMMAND_SCISSOR, sizeof(*cmd));

    if (!cmd) return;
    cmd->x = (short)r.x;
    cmd->y = (short)r.y;
    cmd->w = (unsigned short)UI_MAX(0, r.w);
    cmd->h = (unsigned short)UI_MAX(0, r.h);
}

UI_API void
ui_stroke_line(struct ui_command_buffer *b, float x0, float y0,
    float x1, float y1, float line_thickness, struct ui_color c)
{
    struct ui_command_line *cmd;
    UI_ASSERT(b);
    if (!b) return;
    cmd = (struct ui_command_line*)
        ui_command_buffer_push(b, UI_COMMAND_LINE, sizeof(*cmd));
    if (!cmd) return;
    cmd->line_thickness = (unsigned short)line_thickness;
    cmd->begin.x = (short)x0;
    cmd->begin.y = (short)y0;
    cmd->end.x = (short)x1;
    cmd->end.y = (short)y1;
    cmd->color = c;
}

UI_API void
ui_stroke_curve(struct ui_command_buffer *b, float ax, float ay,
    float ctrl0x, float ctrl0y, float ctrl1x, float ctrl1y,
    float bx, float by, float line_thickness, struct ui_color col)
{
    struct ui_command_curve *cmd;
    UI_ASSERT(b);
    if (!b || col.a == 0) return;

    cmd = (struct ui_command_curve*)
        ui_command_buffer_push(b, UI_COMMAND_CURVE, sizeof(*cmd));
    if (!cmd) return;
    cmd->line_thickness = (unsigned short)line_thickness;
    cmd->begin.x = (short)ax;
    cmd->begin.y = (short)ay;
    cmd->ctrl[0].x = (short)ctrl0x;
    cmd->ctrl[0].y = (short)ctrl0y;
    cmd->ctrl[1].x = (short)ctrl1x;
    cmd->ctrl[1].y = (short)ctrl1y;
    cmd->end.x = (short)bx;
    cmd->end.y = (short)by;
    cmd->color = col;
}

UI_API void
ui_stroke_rect(struct ui_command_buffer *b, struct ui_rect rect,
    float rounding, float line_thickness, struct ui_color c)
{
    struct ui_command_rect *cmd;
    UI_ASSERT(b);
    if (!b || c.a == 0 || rect.w == 0 || rect.h == 0) return;
    if (b->use_clipping) {
        const struct ui_rect *clip = &b->clip;
        if (!UI_INTERSECT(rect.x, rect.y, rect.w, rect.h,
            clip->x, clip->y, clip->w, clip->h)) return;
    }

    cmd = (struct ui_command_rect*)
        ui_command_buffer_push(b, UI_COMMAND_RECT, sizeof(*cmd));
    if (!cmd) return;
    cmd->rounding = (unsigned short)rounding;
    cmd->line_thickness = (unsigned short)line_thickness;
    cmd->x = (short)rect.x;
    cmd->y = (short)rect.y;
    cmd->w = (unsigned short)UI_MAX(0, rect.w);
    cmd->h = (unsigned short)UI_MAX(0, rect.h);
    cmd->color = c;
}

UI_API void
ui_fill_rect(struct ui_command_buffer *b, struct ui_rect rect,
    float rounding, struct ui_color c)
{
    struct ui_command_rect_filled *cmd;
    UI_ASSERT(b);
    if (!b || c.a == 0 || rect.w == 0 || rect.h == 0) return;
    if (b->use_clipping) {
        const struct ui_rect *clip = &b->clip;
        if (!UI_INTERSECT(rect.x, rect.y, rect.w, rect.h,
            clip->x, clip->y, clip->w, clip->h)) return;
    }

    cmd = (struct ui_command_rect_filled*)
        ui_command_buffer_push(b, UI_COMMAND_RECT_FILLED, sizeof(*cmd));
    if (!cmd) return;
    cmd->rounding = (unsigned short)rounding;
    cmd->x = (short)rect.x;
    cmd->y = (short)rect.y;
    cmd->w = (unsigned short)UI_MAX(0, rect.w);
    cmd->h = (unsigned short)UI_MAX(0, rect.h);
    cmd->color = c;
}

UI_API void
ui_fill_rect_multi_color(struct ui_command_buffer *b, struct ui_rect rect,
    struct ui_color left, struct ui_color top, struct ui_color right,
    struct ui_color bottom)
{
    struct ui_command_rect_multi_color *cmd;
    UI_ASSERT(b);
    if (!b || rect.w == 0 || rect.h == 0) return;
    if (b->use_clipping) {
        const struct ui_rect *clip = &b->clip;
        if (!UI_INTERSECT(rect.x, rect.y, rect.w, rect.h,
            clip->x, clip->y, clip->w, clip->h)) return;
    }

    cmd = (struct ui_command_rect_multi_color*)
        ui_command_buffer_push(b, UI_COMMAND_RECT_MULTI_COLOR, sizeof(*cmd));
    if (!cmd) return;
    cmd->x = (short)rect.x;
    cmd->y = (short)rect.y;
    cmd->w = (unsigned short)UI_MAX(0, rect.w);
    cmd->h = (unsigned short)UI_MAX(0, rect.h);
    cmd->left = left;
    cmd->top = top;
    cmd->right = right;
    cmd->bottom = bottom;
}

UI_API void
ui_stroke_circle(struct ui_command_buffer *b, struct ui_rect r,
    float line_thickness, struct ui_color c)
{
    struct ui_command_circle *cmd;
    if (!b || r.w == 0 || r.h == 0) return;
    if (b->use_clipping) {
        const struct ui_rect *clip = &b->clip;
        if (!UI_INTERSECT(r.x, r.y, r.w, r.h, clip->x, clip->y, clip->w, clip->h))
            return;
    }

    cmd = (struct ui_command_circle*)
        ui_command_buffer_push(b, UI_COMMAND_CIRCLE, sizeof(*cmd));
    if (!cmd) return;
    cmd->line_thickness = (unsigned short)line_thickness;
    cmd->x = (short)r.x;
    cmd->y = (short)r.y;
    cmd->w = (unsigned short)UI_MAX(r.w, 0);
    cmd->h = (unsigned short)UI_MAX(r.h, 0);
    cmd->color = c;
}

UI_API void
ui_fill_circle(struct ui_command_buffer *b, struct ui_rect r, struct ui_color c)
{
    struct ui_command_circle_filled *cmd;
    UI_ASSERT(b);
    if (!b || c.a == 0 || r.w == 0 || r.h == 0) return;
    if (b->use_clipping) {
        const struct ui_rect *clip = &b->clip;
        if (!UI_INTERSECT(r.x, r.y, r.w, r.h, clip->x, clip->y, clip->w, clip->h))
            return;
    }

    cmd = (struct ui_command_circle_filled*)
        ui_command_buffer_push(b, UI_COMMAND_CIRCLE_FILLED, sizeof(*cmd));
    if (!cmd) return;
    cmd->x = (short)r.x;
    cmd->y = (short)r.y;
    cmd->w = (unsigned short)UI_MAX(r.w, 0);
    cmd->h = (unsigned short)UI_MAX(r.h, 0);
    cmd->color = c;
}

UI_API void
ui_stroke_arc(struct ui_command_buffer *b, float cx, float cy, float radius,
    float a_min, float a_max, float line_thickness, struct ui_color c)
{
    struct ui_command_arc *cmd;
    if (!b || c.a == 0) return;
    cmd = (struct ui_command_arc*)
        ui_command_buffer_push(b, UI_COMMAND_ARC, sizeof(*cmd));
    if (!cmd) return;
    cmd->line_thickness = (unsigned short)line_thickness;
    cmd->cx = (short)cx;
    cmd->cy = (short)cy;
    cmd->r = (unsigned short)radius;
    cmd->a[0] = a_min;
    cmd->a[1] = a_max;
    cmd->color = c;
}

UI_API void
ui_fill_arc(struct ui_command_buffer *b, float cx, float cy, float radius,
    float a_min, float a_max, struct ui_color c)
{
    struct ui_command_arc_filled *cmd;
    UI_ASSERT(b);
    if (!b || c.a == 0) return;
    cmd = (struct ui_command_arc_filled*)
        ui_command_buffer_push(b, UI_COMMAND_ARC_FILLED, sizeof(*cmd));
    if (!cmd) return;
    cmd->cx = (short)cx;
    cmd->cy = (short)cy;
    cmd->r = (unsigned short)radius;
    cmd->a[0] = a_min;
    cmd->a[1] = a_max;
    cmd->color = c;
}

UI_API void
ui_stroke_triangle(struct ui_command_buffer *b, float x0, float y0, float x1,
    float y1, float x2, float y2, float line_thickness, struct ui_color c)
{
    struct ui_command_triangle *cmd;
    UI_ASSERT(b);
    if (!b || c.a == 0) return;
    if (b->use_clipping) {
        const struct ui_rect *clip = &b->clip;
        if (!UI_INBOX(x0, y0, clip->x, clip->y, clip->w, clip->h) &&
            !UI_INBOX(x1, y1, clip->x, clip->y, clip->w, clip->h) &&
            !UI_INBOX(x2, y2, clip->x, clip->y, clip->w, clip->h))
            return;
    }

    cmd = (struct ui_command_triangle*)
        ui_command_buffer_push(b, UI_COMMAND_TRIANGLE, sizeof(*cmd));
    if (!cmd) return;
    cmd->line_thickness = (unsigned short)line_thickness;
    cmd->a.x = (short)x0;
    cmd->a.y = (short)y0;
    cmd->b.x = (short)x1;
    cmd->b.y = (short)y1;
    cmd->c.x = (short)x2;
    cmd->c.y = (short)y2;
    cmd->color = c;
}

UI_API void
ui_fill_triangle(struct ui_command_buffer *b, float x0, float y0, float x1,
    float y1, float x2, float y2, struct ui_color c)
{
    struct ui_command_triangle_filled *cmd;
    UI_ASSERT(b);
    if (!b || c.a == 0) return;
    if (!b) return;
    if (b->use_clipping) {
        const struct ui_rect *clip = &b->clip;
        if (!UI_INBOX(x0, y0, clip->x, clip->y, clip->w, clip->h) &&
            !UI_INBOX(x1, y1, clip->x, clip->y, clip->w, clip->h) &&
            !UI_INBOX(x2, y2, clip->x, clip->y, clip->w, clip->h))
            return;
    }

    cmd = (struct ui_command_triangle_filled*)
        ui_command_buffer_push(b, UI_COMMAND_TRIANGLE_FILLED, sizeof(*cmd));
    if (!cmd) return;
    cmd->a.x = (short)x0;
    cmd->a.y = (short)y0;
    cmd->b.x = (short)x1;
    cmd->b.y = (short)y1;
    cmd->c.x = (short)x2;
    cmd->c.y = (short)y2;
    cmd->color = c;
}

UI_API void
ui_stroke_polygon(struct ui_command_buffer *b,  float *points, int point_count,
    float line_thickness, struct ui_color col)
{
    int i;
    ui_size size = 0;
    struct ui_command_polygon *cmd;

    UI_ASSERT(b);
    if (!b || col.a == 0) return;
    size = sizeof(*cmd) + sizeof(short) * 2 * (ui_size)point_count;
    cmd = (struct ui_command_polygon*) ui_command_buffer_push(b, UI_COMMAND_POLYGON, size);
    if (!cmd) return;
    cmd->color = col;
    cmd->line_thickness = (unsigned short)line_thickness;
    cmd->point_count = (unsigned short)point_count;
    for (i = 0; i < point_count; ++i) {
        cmd->points[i].x = (short)points[i*2];
        cmd->points[i].y = (short)points[i*2+1];
    }
}

UI_API void
ui_fill_polygon(struct ui_command_buffer *b, float *points, int point_count,
    struct ui_color col)
{
    int i;
    ui_size size = 0;
    struct ui_command_polygon_filled *cmd;

    UI_ASSERT(b);
    if (!b || col.a == 0) return;
    size = sizeof(*cmd) + sizeof(short) * 2 * (ui_size)point_count;
    cmd = (struct ui_command_polygon_filled*)
        ui_command_buffer_push(b, UI_COMMAND_POLYGON_FILLED, size);
    if (!cmd) return;
    cmd->color = col;
    cmd->point_count = (unsigned short)point_count;
    for (i = 0; i < point_count; ++i) {
        cmd->points[i].x = (short)points[i*2+0];
        cmd->points[i].y = (short)points[i*2+1];
    }
}

UI_API void
ui_stroke_polyline(struct ui_command_buffer *b, float *points, int point_count,
    float line_thickness, struct ui_color col)
{
    int i;
    ui_size size = 0;
    struct ui_command_polyline *cmd;

    UI_ASSERT(b);
    if (!b || col.a == 0) return;
    size = sizeof(*cmd) + sizeof(short) * 2 * (ui_size)point_count;
    cmd = (struct ui_command_polyline*) ui_command_buffer_push(b, UI_COMMAND_POLYLINE, size);
    if (!cmd) return;
    cmd->color = col;
    cmd->point_count = (unsigned short)point_count;
    cmd->line_thickness = (unsigned short)line_thickness;
    for (i = 0; i < point_count; ++i) {
        cmd->points[i].x = (short)points[i*2];
        cmd->points[i].y = (short)points[i*2+1];
    }
}

UI_API void
ui_draw_image(struct ui_command_buffer *b, struct ui_rect r,
    const struct ui_image *img, struct ui_color col)
{
    struct ui_command_image *cmd;
    UI_ASSERT(b);
    if (!b) return;
    if (b->use_clipping) {
        const struct ui_rect *c = &b->clip;
        if (c->w == 0 || c->h == 0 || !UI_INTERSECT(r.x, r.y, r.w, r.h, c->x, c->y, c->w, c->h))
            return;
    }

    cmd = (struct ui_command_image*)
        ui_command_buffer_push(b, UI_COMMAND_IMAGE, sizeof(*cmd));
    if (!cmd) return;
    cmd->x = (short)r.x;
    cmd->y = (short)r.y;
    cmd->w = (unsigned short)UI_MAX(0, r.w);
    cmd->h = (unsigned short)UI_MAX(0, r.h);
    cmd->img = *img;
    cmd->col = col;
}

UI_API void
ui_draw_text(struct ui_command_buffer *b, struct ui_rect r,
    const char *string, int length, const struct ui_user_font *font,
    struct ui_color bg, struct ui_color fg)
{
    float text_width = 0;
    struct ui_command_text *cmd;

    UI_ASSERT(b);
    UI_ASSERT(font);
    if (!b || !string || !length || (bg.a == 0 && fg.a == 0)) return;
    if (b->use_clipping) {
        const struct ui_rect *c = &b->clip;
        if (c->w == 0 || c->h == 0 || !UI_INTERSECT(r.x, r.y, r.w, r.h, c->x, c->y, c->w, c->h))
            return;
    }

    /* make sure text fits inside bounds */
    text_width = font->width(font->userdata, font->height, string, length);
    if (text_width > r.w){
        int glyphs = 0;
        float txt_width = (float)text_width;
        length = ui_text_clamp(font, string, length, r.w, &glyphs, &txt_width, 0,0);
    }

    if (!length) return;
    cmd = (struct ui_command_text*)
        ui_command_buffer_push(b, UI_COMMAND_TEXT, sizeof(*cmd) + (ui_size)(length + 1));
    if (!cmd) return;
    cmd->x = (short)r.x;
    cmd->y = (short)r.y;
    cmd->w = (unsigned short)r.w;
    cmd->h = (unsigned short)r.h;
    cmd->background = bg;
    cmd->foreground = fg;
    cmd->font = font;
    cmd->length = length;
    cmd->height = font->height;
    UI_MEMCPY(cmd->string, string, (ui_size)length);
    cmd->string[length] = '\0';
}

/* ==============================================================
 *
 *                          DRAW LIST
 *
 * ===============================================================*/
#ifdef UI_INCLUDE_VERTEX_BUFFER_OUTPUT
UI_API void
ui_draw_list_init(struct ui_draw_list *list)
{
    ui_size i = 0;
    UI_ASSERT(list);
    if (!list) return;
    ui_zero(list, sizeof(*list));
    for (i = 0; i < UI_LEN(list->circle_vtx); ++i) {
        const float a = ((float)i / (float)UI_LEN(list->circle_vtx)) * 2 * UI_PI;
        list->circle_vtx[i].x = (float)UI_COS(a);
        list->circle_vtx[i].y = (float)UI_SIN(a);
    }
}

UI_API void
ui_draw_list_setup(struct ui_draw_list *canvas, const struct ui_convert_config *config,
    struct ui_buffer *cmds, struct ui_buffer *vertices, struct ui_buffer *elements)
{
    UI_ASSERT(canvas);
    UI_ASSERT(config);
    UI_ASSERT(cmds);
    UI_ASSERT(vertices);
    UI_ASSERT(elements);
    if (!canvas || !config || !cmds || !vertices || !elements)
        return;

    canvas->buffer = cmds;
    canvas->config = *config;
    canvas->elements = elements;
    canvas->vertices = vertices;
    canvas->clip_rect = ui_null_rect;
}

UI_API const struct ui_draw_command*
ui__draw_list_begin(const struct ui_draw_list *canvas, const struct ui_buffer *buffer)
{
    ui_byte *memory;
    ui_size offset;
    const struct ui_draw_command *cmd;

    UI_ASSERT(buffer);
    if (!buffer || !buffer->size || !canvas->cmd_count)
        return 0;

    memory = (ui_byte*)buffer->memory.ptr;
    offset = buffer->memory.size - canvas->cmd_offset;
    cmd = ui_ptr_add(const struct ui_draw_command, memory, offset);
    return cmd;
}

UI_API const struct ui_draw_command*
ui__draw_list_end(const struct ui_draw_list *canvas, const struct ui_buffer *buffer)
{
    ui_size size;
    ui_size offset;
    ui_byte *memory;
    const struct ui_draw_command *end;

    UI_ASSERT(buffer);
    UI_ASSERT(canvas);
    if (!buffer || !canvas)
        return 0;

    memory = (ui_byte*)buffer->memory.ptr;
    size = buffer->memory.size;
    offset = size - canvas->cmd_offset;
    end = ui_ptr_add(const struct ui_draw_command, memory, offset);
    end -= (canvas->cmd_count-1);
    return end;
}

UI_API const struct ui_draw_command*
ui__draw_list_next(const struct ui_draw_command *cmd,
    const struct ui_buffer *buffer, const struct ui_draw_list *canvas)
{
    const struct ui_draw_command *end;
    UI_ASSERT(buffer);
    UI_ASSERT(canvas);
    if (!cmd || !buffer || !canvas)
        return 0;

    end = ui__draw_list_end(canvas, buffer);
    if (cmd <= end) return 0;
    return (cmd-1);
}

UI_API void
ui_draw_list_clear(struct ui_draw_list *list)
{
    UI_ASSERT(list);
    if (!list) return;
    if (list->buffer)
        ui_buffer_clear(list->buffer);
    if (list->vertices)
        ui_buffer_clear(list->vertices);
    if (list->elements)
        ui_buffer_clear(list->elements);

    list->element_count = 0;
    list->vertex_count = 0;
    list->cmd_offset = 0;
    list->cmd_count = 0;
    list->path_count = 0;
    list->vertices = 0;
    list->elements = 0;
    list->clip_rect = ui_null_rect;
}

UI_INTERN struct ui_vec2*
ui_draw_list_alloc_path(struct ui_draw_list *list, int count)
{
    struct ui_vec2 *points;
    UI_STORAGE const ui_size point_align = UI_ALIGNOF(struct ui_vec2);
    UI_STORAGE const ui_size point_size = sizeof(struct ui_vec2);
    points = (struct ui_vec2*)
        ui_buffer_alloc(list->buffer, UI_BUFFER_FRONT,
                        point_size * (ui_size)count, point_align);

    if (!points) return 0;
    if (!list->path_offset) {
        void *memory = ui_buffer_memory(list->buffer);
        list->path_offset = (unsigned int)((ui_byte*)points - (ui_byte*)memory);
    }
    list->path_count += (unsigned int)count;
    return points;
}

UI_INTERN struct ui_vec2
ui_draw_list_path_last(struct ui_draw_list *list)
{
    void *memory;
    struct ui_vec2 *point;
    UI_ASSERT(list->path_count);
    memory = ui_buffer_memory(list->buffer);
    point = ui_ptr_add(struct ui_vec2, memory, list->path_offset);
    point += (list->path_count-1);
    return *point;
}

UI_INTERN struct ui_draw_command*
ui_draw_list_push_command(struct ui_draw_list *list, struct ui_rect clip,
    ui_handle texture)
{
    UI_STORAGE const ui_size cmd_align = UI_ALIGNOF(struct ui_draw_command);
    UI_STORAGE const ui_size cmd_size = sizeof(struct ui_draw_command);
    struct ui_draw_command *cmd;

    UI_ASSERT(list);
    cmd = (struct ui_draw_command*)
        ui_buffer_alloc(list->buffer, UI_BUFFER_BACK, cmd_size, cmd_align);

    if (!cmd) return 0;
    if (!list->cmd_count) {
        ui_byte *memory = (ui_byte*)ui_buffer_memory(list->buffer);
        ui_size total = ui_buffer_total(list->buffer);
        memory = ui_ptr_add(ui_byte, memory, total);
        list->cmd_offset = (ui_size)(memory - (ui_byte*)cmd);
    }

    cmd->elem_count = 0;
    cmd->clip_rect = clip;
    cmd->texture = texture;
#ifdef UI_INCLUDE_COMMAND_USERDATA
    cmd->userdata = list->userdata;
#endif

    list->cmd_count++;
    list->clip_rect = clip;
    return cmd;
}

UI_INTERN struct ui_draw_command*
ui_draw_list_command_last(struct ui_draw_list *list)
{
    void *memory;
    ui_size size;
    struct ui_draw_command *cmd;
    UI_ASSERT(list->cmd_count);

    memory = ui_buffer_memory(list->buffer);
    size = ui_buffer_total(list->buffer);
    cmd = ui_ptr_add(struct ui_draw_command, memory, size - list->cmd_offset);
    return (cmd - (list->cmd_count-1));
}

UI_INTERN void
ui_draw_list_add_clip(struct ui_draw_list *list, struct ui_rect rect)
{
    UI_ASSERT(list);
    if (!list) return;
    if (!list->cmd_count) {
        ui_draw_list_push_command(list, rect, list->config.null.texture);
    } else {
        struct ui_draw_command *prev = ui_draw_list_command_last(list);
        if (prev->elem_count == 0)
            prev->clip_rect = rect;
        ui_draw_list_push_command(list, rect, prev->texture);
    }
}

UI_INTERN void
ui_draw_list_push_image(struct ui_draw_list *list, ui_handle texture)
{
    UI_ASSERT(list);
    if (!list) return;
    if (!list->cmd_count) {
        ui_draw_list_push_command(list, ui_null_rect, texture);
    } else {
        struct ui_draw_command *prev = ui_draw_list_command_last(list);
        if (prev->elem_count == 0)
            prev->texture = texture;
        else if (prev->texture.id != texture.id)
            ui_draw_list_push_command(list, prev->clip_rect, texture);
    }
}

#ifdef UI_INCLUDE_COMMAND_USERDATA
UI_API void
ui_draw_list_push_userdata(struct ui_draw_list *list, ui_handle userdata)
{
    list->userdata = userdata;
}
#endif

UI_INTERN void*
ui_draw_list_alloc_vertices(struct ui_draw_list *list, ui_size count)
{
    void *vtx;
    UI_ASSERT(list);
    if (!list) return 0;
    vtx = ui_buffer_alloc(list->vertices, UI_BUFFER_FRONT,
        list->config.vertex_size*count, list->config.vertex_alignment);
    if (!vtx) return 0;
    list->vertex_count += (unsigned int)count;
    return vtx;
}

UI_INTERN ui_draw_index*
ui_draw_list_alloc_elements(struct ui_draw_list *list, ui_size count)
{
    ui_draw_index *ids;
    struct ui_draw_command *cmd;
    UI_STORAGE const ui_size elem_align = UI_ALIGNOF(ui_draw_index);
    UI_STORAGE const ui_size elem_size = sizeof(ui_draw_index);
    UI_ASSERT(list);
    if (!list) return 0;

    ids = (ui_draw_index*)
        ui_buffer_alloc(list->elements, UI_BUFFER_FRONT, elem_size*count, elem_align);
    if (!ids) return 0;
    cmd = ui_draw_list_command_last(list);
    list->element_count += (unsigned int)count;
    cmd->elem_count += (unsigned int)count;
    return ids;
}

static int
ui_draw_vertex_layout_element_is_end_of_layout(
    const struct ui_draw_vertex_layout_element *element)
{
    return (element->attribute == UI_VERTEX_ATTRIBUTE_COUNT ||
            element->format == UI_FORMAT_COUNT);
}

static void
ui_draw_vertex_color(void *attribute, const float *values,
    enum ui_draw_vertex_layout_format format)
{
    /* if this triggers you tried to provide a value format for a color */
    UI_ASSERT(format >= UI_FORMAT_COLOR_BEGIN);
    UI_ASSERT(format <= UI_FORMAT_COLOR_END);
    if (format < UI_FORMAT_COLOR_BEGIN || format > UI_FORMAT_COLOR_END) return;

    switch (format) {
    default: UI_ASSERT(0 && "Invalid vertex layout color format"); break;
    case UI_FORMAT_R8G8B8A8:
    case UI_FORMAT_R8G8B8: {
        struct ui_color col = ui_rgba_fv(values);
        UI_MEMCPY(attribute, &col.r, sizeof(col));
    } break;
    case UI_FORMAT_R16G15B16: {
        ui_ushort col[3];
        col[0] = (ui_ushort)UI_CLAMP(UI_USHORT_MIN, values[0] * UI_USHORT_MAX, UI_USHORT_MAX);
        col[1] = (ui_ushort)UI_CLAMP(UI_USHORT_MIN, values[1] * UI_USHORT_MAX, UI_USHORT_MAX);
        col[2] = (ui_ushort)UI_CLAMP(UI_USHORT_MIN, values[2] * UI_USHORT_MAX, UI_USHORT_MAX);
        UI_MEMCPY(attribute, col, sizeof(col));
    } break;
    case UI_FORMAT_R16G15B16A16: {
        ui_ushort col[4];
        col[0] = (ui_ushort)UI_CLAMP(UI_USHORT_MIN, values[0] * UI_USHORT_MAX, UI_USHORT_MAX);
        col[1] = (ui_ushort)UI_CLAMP(UI_USHORT_MIN, values[1] * UI_USHORT_MAX, UI_USHORT_MAX);
        col[2] = (ui_ushort)UI_CLAMP(UI_USHORT_MIN, values[2] * UI_USHORT_MAX, UI_USHORT_MAX);
        col[3] = (ui_ushort)UI_CLAMP(UI_USHORT_MIN, values[3] * UI_USHORT_MAX, UI_USHORT_MAX);
        UI_MEMCPY(attribute, col, sizeof(col));
    } break;
    case UI_FORMAT_R32G32B32: {
        ui_uint col[3];
        col[0] = (ui_uint)UI_CLAMP(UI_UINT_MIN, values[0] * UI_UINT_MAX, UI_UINT_MAX);
        col[1] = (ui_uint)UI_CLAMP(UI_UINT_MIN, values[1] * UI_UINT_MAX, UI_UINT_MAX);
        col[2] = (ui_uint)UI_CLAMP(UI_UINT_MIN, values[2] * UI_UINT_MAX, UI_UINT_MAX);
        UI_MEMCPY(attribute, col, sizeof(col));
    } break;
    case UI_FORMAT_R32G32B32A32: {
        ui_uint col[4];
        col[0] = (ui_uint)UI_CLAMP(UI_UINT_MIN, values[0] * UI_UINT_MAX, UI_UINT_MAX);
        col[1] = (ui_uint)UI_CLAMP(UI_UINT_MIN, values[1] * UI_UINT_MAX, UI_UINT_MAX);
        col[2] = (ui_uint)UI_CLAMP(UI_UINT_MIN, values[2] * UI_UINT_MAX, UI_UINT_MAX);
        col[3] = (ui_uint)UI_CLAMP(UI_UINT_MIN, values[3] * UI_UINT_MAX, UI_UINT_MAX);
        UI_MEMCPY(attribute, col, sizeof(col));
    } break;
    case UI_FORMAT_R32G32B32A32_FLOAT:
        UI_MEMCPY(attribute, values, sizeof(float)*4);
        break;
    case UI_FORMAT_R32G32B32A32_DOUBLE: {
        double col[4];
        col[0] = (double)UI_SATURATE(values[0]);
        col[1] = (double)UI_SATURATE(values[1]);
        col[2] = (double)UI_SATURATE(values[2]);
        col[3] = (double)UI_SATURATE(values[3]);
        UI_MEMCPY(attribute, col, sizeof(col));
    } break;
    case UI_FORMAT_RGB32:
    case UI_FORMAT_RGBA32: {
        struct ui_color col = ui_rgba_fv(values);
        ui_uint color = ui_color_u32(col);
        UI_MEMCPY(attribute, &color, sizeof(color));
    } break;
    }
}

static void
ui_draw_vertex_element(void *dst, const float *values, int value_count,
    enum ui_draw_vertex_layout_format format)
{
    int value_index;
    void *attribute = dst;
    /* if this triggers you tried to provide a color format for a value */
    UI_ASSERT(format < UI_FORMAT_COLOR_BEGIN);
    if (format >= UI_FORMAT_COLOR_BEGIN && format <= UI_FORMAT_COLOR_END) return;
    for (value_index = 0; value_index < value_count; ++value_index) {
        switch (format) {
        default: UI_ASSERT(0 && "invalid vertex layout format"); break;
        case UI_FORMAT_SCHAR: {
            char value = (char)UI_CLAMP(UI_SCHAR_MIN, values[value_index], UI_SCHAR_MAX);
            UI_MEMCPY(attribute, &value, sizeof(value));
            attribute = (void*)((char*)attribute + sizeof(char));
        } break;
        case UI_FORMAT_SSHORT: {
            ui_short value = (ui_short)UI_CLAMP(UI_SSHORT_MIN, values[value_index], UI_SSHORT_MAX);
            UI_MEMCPY(attribute, &value, sizeof(value));
            attribute = (void*)((char*)attribute + sizeof(value));
        } break;
        case UI_FORMAT_SINT: {
            ui_int value = (ui_int)UI_CLAMP(UI_SINT_MIN, values[value_index], UI_SINT_MAX);
            UI_MEMCPY(attribute, &value, sizeof(value));
            attribute = (void*)((char*)attribute + sizeof(ui_int));
        } break;
        case UI_FORMAT_UCHAR: {
            unsigned char value = (unsigned char)UI_CLAMP(UI_UCHAR_MIN, values[value_index], UI_UCHAR_MAX);
            UI_MEMCPY(attribute, &value, sizeof(value));
            attribute = (void*)((char*)attribute + sizeof(unsigned char));
        } break;
        case UI_FORMAT_USHORT: {
            ui_ushort value = (ui_ushort)UI_CLAMP(UI_USHORT_MIN, values[value_index], UI_USHORT_MAX);
            UI_MEMCPY(attribute, &value, sizeof(value));
            attribute = (void*)((char*)attribute + sizeof(value));
            } break;
        case UI_FORMAT_UINT: {
            ui_uint value = (ui_uint)UI_CLAMP(UI_UINT_MIN, values[value_index], UI_UINT_MAX);
            UI_MEMCPY(attribute, &value, sizeof(value));
            attribute = (void*)((char*)attribute + sizeof(ui_uint));
        } break;
        case UI_FORMAT_FLOAT:
            UI_MEMCPY(attribute, &values[value_index], sizeof(values[value_index]));
            attribute = (void*)((char*)attribute + sizeof(float));
            break;
        case UI_FORMAT_DOUBLE: {
            double value = (double)values[value_index];
            UI_MEMCPY(attribute, &value, sizeof(value));
            attribute = (void*)((char*)attribute + sizeof(double));
            } break;
        }
    }
}

UI_INTERN void*
ui_draw_vertex(void *dst, const struct ui_convert_config *config,
    struct ui_vec2 pos, struct ui_vec2 uv, struct ui_colorf color)
{
    void *result = (void*)((char*)dst + config->vertex_size);
    const struct ui_draw_vertex_layout_element *elem_iter = config->vertex_layout;
    while (!ui_draw_vertex_layout_element_is_end_of_layout(elem_iter)) {
        void *address = (void*)((char*)dst + elem_iter->offset);
        switch (elem_iter->attribute) {
        case UI_VERTEX_ATTRIBUTE_COUNT:
        default: UI_ASSERT(0 && "wrong element attribute");
        case UI_VERTEX_POSITION: ui_draw_vertex_element(address, &pos.x, 2, elem_iter->format); break;
        case UI_VERTEX_TEXCOORD: ui_draw_vertex_element(address, &uv.x, 2, elem_iter->format); break;
        case UI_VERTEX_COLOR: ui_draw_vertex_color(address, &color.r, elem_iter->format); break;
        }
        elem_iter++;
    }
    return result;
}

UI_API void
ui_draw_list_stroke_poly_line(struct ui_draw_list *list, const struct ui_vec2 *points,
    const unsigned int points_count, struct ui_color color, enum ui_draw_list_stroke closed,
    float thickness, enum ui_anti_aliasing aliasing)
{
    ui_size count;
    int thick_line;
    struct ui_colorf col;
    struct ui_colorf col_trans;
    UI_ASSERT(list);
    if (!list || points_count < 2) return;

    color.a = (ui_byte)((float)color.a * list->config.global_alpha);
    count = points_count;
    if (!closed) count = points_count-1;
    thick_line = thickness > 1.0f;

#ifdef UI_INCLUDE_COMMAND_USERDATA
    ui_draw_list_push_userdata(list, list->userdata);
#endif

    color.a = (ui_byte)((float)color.a * list->config.global_alpha);
    ui_color_fv(&col.r, color);
    col_trans = col;
    col_trans.a = 0;

    if (aliasing == UI_ANTI_ALIASING_ON) {
        /* ANTI-ALIASED STROKE */
        const float AA_SIZE = 1.0f;
        UI_STORAGE const ui_size pnt_align = UI_ALIGNOF(struct ui_vec2);
        UI_STORAGE const ui_size pnt_size = sizeof(struct ui_vec2);

        /* allocate vertices and elements  */
        ui_size i1 = 0;
        ui_size vertex_offset;
        ui_size index = list->vertex_count;

        const ui_size idx_count = (thick_line) ?  (count * 18) : (count * 12);
        const ui_size vtx_count = (thick_line) ? (points_count * 4): (points_count *3);

        void *vtx = ui_draw_list_alloc_vertices(list, vtx_count);
        ui_draw_index *ids = ui_draw_list_alloc_elements(list, idx_count);

        ui_size size;
        struct ui_vec2 *normals, *temp;
        UI_ASSERT(vtx && ids);
        if (!vtx || !ids) return;

        /* temporary allocate normals + points */
        vertex_offset = (ui_size)((ui_byte*)vtx - (ui_byte*)list->vertices->memory.ptr);
        ui_buffer_mark(list->vertices, UI_BUFFER_FRONT);
        size = pnt_size * ((thick_line) ? 5 : 3) * points_count;
        normals = (struct ui_vec2*) ui_buffer_alloc(list->vertices, UI_BUFFER_FRONT, size, pnt_align);
        UI_ASSERT(normals);
        if (!normals) return;
        temp = normals + points_count;

        /* make sure vertex pointer is still correct */
        vtx = (void*)((ui_byte*)list->vertices->memory.ptr + vertex_offset);

        /* calculate normals */
        for (i1 = 0; i1 < count; ++i1) {
            const ui_size i2 = ((i1 + 1) == points_count) ? 0 : (i1 + 1);
            struct ui_vec2 diff = ui_vec2_sub(points[i2], points[i1]);
            float len;

            /* vec2 inverted length  */
            len = ui_vec2_len_sqr(diff);
            if (len != 0.0f)
                len = ui_inv_sqrt(len);
            else len = 1.0f;

            diff = ui_vec2_muls(diff, len);
            normals[i1].x = diff.y;
            normals[i1].y = -diff.x;
        }

        if (!closed)
            normals[points_count-1] = normals[points_count-2];

        if (!thick_line) {
            ui_size idx1, i;
            if (!closed) {
                struct ui_vec2 d;
                temp[0] = ui_vec2_add(points[0], ui_vec2_muls(normals[0], AA_SIZE));
                temp[1] = ui_vec2_sub(points[0], ui_vec2_muls(normals[0], AA_SIZE));
                d = ui_vec2_muls(normals[points_count-1], AA_SIZE);
                temp[(points_count-1) * 2 + 0] = ui_vec2_add(points[points_count-1], d);
                temp[(points_count-1) * 2 + 1] = ui_vec2_sub(points[points_count-1], d);
            }

            /* fill elements */
            idx1 = index;
            for (i1 = 0; i1 < count; i1++) {
                struct ui_vec2 dm;
                float dmr2;
                ui_size i2 = ((i1 + 1) == points_count) ? 0 : (i1 + 1);
                ui_size idx2 = ((i1+1) == points_count) ? index: (idx1 + 3);

                /* average normals */
                dm = ui_vec2_muls(ui_vec2_add(normals[i1], normals[i2]), 0.5f);
                dmr2 = dm.x * dm.x + dm.y* dm.y;
                if (dmr2 > 0.000001f) {
                    float scale = 1.0f/dmr2;
                    scale = UI_MIN(100.0f, scale);
                    dm = ui_vec2_muls(dm, scale);
                }

                dm = ui_vec2_muls(dm, AA_SIZE);
                temp[i2*2+0] = ui_vec2_add(points[i2], dm);
                temp[i2*2+1] = ui_vec2_sub(points[i2], dm);

                ids[0] = (ui_draw_index)(idx2 + 0); ids[1] = (ui_draw_index)(idx1+0);
                ids[2] = (ui_draw_index)(idx1 + 2); ids[3] = (ui_draw_index)(idx1+2);
                ids[4] = (ui_draw_index)(idx2 + 2); ids[5] = (ui_draw_index)(idx2+0);
                ids[6] = (ui_draw_index)(idx2 + 1); ids[7] = (ui_draw_index)(idx1+1);
                ids[8] = (ui_draw_index)(idx1 + 0); ids[9] = (ui_draw_index)(idx1+0);
                ids[10]= (ui_draw_index)(idx2 + 0); ids[11]= (ui_draw_index)(idx2+1);
                ids += 12;
                idx1 = idx2;
            }

            /* fill vertices */
            for (i = 0; i < points_count; ++i) {
                const struct ui_vec2 uv = list->config.null.uv;
                vtx = ui_draw_vertex(vtx, &list->config, points[i], uv, col);
                vtx = ui_draw_vertex(vtx, &list->config, temp[i*2+0], uv, col_trans);
                vtx = ui_draw_vertex(vtx, &list->config, temp[i*2+1], uv, col_trans);
            }
        } else {
            ui_size idx1, i;
            const float half_inner_thickness = (thickness - AA_SIZE) * 0.5f;
            if (!closed) {
                struct ui_vec2 d1 = ui_vec2_muls(normals[0], half_inner_thickness + AA_SIZE);
                struct ui_vec2 d2 = ui_vec2_muls(normals[0], half_inner_thickness);

                temp[0] = ui_vec2_add(points[0], d1);
                temp[1] = ui_vec2_add(points[0], d2);
                temp[2] = ui_vec2_sub(points[0], d2);
                temp[3] = ui_vec2_sub(points[0], d1);

                d1 = ui_vec2_muls(normals[points_count-1], half_inner_thickness + AA_SIZE);
                d2 = ui_vec2_muls(normals[points_count-1], half_inner_thickness);

                temp[(points_count-1)*4+0] = ui_vec2_add(points[points_count-1], d1);
                temp[(points_count-1)*4+1] = ui_vec2_add(points[points_count-1], d2);
                temp[(points_count-1)*4+2] = ui_vec2_sub(points[points_count-1], d2);
                temp[(points_count-1)*4+3] = ui_vec2_sub(points[points_count-1], d1);
            }

            /* add all elements */
            idx1 = index;
            for (i1 = 0; i1 < count; ++i1) {
                struct ui_vec2 dm_out, dm_in;
                const ui_size i2 = ((i1+1) == points_count) ? 0: (i1 + 1);
                ui_size idx2 = ((i1+1) == points_count) ? index: (idx1 + 4);

                /* average normals */
                struct ui_vec2 dm = ui_vec2_muls(ui_vec2_add(normals[i1], normals[i2]), 0.5f);
                float dmr2 = dm.x * dm.x + dm.y* dm.y;
                if (dmr2 > 0.000001f) {
                    float scale = 1.0f/dmr2;
                    scale = UI_MIN(100.0f, scale);
                    dm = ui_vec2_muls(dm, scale);
                }

                dm_out = ui_vec2_muls(dm, ((half_inner_thickness) + AA_SIZE));
                dm_in = ui_vec2_muls(dm, half_inner_thickness);
                temp[i2*4+0] = ui_vec2_add(points[i2], dm_out);
                temp[i2*4+1] = ui_vec2_add(points[i2], dm_in);
                temp[i2*4+2] = ui_vec2_sub(points[i2], dm_in);
                temp[i2*4+3] = ui_vec2_sub(points[i2], dm_out);

                /* add indexes */
                ids[0] = (ui_draw_index)(idx2 + 1); ids[1] = (ui_draw_index)(idx1+1);
                ids[2] = (ui_draw_index)(idx1 + 2); ids[3] = (ui_draw_index)(idx1+2);
                ids[4] = (ui_draw_index)(idx2 + 2); ids[5] = (ui_draw_index)(idx2+1);
                ids[6] = (ui_draw_index)(idx2 + 1); ids[7] = (ui_draw_index)(idx1+1);
                ids[8] = (ui_draw_index)(idx1 + 0); ids[9] = (ui_draw_index)(idx1+0);
                ids[10]= (ui_draw_index)(idx2 + 0); ids[11] = (ui_draw_index)(idx2+1);
                ids[12]= (ui_draw_index)(idx2 + 2); ids[13] = (ui_draw_index)(idx1+2);
                ids[14]= (ui_draw_index)(idx1 + 3); ids[15] = (ui_draw_index)(idx1+3);
                ids[16]= (ui_draw_index)(idx2 + 3); ids[17] = (ui_draw_index)(idx2+2);
                ids += 18;
                idx1 = idx2;
            }

            /* add vertices */
            for (i = 0; i < points_count; ++i) {
                const struct ui_vec2 uv = list->config.null.uv;
                vtx = ui_draw_vertex(vtx, &list->config, temp[i*4+0], uv, col_trans);
                vtx = ui_draw_vertex(vtx, &list->config, temp[i*4+1], uv, col);
                vtx = ui_draw_vertex(vtx, &list->config, temp[i*4+2], uv, col);
                vtx = ui_draw_vertex(vtx, &list->config, temp[i*4+3], uv, col_trans);
            }
        }
        /* free temporary normals + points */
        ui_buffer_reset(list->vertices, UI_BUFFER_FRONT);
    } else {
        /* NON ANTI-ALIASED STROKE */
        ui_size i1 = 0;
        ui_size idx = list->vertex_count;
        const ui_size idx_count = count * 6;
        const ui_size vtx_count = count * 4;
        void *vtx = ui_draw_list_alloc_vertices(list, vtx_count);
        ui_draw_index *ids = ui_draw_list_alloc_elements(list, idx_count);
        if (!vtx || !ids) return;

        for (i1 = 0; i1 < count; ++i1) {
            float dx, dy;
            const struct ui_vec2 uv = list->config.null.uv;
            const ui_size i2 = ((i1+1) == points_count) ? 0 : i1 + 1;
            const struct ui_vec2 p1 = points[i1];
            const struct ui_vec2 p2 = points[i2];
            struct ui_vec2 diff = ui_vec2_sub(p2, p1);
            float len;

            /* vec2 inverted length  */
            len = ui_vec2_len_sqr(diff);
            if (len != 0.0f)
                len = ui_inv_sqrt(len);
            else len = 1.0f;
            diff = ui_vec2_muls(diff, len);

            /* add vertices */
            dx = diff.x * (thickness * 0.5f);
            dy = diff.y * (thickness * 0.5f);

            vtx = ui_draw_vertex(vtx, &list->config, ui_vec2(p1.x + dy, p1.y - dx), uv, col);
            vtx = ui_draw_vertex(vtx, &list->config, ui_vec2(p2.x + dy, p2.y - dx), uv, col);
            vtx = ui_draw_vertex(vtx, &list->config, ui_vec2(p2.x - dy, p2.y + dx), uv, col);
            vtx = ui_draw_vertex(vtx, &list->config, ui_vec2(p1.x - dy, p1.y + dx), uv, col);

            ids[0] = (ui_draw_index)(idx+0); ids[1] = (ui_draw_index)(idx+1);
            ids[2] = (ui_draw_index)(idx+2); ids[3] = (ui_draw_index)(idx+0);
            ids[4] = (ui_draw_index)(idx+2); ids[5] = (ui_draw_index)(idx+3);

            ids += 6;
            idx += 4;
        }
    }
}

UI_API void
ui_draw_list_fill_poly_convex(struct ui_draw_list *list,
    const struct ui_vec2 *points, const unsigned int points_count,
    struct ui_color color, enum ui_anti_aliasing aliasing)
{
    struct ui_colorf col;
    struct ui_colorf col_trans;

    UI_STORAGE const ui_size pnt_align = UI_ALIGNOF(struct ui_vec2);
    UI_STORAGE const ui_size pnt_size = sizeof(struct ui_vec2);
    UI_ASSERT(list);
    if (!list || points_count < 3) return;

#ifdef UI_INCLUDE_COMMAND_USERDATA
    ui_draw_list_push_userdata(list, list->userdata);
#endif

    color.a = (ui_byte)((float)color.a * list->config.global_alpha);
    ui_color_fv(&col.r, color);
    col_trans = col;
    col_trans.a = 0;

    if (aliasing == UI_ANTI_ALIASING_ON) {
        ui_size i = 0;
        ui_size i0 = 0;
        ui_size i1 = 0;

        const float AA_SIZE = 1.0f;
        ui_size vertex_offset = 0;
        ui_size index = list->vertex_count;

        const ui_size idx_count = (points_count-2)*3 + points_count*6;
        const ui_size vtx_count = (points_count*2);

        void *vtx = ui_draw_list_alloc_vertices(list, vtx_count);
        ui_draw_index *ids = ui_draw_list_alloc_elements(list, idx_count);

        ui_size size = 0;
        struct ui_vec2 *normals = 0;
        unsigned int vtx_inner_idx = (unsigned int)(index + 0);
        unsigned int vtx_outer_idx = (unsigned int)(index + 1);
        if (!vtx || !ids) return;

        /* temporary allocate normals */
        vertex_offset = (ui_size)((ui_byte*)vtx - (ui_byte*)list->vertices->memory.ptr);
        ui_buffer_mark(list->vertices, UI_BUFFER_FRONT);
        size = pnt_size * points_count;
        normals = (struct ui_vec2*) ui_buffer_alloc(list->vertices, UI_BUFFER_FRONT, size, pnt_align);
        UI_ASSERT(normals);
        if (!normals) return;
        vtx = (void*)((ui_byte*)list->vertices->memory.ptr + vertex_offset);

        /* add elements */
        for (i = 2; i < points_count; i++) {
            ids[0] = (ui_draw_index)(vtx_inner_idx);
            ids[1] = (ui_draw_index)(vtx_inner_idx + ((i-1) << 1));
            ids[2] = (ui_draw_index)(vtx_inner_idx + (i << 1));
            ids += 3;
        }

        /* compute normals */
        for (i0 = points_count-1, i1 = 0; i1 < points_count; i0 = i1++) {
            struct ui_vec2 p0 = points[i0];
            struct ui_vec2 p1 = points[i1];
            struct ui_vec2 diff = ui_vec2_sub(p1, p0);

            /* vec2 inverted length  */
            float len = ui_vec2_len_sqr(diff);
            if (len != 0.0f)
                len = ui_inv_sqrt(len);
            else len = 1.0f;
            diff = ui_vec2_muls(diff, len);

            normals[i0].x = diff.y;
            normals[i0].y = -diff.x;
        }

        /* add vertices + indexes */
        for (i0 = points_count-1, i1 = 0; i1 < points_count; i0 = i1++) {
            const struct ui_vec2 uv = list->config.null.uv;
            struct ui_vec2 n0 = normals[i0];
            struct ui_vec2 n1 = normals[i1];
            struct ui_vec2 dm = ui_vec2_muls(ui_vec2_add(n0, n1), 0.5f);
            float dmr2 = dm.x*dm.x + dm.y*dm.y;
            if (dmr2 > 0.000001f) {
                float scale = 1.0f / dmr2;
                scale = UI_MIN(scale, 100.0f);
                dm = ui_vec2_muls(dm, scale);
            }
            dm = ui_vec2_muls(dm, AA_SIZE * 0.5f);

            /* add vertices */
            vtx = ui_draw_vertex(vtx, &list->config, ui_vec2_sub(points[i1], dm), uv, col);
            vtx = ui_draw_vertex(vtx, &list->config, ui_vec2_add(points[i1], dm), uv, col_trans);

            /* add indexes */
            ids[0] = (ui_draw_index)(vtx_inner_idx+(i1<<1));
            ids[1] = (ui_draw_index)(vtx_inner_idx+(i0<<1));
            ids[2] = (ui_draw_index)(vtx_outer_idx+(i0<<1));
            ids[3] = (ui_draw_index)(vtx_outer_idx+(i0<<1));
            ids[4] = (ui_draw_index)(vtx_outer_idx+(i1<<1));
            ids[5] = (ui_draw_index)(vtx_inner_idx+(i1<<1));
            ids += 6;
        }
        /* free temporary normals + points */
        ui_buffer_reset(list->vertices, UI_BUFFER_FRONT);
    } else {
        ui_size i = 0;
        ui_size index = list->vertex_count;
        const ui_size idx_count = (points_count-2)*3;
        const ui_size vtx_count = points_count;
        void *vtx = ui_draw_list_alloc_vertices(list, vtx_count);
        ui_draw_index *ids = ui_draw_list_alloc_elements(list, idx_count);

        if (!vtx || !ids) return;
        for (i = 0; i < vtx_count; ++i)
            vtx = ui_draw_vertex(vtx, &list->config, points[i], list->config.null.uv, col);
        for (i = 2; i < points_count; ++i) {
            ids[0] = (ui_draw_index)index;
            ids[1] = (ui_draw_index)(index+ i - 1);
            ids[2] = (ui_draw_index)(index+i);
            ids += 3;
        }
    }
}

UI_API void
ui_draw_list_path_clear(struct ui_draw_list *list)
{
    UI_ASSERT(list);
    if (!list) return;
    ui_buffer_reset(list->buffer, UI_BUFFER_FRONT);
    list->path_count = 0;
    list->path_offset = 0;
}

UI_API void
ui_draw_list_path_line_to(struct ui_draw_list *list, struct ui_vec2 pos)
{
    struct ui_vec2 *points = 0;
    struct ui_draw_command *cmd = 0;
    UI_ASSERT(list);
    if (!list) return;
    if (!list->cmd_count)
        ui_draw_list_add_clip(list, ui_null_rect);

    cmd = ui_draw_list_command_last(list);
    if (cmd && cmd->texture.ptr != list->config.null.texture.ptr)
        ui_draw_list_push_image(list, list->config.null.texture);

    points = ui_draw_list_alloc_path(list, 1);
    if (!points) return;
    points[0] = pos;
}

UI_API void
ui_draw_list_path_arc_to_fast(struct ui_draw_list *list, struct ui_vec2 center,
    float radius, int a_min, int a_max)
{
    int a = 0;
    UI_ASSERT(list);
    if (!list) return;
    if (a_min <= a_max) {
        for (a = a_min; a <= a_max; a++) {
            const struct ui_vec2 c = list->circle_vtx[(ui_size)a % UI_LEN(list->circle_vtx)];
            const float x = center.x + c.x * radius;
            const float y = center.y + c.y * radius;
            ui_draw_list_path_line_to(list, ui_vec2(x, y));
        }
    }
}

UI_API void
ui_draw_list_path_arc_to(struct ui_draw_list *list, struct ui_vec2 center,
    float radius, float a_min, float a_max, unsigned int segments)
{
    unsigned int i = 0;
    UI_ASSERT(list);
    if (!list) return;
    if (radius == 0.0f) return;
    for (i = 0; i <= segments; ++i) {
        const float a = a_min + ((float)i / ((float)segments) * (a_max - a_min));
        const float x = center.x + (float)UI_COS(a) * radius;
        const float y = center.y + (float)UI_SIN(a) * radius;
        ui_draw_list_path_line_to(list, ui_vec2(x, y));
    }
}

UI_API void
ui_draw_list_path_rect_to(struct ui_draw_list *list, struct ui_vec2 a,
    struct ui_vec2 b, float rounding)
{
    float r;
    UI_ASSERT(list);
    if (!list) return;
    r = rounding;
    r = UI_MIN(r, ((b.x-a.x) < 0) ? -(b.x-a.x): (b.x-a.x));
    r = UI_MIN(r, ((b.y-a.y) < 0) ? -(b.y-a.y): (b.y-a.y));

    if (r == 0.0f) {
        ui_draw_list_path_line_to(list, a);
        ui_draw_list_path_line_to(list, ui_vec2(b.x,a.y));
        ui_draw_list_path_line_to(list, b);
        ui_draw_list_path_line_to(list, ui_vec2(a.x,b.y));
    } else {
        ui_draw_list_path_arc_to_fast(list, ui_vec2(a.x + r, a.y + r), r, 6, 9);
        ui_draw_list_path_arc_to_fast(list, ui_vec2(b.x - r, a.y + r), r, 9, 12);
        ui_draw_list_path_arc_to_fast(list, ui_vec2(b.x - r, b.y - r), r, 0, 3);
        ui_draw_list_path_arc_to_fast(list, ui_vec2(a.x + r, b.y - r), r, 3, 6);
    }
}

UI_API void
ui_draw_list_path_curve_to(struct ui_draw_list *list, struct ui_vec2 p2,
    struct ui_vec2 p3, struct ui_vec2 p4, unsigned int num_segments)
{
    float t_step;
    unsigned int i_step;
    struct ui_vec2 p1;

    UI_ASSERT(list);
    UI_ASSERT(list->path_count);
    if (!list || !list->path_count) return;
    num_segments = UI_MAX(num_segments, 1);

    p1 = ui_draw_list_path_last(list);
    t_step = 1.0f/(float)num_segments;
    for (i_step = 1; i_step <= num_segments; ++i_step) {
        float t = t_step * (float)i_step;
        float u = 1.0f - t;
        float w1 = u*u*u;
        float w2 = 3*u*u*t;
        float w3 = 3*u*t*t;
        float w4 = t * t *t;
        float x = w1 * p1.x + w2 * p2.x + w3 * p3.x + w4 * p4.x;
        float y = w1 * p1.y + w2 * p2.y + w3 * p3.y + w4 * p4.y;
        ui_draw_list_path_line_to(list, ui_vec2(x,y));
    }
}

UI_API void
ui_draw_list_path_fill(struct ui_draw_list *list, struct ui_color color)
{
    struct ui_vec2 *points;
    UI_ASSERT(list);
    if (!list) return;
    points = (struct ui_vec2*)ui_buffer_memory(list->buffer);
    ui_draw_list_fill_poly_convex(list, points, list->path_count, color, list->config.shape_AA);
    ui_draw_list_path_clear(list);
}

UI_API void
ui_draw_list_path_stroke(struct ui_draw_list *list, struct ui_color color,
    enum ui_draw_list_stroke closed, float thickness)
{
    struct ui_vec2 *points;
    UI_ASSERT(list);
    if (!list) return;
    points = (struct ui_vec2*)ui_buffer_memory(list->buffer);
    ui_draw_list_stroke_poly_line(list, points, list->path_count, color,
        closed, thickness, list->config.line_AA);
    ui_draw_list_path_clear(list);
}

UI_API void
ui_draw_list_stroke_line(struct ui_draw_list *list, struct ui_vec2 a,
    struct ui_vec2 b, struct ui_color col, float thickness)
{
    UI_ASSERT(list);
    if (!list || !col.a) return;
    ui_draw_list_path_line_to(list, ui_vec2_add(a, ui_vec2(0.5f, 0.5f)));
    ui_draw_list_path_line_to(list, ui_vec2_add(b, ui_vec2(0.5f, 0.5f)));
    ui_draw_list_path_stroke(list,  col, UI_STROKE_OPEN, thickness);
}

UI_API void
ui_draw_list_fill_rect(struct ui_draw_list *list, struct ui_rect rect,
    struct ui_color col, float rounding)
{
    UI_ASSERT(list);
    if (!list || !col.a) return;
    ui_draw_list_path_rect_to(list, ui_vec2(rect.x + 0.5f, rect.y + 0.5f),
        ui_vec2(rect.x + rect.w + 0.5f, rect.y + rect.h + 0.5f), rounding);
    ui_draw_list_path_fill(list,  col);
}

UI_API void
ui_draw_list_stroke_rect(struct ui_draw_list *list, struct ui_rect rect,
    struct ui_color col, float rounding, float thickness)
{
    UI_ASSERT(list);
    if (!list || !col.a) return;
    ui_draw_list_path_rect_to(list, ui_vec2(rect.x + 0.5f, rect.y + 0.5f),
        ui_vec2(rect.x + rect.w + 0.5f, rect.y + rect.h + 0.5f), rounding);
    ui_draw_list_path_stroke(list,  col, UI_STROKE_CLOSED, thickness);
}

UI_API void
ui_draw_list_fill_rect_multi_color(struct ui_draw_list *list, struct ui_rect rect,
    struct ui_color left, struct ui_color top, struct ui_color right,
    struct ui_color bottom)
{
    void *vtx;
    struct ui_colorf col_left, col_top;
    struct ui_colorf col_right, col_bottom;
    ui_draw_index *idx;
    ui_draw_index index;

    ui_color_fv(&col_left.r, left);
    ui_color_fv(&col_right.r, right);
    ui_color_fv(&col_top.r, top);
    ui_color_fv(&col_bottom.r, bottom);

    UI_ASSERT(list);
    if (!list) return;

    ui_draw_list_push_image(list, list->config.null.texture);
    index = (ui_draw_index)list->vertex_count;
    vtx = ui_draw_list_alloc_vertices(list, 4);
    idx = ui_draw_list_alloc_elements(list, 6);
    if (!vtx || !idx) return;

    idx[0] = (ui_draw_index)(index+0); idx[1] = (ui_draw_index)(index+1);
    idx[2] = (ui_draw_index)(index+2); idx[3] = (ui_draw_index)(index+0);
    idx[4] = (ui_draw_index)(index+2); idx[5] = (ui_draw_index)(index+3);

    vtx = ui_draw_vertex(vtx, &list->config, ui_vec2(rect.x, rect.y), list->config.null.uv, col_left);
    vtx = ui_draw_vertex(vtx, &list->config, ui_vec2(rect.x + rect.w, rect.y), list->config.null.uv, col_top);
    vtx = ui_draw_vertex(vtx, &list->config, ui_vec2(rect.x + rect.w, rect.y + rect.h), list->config.null.uv, col_right);
    vtx = ui_draw_vertex(vtx, &list->config, ui_vec2(rect.x, rect.y + rect.h), list->config.null.uv, col_bottom);
}

UI_API void
ui_draw_list_fill_triangle(struct ui_draw_list *list, struct ui_vec2 a,
    struct ui_vec2 b, struct ui_vec2 c, struct ui_color col)
{
    UI_ASSERT(list);
    if (!list || !col.a) return;
    ui_draw_list_path_line_to(list, a);
    ui_draw_list_path_line_to(list, b);
    ui_draw_list_path_line_to(list, c);
    ui_draw_list_path_fill(list, col);
}

UI_API void
ui_draw_list_stroke_triangle(struct ui_draw_list *list, struct ui_vec2 a,
    struct ui_vec2 b, struct ui_vec2 c, struct ui_color col, float thickness)
{
    UI_ASSERT(list);
    if (!list || !col.a) return;
    ui_draw_list_path_line_to(list, a);
    ui_draw_list_path_line_to(list, b);
    ui_draw_list_path_line_to(list, c);
    ui_draw_list_path_stroke(list, col, UI_STROKE_CLOSED, thickness);
}

UI_API void
ui_draw_list_fill_circle(struct ui_draw_list *list, struct ui_vec2 center,
    float radius, struct ui_color col, unsigned int segs)
{
    float a_max;
    UI_ASSERT(list);
    if (!list || !col.a) return;
    a_max = UI_PI * 2.0f * ((float)segs - 1.0f) / (float)segs;
    ui_draw_list_path_arc_to(list, center, radius, 0.0f, a_max, segs);
    ui_draw_list_path_fill(list, col);
}

UI_API void
ui_draw_list_stroke_circle(struct ui_draw_list *list, struct ui_vec2 center,
    float radius, struct ui_color col, unsigned int segs, float thickness)
{
    float a_max;
    UI_ASSERT(list);
    if (!list || !col.a) return;
    a_max = UI_PI * 2.0f * ((float)segs - 1.0f) / (float)segs;
    ui_draw_list_path_arc_to(list, center, radius, 0.0f, a_max, segs);
    ui_draw_list_path_stroke(list, col, UI_STROKE_CLOSED, thickness);
}

UI_API void
ui_draw_list_stroke_curve(struct ui_draw_list *list, struct ui_vec2 p0,
    struct ui_vec2 cp0, struct ui_vec2 cp1, struct ui_vec2 p1,
    struct ui_color col, unsigned int segments, float thickness)
{
    UI_ASSERT(list);
    if (!list || !col.a) return;
    ui_draw_list_path_line_to(list, p0);
    ui_draw_list_path_curve_to(list, cp0, cp1, p1, segments);
    ui_draw_list_path_stroke(list, col, UI_STROKE_OPEN, thickness);
}

UI_INTERN void
ui_draw_list_push_rect_uv(struct ui_draw_list *list, struct ui_vec2 a,
    struct ui_vec2 c, struct ui_vec2 uva, struct ui_vec2 uvc,
    struct ui_color color)
{
    void *vtx;
    struct ui_vec2 uvb;
    struct ui_vec2 uvd;
    struct ui_vec2 b;
    struct ui_vec2 d;

    struct ui_colorf col;
    ui_draw_index *idx;
    ui_draw_index index;
    UI_ASSERT(list);
    if (!list) return;

    ui_color_fv(&col.r, color);
    uvb = ui_vec2(uvc.x, uva.y);
    uvd = ui_vec2(uva.x, uvc.y);
    b = ui_vec2(c.x, a.y);
    d = ui_vec2(a.x, c.y);

    index = (ui_draw_index)list->vertex_count;
    vtx = ui_draw_list_alloc_vertices(list, 4);
    idx = ui_draw_list_alloc_elements(list, 6);
    if (!vtx || !idx) return;

    idx[0] = (ui_draw_index)(index+0); idx[1] = (ui_draw_index)(index+1);
    idx[2] = (ui_draw_index)(index+2); idx[3] = (ui_draw_index)(index+0);
    idx[4] = (ui_draw_index)(index+2); idx[5] = (ui_draw_index)(index+3);

    vtx = ui_draw_vertex(vtx, &list->config, a, uva, col);
    vtx = ui_draw_vertex(vtx, &list->config, b, uvb, col);
    vtx = ui_draw_vertex(vtx, &list->config, c, uvc, col);
    vtx = ui_draw_vertex(vtx, &list->config, d, uvd, col);
}

UI_API void
ui_draw_list_add_image(struct ui_draw_list *list, struct ui_image texture,
    struct ui_rect rect, struct ui_color color)
{
    UI_ASSERT(list);
    if (!list) return;
    /* push new command with given texture */
    ui_draw_list_push_image(list, texture.handle);
    if (ui_image_is_subimage(&texture)) {
        /* add region inside of the texture  */
        struct ui_vec2 uv[2];
        uv[0].x = (float)texture.region[0]/(float)texture.w;
        uv[0].y = (float)texture.region[1]/(float)texture.h;
        uv[1].x = (float)(texture.region[0] + texture.region[2])/(float)texture.w;
        uv[1].y = (float)(texture.region[1] + texture.region[3])/(float)texture.h;
        ui_draw_list_push_rect_uv(list, ui_vec2(rect.x, rect.y),
            ui_vec2(rect.x + rect.w, rect.y + rect.h),  uv[0], uv[1], color);
    } else ui_draw_list_push_rect_uv(list, ui_vec2(rect.x, rect.y),
            ui_vec2(rect.x + rect.w, rect.y + rect.h),
            ui_vec2(0.0f, 0.0f), ui_vec2(1.0f, 1.0f),color);
}

UI_API void
ui_draw_list_add_text(struct ui_draw_list *list, const struct ui_user_font *font,
    struct ui_rect rect, const char *text, int len, float font_height,
    struct ui_color fg)
{
    float x = 0;
    int text_len = 0;
    ui_rune unicode = 0;
    ui_rune next = 0;
    int glyph_len = 0;
    int next_glyph_len = 0;
    struct ui_user_font_glyph g;

    UI_ASSERT(list);
    if (!list || !len || !text) return;
    if (!UI_INTERSECT(rect.x, rect.y, rect.w, rect.h,
        list->clip_rect.x, list->clip_rect.y, list->clip_rect.w, list->clip_rect.h)) return;

    ui_draw_list_push_image(list, font->texture);
    x = rect.x;
    glyph_len = ui_utf_decode(text, &unicode, len);
    if (!glyph_len) return;

    /* draw every glyph image */
    fg.a = (ui_byte)((float)fg.a * list->config.global_alpha);
    while (text_len < len && glyph_len) {
        float gx, gy, gh, gw;
        float char_width = 0;
        if (unicode == UI_UTF_INVALID) break;

        /* query currently drawn glyph information */
        next_glyph_len = ui_utf_decode(text + text_len + glyph_len, &next, (int)len - text_len);
        font->query(font->userdata, font_height, &g, unicode,
                    (next == UI_UTF_INVALID) ? '\0' : next);

        /* calculate and draw glyph drawing rectangle and image */
        gx = x + g.offset.x;
        gy = rect.y + g.offset.y;
        gw = g.width; gh = g.height;
        char_width = g.xadvance;
        ui_draw_list_push_rect_uv(list, ui_vec2(gx,gy), ui_vec2(gx + gw, gy+ gh),
            g.uv[0], g.uv[1], fg);

        /* offset next glyph */
        text_len += glyph_len;
        x += char_width;
        glyph_len = next_glyph_len;
        unicode = next;
    }
}

UI_API void
ui_convert(struct ui_context *ctx, struct ui_buffer *cmds,
    struct ui_buffer *vertices, struct ui_buffer *elements,
    const struct ui_convert_config *config)
{
    const struct ui_command *cmd;
    UI_ASSERT(ctx);
    UI_ASSERT(cmds);
    UI_ASSERT(vertices);
    UI_ASSERT(elements);
    UI_ASSERT(config);
    UI_ASSERT(config->vertex_layout);
    UI_ASSERT(config->vertex_size);
    if (!ctx || !cmds || !vertices || !elements || !config || !config->vertex_layout)
        return;

    ui_draw_list_setup(&ctx->draw_list, config, cmds, vertices, elements);
    ui_foreach(cmd, ctx)
    {
#ifdef UI_INCLUDE_COMMAND_USERDATA
        ctx->draw_list.userdata = cmd->userdata;
#endif
        switch (cmd->type) {
        case UI_COMMAND_NOP: break;
        case UI_COMMAND_SCISSOR: {
            const struct ui_command_scissor *s = (const struct ui_command_scissor*)cmd;
            ui_draw_list_add_clip(&ctx->draw_list, ui_rect(s->x, s->y, s->w, s->h));
        } break;
        case UI_COMMAND_LINE: {
            const struct ui_command_line *l = (const struct ui_command_line*)cmd;
            ui_draw_list_stroke_line(&ctx->draw_list, ui_vec2(l->begin.x, l->begin.y),
                ui_vec2(l->end.x, l->end.y), l->color, l->line_thickness);
        } break;
        case UI_COMMAND_CURVE: {
            const struct ui_command_curve *q = (const struct ui_command_curve*)cmd;
            ui_draw_list_stroke_curve(&ctx->draw_list, ui_vec2(q->begin.x, q->begin.y),
                ui_vec2(q->ctrl[0].x, q->ctrl[0].y), ui_vec2(q->ctrl[1].x,
                q->ctrl[1].y), ui_vec2(q->end.x, q->end.y), q->color,
                config->curve_segment_count, q->line_thickness);
        } break;
        case UI_COMMAND_RECT: {
            const struct ui_command_rect *r = (const struct ui_command_rect*)cmd;
            ui_draw_list_stroke_rect(&ctx->draw_list, ui_rect(r->x, r->y, r->w, r->h),
                r->color, (float)r->rounding, r->line_thickness);
        } break;
        case UI_COMMAND_RECT_FILLED: {
            const struct ui_command_rect_filled *r = (const struct ui_command_rect_filled*)cmd;
            ui_draw_list_fill_rect(&ctx->draw_list, ui_rect(r->x, r->y, r->w, r->h),
                r->color, (float)r->rounding);
        } break;
        case UI_COMMAND_RECT_MULTI_COLOR: {
            const struct ui_command_rect_multi_color *r = (const struct ui_command_rect_multi_color*)cmd;
            ui_draw_list_fill_rect_multi_color(&ctx->draw_list, ui_rect(r->x, r->y, r->w, r->h),
                r->left, r->top, r->right, r->bottom);
        } break;
        case UI_COMMAND_CIRCLE: {
            const struct ui_command_circle *c = (const struct ui_command_circle*)cmd;
            ui_draw_list_stroke_circle(&ctx->draw_list, ui_vec2((float)c->x + (float)c->w/2,
                (float)c->y + (float)c->h/2), (float)c->w/2, c->color,
                config->circle_segment_count, c->line_thickness);
        } break;
        case UI_COMMAND_CIRCLE_FILLED: {
            const struct ui_command_circle_filled *c = (const struct ui_command_circle_filled *)cmd;
            ui_draw_list_fill_circle(&ctx->draw_list, ui_vec2((float)c->x + (float)c->w/2,
                (float)c->y + (float)c->h/2), (float)c->w/2, c->color,
                config->circle_segment_count);
        } break;
        case UI_COMMAND_ARC: {
            const struct ui_command_arc *c = (const struct ui_command_arc*)cmd;
            ui_draw_list_path_line_to(&ctx->draw_list, ui_vec2(c->cx, c->cy));
            ui_draw_list_path_arc_to(&ctx->draw_list, ui_vec2(c->cx, c->cy), c->r,
                c->a[0], c->a[1], config->arc_segment_count);
            ui_draw_list_path_stroke(&ctx->draw_list, c->color, UI_STROKE_CLOSED, c->line_thickness);
        } break;
        case UI_COMMAND_ARC_FILLED: {
            const struct ui_command_arc_filled *c = (const struct ui_command_arc_filled*)cmd;
            ui_draw_list_path_line_to(&ctx->draw_list, ui_vec2(c->cx, c->cy));
            ui_draw_list_path_arc_to(&ctx->draw_list, ui_vec2(c->cx, c->cy), c->r,
                c->a[0], c->a[1], config->arc_segment_count);
            ui_draw_list_path_fill(&ctx->draw_list, c->color);
        } break;
        case UI_COMMAND_TRIANGLE: {
            const struct ui_command_triangle *t = (const struct ui_command_triangle*)cmd;
            ui_draw_list_stroke_triangle(&ctx->draw_list, ui_vec2(t->a.x, t->a.y),
                ui_vec2(t->b.x, t->b.y), ui_vec2(t->c.x, t->c.y), t->color,
                t->line_thickness);
        } break;
        case UI_COMMAND_TRIANGLE_FILLED: {
            const struct ui_command_triangle_filled *t = (const struct ui_command_triangle_filled*)cmd;
            ui_draw_list_fill_triangle(&ctx->draw_list, ui_vec2(t->a.x, t->a.y),
                ui_vec2(t->b.x, t->b.y), ui_vec2(t->c.x, t->c.y), t->color);
        } break;
        case UI_COMMAND_POLYGON: {
            int i;
            const struct ui_command_polygon*p = (const struct ui_command_polygon*)cmd;
            for (i = 0; i < p->point_count; ++i) {
                struct ui_vec2 pnt = ui_vec2((float)p->points[i].x, (float)p->points[i].y);
                ui_draw_list_path_line_to(&ctx->draw_list, pnt);
            }
            ui_draw_list_path_stroke(&ctx->draw_list, p->color, UI_STROKE_CLOSED, p->line_thickness);
        } break;
        case UI_COMMAND_POLYGON_FILLED: {
            int i;
            const struct ui_command_polygon_filled *p = (const struct ui_command_polygon_filled*)cmd;
            for (i = 0; i < p->point_count; ++i) {
                struct ui_vec2 pnt = ui_vec2((float)p->points[i].x, (float)p->points[i].y);
                ui_draw_list_path_line_to(&ctx->draw_list, pnt);
            }
            ui_draw_list_path_fill(&ctx->draw_list, p->color);
        } break;
        case UI_COMMAND_POLYLINE: {
            int i;
            const struct ui_command_polyline *p = (const struct ui_command_polyline*)cmd;
            for (i = 0; i < p->point_count; ++i) {
                struct ui_vec2 pnt = ui_vec2((float)p->points[i].x, (float)p->points[i].y);
                ui_draw_list_path_line_to(&ctx->draw_list, pnt);
            }
            ui_draw_list_path_stroke(&ctx->draw_list, p->color, UI_STROKE_OPEN, p->line_thickness);
        } break;
        case UI_COMMAND_TEXT: {
            const struct ui_command_text *t = (const struct ui_command_text*)cmd;
            ui_draw_list_add_text(&ctx->draw_list, t->font, ui_rect(t->x, t->y, t->w, t->h),
                t->string, t->length, t->height, t->foreground);
        } break;
        case UI_COMMAND_IMAGE: {
            const struct ui_command_image *i = (const struct ui_command_image*)cmd;
            ui_draw_list_add_image(&ctx->draw_list, i->img, ui_rect(i->x, i->y, i->w, i->h), i->col);
        } break;
        default: break;
        }
    }
}

UI_API const struct ui_draw_command*
ui__draw_begin(const struct ui_context *ctx,
    const struct ui_buffer *buffer)
{return ui__draw_list_begin(&ctx->draw_list, buffer);}

UI_API const struct ui_draw_command*
ui__draw_end(const struct ui_context *ctx, const struct ui_buffer *buffer)
{return ui__draw_list_end(&ctx->draw_list, buffer);}

UI_API const struct ui_draw_command*
ui__draw_next(const struct ui_draw_command *cmd,
    const struct ui_buffer *buffer, const struct ui_context *ctx)
{return ui__draw_list_next(cmd, buffer, &ctx->draw_list);}

#endif

/*
 * ==============================================================
 *
 *                          FONT HANDLING
 *
 * ===============================================================
 */
#ifdef UI_INCLUDE_FONT_BAKING
/* -------------------------------------------------------------
 *
 *                          RECT PACK
 *
 * --------------------------------------------------------------*/
/* stb_rect_pack.h - v0.05 - public domain - rectangle packing */
/* Sean Barrett 2014 */
#define UI_RP__MAXVAL  0xffff
typedef unsigned short ui_rp_coord;

struct ui_rp_rect {
    /* reserved for your use: */
    int id;
    /* input: */
    ui_rp_coord w, h;
    /* output: */
    ui_rp_coord x, y;
    int was_packed;
    /* non-zero if valid packing */
}; /* 16 bytes, nominally */

struct ui_rp_node {
    ui_rp_coord  x,y;
    struct ui_rp_node  *next;
};

struct ui_rp_context {
    int width;
    int height;
    int align;
    int init_mode;
    int heuristic;
    int num_nodes;
    struct ui_rp_node *active_head;
    struct ui_rp_node *free_head;
    struct ui_rp_node extra[2];
    /* we allocate two extra nodes so optimal user-node-count is 'width' not 'width+2' */
};

struct ui_rp__findresult {
    int x,y;
    struct ui_rp_node **prev_liui;
};

enum UI_RP_HEURISTIC {
    UI_RP_HEURISTIC_Skyline_default=0,
    UI_RP_HEURISTIC_Skyline_BL_sortHeight = UI_RP_HEURISTIC_Skyline_default,
    UI_RP_HEURISTIC_Skyline_BF_sortHeight
};
enum UI_RP_INIT_STATE{UI_RP__INIT_skyline = 1};

UI_INTERN void
ui_rp_setup_allow_out_of_mem(struct ui_rp_context *context, int allow_out_of_mem)
{
    if (allow_out_of_mem)
        /* if it's ok to run out of memory, then don't bother aligning them; */
        /* this gives better packing, but may fail due to OOM (even though */
        /* the rectangles easily fit). @TODO a smarter approach would be to only */
        /* quantize once we've hit OOM, then we could get rid of this parameter. */
        context->align = 1;
    else {
        /* if it's not ok to run out of memory, then quantize the widths */
        /* so that num_nodes is always enough nodes. */
        /* */
        /* I.e. num_nodes * align >= width */
        /*                  align >= width / num_nodes */
        /*                  align = ceil(width/num_nodes) */
        context->align = (context->width + context->num_nodes-1) / context->num_nodes;
    }
}

UI_INTERN void
ui_rp_init_target(struct ui_rp_context *context, int width, int height,
    struct ui_rp_node *nodes, int num_nodes)
{
    int i;
#ifndef STBRP_LARGE_RECTS
    UI_ASSERT(width <= 0xffff && height <= 0xffff);
#endif

    for (i=0; i < num_nodes-1; ++i)
        nodes[i].next = &nodes[i+1];
    nodes[i].next = 0;
    context->init_mode = UI_RP__INIT_skyline;
    context->heuristic = UI_RP_HEURISTIC_Skyline_default;
    context->free_head = &nodes[0];
    context->active_head = &context->extra[0];
    context->width = width;
    context->height = height;
    context->num_nodes = num_nodes;
    ui_rp_setup_allow_out_of_mem(context, 0);

    /* node 0 is the full width, node 1 is the sentinel (lets us not store width explicitly) */
    context->extra[0].x = 0;
    context->extra[0].y = 0;
    context->extra[0].next = &context->extra[1];
    context->extra[1].x = (ui_rp_coord) width;
    context->extra[1].y = 65535;
    context->extra[1].next = 0;
}

/* find minimum y position if it starts at x1 */
UI_INTERN int
ui_rp__skyline_find_min_y(struct ui_rp_context *c, struct ui_rp_node *first,
    int x0, int width, int *pwaste)
{
    struct ui_rp_node *node = first;
    int x1 = x0 + width;
    int min_y, visited_width, waste_area;
    UI_ASSERT(first->x <= x0);
    UI_UNUSED(c);

    UI_ASSERT(node->next->x > x0);
    /* we ended up handling this in the caller for efficiency */
    UI_ASSERT(node->x <= x0);

    min_y = 0;
    waste_area = 0;
    visited_width = 0;
    while (node->x < x1)
    {
        if (node->y > min_y) {
            /* raise min_y higher. */
            /* we've accounted for all waste up to min_y, */
            /* but we'll now add more waste for everything we've visited */
            waste_area += visited_width * (node->y - min_y);
            min_y = node->y;
            /* the first time through, visited_width might be reduced */
            if (node->x < x0)
            visited_width += node->next->x - x0;
            else
            visited_width += node->next->x - node->x;
        } else {
            /* add waste area */
            int under_width = node->next->x - node->x;
            if (under_width + visited_width > width)
            under_width = width - visited_width;
            waste_area += under_width * (min_y - node->y);
            visited_width += under_width;
        }
        node = node->next;
    }
    *pwaste = waste_area;
    return min_y;
}

UI_INTERN struct ui_rp__findresult
ui_rp__skyline_find_best_pos(struct ui_rp_context *c, int width, int height)
{
    int best_waste = (1<<30), best_x, best_y = (1 << 30);
    struct ui_rp__findresult fr;
    struct ui_rp_node **prev, *node, *tail, **best = 0;

    /* align to multiple of c->align */
    width = (width + c->align - 1);
    width -= width % c->align;
    UI_ASSERT(width % c->align == 0);

    node = c->active_head;
    prev = &c->active_head;
    while (node->x + width <= c->width) {
        int y,waste;
        y = ui_rp__skyline_find_min_y(c, node, node->x, width, &waste);
        /* actually just want to test BL */
        if (c->heuristic == UI_RP_HEURISTIC_Skyline_BL_sortHeight) {
            /* bottom left */
            if (y < best_y) {
            best_y = y;
            best = prev;
            }
        } else {
            /* best-fit */
            if (y + height <= c->height) {
                /* can only use it if it first vertically */
                if (y < best_y || (y == best_y && waste < best_waste)) {
                    best_y = y;
                    best_waste = waste;
                    best = prev;
                }
            }
        }
        prev = &node->next;
        node = node->next;
    }
    best_x = (best == 0) ? 0 : (*best)->x;

    /* if doing best-fit (BF), we also have to try aligning right edge to each node position */
    /* */
    /* e.g, if fitting */
    /* */
    /*     ____________________ */
    /*    |____________________| */
    /* */
    /*            into */
    /* */
    /*   |                         | */
    /*   |             ____________| */
    /*   |____________| */
    /* */
    /* then right-aligned reduces waste, but bottom-left BL is always chooses left-aligned */
    /* */
    /* This makes BF take about 2x the time */
    if (c->heuristic == UI_RP_HEURISTIC_Skyline_BF_sortHeight)
    {
        tail = c->active_head;
        node = c->active_head;
        prev = &c->active_head;
        /* find first node that's admissible */
        while (tail->x < width)
            tail = tail->next;
        while (tail)
        {
            int xpos = tail->x - width;
            int y,waste;
            UI_ASSERT(xpos >= 0);
            /* find the left position that matches this */
            while (node->next->x <= xpos) {
                prev = &node->next;
                node = node->next;
            }
            UI_ASSERT(node->next->x > xpos && node->x <= xpos);
            y = ui_rp__skyline_find_min_y(c, node, xpos, width, &waste);
            if (y + height < c->height) {
                if (y <= best_y) {
                    if (y < best_y || waste < best_waste || (waste==best_waste && xpos < best_x)) {
                        best_x = xpos;
                        UI_ASSERT(y <= best_y);
                        best_y = y;
                        best_waste = waste;
                        best = prev;
                    }
                }
            }
            tail = tail->next;
        }
    }
    fr.prev_liui = best;
    fr.x = best_x;
    fr.y = best_y;
    return fr;
}

UI_INTERN struct ui_rp__findresult
ui_rp__skyline_pack_rectangle(struct ui_rp_context *context, int width, int height)
{
    /* find best position according to heuristic */
    struct ui_rp__findresult res = ui_rp__skyline_find_best_pos(context, width, height);
    struct ui_rp_node *node, *cur;

    /* bail if: */
    /*    1. it failed */
    /*    2. the best node doesn't fit (we don't always check this) */
    /*    3. we're out of memory */
    if (res.prev_liui == 0 || res.y + height > context->height || context->free_head == 0) {
        res.prev_liui = 0;
        return res;
    }

    /* on success, create new node */
    node = context->free_head;
    node->x = (ui_rp_coord) res.x;
    node->y = (ui_rp_coord) (res.y + height);

    context->free_head = node->next;

    /* insert the new node into the right starting point, and */
    /* let 'cur' point to the remaining nodes needing to be */
    /* stitched back in */
    cur = *res.prev_liui;
    if (cur->x < res.x) {
        /* preserve the existing one, so start testing with the next one */
        struct ui_rp_node *next = cur->next;
        cur->next = node;
        cur = next;
    } else {
        *res.prev_liui = node;
    }

    /* from here, traverse cur and free the nodes, until we get to one */
    /* that shouldn't be freed */
    while (cur->next && cur->next->x <= res.x + width) {
        struct ui_rp_node *next = cur->next;
        /* move the current node to the free list */
        cur->next = context->free_head;
        context->free_head = cur;
        cur = next;
    }
    /* stitch the list back in */
    node->next = cur;

    if (cur->x < res.x + width)
        cur->x = (ui_rp_coord) (res.x + width);
    return res;
}

UI_INTERN int
ui_rect_height_compare(const void *a, const void *b)
{
    const struct ui_rp_rect *p = (const struct ui_rp_rect *) a;
    const struct ui_rp_rect *q = (const struct ui_rp_rect *) b;
    if (p->h > q->h)
        return -1;
    if (p->h < q->h)
        return  1;
    return (p->w > q->w) ? -1 : (p->w < q->w);
}

UI_INTERN int
ui_rect_original_order(const void *a, const void *b)
{
    const struct ui_rp_rect *p = (const struct ui_rp_rect *) a;
    const struct ui_rp_rect *q = (const struct ui_rp_rect *) b;
    return (p->was_packed < q->was_packed) ? -1 : (p->was_packed > q->was_packed);
}

static void
ui_rp_qsort(struct ui_rp_rect *array, unsigned int len, int(*cmp)(const void*,const void*))
{
    /* iterative quick sort */
    #define UI_MAX_SORT_STACK 64
    unsigned right, left = 0, stack[UI_MAX_SORT_STACK], pos = 0;
    unsigned seed = len/2 * 69069+1;
    for (;;) {
        for (; left+1 < len; len++) {
            struct ui_rp_rect pivot, tmp;
            if (pos == UI_MAX_SORT_STACK) len = stack[pos = 0];
            pivot = array[left+seed%(len-left)];
            seed = seed * 69069 + 1;
            stack[pos++] = len;
            for (right = left-1;;) {
                while (cmp(&array[++right], &pivot) < 0);
                while (cmp(&pivot, &array[--len]) < 0);
                if (right >= len) break;
                tmp = array[right];
                array[right] = array[len];
                array[len] = tmp;
            }
        }
        if (pos == 0) break;
        left = len;
        len = stack[--pos];
    }
    #undef UI_MAX_SORT_STACK
}

UI_INTERN void
ui_rp_pack_rects(struct ui_rp_context *context, struct ui_rp_rect *rects, int num_rects)
{
    int i;
    /* we use the 'was_packed' field internally to allow sorting/unsorting */
    for (i=0; i < num_rects; ++i) {
        rects[i].was_packed = i;
    }

    /* sort according to heuristic */
    ui_rp_qsort(rects, (unsigned)num_rects, ui_rect_height_compare);

    for (i=0; i < num_rects; ++i) {
        struct ui_rp__findresult fr = ui_rp__skyline_pack_rectangle(context, rects[i].w, rects[i].h);
        if (fr.prev_liui) {
            rects[i].x = (ui_rp_coord) fr.x;
            rects[i].y = (ui_rp_coord) fr.y;
        } else {
            rects[i].x = rects[i].y = UI_RP__MAXVAL;
        }
    }

    /* unsort */
    ui_rp_qsort(rects, (unsigned)num_rects, ui_rect_original_order);

    /* set was_packed flags */
    for (i=0; i < num_rects; ++i)
        rects[i].was_packed = !(rects[i].x == UI_RP__MAXVAL && rects[i].y == UI_RP__MAXVAL);
}

/*
 * ==============================================================
 *
 *                          TRUETYPE
 *
 * ===============================================================
 */
/* stb_truetype.h - v1.07 - public domain */
#define UI_TT_MAX_OVERSAMPLE   8
#define UI_TT__OVER_MASK  (UI_TT_MAX_OVERSAMPLE-1)

struct ui_tt_bakedchar {
    unsigned short x0,y0,x1,y1;
    /* coordinates of bbox in bitmap */
    float xoff,yoff,xadvance;
};

struct ui_tt_aligned_quad{
    float x0,y0,s0,t0; /* top-left */
    float x1,y1,s1,t1; /* bottom-right */
};

struct ui_tt_packedchar {
    unsigned short x0,y0,x1,y1;
    /* coordinates of bbox in bitmap */
    float xoff,yoff,xadvance;
    float xoff2,yoff2;
};

struct ui_tt_pack_range {
    float font_size;
    int first_unicode_codepoint_in_range;
    /* if non-zero, then the chars are continuous, and this is the first codepoint */
    int *array_of_unicode_codepoints;
    /* if non-zero, then this is an array of unicode codepoints */
    int num_chars;
    struct ui_tt_packedchar *chardata_for_range; /* output */
    unsigned char h_oversample, v_oversample;
    /* don't set these, they're used internally */
};

struct ui_tt_pack_context {
    void *pack_info;
    int   width;
    int   height;
    int   stride_in_bytes;
    int   padding;
    unsigned int   h_oversample, v_oversample;
    unsigned char *pixels;
    void  *nodes;
};

struct ui_tt_fontinfo {
    const unsigned char* data; /* pointer to .ttf file */
    int fontstart;/* offset of start of font */
    int numGlyphs;/* number of glyphs, needed for range checking */
    int loca,head,glyf,hhea,hmtx,kern; /* table locations as offset from start of .ttf */
    int index_map; /* a cmap mapping for our chosen character encoding */
    int indexToLocFormat; /* format needed to map from glyph index to glyph */
};

enum {
  UI_TT_vmove=1,
  UI_TT_vline,
  UI_TT_vcurve
};

struct ui_tt_vertex {
    short x,y,cx,cy;
    unsigned char type,padding;
};

struct ui_tt__bitmap{
   int w,h,stride;
   unsigned char *pixels;
};

struct ui_tt__hheap_chuui {
    struct ui_tt__hheap_chuui *next;
};
struct ui_tt__hheap {
    struct ui_allocator alloc;
    struct ui_tt__hheap_chuui *head;
    void   *first_free;
    int    num_remaining_in_head_chuui;
};

struct ui_tt__edge {
    float x0,y0, x1,y1;
    int invert;
};

struct ui_tt__active_edge {
    struct ui_tt__active_edge *next;
    float fx,fdx,fdy;
    float direction;
    float sy;
    float ey;
};
struct ui_tt__point {float x,y;};

#define UI_TT_MACSTYLE_DONTCARE     0
#define UI_TT_MACSTYLE_BOLD         1
#define UI_TT_MACSTYLE_ITALIC       2
#define UI_TT_MACSTYLE_UNDERSCORE   4
#define UI_TT_MACSTYLE_NONE         8
/* <= not same as 0, this makes us check the bitfield is 0 */

enum { /* platformID */
   UI_TT_PLATFORM_ID_UNICODE   =0,
   UI_TT_PLATFORM_ID_MAC       =1,
   UI_TT_PLATFORM_ID_ISO       =2,
   UI_TT_PLATFORM_ID_MICROSOFT =3
};

enum { /* encodingID for UI_TT_PLATFORM_ID_UNICODE */
   UI_TT_UNICODE_EID_UNICODE_1_0    =0,
   UI_TT_UNICODE_EID_UNICODE_1_1    =1,
   UI_TT_UNICODE_EID_ISO_10646      =2,
   UI_TT_UNICODE_EID_UNICODE_2_0_BMP=3,
   UI_TT_UNICODE_EID_UNICODE_2_0_FULL=4
};

enum { /* encodingID for UI_TT_PLATFORM_ID_MICROSOFT */
   UI_TT_MS_EID_SYMBOL        =0,
   UI_TT_MS_EID_UNICODE_BMP   =1,
   UI_TT_MS_EID_SHIFTJIS      =2,
   UI_TT_MS_EID_UNICODE_FULL  =10
};

enum { /* encodingID for UI_TT_PLATFORM_ID_MAC; same as Script Manager codes */
   UI_TT_MAC_EID_ROMAN        =0,   UI_TT_MAC_EID_ARABIC       =4,
   UI_TT_MAC_EID_JAPANESE     =1,   UI_TT_MAC_EID_HEBREW       =5,
   UI_TT_MAC_EID_CHINESE_TRAD =2,   UI_TT_MAC_EID_GREEK        =6,
   UI_TT_MAC_EID_KOREAN       =3,   UI_TT_MAC_EID_RUSSIAN      =7
};

enum { /* languageID for UI_TT_PLATFORM_ID_MICROSOFT; same as LCID... */
       /* problematic because there are e.g. 16 english LCIDs and 16 arabic LCIDs */
   UI_TT_MS_LANG_ENGLISH     =0x0409,   UI_TT_MS_LANG_ITALIAN     =0x0410,
   UI_TT_MS_LANG_CHINESE     =0x0804,   UI_TT_MS_LANG_JAPANESE    =0x0411,
   UI_TT_MS_LANG_DUTCH       =0x0413,   UI_TT_MS_LANG_KOREAN      =0x0412,
   UI_TT_MS_LANG_FRENCH      =0x040c,   UI_TT_MS_LANG_RUSSIAN     =0x0419,
   UI_TT_MS_LANG_GERMAN      =0x0407,   UI_TT_MS_LANG_SPANISH     =0x0409,
   UI_TT_MS_LANG_HEBREW      =0x040d,   UI_TT_MS_LANG_SWEDISH     =0x041D
};

enum { /* languageID for UI_TT_PLATFORM_ID_MAC */
   UI_TT_MAC_LANG_ENGLISH      =0 ,   UI_TT_MAC_LANG_JAPANESE     =11,
   UI_TT_MAC_LANG_ARABIC       =12,   UI_TT_MAC_LANG_KOREAN       =23,
   UI_TT_MAC_LANG_DUTCH        =4 ,   UI_TT_MAC_LANG_RUSSIAN      =32,
   UI_TT_MAC_LANG_FRENCH       =1 ,   UI_TT_MAC_LANG_SPANISH      =6 ,
   UI_TT_MAC_LANG_GERMAN       =2 ,   UI_TT_MAC_LANG_SWEDISH      =5 ,
   UI_TT_MAC_LANG_HEBREW       =10,   UI_TT_MAC_LANG_CHINESE_SIMPLIFIED =33,
   UI_TT_MAC_LANG_ITALIAN      =3 ,   UI_TT_MAC_LANG_CHINESE_TRAD =19
};

#define ui_ttBYTE(p)     (* (const ui_byte *) (p))
#define ui_ttCHAR(p)     (* (const char *) (p))

#if defined(UI_BIGENDIAN) && !defined(UI_ALLOW_UNALIGNED_TRUETYPE)
   #define ui_ttUSHORT(p)   (* (ui_ushort *) (p))
   #define ui_ttSHORT(p)    (* (ui_short *) (p))
   #define ui_ttULONG(p)    (* (ui_uint *) (p))
   #define ui_ttLONG(p)     (* (ui_int *) (p))
#else
    static ui_ushort ui_ttUSHORT(const ui_byte *p) { return (ui_ushort)(p[0]*256 + p[1]); }
    static ui_short ui_ttSHORT(const ui_byte *p)   { return (ui_short)(p[0]*256 + p[1]); }
    static ui_uint ui_ttULONG(const ui_byte *p)  { return (ui_uint)((p[0]<<24) + (p[1]<<16) + (p[2]<<8) + p[3]); }
#endif

#define ui_tt_tag4(p,c0,c1,c2,c3)\
    ((p)[0] == (c0) && (p)[1] == (c1) && (p)[2] == (c2) && (p)[3] == (c3))
#define ui_tt_tag(p,str) ui_tt_tag4(p,str[0],str[1],str[2],str[3])

UI_INTERN int ui_tt_GetGlyphShape(const struct ui_tt_fontinfo *info, struct ui_allocator *alloc,
                                int glyph_index, struct ui_tt_vertex **pvertices);

UI_INTERN ui_uint
ui_tt__find_table(const ui_byte *data, ui_uint fontstart, const char *tag)
{
    /* @OPTIMIZE: binary search */
    ui_int num_tables = ui_ttUSHORT(data+fontstart+4);
    ui_uint tabledir = fontstart + 12;
    ui_int i;
    for (i = 0; i < num_tables; ++i) {
        ui_uint loc = tabledir + (ui_uint)(16*i);
        if (ui_tt_tag(data+loc+0, tag))
            return ui_ttULONG(data+loc+8);
    }
    return 0;
}

UI_INTERN int
ui_tt_InitFont(struct ui_tt_fontinfo *info, const unsigned char *data2, int fontstart)
{
    ui_uint cmap, t;
    ui_int i,numTables;
    const ui_byte *data = (const ui_byte *) data2;

    info->data = data;
    info->fontstart = fontstart;

    cmap = ui_tt__find_table(data, (ui_uint)fontstart, "cmap");       /* required */
    info->loca = (int)ui_tt__find_table(data, (ui_uint)fontstart, "loca"); /* required */
    info->head = (int)ui_tt__find_table(data, (ui_uint)fontstart, "head"); /* required */
    info->glyf = (int)ui_tt__find_table(data, (ui_uint)fontstart, "glyf"); /* required */
    info->hhea = (int)ui_tt__find_table(data, (ui_uint)fontstart, "hhea"); /* required */
    info->hmtx = (int)ui_tt__find_table(data, (ui_uint)fontstart, "hmtx"); /* required */
    info->kern = (int)ui_tt__find_table(data, (ui_uint)fontstart, "kern"); /* not required */
    if (!cmap || !info->loca || !info->head || !info->glyf || !info->hhea || !info->hmtx)
        return 0;

    t = ui_tt__find_table(data, (ui_uint)fontstart, "maxp");
    if (t) info->numGlyphs = ui_ttUSHORT(data+t+4);
    else info->numGlyphs = 0xffff;

    /* find a cmap encoding table we understand *now* to avoid searching */
    /* later. (todo: could make this installable) */
    /* the same regardless of glyph. */
    numTables = ui_ttUSHORT(data + cmap + 2);
    info->index_map = 0;
    for (i=0; i < numTables; ++i)
    {
        ui_uint encoding_record = cmap + 4 + 8 * (ui_uint)i;
        /* find an encoding we understand: */
        switch(ui_ttUSHORT(data+encoding_record)) {
        case UI_TT_PLATFORM_ID_MICROSOFT:
            switch (ui_ttUSHORT(data+encoding_record+2)) {
            case UI_TT_MS_EID_UNICODE_BMP:
            case UI_TT_MS_EID_UNICODE_FULL:
                /* MS/Unicode */
                info->index_map = (int)(cmap + ui_ttULONG(data+encoding_record+4));
                break;
            default: break;
            } break;
        case UI_TT_PLATFORM_ID_UNICODE:
            /* Mac/iOS has these */
            /* all the encodingIDs are unicode, so we don't bother to check it */
            info->index_map = (int)(cmap + ui_ttULONG(data+encoding_record+4));
            break;
        default: break;
        }
    }
    if (info->index_map == 0)
        return 0;
    info->indexToLocFormat = ui_ttUSHORT(data+info->head + 50);
    return 1;
}

UI_INTERN int
ui_tt_FindGlyphIndex(const struct ui_tt_fontinfo *info, int unicode_codepoint)
{
    const ui_byte *data = info->data;
    ui_uint index_map = (ui_uint)info->index_map;

    ui_ushort format = ui_ttUSHORT(data + index_map + 0);
    if (format == 0) { /* apple byte encoding */
        ui_int bytes = ui_ttUSHORT(data + index_map + 2);
        if (unicode_codepoint < bytes-6)
            return ui_ttBYTE(data + index_map + 6 + unicode_codepoint);
        return 0;
    } else if (format == 6) {
        ui_uint first = ui_ttUSHORT(data + index_map + 6);
        ui_uint count = ui_ttUSHORT(data + index_map + 8);
        if ((ui_uint) unicode_codepoint >= first && (ui_uint) unicode_codepoint < first+count)
            return ui_ttUSHORT(data + index_map + 10 + (unicode_codepoint - (int)first)*2);
        return 0;
    } else if (format == 2) {
        UI_ASSERT(0); /* @TODO: high-byte mapping for japanese/chinese/korean */
        return 0;
    } else if (format == 4) { /* standard mapping for windows fonts: binary search collection of ranges */
        ui_ushort segcount = ui_ttUSHORT(data+index_map+6) >> 1;
        ui_ushort searchRange = ui_ttUSHORT(data+index_map+8) >> 1;
        ui_ushort entrySelector = ui_ttUSHORT(data+index_map+10);
        ui_ushort rangeShift = ui_ttUSHORT(data+index_map+12) >> 1;

        /* do a binary search of the segments */
        ui_uint endCount = index_map + 14;
        ui_uint search = endCount;

        if (unicode_codepoint > 0xffff)
            return 0;

        /* they lie from endCount .. endCount + segCount */
        /* but searchRange is the nearest power of two, so... */
        if (unicode_codepoint >= ui_ttUSHORT(data + search + rangeShift*2))
            search += (ui_uint)(rangeShift*2);

        /* now decrement to bias correctly to find smallest */
        search -= 2;
        while (entrySelector) {
            ui_ushort end;
            searchRange >>= 1;
            end = ui_ttUSHORT(data + search + searchRange*2);
            if (unicode_codepoint > end)
                search += (ui_uint)(searchRange*2);
            --entrySelector;
        }
        search += 2;

      {
         ui_ushort offset, start;
         ui_ushort item = (ui_ushort) ((search - endCount) >> 1);

         UI_ASSERT(unicode_codepoint <= ui_ttUSHORT(data + endCount + 2*item));
         start = ui_ttUSHORT(data + index_map + 14 + segcount*2 + 2 + 2*item);
         if (unicode_codepoint < start)
            return 0;

         offset = ui_ttUSHORT(data + index_map + 14 + segcount*6 + 2 + 2*item);
         if (offset == 0)
            return (ui_ushort) (unicode_codepoint + ui_ttSHORT(data + index_map + 14 + segcount*4 + 2 + 2*item));

         return ui_ttUSHORT(data + offset + (unicode_codepoint-start)*2 + index_map + 14 + segcount*6 + 2 + 2*item);
      }
   } else if (format == 12 || format == 13) {
        ui_uint ngroups = ui_ttULONG(data+index_map+12);
        ui_int low,high;
        low = 0; high = (ui_int)ngroups;
        /* Binary search the right group. */
        while (low < high) {
            ui_int mid = low + ((high-low) >> 1); /* rounds down, so low <= mid < high */
            ui_uint start_char = ui_ttULONG(data+index_map+16+mid*12);
            ui_uint end_char = ui_ttULONG(data+index_map+16+mid*12+4);
            if ((ui_uint) unicode_codepoint < start_char)
                high = mid;
            else if ((ui_uint) unicode_codepoint > end_char)
                low = mid+1;
            else {
                ui_uint start_glyph = ui_ttULONG(data+index_map+16+mid*12+8);
                if (format == 12)
                    return (int)start_glyph + (int)unicode_codepoint - (int)start_char;
                else /* format == 13 */
                    return (int)start_glyph;
            }
        }
        return 0; /* not found */
    }
    /* @TODO */
    UI_ASSERT(0);
    return 0;
}

UI_INTERN void
ui_tt_setvertex(struct ui_tt_vertex *v, ui_byte type, ui_int x, ui_int y, ui_int cx, ui_int cy)
{
    v->type = type;
    v->x = (ui_short) x;
    v->y = (ui_short) y;
    v->cx = (ui_short) cx;
    v->cy = (ui_short) cy;
}

UI_INTERN int
ui_tt__GetGlyfOffset(const struct ui_tt_fontinfo *info, int glyph_index)
{
    int g1,g2;
    if (glyph_index >= info->numGlyphs) return -1; /* glyph index out of range */
    if (info->indexToLocFormat >= 2)    return -1; /* uuinown index->glyph map format */

    if (info->indexToLocFormat == 0) {
        g1 = info->glyf + ui_ttUSHORT(info->data + info->loca + glyph_index * 2) * 2;
        g2 = info->glyf + ui_ttUSHORT(info->data + info->loca + glyph_index * 2 + 2) * 2;
    } else {
        g1 = info->glyf + (int)ui_ttULONG (info->data + info->loca + glyph_index * 4);
        g2 = info->glyf + (int)ui_ttULONG (info->data + info->loca + glyph_index * 4 + 4);
    }
    return g1==g2 ? -1 : g1; /* if length is 0, return -1 */
}

UI_INTERN int
ui_tt_GetGlyphBox(const struct ui_tt_fontinfo *info, int glyph_index,
    int *x0, int *y0, int *x1, int *y1)
{
    int g = ui_tt__GetGlyfOffset(info, glyph_index);
    if (g < 0) return 0;

    if (x0) *x0 = ui_ttSHORT(info->data + g + 2);
    if (y0) *y0 = ui_ttSHORT(info->data + g + 4);
    if (x1) *x1 = ui_ttSHORT(info->data + g + 6);
    if (y1) *y1 = ui_ttSHORT(info->data + g + 8);
    return 1;
}

UI_INTERN int
stbtt__close_shape(struct ui_tt_vertex *vertices, int num_vertices, int was_off,
    int start_off, ui_int sx, ui_int sy, ui_int scx, ui_int scy, ui_int cx, ui_int cy)
{
   if (start_off) {
      if (was_off)
         ui_tt_setvertex(&vertices[num_vertices++], UI_TT_vcurve, (cx+scx)>>1, (cy+scy)>>1, cx,cy);
      ui_tt_setvertex(&vertices[num_vertices++], UI_TT_vcurve, sx,sy,scx,scy);
   } else {
      if (was_off)
         ui_tt_setvertex(&vertices[num_vertices++], UI_TT_vcurve,sx,sy,cx,cy);
      else
         ui_tt_setvertex(&vertices[num_vertices++], UI_TT_vline,sx,sy,0,0);
   }
   return num_vertices;
}

UI_INTERN int
ui_tt_GetGlyphShape(const struct ui_tt_fontinfo *info, struct ui_allocator *alloc,
    int glyph_index, struct ui_tt_vertex **pvertices)
{
    ui_short numberOfContours;
    const ui_byte *endPtsOfContours;
    const ui_byte *data = info->data;
    struct ui_tt_vertex *vertices=0;
    int num_vertices=0;
    int g = ui_tt__GetGlyfOffset(info, glyph_index);
    *pvertices = 0;

    if (g < 0) return 0;
    numberOfContours = ui_ttSHORT(data + g);
    if (numberOfContours > 0) {
        ui_byte flags=0,flagcount;
        ui_int ins, i,j=0,m,n, next_move, was_off=0, off, start_off=0;
        ui_int x,y,cx,cy,sx,sy, scx,scy;
        const ui_byte *points;
        endPtsOfContours = (data + g + 10);
        ins = ui_ttUSHORT(data + g + 10 + numberOfContours * 2);
        points = data + g + 10 + numberOfContours * 2 + 2 + ins;

        n = 1+ui_ttUSHORT(endPtsOfContours + numberOfContours*2-2);
        m = n + 2*numberOfContours;  /* a loose bound on how many vertices we might need */
        vertices = (struct ui_tt_vertex *)alloc->alloc(alloc->userdata, 0, (ui_size)m * sizeof(vertices[0]));
        if (vertices == 0)
            return 0;

        next_move = 0;
        flagcount=0;

        /* in first pass, we load uninterpreted data into the allocated array */
        /* above, shifted to the end of the array so we won't overwrite it when */
        /* we create our final data starting from the front */
        off = m - n; /* starting offset for uninterpreted data, regardless of how m ends up being calculated */

        /* first load flags */
        for (i=0; i < n; ++i) {
            if (flagcount == 0) {
                flags = *points++;
                if (flags & 8)
                    flagcount = *points++;
            } else --flagcount;
            vertices[off+i].type = flags;
        }

        /* now load x coordinates */
        x=0;
        for (i=0; i < n; ++i) {
            flags = vertices[off+i].type;
            if (flags & 2) {
                ui_short dx = *points++;
                x += (flags & 16) ? dx : -dx; /* ??? */
            } else {
                if (!(flags & 16)) {
                    x = x + (ui_short) (points[0]*256 + points[1]);
                    points += 2;
                }
            }
            vertices[off+i].x = (ui_short) x;
        }

        /* now load y coordinates */
        y=0;
        for (i=0; i < n; ++i) {
            flags = vertices[off+i].type;
            if (flags & 4) {
                ui_short dy = *points++;
                y += (flags & 32) ? dy : -dy; /* ??? */
            } else {
                if (!(flags & 32)) {
                    y = y + (ui_short) (points[0]*256 + points[1]);
                    points += 2;
                }
            }
            vertices[off+i].y = (ui_short) y;
        }

        /* now convert them to our format */
        num_vertices=0;
        sx = sy = cx = cy = scx = scy = 0;
        for (i=0; i < n; ++i)
        {
            flags = vertices[off+i].type;
            x     = (ui_short) vertices[off+i].x;
            y     = (ui_short) vertices[off+i].y;

            if (next_move == i) {
                if (i != 0)
                    num_vertices = stbtt__close_shape(vertices, num_vertices, was_off, start_off, sx,sy,scx,scy,cx,cy);

                /* now start the new one                */
                start_off = !(flags & 1);
                if (start_off) {
                    /* if we start off with an off-curve point, then when we need to find a point on the curve */
                    /* where we can start, and we need to save some state for when we wraparound. */
                    scx = x;
                    scy = y;
                    if (!(vertices[off+i+1].type & 1)) {
                        /* next point is also a curve point, so interpolate an on-point curve */
                        sx = (x + (ui_int) vertices[off+i+1].x) >> 1;
                        sy = (y + (ui_int) vertices[off+i+1].y) >> 1;
                    } else {
                        /* otherwise just use the next point as our start point */
                        sx = (ui_int) vertices[off+i+1].x;
                        sy = (ui_int) vertices[off+i+1].y;
                        ++i; /* we're using point i+1 as the starting point, so skip it */
                    }
                } else {
                    sx = x;
                    sy = y;
                }
                ui_tt_setvertex(&vertices[num_vertices++], UI_TT_vmove,sx,sy,0,0);
                was_off = 0;
                next_move = 1 + ui_ttUSHORT(endPtsOfContours+j*2);
                ++j;
            } else {
                if (!(flags & 1))
                { /* if it's a curve */
                    if (was_off) /* two off-curve control points in a row means interpolate an on-curve midpoint */
                        ui_tt_setvertex(&vertices[num_vertices++], UI_TT_vcurve, (cx+x)>>1, (cy+y)>>1, cx, cy);
                    cx = x;
                    cy = y;
                    was_off = 1;
                } else {
                    if (was_off)
                        ui_tt_setvertex(&vertices[num_vertices++], UI_TT_vcurve, x,y, cx, cy);
                    else ui_tt_setvertex(&vertices[num_vertices++], UI_TT_vline, x,y,0,0);
                    was_off = 0;
                }
            }
        }
        num_vertices = stbtt__close_shape(vertices, num_vertices, was_off, start_off, sx,sy,scx,scy,cx,cy);
    } else if (numberOfContours == -1) {
        /* Compound shapes. */
        int more = 1;
        const ui_byte *comp = data + g + 10;
        num_vertices = 0;
        vertices = 0;

        while (more)
        {
            ui_ushort flags, gidx;
            int comp_num_verts = 0, i;
            struct ui_tt_vertex *comp_verts = 0, *tmp = 0;
            float mtx[6] = {1,0,0,1,0,0}, m, n;

            flags = (ui_ushort)ui_ttSHORT(comp); comp+=2;
            gidx = (ui_ushort)ui_ttSHORT(comp); comp+=2;

            if (flags & 2) { /* XY values */
                if (flags & 1) { /* shorts */
                    mtx[4] = ui_ttSHORT(comp); comp+=2;
                    mtx[5] = ui_ttSHORT(comp); comp+=2;
                } else {
                    mtx[4] = ui_ttCHAR(comp); comp+=1;
                    mtx[5] = ui_ttCHAR(comp); comp+=1;
                }
            } else {
                /* @TODO handle matching point */
                UI_ASSERT(0);
            }
            if (flags & (1<<3)) { /* WE_HAVE_A_SCALE */
                mtx[0] = mtx[3] = ui_ttSHORT(comp)/16384.0f; comp+=2;
                mtx[1] = mtx[2] = 0;
            } else if (flags & (1<<6)) { /* WE_HAVE_AN_X_AND_YSCALE */
                mtx[0] = ui_ttSHORT(comp)/16384.0f; comp+=2;
                mtx[1] = mtx[2] = 0;
                mtx[3] = ui_ttSHORT(comp)/16384.0f; comp+=2;
            } else if (flags & (1<<7)) { /* WE_HAVE_A_TWO_BY_TWO */
                mtx[0] = ui_ttSHORT(comp)/16384.0f; comp+=2;
                mtx[1] = ui_ttSHORT(comp)/16384.0f; comp+=2;
                mtx[2] = ui_ttSHORT(comp)/16384.0f; comp+=2;
                mtx[3] = ui_ttSHORT(comp)/16384.0f; comp+=2;
            }

             /* Find transformation scales. */
            m = (float) UI_SQRT(mtx[0]*mtx[0] + mtx[1]*mtx[1]);
            n = (float) UI_SQRT(mtx[2]*mtx[2] + mtx[3]*mtx[3]);

             /* Get indexed glyph. */
            comp_num_verts = ui_tt_GetGlyphShape(info, alloc, gidx, &comp_verts);
            if (comp_num_verts > 0)
            {
                /* Transform vertices. */
                for (i = 0; i < comp_num_verts; ++i) {
                    struct ui_tt_vertex* v = &comp_verts[i];
                    short x,y;
                    x=v->x; y=v->y;
                    v->x = (short)(m * (mtx[0]*x + mtx[2]*y + mtx[4]));
                    v->y = (short)(n * (mtx[1]*x + mtx[3]*y + mtx[5]));
                    x=v->cx; y=v->cy;
                    v->cx = (short)(m * (mtx[0]*x + mtx[2]*y + mtx[4]));
                    v->cy = (short)(n * (mtx[1]*x + mtx[3]*y + mtx[5]));
                }
                /* Append vertices. */
                tmp = (struct ui_tt_vertex*)alloc->alloc(alloc->userdata, 0,
                    (ui_size)(num_vertices+comp_num_verts)*sizeof(struct ui_tt_vertex));
                if (!tmp) {
                    if (vertices) alloc->free(alloc->userdata, vertices);
                    if (comp_verts) alloc->free(alloc->userdata, comp_verts);
                    return 0;
                }
                if (num_vertices > 0) UI_MEMCPY(tmp, vertices, (ui_size)num_vertices*sizeof(struct ui_tt_vertex));
                UI_MEMCPY(tmp+num_vertices, comp_verts, (ui_size)comp_num_verts*sizeof(struct ui_tt_vertex));
                if (vertices) alloc->free(alloc->userdata,vertices);
                vertices = tmp;
                alloc->free(alloc->userdata,comp_verts);
                num_vertices += comp_num_verts;
            }
            /* More components ? */
            more = flags & (1<<5);
        }
    } else if (numberOfContours < 0) {
        /* @TODO other compound variations? */
        UI_ASSERT(0);
    } else {
        /* numberOfCounters == 0, do nothing */
    }
    *pvertices = vertices;
    return num_vertices;
}

UI_INTERN void
ui_tt_GetGlyphHMetrics(const struct ui_tt_fontinfo *info, int glyph_index,
    int *advanceWidth, int *leftSideBearing)
{
    ui_ushort numOfLongHorMetrics = ui_ttUSHORT(info->data+info->hhea + 34);
    if (glyph_index < numOfLongHorMetrics) {
        if (advanceWidth)
            *advanceWidth    = ui_ttSHORT(info->data + info->hmtx + 4*glyph_index);
        if (leftSideBearing)
            *leftSideBearing = ui_ttSHORT(info->data + info->hmtx + 4*glyph_index + 2);
    } else {
        if (advanceWidth)
            *advanceWidth    = ui_ttSHORT(info->data + info->hmtx + 4*(numOfLongHorMetrics-1));
        if (leftSideBearing)
            *leftSideBearing = ui_ttSHORT(info->data + info->hmtx + 4*numOfLongHorMetrics + 2*(glyph_index - numOfLongHorMetrics));
    }
}

UI_INTERN void
ui_tt_GetFontVMetrics(const struct ui_tt_fontinfo *info,
    int *ascent, int *descent, int *lineGap)
{
   if (ascent ) *ascent  = ui_ttSHORT(info->data+info->hhea + 4);
   if (descent) *descent = ui_ttSHORT(info->data+info->hhea + 6);
   if (lineGap) *lineGap = ui_ttSHORT(info->data+info->hhea + 8);
}

UI_INTERN float
ui_tt_ScaleForPixelHeight(const struct ui_tt_fontinfo *info, float height)
{
   int fheight = ui_ttSHORT(info->data + info->hhea + 4) - ui_ttSHORT(info->data + info->hhea + 6);
   return (float) height / (float)fheight;
}

UI_INTERN float
ui_tt_ScaleForMappingEmToPixels(const struct ui_tt_fontinfo *info, float pixels)
{
   int unitsPerEm = ui_ttUSHORT(info->data + info->head + 18);
   return pixels / (float)unitsPerEm;
}

/*-------------------------------------------------------------
 *            antialiasing software rasterizer
 * --------------------------------------------------------------*/
UI_INTERN void
ui_tt_GetGlyphBitmapBoxSubpixel(const struct ui_tt_fontinfo *font,
    int glyph, float scale_x, float scale_y,float shift_x, float shift_y,
    int *ix0, int *iy0, int *ix1, int *iy1)
{
    int x0,y0,x1,y1;
    if (!ui_tt_GetGlyphBox(font, glyph, &x0,&y0,&x1,&y1)) {
        /* e.g. space character */
        if (ix0) *ix0 = 0;
        if (iy0) *iy0 = 0;
        if (ix1) *ix1 = 0;
        if (iy1) *iy1 = 0;
    } else {
        /* move to integral bboxes (treating pixels as little squares, what pixels get touched)? */
        if (ix0) *ix0 = ui_ifloorf((float)x0 * scale_x + shift_x);
        if (iy0) *iy0 = ui_ifloorf((float)-y1 * scale_y + shift_y);
        if (ix1) *ix1 = ui_iceilf ((float)x1 * scale_x + shift_x);
        if (iy1) *iy1 = ui_iceilf ((float)-y0 * scale_y + shift_y);
    }
}

UI_INTERN void
ui_tt_GetGlyphBitmapBox(const struct ui_tt_fontinfo *font, int glyph,
    float scale_x, float scale_y, int *ix0, int *iy0, int *ix1, int *iy1)
{
   ui_tt_GetGlyphBitmapBoxSubpixel(font, glyph, scale_x, scale_y,0.0f,0.0f, ix0, iy0, ix1, iy1);
}

/*-------------------------------------------------------------
 *                          Rasterizer
 * --------------------------------------------------------------*/
UI_INTERN void*
ui_tt__hheap_alloc(struct ui_tt__hheap *hh, ui_size size)
{
    if (hh->first_free) {
        void *p = hh->first_free;
        hh->first_free = * (void **) p;
        return p;
    } else {
        if (hh->num_remaining_in_head_chuui == 0) {
            int count = (size < 32 ? 2000 : size < 128 ? 800 : 100);
            struct ui_tt__hheap_chuui *c = (struct ui_tt__hheap_chuui *)
                hh->alloc.alloc(hh->alloc.userdata, 0,
                sizeof(struct ui_tt__hheap_chuui) + size * (ui_size)count);
            if (c == 0) return 0;
            c->next = hh->head;
            hh->head = c;
            hh->num_remaining_in_head_chuui = count;
        }
        --hh->num_remaining_in_head_chuui;
        return (char *) (hh->head) + size * (ui_size)hh->num_remaining_in_head_chuui;
    }
}

UI_INTERN void
ui_tt__hheap_free(struct ui_tt__hheap *hh, void *p)
{
    *(void **) p = hh->first_free;
    hh->first_free = p;
}

UI_INTERN void
ui_tt__hheap_cleanup(struct ui_tt__hheap *hh)
{
    struct ui_tt__hheap_chuui *c = hh->head;
    while (c) {
        struct ui_tt__hheap_chuui *n = c->next;
        hh->alloc.free(hh->alloc.userdata, c);
        c = n;
    }
}

UI_INTERN struct ui_tt__active_edge*
ui_tt__new_active(struct ui_tt__hheap *hh, struct ui_tt__edge *e,
    int off_x, float start_point)
{
    struct ui_tt__active_edge *z = (struct ui_tt__active_edge *)
        ui_tt__hheap_alloc(hh, sizeof(*z));
    float dxdy = (e->x1 - e->x0) / (e->y1 - e->y0);
    /*STBTT_assert(e->y0 <= start_point); */
    if (!z) return z;
    z->fdx = dxdy;
    z->fdy = (dxdy != 0) ? (1/dxdy): 0;
    z->fx = e->x0 + dxdy * (start_point - e->y0);
    z->fx -= (float)off_x;
    z->direction = e->invert ? 1.0f : -1.0f;
    z->sy = e->y0;
    z->ey = e->y1;
    z->next = 0;
    return z;
}

UI_INTERN void
ui_tt__handle_clipped_edge(float *scanline, int x, struct ui_tt__active_edge *e,
    float x0, float y0, float x1, float y1)
{
    if (y0 == y1) return;
    UI_ASSERT(y0 < y1);
    UI_ASSERT(e->sy <= e->ey);
    if (y0 > e->ey) return;
    if (y1 < e->sy) return;
    if (y0 < e->sy) {
        x0 += (x1-x0) * (e->sy - y0) / (y1-y0);
        y0 = e->sy;
    }
    if (y1 > e->ey) {
        x1 += (x1-x0) * (e->ey - y1) / (y1-y0);
        y1 = e->ey;
    }

    if (x0 == x) UI_ASSERT(x1 <= x+1);
    else if (x0 == x+1) UI_ASSERT(x1 >= x);
    else if (x0 <= x) UI_ASSERT(x1 <= x);
    else if (x0 >= x+1) UI_ASSERT(x1 >= x+1);
    else UI_ASSERT(x1 >= x && x1 <= x+1);

    if (x0 <= x && x1 <= x)
        scanline[x] += e->direction * (y1-y0);
    else if (x0 >= x+1 && x1 >= x+1);
    else {
        UI_ASSERT(x0 >= x && x0 <= x+1 && x1 >= x && x1 <= x+1);
        /* coverage = 1 - average x position */
        scanline[x] += (float)e->direction * (float)(y1-y0) * (1.0f-((x0-(float)x)+(x1-(float)x))/2.0f);
    }
}

UI_INTERN void
ui_tt__fill_active_edges_new(float *scanline, float *scanline_fill, int len,
    struct ui_tt__active_edge *e, float y_top)
{
    float y_bottom = y_top+1;
    while (e)
    {
        /* brute force every pixel */
        /* compute intersection points with top & bottom */
        UI_ASSERT(e->ey >= y_top);
        if (e->fdx == 0) {
            float x0 = e->fx;
            if (x0 < len) {
                if (x0 >= 0) {
                    ui_tt__handle_clipped_edge(scanline,(int) x0,e, x0,y_top, x0,y_bottom);
                    ui_tt__handle_clipped_edge(scanline_fill-1,(int) x0+1,e, x0,y_top, x0,y_bottom);
                } else {
                    ui_tt__handle_clipped_edge(scanline_fill-1,0,e, x0,y_top, x0,y_bottom);
                }
            }
        } else {
            float x0 = e->fx;
            float dx = e->fdx;
            float xb = x0 + dx;
            float x_top, x_bottom;
            float y0,y1;
            float dy = e->fdy;
            UI_ASSERT(e->sy <= y_bottom && e->ey >= y_top);

            /* compute endpoints of line segment clipped to this scanline (if the */
            /* line segment starts on this scanline. x0 is the intersection of the */
            /* line with y_top, but that may be off the line segment. */
            if (e->sy > y_top) {
                x_top = x0 + dx * (e->sy - y_top);
                y0 = e->sy;
            } else {
                x_top = x0;
                y0 = y_top;
            }

            if (e->ey < y_bottom) {
                x_bottom = x0 + dx * (e->ey - y_top);
                y1 = e->ey;
            } else {
                x_bottom = xb;
                y1 = y_bottom;
            }

            if (x_top >= 0 && x_bottom >= 0 && x_top < len && x_bottom < len)
            {
                /* from here on, we don't have to range check x values */
                if ((int) x_top == (int) x_bottom) {
                    float height;
                    /* simple case, only spans one pixel */
                    int x = (int) x_top;
                    height = y1 - y0;
                    UI_ASSERT(x >= 0 && x < len);
                    scanline[x] += e->direction * (1.0f-(((float)x_top - (float)x) + ((float)x_bottom-(float)x))/2.0f)  * (float)height;
                    scanline_fill[x] += e->direction * (float)height; /* everything right of this pixel is filled */
                } else {
                    int x,x1,x2;
                    float y_crossing, step, sign, area;
                    /* covers 2+ pixels */
                    if (x_top > x_bottom)
                    {
                        /* flip scanline vertically; signed area is the same */
                        float t;
                        y0 = y_bottom - (y0 - y_top);
                        y1 = y_bottom - (y1 - y_top);
                        t = y0; y0 = y1; y1 = t;
                        t = x_bottom; x_bottom = x_top; x_top = t;
                        dx = -dx;
                        dy = -dy;
                        t = x0; x0 = xb; xb = t;
                    }

                    x1 = (int) x_top;
                    x2 = (int) x_bottom;
                    /* compute intersection with y axis at x1+1 */
                    y_crossing = ((float)x1+1 - (float)x0) * (float)dy + (float)y_top;

                    sign = e->direction;
                    /* area of the rectangle covered from y0..y_crossing */
                    area = sign * (y_crossing-y0);
                    /* area of the triangle (x_top,y0), (x+1,y0), (x+1,y_crossing) */
                    scanline[x1] += area * (1.0f-((float)((float)x_top - (float)x1)+(float)(x1+1-x1))/2.0f);

                    step = sign * dy;
                    for (x = x1+1; x < x2; ++x) {
                        scanline[x] += area + step/2;
                        area += step;
                    }
                    y_crossing += (float)dy * (float)(x2 - (x1+1));

                    scanline[x2] += area + sign * (1.0f-((float)(x2-x2)+((float)x_bottom-(float)x2))/2.0f) * (y1-y_crossing);
                    scanline_fill[x2] += sign * (y1-y0);
                }
            }
            else
            {
                /* if edge goes outside of box we're drawing, we require */
                /* clipping logic. since this does not match the intended use */
                /* of this library, we use a different, very slow brute */
                /* force implementation */
                int x;
                for (x=0; x < len; ++x)
                {
                    /* cases: */
                    /* */
                    /* there can be up to two intersections with the pixel. any intersection */
                    /* with left or right edges can be handled by splitting into two (or three) */
                    /* regions. intersections with top & bottom do not necessitate case-wise logic. */
                    /* */
                    /* the old way of doing this found the intersections with the left & right edges, */
                    /* then used some simple logic to produce up to three segments in sorted order */
                    /* from top-to-bottom. however, this had a problem: if an x edge was epsilon */
                    /* across the x border, then the corresponding y position might not be distinct */
                    /* from the other y segment, and it might ignored as an empty segment. to avoid */
                    /* that, we need to explicitly produce segments based on x positions. */

                    /* rename variables to clear pairs */
                    float ya = y_top;
                    float x1 = (float) (x);
                    float x2 = (float) (x+1);
                    float x3 = xb;
                    float y3 = y_bottom;
                    float yb,y2;

                    yb = ((float)x - x0) / dx + y_top;
                    y2 = ((float)x+1 - x0) / dx + y_top;

                    if (x0 < x1 && x3 > x2) {         /* three segments descending down-right */
                        ui_tt__handle_clipped_edge(scanline,x,e, x0,ya, x1,yb);
                        ui_tt__handle_clipped_edge(scanline,x,e, x1,yb, x2,y2);
                        ui_tt__handle_clipped_edge(scanline,x,e, x2,y2, x3,y3);
                    } else if (x3 < x1 && x0 > x2) {  /* three segments descending down-left */
                        ui_tt__handle_clipped_edge(scanline,x,e, x0,ya, x2,y2);
                        ui_tt__handle_clipped_edge(scanline,x,e, x2,y2, x1,yb);
                        ui_tt__handle_clipped_edge(scanline,x,e, x1,yb, x3,y3);
                    } else if (x0 < x1 && x3 > x1) {  /* two segments across x, down-right */
                        ui_tt__handle_clipped_edge(scanline,x,e, x0,ya, x1,yb);
                        ui_tt__handle_clipped_edge(scanline,x,e, x1,yb, x3,y3);
                    } else if (x3 < x1 && x0 > x1) {  /* two segments across x, down-left */
                        ui_tt__handle_clipped_edge(scanline,x,e, x0,ya, x1,yb);
                        ui_tt__handle_clipped_edge(scanline,x,e, x1,yb, x3,y3);
                    } else if (x0 < x2 && x3 > x2) {  /* two segments across x+1, down-right */
                        ui_tt__handle_clipped_edge(scanline,x,e, x0,ya, x2,y2);
                        ui_tt__handle_clipped_edge(scanline,x,e, x2,y2, x3,y3);
                    } else if (x3 < x2 && x0 > x2) {  /* two segments across x+1, down-left */
                        ui_tt__handle_clipped_edge(scanline,x,e, x0,ya, x2,y2);
                        ui_tt__handle_clipped_edge(scanline,x,e, x2,y2, x3,y3);
                    } else {  /* one segment */
                        ui_tt__handle_clipped_edge(scanline,x,e, x0,ya, x3,y3);
                    }
                }
            }
        }
        e = e->next;
    }
}

/* directly AA rasterize edges w/o supersampling */
UI_INTERN void
ui_tt__rasterize_sorted_edges(struct ui_tt__bitmap *result, struct ui_tt__edge *e,
    int n, int vsubsample, int off_x, int off_y, struct ui_allocator *alloc)
{
    struct ui_tt__hheap hh;
    struct ui_tt__active_edge *active = 0;
    int y,j=0, i;
    float scanline_data[129], *scanline, *scanline2;

    UI_UNUSED(vsubsample);
    ui_zero_struct(hh);
    hh.alloc = *alloc;

    if (result->w > 64)
        scanline = (float *) alloc->alloc(alloc->userdata,0, (ui_size)(result->w*2+1) * sizeof(float));
    else scanline = scanline_data;

    scanline2 = scanline + result->w;
    y = off_y;
    e[n].y0 = (float) (off_y + result->h) + 1;

    while (j < result->h)
    {
        /* find center of pixel for this scanline */
        float scan_y_top    = (float)y + 0.0f;
        float scan_y_bottom = (float)y + 1.0f;
        struct ui_tt__active_edge **step = &active;

        UI_MEMSET(scanline , 0, (ui_size)result->w*sizeof(scanline[0]));
        UI_MEMSET(scanline2, 0, (ui_size)(result->w+1)*sizeof(scanline[0]));

        /* update all active edges; */
        /* remove all active edges that terminate before the top of this scanline */
        while (*step) {
            struct ui_tt__active_edge * z = *step;
            if (z->ey <= scan_y_top) {
                *step = z->next; /* delete from list */
                UI_ASSERT(z->direction);
                z->direction = 0;
                ui_tt__hheap_free(&hh, z);
            } else {
                step = &((*step)->next); /* advance through list */
            }
        }

        /* insert all edges that start before the bottom of this scanline */
        while (e->y0 <= scan_y_bottom) {
            if (e->y0 != e->y1) {
                struct ui_tt__active_edge *z = ui_tt__new_active(&hh, e, off_x, scan_y_top);
                if (z != 0) {
                    UI_ASSERT(z->ey >= scan_y_top);
                    /* insert at front */
                    z->next = active;
                    active = z;
                }
            }
            ++e;
        }

        /* now process all active edges */
        if (active)
            ui_tt__fill_active_edges_new(scanline, scanline2+1, result->w, active, scan_y_top);

        {
            float sum = 0;
            for (i=0; i < result->w; ++i) {
                float k;
                int m;
                sum += scanline2[i];
                k = scanline[i] + sum;
                k = (float) UI_ABS(k) * 255.0f + 0.5f;
                m = (int) k;
                if (m > 255) m = 255;
                result->pixels[j*result->stride + i] = (unsigned char) m;
            }
        }
        /* advance all the edges */
        step = &active;
        while (*step) {
            struct ui_tt__active_edge *z = *step;
            z->fx += z->fdx; /* advance to position for current scanline */
            step = &((*step)->next); /* advance through list */
        }
        ++y;
        ++j;
    }
    ui_tt__hheap_cleanup(&hh);
    if (scanline != scanline_data)
        alloc->free(alloc->userdata, scanline);
}

#define UI_TT__COMPARE(a,b)  ((a)->y0 < (b)->y0)
UI_INTERN void
ui_tt__sort_edges_ins_sort(struct ui_tt__edge *p, int n)
{
    int i,j;
    for (i=1; i < n; ++i) {
        struct ui_tt__edge t = p[i], *a = &t;
        j = i;
        while (j > 0) {
            struct ui_tt__edge *b = &p[j-1];
            int c = UI_TT__COMPARE(a,b);
            if (!c) break;
            p[j] = p[j-1];
            --j;
        }
        if (i != j)
            p[j] = t;
    }
}

UI_INTERN void
ui_tt__sort_edges_quicksort(struct ui_tt__edge *p, int n)
{
    /* threshold for transitioning to insertion sort */
    while (n > 12) {
        struct ui_tt__edge t;
        int c01,c12,c,m,i,j;

        /* compute median of three */
        m = n >> 1;
        c01 = UI_TT__COMPARE(&p[0],&p[m]);
        c12 = UI_TT__COMPARE(&p[m],&p[n-1]);

        /* if 0 >= mid >= end, or 0 < mid < end, then use mid */
        if (c01 != c12) {
            /* otherwise, we'll need to swap something else to middle */
            int z;
            c = UI_TT__COMPARE(&p[0],&p[n-1]);
            /* 0>mid && mid<n:  0>n => n; 0<n => 0 */
            /* 0<mid && mid>n:  0>n => 0; 0<n => n */
            z = (c == c12) ? 0 : n-1;
            t = p[z];
            p[z] = p[m];
            p[m] = t;
        }

        /* now p[m] is the median-of-three */
        /* swap it to the beginning so it won't move around */
        t = p[0];
        p[0] = p[m];
        p[m] = t;

        /* partition loop */
        i=1;
        j=n-1;
        for(;;) {
            /* handling of equality is crucial here */
            /* for sentinels & efficiency with duplicates */
            for (;;++i) {
                if (!UI_TT__COMPARE(&p[i], &p[0])) break;
            }
            for (;;--j) {
                if (!UI_TT__COMPARE(&p[0], &p[j])) break;
            }

            /* make sure we haven't crossed */
             if (i >= j) break;
             t = p[i];
             p[i] = p[j];
             p[j] = t;

            ++i;
            --j;

        }

        /* recurse on smaller side, iterate on larger */
        if (j < (n-i)) {
            ui_tt__sort_edges_quicksort(p,j);
            p = p+i;
            n = n-i;
        } else {
            ui_tt__sort_edges_quicksort(p+i, n-i);
            n = j;
        }
    }
}

UI_INTERN void
ui_tt__sort_edges(struct ui_tt__edge *p, int n)
{
   ui_tt__sort_edges_quicksort(p, n);
   ui_tt__sort_edges_ins_sort(p, n);
}

UI_INTERN void
ui_tt__rasterize(struct ui_tt__bitmap *result, struct ui_tt__point *pts,
    int *wcount, int windings, float scale_x, float scale_y,
    float shift_x, float shift_y, int off_x, int off_y, int invert,
    struct ui_allocator *alloc)
{
    float y_scale_inv = invert ? -scale_y : scale_y;
    struct ui_tt__edge *e;
    int n,i,j,k,m;
    int vsubsample = 1;
    /* vsubsample should divide 255 evenly; otherwise we won't reach full opacity */

    /* now we have to blow out the windings into explicit edge lists */
    n = 0;
    for (i=0; i < windings; ++i)
        n += wcount[i];

    e = (struct ui_tt__edge*)
       alloc->alloc(alloc->userdata, 0,(sizeof(*e) * (ui_size)(n+1)));
    if (e == 0) return;
    n = 0;

    m=0;
    for (i=0; i < windings; ++i)
    {
        struct ui_tt__point *p = pts + m;
        m += wcount[i];
        j = wcount[i]-1;
        for (k=0; k < wcount[i]; j=k++) {
            int a=k,b=j;
            /* skip the edge if horizontal */
            if (p[j].y == p[k].y)
                continue;

            /* add edge from j to k to the list */
            e[n].invert = 0;
            if (invert ? p[j].y > p[k].y : p[j].y < p[k].y) {
                e[n].invert = 1;
                a=j,b=k;
            }
            e[n].x0 = p[a].x * scale_x + shift_x;
            e[n].y0 = (p[a].y * y_scale_inv + shift_y) * (float)vsubsample;
            e[n].x1 = p[b].x * scale_x + shift_x;
            e[n].y1 = (p[b].y * y_scale_inv + shift_y) * (float)vsubsample;
            ++n;
        }
    }

    /* now sort the edges by their highest point (should snap to integer, and then by x) */
    /*STBTT_sort(e, n, sizeof(e[0]), stbtt__edge_compare); */
    ui_tt__sort_edges(e, n);
    /* now, traverse the scanlines and find the intersections on each scanline, use xor winding rule */
    ui_tt__rasterize_sorted_edges(result, e, n, vsubsample, off_x, off_y, alloc);
    alloc->free(alloc->userdata, e);
}

UI_INTERN void
ui_tt__add_point(struct ui_tt__point *points, int n, float x, float y)
{
    if (!points) return; /* during first pass, it's unallocated */
    points[n].x = x;
    points[n].y = y;
}

UI_INTERN int
ui_tt__tesselate_curve(struct ui_tt__point *points, int *num_points,
    float x0, float y0, float x1, float y1, float x2, float y2,
    float objspace_flatness_squared, int n)
{
    /* tesselate until threshold p is happy...
     * @TODO warped to compensate for non-linear stretching */
    /* midpoint */
    float mx = (x0 + 2*x1 + x2)/4;
    float my = (y0 + 2*y1 + y2)/4;
    /* versus directly drawn line */
    float dx = (x0+x2)/2 - mx;
    float dy = (y0+y2)/2 - my;
    if (n > 16) /* 65536 segments on one curve better be enough! */
        return 1;

    /* half-pixel error allowed... need to be smaller if AA */
    if (dx*dx+dy*dy > objspace_flatness_squared) {
        ui_tt__tesselate_curve(points, num_points, x0,y0,
            (x0+x1)/2.0f,(y0+y1)/2.0f, mx,my, objspace_flatness_squared,n+1);
        ui_tt__tesselate_curve(points, num_points, mx,my,
            (x1+x2)/2.0f,(y1+y2)/2.0f, x2,y2, objspace_flatness_squared,n+1);
    } else {
        ui_tt__add_point(points, *num_points,x2,y2);
        *num_points = *num_points+1;
    }
    return 1;
}

/* returns number of contours */
UI_INTERN struct ui_tt__point*
ui_tt_FlattenCurves(struct ui_tt_vertex *vertices, int num_verts,
    float objspace_flatness, int **contour_lengths, int *num_contours,
    struct ui_allocator *alloc)
{
    struct ui_tt__point *points=0;
    int num_points=0;
    float objspace_flatness_squared = objspace_flatness * objspace_flatness;
    int i;
    int n=0;
    int start=0;
    int pass;

    /* count how many "moves" there are to get the contour count */
    for (i=0; i < num_verts; ++i)
        if (vertices[i].type == UI_TT_vmove) ++n;

    *num_contours = n;
    if (n == 0) return 0;

    *contour_lengths = (int *)
        alloc->alloc(alloc->userdata,0, (sizeof(**contour_lengths) * (ui_size)n));
    if (*contour_lengths == 0) {
        *num_contours = 0;
        return 0;
    }

    /* make two passes through the points so we don't need to realloc */
    for (pass=0; pass < 2; ++pass)
    {
        float x=0,y=0;
        if (pass == 1) {
            points = (struct ui_tt__point *)
                alloc->alloc(alloc->userdata,0, (ui_size)num_points * sizeof(points[0]));
            if (points == 0) goto error;
        }
        num_points = 0;
        n= -1;

        for (i=0; i < num_verts; ++i)
        {
            switch (vertices[i].type) {
            case UI_TT_vmove:
                /* start the next contour */
                if (n >= 0)
                (*contour_lengths)[n] = num_points - start;
                ++n;
                start = num_points;

                x = vertices[i].x, y = vertices[i].y;
                ui_tt__add_point(points, num_points++, x,y);
                break;
            case UI_TT_vline:
               x = vertices[i].x, y = vertices[i].y;
               ui_tt__add_point(points, num_points++, x, y);
               break;
            case UI_TT_vcurve:
               ui_tt__tesselate_curve(points, &num_points, x,y,
                                        vertices[i].cx, vertices[i].cy,
                                        vertices[i].x,  vertices[i].y,
                                        objspace_flatness_squared, 0);
               x = vertices[i].x, y = vertices[i].y;
               break;
            default: break;
         }
      }
      (*contour_lengths)[n] = num_points - start;
   }
   return points;

error:
   alloc->free(alloc->userdata, points);
   alloc->free(alloc->userdata, *contour_lengths);
   *contour_lengths = 0;
   *num_contours = 0;
   return 0;
}

UI_INTERN void
ui_tt_Rasterize(struct ui_tt__bitmap *result, float flatness_in_pixels,
    struct ui_tt_vertex *vertices, int num_verts,
    float scale_x, float scale_y, float shift_x, float shift_y,
    int x_off, int y_off, int invert, struct ui_allocator *alloc)
{
    float scale = scale_x > scale_y ? scale_y : scale_x;
    int winding_count, *winding_lengths;
    struct ui_tt__point *windings = ui_tt_FlattenCurves(vertices, num_verts,
        flatness_in_pixels / scale, &winding_lengths, &winding_count, alloc);

    UI_ASSERT(alloc);
    if (windings) {
        ui_tt__rasterize(result, windings, winding_lengths, winding_count,
            scale_x, scale_y, shift_x, shift_y, x_off, y_off, invert, alloc);
        alloc->free(alloc->userdata, winding_lengths);
        alloc->free(alloc->userdata, windings);
    }
}

UI_INTERN void
ui_tt_MakeGlyphBitmapSubpixel(const struct ui_tt_fontinfo *info, unsigned char *output,
    int out_w, int out_h, int out_stride, float scale_x, float scale_y,
    float shift_x, float shift_y, int glyph, struct ui_allocator *alloc)
{
    int ix0,iy0;
    struct ui_tt_vertex *vertices;
    int num_verts = ui_tt_GetGlyphShape(info, alloc, glyph, &vertices);
    struct ui_tt__bitmap gbm;

    ui_tt_GetGlyphBitmapBoxSubpixel(info, glyph, scale_x, scale_y, shift_x,
        shift_y, &ix0,&iy0,0,0);
    gbm.pixels = output;
    gbm.w = out_w;
    gbm.h = out_h;
    gbm.stride = out_stride;

    if (gbm.w && gbm.h)
        ui_tt_Rasterize(&gbm, 0.35f, vertices, num_verts, scale_x, scale_y,
            shift_x, shift_y, ix0,iy0, 1, alloc);
    alloc->free(alloc->userdata, vertices);
}

/*-------------------------------------------------------------
 *                          Bitmap baking
 * --------------------------------------------------------------*/
UI_INTERN int
ui_tt_PackBegin(struct ui_tt_pack_context *spc, unsigned char *pixels,
    int pw, int ph, int stride_in_bytes, int padding, struct ui_allocator *alloc)
{
    int num_nodes = pw - padding;
    struct ui_rp_context *context = (struct ui_rp_context *)
        alloc->alloc(alloc->userdata,0, sizeof(*context));
    struct ui_rp_node *nodes = (struct ui_rp_node*)
        alloc->alloc(alloc->userdata,0, (sizeof(*nodes  ) * (ui_size)num_nodes));

    if (context == 0 || nodes == 0) {
        if (context != 0) alloc->free(alloc->userdata, context);
        if (nodes   != 0) alloc->free(alloc->userdata, nodes);
        return 0;
    }

    spc->width = pw;
    spc->height = ph;
    spc->pixels = pixels;
    spc->pack_info = context;
    spc->nodes = nodes;
    spc->padding = padding;
    spc->stride_in_bytes = (stride_in_bytes != 0) ? stride_in_bytes : pw;
    spc->h_oversample = 1;
    spc->v_oversample = 1;

    ui_rp_init_target(context, pw-padding, ph-padding, nodes, num_nodes);
    if (pixels)
        UI_MEMSET(pixels, 0, (ui_size)(pw*ph)); /* background of 0 around pixels */
    return 1;
}

UI_INTERN void
ui_tt_PackEnd(struct ui_tt_pack_context *spc, struct ui_allocator *alloc)
{
    alloc->free(alloc->userdata, spc->nodes);
    alloc->free(alloc->userdata, spc->pack_info);
}

UI_INTERN void
ui_tt_PackSetOversampling(struct ui_tt_pack_context *spc,
    unsigned int h_oversample, unsigned int v_oversample)
{
   UI_ASSERT(h_oversample <= UI_TT_MAX_OVERSAMPLE);
   UI_ASSERT(v_oversample <= UI_TT_MAX_OVERSAMPLE);
   if (h_oversample <= UI_TT_MAX_OVERSAMPLE)
      spc->h_oversample = h_oversample;
   if (v_oversample <= UI_TT_MAX_OVERSAMPLE)
      spc->v_oversample = v_oversample;
}

UI_INTERN void
ui_tt__h_prefilter(unsigned char *pixels, int w, int h, int stride_in_bytes,
    int kernel_width)
{
    unsigned char buffer[UI_TT_MAX_OVERSAMPLE];
    int safe_w = w - kernel_width;
    int j;

    for (j=0; j < h; ++j)
    {
        int i;
        unsigned int total;
        UI_MEMSET(buffer, 0, (ui_size)kernel_width);

        total = 0;

        /* make kernel_width a constant in common cases so compiler can optimize out the divide */
        switch (kernel_width) {
        case 2:
            for (i=0; i <= safe_w; ++i) {
                total += (unsigned int)(pixels[i] - buffer[i & UI_TT__OVER_MASK]);
                buffer[(i+kernel_width) & UI_TT__OVER_MASK] = pixels[i];
                pixels[i] = (unsigned char) (total / 2);
            }
            break;
        case 3:
            for (i=0; i <= safe_w; ++i) {
                total += (unsigned int)(pixels[i] - buffer[i & UI_TT__OVER_MASK]);
                buffer[(i+kernel_width) & UI_TT__OVER_MASK] = pixels[i];
                pixels[i] = (unsigned char) (total / 3);
            }
            break;
        case 4:
            for (i=0; i <= safe_w; ++i) {
                total += (unsigned int)pixels[i] - buffer[i & UI_TT__OVER_MASK];
                buffer[(i+kernel_width) & UI_TT__OVER_MASK] = pixels[i];
                pixels[i] = (unsigned char) (total / 4);
            }
            break;
        case 5:
            for (i=0; i <= safe_w; ++i) {
                total += (unsigned int)(pixels[i] - buffer[i & UI_TT__OVER_MASK]);
                buffer[(i+kernel_width) & UI_TT__OVER_MASK] = pixels[i];
                pixels[i] = (unsigned char) (total / 5);
            }
            break;
        default:
            for (i=0; i <= safe_w; ++i) {
                total += (unsigned int)(pixels[i] - buffer[i & UI_TT__OVER_MASK]);
                buffer[(i+kernel_width) & UI_TT__OVER_MASK] = pixels[i];
                pixels[i] = (unsigned char) (total / (unsigned int)kernel_width);
            }
            break;
        }

        for (; i < w; ++i) {
            UI_ASSERT(pixels[i] == 0);
            total -= (unsigned int)(buffer[i & UI_TT__OVER_MASK]);
            pixels[i] = (unsigned char) (total / (unsigned int)kernel_width);
        }
        pixels += stride_in_bytes;
    }
}

UI_INTERN void
ui_tt__v_prefilter(unsigned char *pixels, int w, int h, int stride_in_bytes,
    int kernel_width)
{
    unsigned char buffer[UI_TT_MAX_OVERSAMPLE];
    int safe_h = h - kernel_width;
    int j;

    for (j=0; j < w; ++j)
    {
        int i;
        unsigned int total;
        UI_MEMSET(buffer, 0, (ui_size)kernel_width);

        total = 0;

        /* make kernel_width a constant in common cases so compiler can optimize out the divide */
        switch (kernel_width) {
        case 2:
            for (i=0; i <= safe_h; ++i) {
                total += (unsigned int)(pixels[i*stride_in_bytes] - buffer[i & UI_TT__OVER_MASK]);
                buffer[(i+kernel_width) & UI_TT__OVER_MASK] = pixels[i*stride_in_bytes];
                pixels[i*stride_in_bytes] = (unsigned char) (total / 2);
            }
            break;
         case 3:
            for (i=0; i <= safe_h; ++i) {
                total += (unsigned int)(pixels[i*stride_in_bytes] - buffer[i & UI_TT__OVER_MASK]);
                buffer[(i+kernel_width) & UI_TT__OVER_MASK] = pixels[i*stride_in_bytes];
                pixels[i*stride_in_bytes] = (unsigned char) (total / 3);
            }
            break;
         case 4:
            for (i=0; i <= safe_h; ++i) {
                total += (unsigned int)(pixels[i*stride_in_bytes] - buffer[i & UI_TT__OVER_MASK]);
                buffer[(i+kernel_width) & UI_TT__OVER_MASK] = pixels[i*stride_in_bytes];
                pixels[i*stride_in_bytes] = (unsigned char) (total / 4);
            }
            break;
         case 5:
            for (i=0; i <= safe_h; ++i) {
                total += (unsigned int)(pixels[i*stride_in_bytes] - buffer[i & UI_TT__OVER_MASK]);
                buffer[(i+kernel_width) & UI_TT__OVER_MASK] = pixels[i*stride_in_bytes];
                pixels[i*stride_in_bytes] = (unsigned char) (total / 5);
            }
            break;
         default:
            for (i=0; i <= safe_h; ++i) {
                total += (unsigned int)(pixels[i*stride_in_bytes] - buffer[i & UI_TT__OVER_MASK]);
                buffer[(i+kernel_width) & UI_TT__OVER_MASK] = pixels[i*stride_in_bytes];
                pixels[i*stride_in_bytes] = (unsigned char) (total / (unsigned int)kernel_width);
            }
            break;
        }

        for (; i < h; ++i) {
            UI_ASSERT(pixels[i*stride_in_bytes] == 0);
            total -= (unsigned int)(buffer[i & UI_TT__OVER_MASK]);
            pixels[i*stride_in_bytes] = (unsigned char) (total / (unsigned int)kernel_width);
        }
        pixels += 1;
    }
}

UI_INTERN float
ui_tt__oversample_shift(int oversample)
{
    if (!oversample)
        return 0.0f;

    /* The prefilter is a box filter of width "oversample", */
    /* which shifts phase by (oversample - 1)/2 pixels in */
    /* oversampled space. We want to shift in the opposite */
    /* direction to counter this. */
    return (float)-(oversample - 1) / (2.0f * (float)oversample);
}

/* rects array must be big enough to accommodate all characters in the given ranges */
UI_INTERN int
ui_tt_PackFontRangesGatherRects(struct ui_tt_pack_context *spc,
    struct ui_tt_fontinfo *info, struct ui_tt_pack_range *ranges,
    int num_ranges, struct ui_rp_rect *rects)
{
    int i,j,k;
    k = 0;

    for (i=0; i < num_ranges; ++i) {
        float fh = ranges[i].font_size;
        float scale = (fh > 0) ? ui_tt_ScaleForPixelHeight(info, fh):
            ui_tt_ScaleForMappingEmToPixels(info, -fh);
        ranges[i].h_oversample = (unsigned char) spc->h_oversample;
        ranges[i].v_oversample = (unsigned char) spc->v_oversample;
        for (j=0; j < ranges[i].num_chars; ++j) {
            int x0,y0,x1,y1;
            int codepoint = ranges[i].first_unicode_codepoint_in_range ?
                ranges[i].first_unicode_codepoint_in_range + j :
                ranges[i].array_of_unicode_codepoints[j];

            int glyph = ui_tt_FindGlyphIndex(info, codepoint);
            ui_tt_GetGlyphBitmapBoxSubpixel(info,glyph, scale * (float)spc->h_oversample,
                scale * (float)spc->v_oversample, 0,0, &x0,&y0,&x1,&y1);
            rects[k].w = (ui_rp_coord) (x1-x0 + spc->padding + (int)spc->h_oversample-1);
            rects[k].h = (ui_rp_coord) (y1-y0 + spc->padding + (int)spc->v_oversample-1);
            ++k;
        }
    }
    return k;
}

UI_INTERN int
ui_tt_PackFontRangesRenderIntoRects(struct ui_tt_pack_context *spc,
    struct ui_tt_fontinfo *info, struct ui_tt_pack_range *ranges,
    int num_ranges, struct ui_rp_rect *rects, struct ui_allocator *alloc)
{
    int i,j,k, return_value = 1;
    /* save current values */
    int old_h_over = (int)spc->h_oversample;
    int old_v_over = (int)spc->v_oversample;
    /* rects array must be big enough to accommodate all characters in the given ranges */

    k = 0;
    for (i=0; i < num_ranges; ++i)
    {
        float fh = ranges[i].font_size;
        float recip_h,recip_v,sub_x,sub_y;
        float scale = fh > 0 ? ui_tt_ScaleForPixelHeight(info, fh):
            ui_tt_ScaleForMappingEmToPixels(info, -fh);

        spc->h_oversample = ranges[i].h_oversample;
        spc->v_oversample = ranges[i].v_oversample;

        recip_h = 1.0f / (float)spc->h_oversample;
        recip_v = 1.0f / (float)spc->v_oversample;

        sub_x = ui_tt__oversample_shift((int)spc->h_oversample);
        sub_y = ui_tt__oversample_shift((int)spc->v_oversample);

        for (j=0; j < ranges[i].num_chars; ++j)
        {
            struct ui_rp_rect *r = &rects[k];
            if (r->was_packed)
            {
                struct ui_tt_packedchar *bc = &ranges[i].chardata_for_range[j];
                int advance, lsb, x0,y0,x1,y1;
                int codepoint = ranges[i].first_unicode_codepoint_in_range ?
                    ranges[i].first_unicode_codepoint_in_range + j :
                    ranges[i].array_of_unicode_codepoints[j];
                int glyph = ui_tt_FindGlyphIndex(info, codepoint);
                ui_rp_coord pad = (ui_rp_coord) spc->padding;

                /* pad on left and top */
                r->x = (ui_rp_coord)((int)r->x + (int)pad);
                r->y = (ui_rp_coord)((int)r->y + (int)pad);
                r->w = (ui_rp_coord)((int)r->w - (int)pad);
                r->h = (ui_rp_coord)((int)r->h - (int)pad);

                ui_tt_GetGlyphHMetrics(info, glyph, &advance, &lsb);
                ui_tt_GetGlyphBitmapBox(info, glyph, scale * (float)spc->h_oversample,
                        (scale * (float)spc->v_oversample), &x0,&y0,&x1,&y1);
                ui_tt_MakeGlyphBitmapSubpixel(info, spc->pixels + r->x + r->y*spc->stride_in_bytes,
                    (int)(r->w - spc->h_oversample+1), (int)(r->h - spc->v_oversample+1),
                    spc->stride_in_bytes, scale * (float)spc->h_oversample,
                    scale * (float)spc->v_oversample, 0,0, glyph, alloc);

                if (spc->h_oversample > 1)
                   ui_tt__h_prefilter(spc->pixels + r->x + r->y*spc->stride_in_bytes,
                        r->w, r->h, spc->stride_in_bytes, (int)spc->h_oversample);

                if (spc->v_oversample > 1)
                   ui_tt__v_prefilter(spc->pixels + r->x + r->y*spc->stride_in_bytes,
                        r->w, r->h, spc->stride_in_bytes, (int)spc->v_oversample);

                bc->x0       = (ui_ushort)  r->x;
                bc->y0       = (ui_ushort)  r->y;
                bc->x1       = (ui_ushort) (r->x + r->w);
                bc->y1       = (ui_ushort) (r->y + r->h);
                bc->xadvance = scale * (float)advance;
                bc->xoff     = (float)  x0 * recip_h + sub_x;
                bc->yoff     = (float)  y0 * recip_v + sub_y;
                bc->xoff2    = ((float)x0 + r->w) * recip_h + sub_x;
                bc->yoff2    = ((float)y0 + r->h) * recip_v + sub_y;
            } else {
                return_value = 0; /* if any fail, report failure */
            }
            ++k;
        }
    }
    /* restore original values */
    spc->h_oversample = (unsigned int)old_h_over;
    spc->v_oversample = (unsigned int)old_v_over;
    return return_value;
}

UI_INTERN void
ui_tt_GetPackedQuad(struct ui_tt_packedchar *chardata, int pw, int ph,
    int char_index, float *xpos, float *ypos, struct ui_tt_aligned_quad *q,
    int align_to_integer)
{
    float ipw = 1.0f / (float)pw, iph = 1.0f / (float)ph;
    struct ui_tt_packedchar *b = (struct ui_tt_packedchar*)(chardata + char_index);
    if (align_to_integer) {
        int tx = ui_ifloorf((*xpos + b->xoff) + 0.5f);
        int ty = ui_ifloorf((*ypos + b->yoff) + 0.5f);

        float x = (float)tx;
        float y = (float)ty;

        q->x0 = x;
        q->y0 = y;
        q->x1 = x + b->xoff2 - b->xoff;
        q->y1 = y + b->yoff2 - b->yoff;
    } else {
        q->x0 = *xpos + b->xoff;
        q->y0 = *ypos + b->yoff;
        q->x1 = *xpos + b->xoff2;
        q->y1 = *ypos + b->yoff2;
    }
    q->s0 = b->x0 * ipw;
    q->t0 = b->y0 * iph;
    q->s1 = b->x1 * ipw;
    q->t1 = b->y1 * iph;
    *xpos += b->xadvance;
}

/* -------------------------------------------------------------
 *
 *                          FONT BAKING
 *
 * --------------------------------------------------------------*/
struct ui_font_bake_data {
    struct ui_tt_fontinfo info;
    struct ui_rp_rect *rects;
    struct ui_tt_pack_range *ranges;
    ui_rune range_count;
};

struct ui_font_baker {
    struct ui_allocator alloc;
    struct ui_tt_pack_context spc;
    struct ui_font_bake_data *build;
    struct ui_tt_packedchar *packed_chars;
    struct ui_rp_rect *rects;
    struct ui_tt_pack_range *ranges;
};

UI_GLOBAL const ui_size ui_rect_align = UI_ALIGNOF(struct ui_rp_rect);
UI_GLOBAL const ui_size ui_range_align = UI_ALIGNOF(struct ui_tt_pack_range);
UI_GLOBAL const ui_size ui_char_align = UI_ALIGNOF(struct ui_tt_packedchar);
UI_GLOBAL const ui_size ui_build_align = UI_ALIGNOF(struct ui_font_bake_data);
UI_GLOBAL const ui_size ui_baker_align = UI_ALIGNOF(struct ui_font_baker);

UI_INTERN int
ui_range_count(const ui_rune *range)
{
    const ui_rune *iter = range;
    UI_ASSERT(range);
    if (!range) return 0;
    while (*(iter++) != 0);
    return (iter == range) ? 0 : (int)((iter - range)/2);
}

UI_INTERN int
ui_range_glyph_count(const ui_rune *range, int count)
{
    int i = 0;
    int total_glyphs = 0;
    for (i = 0; i < count; ++i) {
        int diff;
        ui_rune f = range[(i*2)+0];
        ui_rune t = range[(i*2)+1];
        UI_ASSERT(t >= f);
        diff = (int)((t - f) + 1);
        total_glyphs += diff;
    }
    return total_glyphs;
}

UI_API const ui_rune*
ui_font_default_glyph_ranges(void)
{
    UI_STORAGE const ui_rune ranges[] = {0x0020, 0x00FF, 0};
    return ranges;
}

UI_API const ui_rune*
ui_font_chinese_glyph_ranges(void)
{
    UI_STORAGE const ui_rune ranges[] = {
        0x0020, 0x00FF,
        0x3000, 0x30FF,
        0x31F0, 0x31FF,
        0xFF00, 0xFFEF,
        0x4e00, 0x9FAF,
        0
    };
    return ranges;
}

UI_API const ui_rune*
ui_font_cyrillic_glyph_ranges(void)
{
    UI_STORAGE const ui_rune ranges[] = {
        0x0020, 0x00FF,
        0x0400, 0x052F,
        0x2DE0, 0x2DFF,
        0xA640, 0xA69F,
        0
    };
    return ranges;
}

UI_API const ui_rune*
ui_font_korean_glyph_ranges(void)
{
    UI_STORAGE const ui_rune ranges[] = {
        0x0020, 0x00FF,
        0x3131, 0x3163,
        0xAC00, 0xD79D,
        0
    };
    return ranges;
}

UI_INTERN void
ui_font_baker_memory(ui_size *temp, int *glyph_count,
    struct ui_font_config *config_list, int count)
{
    int range_count = 0;
    int total_range_count = 0;
    struct ui_font_config *iter;

    UI_ASSERT(config_list);
    UI_ASSERT(glyph_count);
    if (!config_list) {
        *temp = 0;
        *glyph_count = 0;
        return;
    }

    *glyph_count = 0;
    if (!config_list->range)
        config_list->range = ui_font_default_glyph_ranges();
    for (iter = config_list; iter; iter = iter->next) {
        range_count = ui_range_count(iter->range);
        total_range_count += range_count;
        *glyph_count += ui_range_glyph_count(iter->range, range_count);
    }

    *temp = (ui_size)*glyph_count * sizeof(struct ui_rp_rect);
    *temp += (ui_size)total_range_count * sizeof(struct ui_tt_pack_range);
    *temp += (ui_size)*glyph_count * sizeof(struct ui_tt_packedchar);
    *temp += (ui_size)count * sizeof(struct ui_font_bake_data);
    *temp += sizeof(struct ui_font_baker);
    *temp += ui_rect_align + ui_range_align + ui_char_align;
    *temp += ui_build_align + ui_baker_align;
}

UI_INTERN struct ui_font_baker*
ui_font_baker(void *memory, int glyph_count, int count, struct ui_allocator *alloc)
{
    struct ui_font_baker *baker;
    if (!memory) return 0;
    /* setup baker inside a memory block  */
    baker = (struct ui_font_baker*)UI_ALIGN_PTR(memory, ui_baker_align);
    baker->build = (struct ui_font_bake_data*)UI_ALIGN_PTR((baker + 1), ui_build_align);
    baker->packed_chars = (struct ui_tt_packedchar*)UI_ALIGN_PTR((baker->build + count), ui_char_align);
    baker->rects = (struct ui_rp_rect*)UI_ALIGN_PTR((baker->packed_chars + glyph_count), ui_rect_align);
    baker->ranges = (struct ui_tt_pack_range*)UI_ALIGN_PTR((baker->rects + glyph_count), ui_range_align);
    baker->alloc = *alloc;
    return baker;
}

UI_INTERN int
ui_font_bake_pack(struct ui_font_baker *baker,
    ui_size *image_memory, int *width, int *height, struct ui_recti *custom,
    const struct ui_font_config *config_list, int count,
    struct ui_allocator *alloc)
{
    UI_STORAGE const ui_size max_height = 1024 * 32;
    const struct ui_font_config *config_iter;
    int total_glyph_count = 0;
    int total_range_count = 0;
    int range_count = 0;
    int i = 0;

    UI_ASSERT(image_memory);
    UI_ASSERT(width);
    UI_ASSERT(height);
    UI_ASSERT(config_list);
    UI_ASSERT(count);
    UI_ASSERT(alloc);

    if (!image_memory || !width || !height || !config_list || !count) return ui_false;
    for (config_iter = config_list; config_iter; config_iter = config_iter->next) {
        range_count = ui_range_count(config_iter->range);
        total_range_count += range_count;
        total_glyph_count += ui_range_glyph_count(config_iter->range, range_count);
    }

    /* setup font baker from temporary memory */
    for (config_iter = config_list; config_iter; config_iter = config_iter->next) {
        const struct ui_font_config *cfg = config_iter;
        if (!ui_tt_InitFont(&baker->build[i++].info, (const unsigned char*)cfg->ttf_blob, 0))
            return ui_false;
    }

    *height = 0;
    *width = (total_glyph_count > 1000) ? 1024 : 512;
    ui_tt_PackBegin(&baker->spc, 0, (int)*width, (int)max_height, 0, 1, alloc);
    {
        int input_i = 0;
        int range_n = 0;
        int rect_n = 0;
        int char_n = 0;

        /* pack custom user data first so it will be in the upper left corner*/
        if (custom) {
            struct ui_rp_rect custom_space;
            ui_zero(&custom_space, sizeof(custom_space));
            custom_space.w = (ui_rp_coord)((custom->w * 2) + 1);
            custom_space.h = (ui_rp_coord)(custom->h + 1);

            ui_tt_PackSetOversampling(&baker->spc, 1, 1);
            ui_rp_pack_rects((struct ui_rp_context*)baker->spc.pack_info, &custom_space, 1);
            *height = UI_MAX(*height, (int)(custom_space.y + custom_space.h));

            custom->x = (short)custom_space.x;
            custom->y = (short)custom_space.y;
            custom->w = (short)custom_space.w;
            custom->h = (short)custom_space.h;
        }

        /* first font pass: pack all glyphs */
        for (input_i = 0, config_iter = config_list; input_i < count && config_iter;
            input_i++, config_iter = config_iter->next)
        {
            int n = 0;
            int glyph_count;
            const ui_rune *in_range;
            const struct ui_font_config *cfg = config_iter;
            struct ui_font_bake_data *tmp = &baker->build[input_i];

            /* count glyphs + ranges in current font */
            glyph_count = 0; range_count = 0;
            for (in_range = cfg->range; in_range[0] && in_range[1]; in_range += 2) {
                glyph_count += (int)(in_range[1] - in_range[0]) + 1;
                range_count++;
            }

            /* setup ranges  */
            tmp->ranges = baker->ranges + range_n;
            tmp->range_count = (ui_rune)range_count;
            range_n += range_count;
            for (i = 0; i < range_count; ++i) {
                in_range = &cfg->range[i * 2];
                tmp->ranges[i].font_size = cfg->size;
                tmp->ranges[i].first_unicode_codepoint_in_range = (int)in_range[0];
                tmp->ranges[i].num_chars = (int)(in_range[1]- in_range[0]) + 1;
                tmp->ranges[i].chardata_for_range = baker->packed_chars + char_n;
                char_n += tmp->ranges[i].num_chars;
            }

            /* pack */
            tmp->rects = baker->rects + rect_n;
            rect_n += glyph_count;
            ui_tt_PackSetOversampling(&baker->spc, cfg->oversample_h, cfg->oversample_v);
            n = ui_tt_PackFontRangesGatherRects(&baker->spc, &tmp->info,
                tmp->ranges, (int)tmp->range_count, tmp->rects);
            ui_rp_pack_rects((struct ui_rp_context*)baker->spc.pack_info, tmp->rects, (int)n);

            /* texture height */
            for (i = 0; i < n; ++i) {
                if (tmp->rects[i].was_packed)
                    *height = UI_MAX(*height, tmp->rects[i].y + tmp->rects[i].h);
            }
        }
        UI_ASSERT(rect_n == total_glyph_count);
        UI_ASSERT(char_n == total_glyph_count);
        UI_ASSERT(range_n == total_range_count);
    }
    *height = (int)ui_round_up_pow2((ui_uint)*height);
    *image_memory = (ui_size)(*width) * (ui_size)(*height);
    return ui_true;
}

UI_INTERN void
ui_font_bake(struct ui_font_baker *baker, void *image_memory, int width, int height,
    struct ui_font_glyph *glyphs, int glyphs_count,
    const struct ui_font_config *config_list, int font_count)
{
    int input_i = 0;
    ui_rune glyph_n = 0;
    const struct ui_font_config *config_iter;

    UI_ASSERT(image_memory);
    UI_ASSERT(width);
    UI_ASSERT(height);
    UI_ASSERT(config_list);
    UI_ASSERT(baker);
    UI_ASSERT(font_count);
    UI_ASSERT(glyphs_count);
    if (!image_memory || !width || !height || !config_list ||
        !font_count || !glyphs || !glyphs_count)
        return;

    /* second font pass: render glyphs */
    ui_zero(image_memory, (ui_size)((ui_size)width * (ui_size)height));
    baker->spc.pixels = (unsigned char*)image_memory;
    baker->spc.height = (int)height;
    for (input_i = 0, config_iter = config_list; input_i < font_count && config_iter;
        ++input_i, config_iter = config_iter->next)
    {
        const struct ui_font_config *cfg = config_iter;
        struct ui_font_bake_data *tmp = &baker->build[input_i];
        ui_tt_PackSetOversampling(&baker->spc, cfg->oversample_h, cfg->oversample_v);
        ui_tt_PackFontRangesRenderIntoRects(&baker->spc, &tmp->info, tmp->ranges,
            (int)tmp->range_count, tmp->rects, &baker->alloc);
    }
    ui_tt_PackEnd(&baker->spc, &baker->alloc);

    /* third pass: setup font and glyphs */
    for (input_i = 0, config_iter = config_list; input_i < font_count && config_iter;
        ++input_i, config_iter = config_iter->next)
    {
        ui_size i = 0;
        int char_idx = 0;
        ui_rune glyph_count = 0;
        const struct ui_font_config *cfg = config_iter;
        struct ui_font_bake_data *tmp = &baker->build[input_i];
        struct ui_baked_font *dst_font = cfg->font;

        float font_scale = ui_tt_ScaleForPixelHeight(&tmp->info, cfg->size);
        int unscaled_ascent, unscaled_descent, unscaled_line_gap;
        ui_tt_GetFontVMetrics(&tmp->info, &unscaled_ascent, &unscaled_descent,
                                &unscaled_line_gap);

        /* fill baked font */
        if (!cfg->merge_mode) {
            dst_font->ranges = cfg->range;
            dst_font->height = cfg->size;
            dst_font->ascent = ((float)unscaled_ascent * font_scale);
            dst_font->descent = ((float)unscaled_descent * font_scale);
            dst_font->glyph_offset = glyph_n;
        }

        /* fill own baked font glyph array */
        for (i = 0; i < tmp->range_count; ++i)
        {
            struct ui_tt_pack_range *range = &tmp->ranges[i];
            for (char_idx = 0; char_idx < range->num_chars; char_idx++)
            {
                ui_rune codepoint = 0;
                float dummy_x = 0, dummy_y = 0;
                struct ui_tt_aligned_quad q;
                struct ui_font_glyph *glyph;

                /* query glyph bounds from stb_truetype */
                const struct ui_tt_packedchar *pc = &range->chardata_for_range[char_idx];
                if (!pc->x0 && !pc->x1 && !pc->y0 && !pc->y1) continue;
                codepoint = (ui_rune)(range->first_unicode_codepoint_in_range + char_idx);
                ui_tt_GetPackedQuad(range->chardata_for_range, (int)width,
                    (int)height, char_idx, &dummy_x, &dummy_y, &q, 0);

                /* fill own glyph type with data */
                glyph = &glyphs[dst_font->glyph_offset + (unsigned int)glyph_count];
                glyph->codepoint = codepoint;
                glyph->x0 = q.x0; glyph->y0 = q.y0;
                glyph->x1 = q.x1; glyph->y1 = q.y1;
                glyph->y0 += (dst_font->ascent + 0.5f);
                glyph->y1 += (dst_font->ascent + 0.5f);
                glyph->w = glyph->x1 - glyph->x0 + 0.5f;
                glyph->h = glyph->y1 - glyph->y0;

                if (cfg->coord_type == UI_COORD_PIXEL) {
                    glyph->u0 = q.s0 * (float)width;
                    glyph->v0 = q.t0 * (float)height;
                    glyph->u1 = q.s1 * (float)width;
                    glyph->v1 = q.t1 * (float)height;
                } else {
                    glyph->u0 = q.s0;
                    glyph->v0 = q.t0;
                    glyph->u1 = q.s1;
                    glyph->v1 = q.t1;
                }
                glyph->xadvance = (pc->xadvance + cfg->spacing.x);
                if (cfg->pixel_snap)
                    glyph->xadvance = (float)(int)(glyph->xadvance + 0.5f);
                glyph_count++;
            }
        }
        dst_font->glyph_count = glyph_count;
        glyph_n += dst_font->glyph_count;
    }
}

UI_INTERN void
ui_font_bake_custom_data(void *img_memory, int img_width, int img_height,
    struct ui_recti img_dst, const char *texture_data_mask, int tex_width,
    int tex_height, char white, char black)
{
    ui_byte *pixels;
    int y = 0;
    int x = 0;
    int n = 0;

    UI_ASSERT(img_memory);
    UI_ASSERT(img_width);
    UI_ASSERT(img_height);
    UI_ASSERT(texture_data_mask);
    UI_UNUSED(tex_height);
    if (!img_memory || !img_width || !img_height || !texture_data_mask)
        return;

    pixels = (ui_byte*)img_memory;
    for (y = 0, n = 0; y < tex_height; ++y) {
        for (x = 0; x < tex_width; ++x, ++n) {
            const int off0 = ((img_dst.x + x) + (img_dst.y + y) * img_width);
            const int off1 = off0 + 1 + tex_width;
            pixels[off0] = (texture_data_mask[n] == white) ? 0xFF : 0x00;
            pixels[off1] = (texture_data_mask[n] == black) ? 0xFF : 0x00;
        }
    }
}

UI_INTERN void
ui_font_bake_convert(void *out_memory, int img_width, int img_height,
    const void *in_memory)
{
    int n = 0;
    ui_rune *dst;
    const ui_byte *src;

    UI_ASSERT(out_memory);
    UI_ASSERT(in_memory);
    UI_ASSERT(img_width);
    UI_ASSERT(img_height);
    if (!out_memory || !in_memory || !img_height || !img_width) return;

    dst = (ui_rune*)out_memory;
    src = (const ui_byte*)in_memory;
    for (n = (int)(img_width * img_height); n > 0; n--)
        *dst++ = ((ui_rune)(*src++) << 24) | 0x00FFFFFF;
}

/* -------------------------------------------------------------
 *
 *                          FONT
 *
 * --------------------------------------------------------------*/
UI_INTERN float
ui_font_text_width(ui_handle handle, float height, const char *text, int len)
{
    ui_rune unicode;
    int text_len  = 0;
    float text_width = 0;
    int glyph_len = 0;
    float scale = 0;

    struct ui_font *font = (struct ui_font*)handle.ptr;
    UI_ASSERT(font);
    UI_ASSERT(font->glyphs);
    if (!font || !text || !len)
        return 0;

    scale = height/font->info.height;
    glyph_len = text_len = ui_utf_decode(text, &unicode, (int)len);
    if (!glyph_len) return 0;
    while (text_len <= (int)len && glyph_len) {
        const struct ui_font_glyph *g;
        if (unicode == UI_UTF_INVALID) break;

        /* query currently drawn glyph information */
        g = ui_font_find_glyph(font, unicode);
        text_width += g->xadvance * scale;

        /* offset next glyph */
        glyph_len = ui_utf_decode(text + text_len, &unicode, (int)len - text_len);
        text_len += glyph_len;
    }
    return text_width;
}

#ifdef UI_INCLUDE_VERTEX_BUFFER_OUTPUT
UI_INTERN void
ui_font_query_font_glyph(ui_handle handle, float height,
    struct ui_user_font_glyph *glyph, ui_rune codepoint, ui_rune next_codepoint)
{
    float scale;
    const struct ui_font_glyph *g;
    struct ui_font *font;

    UI_ASSERT(glyph);
    UI_UNUSED(next_codepoint);

    font = (struct ui_font*)handle.ptr;
    UI_ASSERT(font);
    UI_ASSERT(font->glyphs);
    if (!font || !glyph)
        return;

    scale = height/font->info.height;
    g = ui_font_find_glyph(font, codepoint);
    glyph->width = (g->x1 - g->x0) * scale;
    glyph->height = (g->y1 - g->y0) * scale;
    glyph->offset = ui_vec2(g->x0 * scale, g->y0 * scale);
    glyph->xadvance = (g->xadvance * scale);
    glyph->uv[0] = ui_vec2(g->u0, g->v0);
    glyph->uv[1] = ui_vec2(g->u1, g->v1);
}
#endif

UI_API const struct ui_font_glyph*
ui_font_find_glyph(struct ui_font *font, ui_rune unicode)
{
    int i = 0;
    int count;
    int total_glyphs = 0;
    const struct ui_font_glyph *glyph = 0;

    UI_ASSERT(font);
    UI_ASSERT(font->glyphs);
    UI_ASSERT(font->info.ranges);
    if (!font || !font->glyphs) return 0;

    glyph = font->fallback;
    count = ui_range_count(font->info.ranges);
    for (i = 0; i < count; ++i) {
        int diff;
        ui_rune f = font->info.ranges[(i*2)+0];
        ui_rune t = font->info.ranges[(i*2)+1];
        diff = (int)((t - f) + 1);
        if (unicode >= f && unicode <= t)
            return &font->glyphs[((ui_rune)total_glyphs + (unicode - f))];
        total_glyphs += diff;
    }
    return glyph;
}

UI_INTERN void
ui_font_init(struct ui_font *font, float pixel_height,
    ui_rune fallback_codepoint, struct ui_font_glyph *glyphs,
    const struct ui_baked_font *baked_font, ui_handle atlas)
{
    struct ui_baked_font baked;
    UI_ASSERT(font);
    UI_ASSERT(glyphs);
    UI_ASSERT(baked_font);
    if (!font || !glyphs || !baked_font)
        return;

    baked = *baked_font;
    font->info = baked;
    font->scale = (float)pixel_height / (float)font->info.height;
    font->glyphs = &glyphs[baked_font->glyph_offset];
    font->texture = atlas;
    font->fallback_codepoint = fallback_codepoint;
    font->fallback = ui_font_find_glyph(font, fallback_codepoint);

    font->handle.height = font->info.height * font->scale;
    font->handle.width = ui_font_text_width;
    font->handle.userdata.ptr = font;
#ifdef UI_INCLUDE_VERTEX_BUFFER_OUTPUT
    font->handle.query = ui_font_query_font_glyph;
    font->handle.texture = font->texture;
#endif
}

/* ---------------------------------------------------------------------------
 *
 *                          DEFAULT FONT
 *
 * ProggyClean.ttf
 * Copyright (c) 2004, 2005 Tristan Grimmer
 * MIT license (see License.txt in http://www.upperbounds.net/download/ProggyClean.ttf.zip)
 * Download and more information at http://upperbounds.net
 *-----------------------------------------------------------------------------*/
#ifdef UI_INCLUDE_DEFAULT_FONT

 #ifdef __clang__
#pragma clang diagnostic push

#pragma clang diagnostic ignored "-Woverlength-strings"
#elif defined(__GNUC__) || defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Woverlength-strings"
#endif

UI_GLOBAL const char ui_proggy_clean_ttf_compressed_data_base85[11980+1] =
    "7])#######hV0qs'/###[),##/l:$#Q6>##5[n42>c-TH`->>#/e>11NNV=Bv(*:.F?uu#(gRU.o0XGH`$vhLG1hxt9?W`#,5LsCp#-i>.r$<$6pD>Lb';9Crc6tgXmKVeU2cD4Eo3R/"
    "2*>]b(MC;$jPfY.;h^`IWM9<Lh2TlS+f-s$o6Q<BWH`YiU.xfLq$N;$0iR/GX:U(jcW2p/W*q?-qmnUCI;jHSAiFWM.R*kU@C=GH?a9wp8f$e.-4^Qg1)Q-GL(lf(r/7GrRgwV%MS=C#"
    "`8ND>Qo#t'X#(v#Y9w0#1D$CIf;W'#pWUPXOuxXuU(H9M(1<q-UE31#^-V'8IRUo7Qf./L>=Ke$$'5F%)]0^#0X@U.a<r:QLtFsLcL6##lOj)#.Y5<-R&KgLwqJfLgN&;Q?gI^#DY2uL"
    "i@^rMl9t=cWq6##weg>$FBjVQTSDgEKnIS7EM9>ZY9w0#L;>>#Mx&4Mvt//L[MkA#W@lK.N'[0#7RL_&#w+F%HtG9M#XL`N&.,GM4Pg;-<nLENhvx>-VsM.M0rJfLH2eTM`*oJMHRC`N"
    "kfimM2J,W-jXS:)r0wK#@Fge$U>`w'N7G#$#fB#$E^$#:9:hk+eOe--6x)F7*E%?76%^GMHePW-Z5l'&GiF#$956:rS?dA#fiK:)Yr+`&#0j@'DbG&#^$PG.Ll+DNa<XCMKEV*N)LN/N"
    "*b=%Q6pia-Xg8I$<MR&,VdJe$<(7G;Ckl'&hF;;$<_=X(b.RS%%)###MPBuuE1V:v&cX&#2m#(&cV]`k9OhLMbn%s$G2,B$BfD3X*sp5#l,$R#]x_X1xKX%b5U*[r5iMfUo9U`N99hG)"
    "tm+/Us9pG)XPu`<0s-)WTt(gCRxIg(%6sfh=ktMKn3j)<6<b5Sk_/0(^]AaN#(p/L>&VZ>1i%h1S9u5o@YaaW$e+b<TWFn/Z:Oh(Cx2$lNEoN^e)#CFY@@I;BOQ*sRwZtZxRcU7uW6CX"
    "ow0i(?$Q[cjOd[P4d)]>ROPOpxTO7Stwi1::iB1q)C_=dV26J;2,]7op$]uQr@_V7$q^%lQwtuHY]=DX,n3L#0PHDO4f9>dC@O>HBuKPpP*E,N+b3L#lpR/MrTEH.IAQk.a>D[.e;mc."
    "x]Ip.PH^'/aqUO/$1WxLoW0[iLA<QT;5HKD+@qQ'NQ(3_PLhE48R.qAPSwQ0/WK?Z,[x?-J;jQTWA0X@KJ(_Y8N-:/M74:/-ZpKrUss?d#dZq]DAbkU*JqkL+nwX@@47`5>w=4h(9.`G"
    "CRUxHPeR`5Mjol(dUWxZa(>STrPkrJiWx`5U7F#.g*jrohGg`cg:lSTvEY/EV_7H4Q9[Z%cnv;JQYZ5q.l7Zeas:HOIZOB?G<Nald$qs]@]L<J7bR*>gv:[7MI2k).'2($5FNP&EQ(,)"
    "U]W]+fh18.vsai00);D3@4ku5P?DP8aJt+;qUM]=+b'8@;mViBKx0DE[-auGl8:PJ&Dj+M6OC]O^((##]`0i)drT;-7X`=-H3[igUnPG-NZlo.#k@h#=Ork$m>a>$-?Tm$UV(?#P6YY#"
    "'/###xe7q.73rI3*pP/$1>s9)W,JrM7SN]'/4C#v$U`0#V.[0>xQsH$fEmPMgY2u7Kh(G%siIfLSoS+MK2eTM$=5,M8p`A.;_R%#u[K#$x4AG8.kK/HSB==-'Ie/QTtG?-.*^N-4B/ZM"
    "_3YlQC7(p7q)&](`6_c)$/*JL(L-^(]$wIM`dPtOdGA,U3:w2M-0<q-]L_?^)1vw'.,MRsqVr.L;aN&#/EgJ)PBc[-f>+WomX2u7lqM2iEumMTcsF?-aT=Z-97UEnXglEn1K-bnEO`gu"
    "Ft(c%=;Am_Qs@jLooI&NX;]0#j4#F14;gl8-GQpgwhrq8'=l_f-b49'UOqkLu7-##oDY2L(te+Mch&gLYtJ,MEtJfLh'x'M=$CS-ZZ%P]8bZ>#S?YY#%Q&q'3^Fw&?D)UDNrocM3A76/"
    "/oL?#h7gl85[qW/NDOk%16ij;+:1a'iNIdb-ou8.P*w,v5#EI$TWS>Pot-R*H'-SEpA:g)f+O$%%`kA#G=8RMmG1&O`>to8bC]T&$,n.LoO>29sp3dt-52U%VM#q7'DHpg+#Z9%H[K<L"
    "%a2E-grWVM3@2=-k22tL]4$##6We'8UJCKE[d_=%wI;'6X-GsLX4j^SgJ$##R*w,vP3wK#iiW&#*h^D&R?jp7+/u&#(AP##XU8c$fSYW-J95_-Dp[g9wcO&#M-h1OcJlc-*vpw0xUX&#"
    "OQFKNX@QI'IoPp7nb,QU//MQ&ZDkKP)X<WSVL(68uVl&#c'[0#(s1X&xm$Y%B7*K:eDA323j998GXbA#pwMs-jgD$9QISB-A_(aN4xoFM^@C58D0+Q+q3n0#3U1InDjF682-SjMXJK)("
    "h$hxua_K]ul92%'BOU&#BRRh-slg8KDlr:%L71Ka:.A;%YULjDPmL<LYs8i#XwJOYaKPKc1h:'9Ke,g)b),78=I39B;xiY$bgGw-&.Zi9InXDuYa%G*f2Bq7mn9^#p1vv%#(Wi-;/Z5h"
    "o;#2:;%d&#x9v68C5g?ntX0X)pT`;%pB3q7mgGN)3%(P8nTd5L7GeA-GL@+%J3u2:(Yf>et`e;)f#Km8&+DC$I46>#Kr]]u-[=99tts1.qb#q72g1WJO81q+eN'03'eM>&1XxY-caEnO"
    "j%2n8)),?ILR5^.Ibn<-X-Mq7[a82Lq:F&#ce+S9wsCK*x`569E8ew'He]h:sI[2LM$[guka3ZRd6:t%IG:;$%YiJ:Nq=?eAw;/:nnDq0(CYcMpG)qLN4$##&J<j$UpK<Q4a1]MupW^-"
    "sj_$%[HK%'F####QRZJ::Y3EGl4'@%FkiAOg#p[##O`gukTfBHagL<LHw%q&OV0##F=6/:chIm0@eCP8X]:kFI%hl8hgO@RcBhS-@Qb$%+m=hPDLg*%K8ln(wcf3/'DW-$.lR?n[nCH-"
    "eXOONTJlh:.RYF%3'p6sq:UIMA945&^HFS87@$EP2iG<-lCO$%c`uKGD3rC$x0BL8aFn--`ke%#HMP'vh1/R&O_J9'um,.<tx[@%wsJk&bUT2`0uMv7gg#qp/ij.L56'hl;.s5CUrxjO"
    "M7-##.l+Au'A&O:-T72L]P`&=;ctp'XScX*rU.>-XTt,%OVU4)S1+R-#dg0/Nn?Ku1^0f$B*P:Rowwm-`0PKjYDDM'3]d39VZHEl4,.j']Pk-M.h^&:0FACm$maq-&sgw0t7/6(^xtk%"
    "LuH88Fj-ekm>GA#_>568x6(OFRl-IZp`&b,_P'$M<Jnq79VsJW/mWS*PUiq76;]/NM_>hLbxfc$mj`,O;&%W2m`Zh:/)Uetw:aJ%]K9h:TcF]u_-Sj9,VK3M.*'&0D[Ca]J9gp8,kAW]"
    "%(?A%R$f<->Zts'^kn=-^@c4%-pY6qI%J%1IGxfLU9CP8cbPlXv);C=b),<2mOvP8up,UVf3839acAWAW-W?#ao/^#%KYo8fRULNd2.>%m]UK:n%r$'sw]J;5pAoO_#2mO3n,'=H5(et"
    "Hg*`+RLgv>=4U8guD$I%D:W>-r5V*%j*W:Kvej.Lp$<M-SGZ':+Q_k+uvOSLiEo(<aD/K<CCc`'Lx>'?;++O'>()jLR-^u68PHm8ZFWe+ej8h:9r6L*0//c&iH&R8pRbA#Kjm%upV1g:"
    "a_#Ur7FuA#(tRh#.Y5K+@?3<-8m0$PEn;J:rh6?I6uG<-`wMU'ircp0LaE_OtlMb&1#6T.#FDKu#1Lw%u%+GM+X'e?YLfjM[VO0MbuFp7;>Q&#WIo)0@F%q7c#4XAXN-U&VB<HFF*qL("
    "$/V,;(kXZejWO`<[5?\?ewY(*9=%wDc;,u<'9t3W-(H1th3+G]ucQ]kLs7df($/*JL]@*t7Bu_G3_7mp7<iaQjO@.kLg;x3B0lqp7Hf,^Ze7-##@/c58Mo(3;knp0%)A7?-W+eI'o8)b<"
    "nKnw'Ho8C=Y>pqB>0ie&jhZ[?iLR@@_AvA-iQC(=ksRZRVp7`.=+NpBC%rh&3]R:8XDmE5^V8O(x<<aG/1N$#FX$0V5Y6x'aErI3I$7x%E`v<-BY,)%-?Psf*l?%C3.mM(=/M0:JxG'?"
    "7WhH%o'a<-80g0NBxoO(GH<dM]n.+%q@jH?f.UsJ2Ggs&4<-e47&Kl+f//9@`b+?.TeN_&B8Ss?v;^Trk;f#YvJkl&w$]>-+k?'(<S:68tq*WoDfZu';mM?8X[ma8W%*`-=;D.(nc7/;"
    ")g:T1=^J$&BRV(-lTmNB6xqB[@0*o.erM*<SWF]u2=st-*(6v>^](H.aREZSi,#1:[IXaZFOm<-ui#qUq2$##Ri;u75OK#(RtaW-K-F`S+cF]uN`-KMQ%rP/Xri.LRcB##=YL3BgM/3M"
    "D?@f&1'BW-)Ju<L25gl8uhVm1hL$##*8###'A3/LkKW+(^rWX?5W_8g)a(m&K8P>#bmmWCMkk&#TR`C,5d>g)F;t,4:@_l8G/5h4vUd%&%950:VXD'QdWoY-F$BtUwmfe$YqL'8(PWX("
    "P?^@Po3$##`MSs?DWBZ/S>+4%>fX,VWv/w'KD`LP5IbH;rTV>n3cEK8U#bX]l-/V+^lj3;vlMb&[5YQ8#pekX9JP3XUC72L,,?+Ni&co7ApnO*5UI,((W-i:$,kp'UDAO(G0Sq7MVjJs"
    "bIu)'Z,*[>br5fX^:FPAWr-m2KgL<LUN098kTF&#lvo58=/vjDo;.;)Ka*hLR#/k=rKbxuV`>Q_nN6'8uTG&#1T5g)uLv:873UpTLgH+#FgpH'_o1780Ph8KmxQJ8#H72L4@768@Tm&Q"
    "h4CB/5OvmA&,Q&QbUoi$a_%3M01H)4x7I^&KQVgtFnV+;[Pc>[m4k//,]1?#`VY[Jr*3&&slRfLiVZJ:]?=K3Sw=[$=uRB?3xk48@aeg<Z'<$#4H)6,>e0jT6'N#(q%.O=?2S]u*(m<-"
    "V8J'(1)G][68hW$5'q[GC&5j`TE?m'esFGNRM)j,ffZ?-qx8;->g4t*:CIP/[Qap7/9'#(1sao7w-.qNUdkJ)tCF&#B^;xGvn2r9FEPFFFcL@.iFNkTve$m%#QvQS8U@)2Z+3K:AKM5i"
    "sZ88+dKQ)W6>J%CL<KE>`.d*(B`-n8D9oK<Up]c$X$(,)M8Zt7/[rdkqTgl-0cuGMv'?>-XV1q['-5k'cAZ69e;D_?$ZPP&s^+7])$*$#@QYi9,5P&#9r+$%CE=68>K8r0=dSC%%(@p7"
    ".m7jilQ02'0-VWAg<a/''3u.=4L$Y)6k/K:_[3=&jvL<L0C/2'v:^;-DIBW,B4E68:kZ;%?8(Q8BH=kO65BW?xSG&#@uU,DS*,?.+(o(#1vCS8#CHF>TlGW'b)Tq7VT9q^*^$$.:&N@@"
    "$&)WHtPm*5_rO0&e%K&#-30j(E4#'Zb.o/(Tpm$>K'f@[PvFl,hfINTNU6u'0pao7%XUp9]5.>%h`8_=VYbxuel.NTSsJfLacFu3B'lQSu/m6-Oqem8T+oE--$0a/k]uj9EwsG>%veR*"
    "hv^BFpQj:K'#SJ,sB-'#](j.Lg92rTw-*n%@/;39rrJF,l#qV%OrtBeC6/,;qB3ebNW[?,Hqj2L.1NP&GjUR=1D8QaS3Up&@*9wP?+lo7b?@%'k4`p0Z$22%K3+iCZj?XJN4Nm&+YF]u"
    "@-W$U%VEQ/,,>>#)D<h#`)h0:<Q6909ua+&VU%n2:cG3FJ-%@Bj-DgLr`Hw&HAKjKjseK</xKT*)B,N9X3]krc12t'pgTV(Lv-tL[xg_%=M_q7a^x?7Ubd>#%8cY#YZ?=,`Wdxu/ae&#"
    "w6)R89tI#6@s'(6Bf7a&?S=^ZI_kS&ai`&=tE72L_D,;^R)7[$s<Eh#c&)q.MXI%#v9ROa5FZO%sF7q7Nwb&#ptUJ:aqJe$Sl68%.D###EC><?-aF&#RNQv>o8lKN%5/$(vdfq7+ebA#"
    "u1p]ovUKW&Y%q]'>$1@-[xfn$7ZTp7mM,G,Ko7a&Gu%G[RMxJs[0MM%wci.LFDK)(<c`Q8N)jEIF*+?P2a8g%)$q]o2aH8C&<SibC/q,(e:v;-b#6[$NtDZ84Je2KNvB#$P5?tQ3nt(0"
    "d=j.LQf./Ll33+(;q3L-w=8dX$#WF&uIJ@-bfI>%:_i2B5CsR8&9Z&#=mPEnm0f`<&c)QL5uJ#%u%lJj+D-r;BoF&#4DoS97h5g)E#o:&S4weDF,9^Hoe`h*L+_a*NrLW-1pG_&2UdB8"
    "6e%B/:=>)N4xeW.*wft-;$'58-ESqr<b?UI(_%@[P46>#U`'6AQ]m&6/`Z>#S?YY#Vc;r7U2&326d=w&H####?TZ`*4?&.MK?LP8Vxg>$[QXc%QJv92.(Db*B)gb*BM9dM*hJMAo*c&#"
    "b0v=Pjer]$gG&JXDf->'StvU7505l9$AFvgYRI^&<^b68?j#q9QX4SM'RO#&sL1IM.rJfLUAj221]d##DW=m83u5;'bYx,*Sl0hL(W;;$doB&O/TQ:(Z^xBdLjL<Lni;''X.`$#8+1GD"
    ":k$YUWsbn8ogh6rxZ2Z9]%nd+>V#*8U_72Lh+2Q8Cj0i:6hp&$C/:p(HK>T8Y[gHQ4`4)'$Ab(Nof%V'8hL&#<NEdtg(n'=S1A(Q1/I&4([%dM`,Iu'1:_hL>SfD07&6D<fp8dHM7/g+"
    "tlPN9J*rKaPct&?'uBCem^jn%9_K)<,C5K3s=5g&GmJb*[SYq7K;TRLGCsM-$$;S%:Y@r7AK0pprpL<Lrh,q7e/%KWK:50I^+m'vi`3?%Zp+<-d+$L-Sv:@.o19n$s0&39;kn;S%BSq*"
    "$3WoJSCLweV[aZ'MQIjO<7;X-X;&+dMLvu#^UsGEC9WEc[X(wI7#2.(F0jV*eZf<-Qv3J-c+J5AlrB#$p(H68LvEA'q3n0#m,[`*8Ft)FcYgEud]CWfm68,(aLA$@EFTgLXoBq/UPlp7"
    ":d[/;r_ix=:TF`S5H-b<LI&HY(K=h#)]Lk$K14lVfm:x$H<3^Ql<M`$OhapBuiup'D#L$Pb_`N*g]2e;X/Dtg,bsj&K#2[-:iYr'_wgH)NUIR8a1n#S?Yej'h8^58UbZd+^FKD*T@;6A"
    "7aQC[K8d-(v6GI$x:T<&'Gp5Uf>@M.*J:;$-rv29'M]8qMv-tLp,'886iaC=Hb*YJoKJ,(j%K=H`K.v9HggqBIiZu'QvBT.#=)0ukruV&.)3=(^1`o*Pj4<-<aN((^7('#Z0wK#5GX@7"
    "u][`*S^43933A4rl][`*O4CgLEl]v$1Q3AeF37dbXk,.)vj#x'd`;qgbQR%FW,2(?LO=s%Sc68%NP'##Aotl8x=BE#j1UD([3$M(]UI2LX3RpKN@;/#f'f/&_mt&F)XdF<9t4)Qa.*kT"
    "LwQ'(TTB9.xH'>#MJ+gLq9-##@HuZPN0]u:h7.T..G:;$/Usj(T7`Q8tT72LnYl<-qx8;-HV7Q-&Xdx%1a,hC=0u+HlsV>nuIQL-5<N?)NBS)QN*_I,?&)2'IM%L3I)X((e/dl2&8'<M"
    ":^#M*Q+[T.Xri.LYS3v%fF`68h;b-X[/En'CR.q7E)p'/kle2HM,u;^%OKC-N+Ll%F9CF<Nf'^#t2L,;27W:0O@6##U6W7:$rJfLWHj$#)woqBefIZ.PK<b*t7ed;p*_m;4ExK#h@&]>"
    "_>@kXQtMacfD.m-VAb8;IReM3$wf0''hra*so568'Ip&vRs849'MRYSp%:t:h5qSgwpEr$B>Q,;s(C#$)`svQuF$##-D,##,g68@2[T;.XSdN9Qe)rpt._K-#5wF)sP'##p#C0c%-Gb%"
    "hd+<-j'Ai*x&&HMkT]C'OSl##5RG[JXaHN;d'uA#x._U;.`PU@(Z3dt4r152@:v,'R.Sj'w#0<-;kPI)FfJ&#AYJ&#//)>-k=m=*XnK$>=)72L]0I%>.G690a:$##<,);?;72#?x9+d;"
    "^V'9;jY@;)br#q^YQpx:X#Te$Z^'=-=bGhLf:D6&bNwZ9-ZD#n^9HhLMr5G;']d&6'wYmTFmL<LD)F^%[tC'8;+9E#C$g%#5Y>q9wI>P(9mI[>kC-ekLC/R&CH+s'B;K-M6$EB%is00:"
    "+A4[7xks.LrNk0&E)wILYF@2L'0Nb$+pv<(2.768/FrY&h$^3i&@+G%JT'<-,v`3;_)I9M^AE]CN?Cl2AZg+%4iTpT3<n-&%H%b<FDj2M<hH=&Eh<2Len$b*aTX=-8QxN)k11IM1c^j%"
    "9s<L<NFSo)B?+<-(GxsF,^-Eh@$4dXhN$+#rxK8'je'D7k`e;)2pYwPA'_p9&@^18ml1^[@g4t*[JOa*[=Qp7(qJ_oOL^('7fB&Hq-:sf,sNj8xq^>$U4O]GKx'm9)b@p7YsvK3w^YR-"
    "CdQ*:Ir<($u&)#(&?L9Rg3H)4fiEp^iI9O8KnTj,]H?D*r7'M;PwZ9K0E^k&-cpI;.p/6_vwoFMV<->#%Xi.LxVnrU(4&8/P+:hLSKj$#U%]49t'I:rgMi'FL@a:0Y-uA[39',(vbma*"
    "hU%<-SRF`Tt:542R_VV$p@[p8DV[A,?1839FWdF<TddF<9Ah-6&9tWoDlh]&1SpGMq>Ti1O*H&#(AL8[_P%.M>v^-))qOT*F5Cq0`Ye%+$B6i:7@0IX<N+T+0MlMBPQ*Vj>SsD<U4JHY"
    "8kD2)2fU/M#$e.)T4,_=8hLim[&);?UkK'-x?'(:siIfL<$pFM`i<?%W(mGDHM%>iWP,##P`%/L<eXi:@Z9C.7o=@(pXdAO/NLQ8lPl+HPOQa8wD8=^GlPa8TKI1CjhsCTSLJM'/Wl>-"
    "S(qw%sf/@%#B6;/U7K]uZbi^Oc^2n<bhPmUkMw>%t<)'mEVE''n`WnJra$^TKvX5B>;_aSEK',(hwa0:i4G?.Bci.(X[?b*($,=-n<.Q%`(X=?+@Am*Js0&=3bh8K]mL<LoNs'6,'85`"
    "0?t/'_U59@]ddF<#LdF<eWdF<OuN/45rY<-L@&#+fm>69=Lb,OcZV/);TTm8VI;?%OtJ<(b4mq7M6:u?KRdF<gR@2L=FNU-<b[(9c/ML3m;Z[$oF3g)GAWqpARc=<ROu7cL5l;-[A]%/"
    "+fsd;l#SafT/f*W]0=O'$(Tb<[)*@e775R-:Yob%g*>l*:xP?Yb.5)%w_I?7uk5JC+FS(m#i'k.'a0i)9<7b'fs'59hq$*5Uhv##pi^8+hIEBF`nvo`;'l0.^S1<-wUK2/Coh58KKhLj"
    "M=SO*rfO`+qC`W-On.=AJ56>>i2@2LH6A:&5q`?9I3@@'04&p2/LVa*T-4<-i3;M9UvZd+N7>b*eIwg:CC)c<>nO&#<IGe;__.thjZl<%w(Wk2xmp4Q@I#I9,DF]u7-P=.-_:YJ]aS@V"
    "?6*C()dOp7:WL,b&3Rg/.cmM9&r^>$(>.Z-I&J(Q0Hd5Q%7Co-b`-c<N(6r@ip+AurK<m86QIth*#v;-OBqi+L7wDE-Ir8K['m+DDSLwK&/.?-V%U_%3:qKNu$_b*B-kp7NaD'QdWQPK"
    "Yq[@>P)hI;*_F]u`Rb[.j8_Q/<&>uu+VsH$sM9TA%?)(vmJ80),P7E>)tjD%2L=-t#fK[%`v=Q8<FfNkgg^oIbah*#8/Qt$F&:K*-(N/'+1vMB,u()-a.VUU*#[e%gAAO(S>WlA2);Sa"
    ">gXm8YB`1d@K#n]76-a$U,mF<fX]idqd)<3,]J7JmW4`6]uks=4-72L(jEk+:bJ0M^q-8Dm_Z?0olP1C9Sa&H[d&c$ooQUj]Exd*3ZM@-WGW2%s',B-_M%>%Ul:#/'xoFM9QX-$.QN'>"
    "[%$Z$uF6pA6Ki2O5:8w*vP1<-1`[G,)-m#>0`P&#eb#.3i)rtB61(o'$?X3B</R90;eZ]%Ncq;-Tl]#F>2Qft^ae_5tKL9MUe9b*sLEQ95C&`=G?@Mj=wh*'3E>=-<)Gt*Iw)'QG:`@I"
    "wOf7&]1i'S01B+Ev/Nac#9S;=;YQpg_6U`*kVY39xK,[/6Aj7:'1Bm-_1EYfa1+o&o4hp7KN_Q(OlIo@S%;jVdn0'1<Vc52=u`3^o-n1'g4v58Hj&6_t7$##?M)c<$bgQ_'SY((-xkA#"
    "Y(,p'H9rIVY-b,'%bCPF7.J<Up^,(dU1VY*5#WkTU>h19w,WQhLI)3S#f$2(eb,jr*b;3Vw]*7NH%$c4Vs,eD9>XW8?N]o+(*pgC%/72LV-u<Hp,3@e^9UB1J+ak9-TN/mhKPg+AJYd$"
    "MlvAF_jCK*.O-^(63adMT->W%iewS8W6m2rtCpo'RS1R84=@paTKt)>=%&1[)*vp'u+x,VrwN;&]kuO9JDbg=pO$J*.jVe;u'm0dr9l,<*wMK*Oe=g8lV_KEBFkO'oU]^=[-792#ok,)"
    "i]lR8qQ2oA8wcRCZ^7w/Njh;?.stX?Q1>S1q4Bn$)K1<-rGdO'$Wr.Lc.CG)$/*JL4tNR/,SVO3,aUw'DJN:)Ss;wGn9A32ijw%FL+Z0Fn.U9;reSq)bmI32U==5ALuG&#Vf1398/pVo"
    "1*c-(aY168o<`JsSbk-,1N;$>0:OUas(3:8Z972LSfF8eb=c-;>SPw7.6hn3m`9^Xkn(r.qS[0;T%&Qc=+STRxX'q1BNk3&*eu2;&8q$&x>Q#Q7^Tf+6<(d%ZVmj2bDi%.3L2n+4W'$P"
    "iDDG)g,r%+?,$@?uou5tSe2aN_AQU*<h`e-GI7)?OK2A.d7_c)?wQ5AS@DL3r#7fSkgl6-++D:'A,uq7SvlB$pcpH'q3n0#_%dY#xCpr-l<F0NR@-##FEV6NTF6##$l84N1w?AO>'IAO"
    "URQ##V^Fv-XFbGM7Fl(N<3DhLGF%q.1rC$#:T__&Pi68%0xi_&[qFJ(77j_&JWoF.V735&T,[R*:xFR*K5>>#`bW-?4Ne_&6Ne_&6Ne_&n`kr-#GJcM6X;uM6X;uM(.a..^2TkL%oR(#"
    ";u.T%fAr%4tJ8&><1=GHZ_+m9/#H1F^R#SC#*N=BA9(D?v[UiFY>>^8p,KKF.W]L29uLkLlu/+4T<XoIB&hx=T1PcDaB&;HH+-AFr?(m9HZV)FKS8JCw;SD=6[^/DZUL`EUDf]GGlG&>"
    "w$)F./^n3+rlo+DB;5sIYGNk+i1t-69Jg--0pao7Sm#K)pdHW&;LuDNH@H>#/X-TI(;P>#,Gc>#0Su>#4`1?#8lC?#<xU?#@.i?#D:%@#HF7@#LRI@#P_[@#Tkn@#Xw*A#]-=A#a9OA#"
    "d<F&#*;G##.GY##2Sl##6`($#:l:$#>xL$#B.`$#F:r$#JF.%#NR@%#R_R%#Vke%#Zww%#_-4&#3^Rh%Sflr-k'MS.o?.5/sWel/wpEM0%3'/1)K^f1-d>G21&v(35>V`39V7A4=onx4"
    "A1OY5EI0;6Ibgr6M$HS7Q<)58C5w,;WoA*#[%T*#`1g*#d=#+#hI5+#lUG+#pbY+#tnl+#x$),#&1;,#*=M,#.I`,#2Ur,#6b.-#;w[H#iQtA#m^0B#qjBB#uvTB##-hB#'9$C#+E6C#"
    "/QHC#3^ZC#7jmC#;v)D#?,<D#C8ND#GDaD#KPsD#O]/E#g1A5#KA*1#gC17#MGd;#8(02#L-d3#rWM4#Hga1#,<w0#T.j<#O#'2#CYN1#qa^:#_4m3#o@/=#eG8=#t8J5#`+78#4uI-#"
    "m3B2#SB[8#Q0@8#i[*9#iOn8#1Nm;#^sN9#qh<9#:=x-#P;K2#$%X9#bC+.#Rg;<#mN=.#MTF.#RZO.#2?)4#Y#(/#[)1/#b;L/#dAU/#0Sv;#lY$0#n`-0#sf60#(F24#wrH0#%/e0#"
    "TmD<#%JSMFove:CTBEXI:<eh2g)B,3h2^G3i;#d3jD>)4kMYD4lVu`4m`:&5niUA5@(A5BA1]PBB:xlBCC=2CDLXMCEUtiCf&0g2'tN?PGT4CPGT4CPGT4CPGT4CPGT4CPGT4CPGT4CP"
    "GT4CPGT4CPGT4CPGT4CPGT4CPGT4CP-qekC`.9kEg^+F$kwViFJTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5o,^<-28ZI'O?;xp"
    "O?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xp;7q-#lLYI:xvD=#";
#endif /* UI_INCLUDE_DEFAULT_FONT */

#define UI_CURSOR_DATA_W 90
#define UI_CURSOR_DATA_H 27
UI_GLOBAL const char ui_custom_cursor_data[UI_CURSOR_DATA_W * UI_CURSOR_DATA_H + 1] =
{
    "..-         -XXXXXXX-    X    -           X           -XXXXXXX          -          XXXXXXX"
    "..-         -X.....X-   X.X   -          X.X          -X.....X          -          X.....X"
    "---         -XXX.XXX-  X...X  -         X...X         -X....X           -           X....X"
    "X           -  X.X  - X.....X -        X.....X        -X...X            -            X...X"
    "XX          -  X.X  -X.......X-       X.......X       -X..X.X           -           X.X..X"
    "X.X         -  X.X  -XXXX.XXXX-       XXXX.XXXX       -X.X X.X          -          X.X X.X"
    "X..X        -  X.X  -   X.X   -          X.X          -XX   X.X         -         X.X   XX"
    "X...X       -  X.X  -   X.X   -    XX    X.X    XX    -      X.X        -        X.X      "
    "X....X      -  X.X  -   X.X   -   X.X    X.X    X.X   -       X.X       -       X.X       "
    "X.....X     -  X.X  -   X.X   -  X..X    X.X    X..X  -        X.X      -      X.X        "
    "X......X    -  X.X  -   X.X   - X...XXXXXX.XXXXXX...X -         X.X   XX-XX   X.X         "
    "X.......X   -  X.X  -   X.X   -X.....................X-          X.X X.X-X.X X.X          "
    "X........X  -  X.X  -   X.X   - X...XXXXXX.XXXXXX...X -           X.X..X-X..X.X           "
    "X.........X -XXX.XXX-   X.X   -  X..X    X.X    X..X  -            X...X-X...X            "
    "X..........X-X.....X-   X.X   -   X.X    X.X    X.X   -           X....X-X....X           "
    "X......XXXXX-XXXXXXX-   X.X   -    XX    X.X    XX    -          X.....X-X.....X          "
    "X...X..X    ---------   X.X   -          X.X          -          XXXXXXX-XXXXXXX          "
    "X..X X..X   -       -XXXX.XXXX-       XXXX.XXXX       ------------------------------------"
    "X.X  X..X   -       -X.......X-       X.......X       -    XX           XX    -           "
    "XX    X..X  -       - X.....X -        X.....X        -   X.X           X.X   -           "
    "      X..X          -  X...X  -         X...X         -  X..X           X..X  -           "
    "       XX           -   X.X   -          X.X          - X...XXXXXXXXXXXXX...X -           "
    "------------        -    X    -           X           -X.....................X-           "
    "                    ----------------------------------- X...XXXXXXXXXXXXX...X -           "
    "                                                      -  X..X           X..X  -           "
    "                                                      -   X.X           X.X   -           "
    "                                                      -    XX           XX    -           "
};

#ifdef __clang__
#pragma clang diagnostic pop
#elif defined(__GNUC__) || defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

UI_INTERN unsigned int
ui_decompress_length(unsigned char *input)
{
    return (unsigned int)((input[8] << 24) + (input[9] << 16) + (input[10] << 8) + input[11]);
}

UI_GLOBAL unsigned char *ui__barrier;
UI_GLOBAL unsigned char *ui__barrier2;
UI_GLOBAL unsigned char *ui__barrier3;
UI_GLOBAL unsigned char *ui__barrier4;
UI_GLOBAL unsigned char *ui__dout;

UI_INTERN void
ui__match(unsigned char *data, unsigned int length)
{
    /* INVERSE of memmove... write each byte before copying the next...*/
    UI_ASSERT (ui__dout + length <= ui__barrier);
    if (ui__dout + length > ui__barrier) { ui__dout += length; return; }
    if (data < ui__barrier4) { ui__dout = ui__barrier+1; return; }
    while (length--) *ui__dout++ = *data++;
}

UI_INTERN void
ui__lit(unsigned char *data, unsigned int length)
{
    UI_ASSERT (ui__dout + length <= ui__barrier);
    if (ui__dout + length > ui__barrier) { ui__dout += length; return; }
    if (data < ui__barrier2) { ui__dout = ui__barrier+1; return; }
    UI_MEMCPY(ui__dout, data, length);
    ui__dout += length;
}

#define ui__in2(x)   ((i[x] << 8) + i[(x)+1])
#define ui__in3(x)   ((i[x] << 16) + ui__in2((x)+1))
#define ui__in4(x)   ((i[x] << 24) + ui__in3((x)+1))

UI_INTERN unsigned char*
ui_decompress_token(unsigned char *i)
{
    if (*i >= 0x20) { /* use fewer if's for cases that expand small */
        if (*i >= 0x80)       ui__match(ui__dout-i[1]-1, (unsigned int)i[0] - 0x80 + 1), i += 2;
        else if (*i >= 0x40)  ui__match(ui__dout-(ui__in2(0) - 0x4000 + 1), (unsigned int)i[2]+1), i += 3;
        else /* *i >= 0x20 */ ui__lit(i+1, (unsigned int)i[0] - 0x20 + 1), i += 1 + (i[0] - 0x20 + 1);
    } else { /* more ifs for cases that expand large, since overhead is amortized */
        if (*i >= 0x18)       ui__match(ui__dout-(unsigned int)(ui__in3(0) - 0x180000 + 1), (unsigned int)i[3]+1), i += 4;
        else if (*i >= 0x10)  ui__match(ui__dout-(unsigned int)(ui__in3(0) - 0x100000 + 1), (unsigned int)ui__in2(3)+1), i += 5;
        else if (*i >= 0x08)  ui__lit(i+2, (unsigned int)ui__in2(0) - 0x0800 + 1), i += 2 + (ui__in2(0) - 0x0800 + 1);
        else if (*i == 0x07)  ui__lit(i+3, (unsigned int)ui__in2(1) + 1), i += 3 + (ui__in2(1) + 1);
        else if (*i == 0x06)  ui__match(ui__dout-(unsigned int)(ui__in3(1)+1), i[4]+1u), i += 5;
        else if (*i == 0x04)  ui__match(ui__dout-(unsigned int)(ui__in3(1)+1), (unsigned int)ui__in2(4)+1u), i += 6;
    }
    return i;
}

UI_INTERN unsigned int
ui_adler32(unsigned int adler32, unsigned char *buffer, unsigned int buflen)
{
    const unsigned long ADLER_MOD = 65521;
    unsigned long s1 = adler32 & 0xffff, s2 = adler32 >> 16;
    unsigned long blocklen, i;

    blocklen = buflen % 5552;
    while (buflen) {
        for (i=0; i + 7 < blocklen; i += 8) {
            s1 += buffer[0]; s2 += s1;
            s1 += buffer[1]; s2 += s1;
            s1 += buffer[2]; s2 += s1;
            s1 += buffer[3]; s2 += s1;
            s1 += buffer[4]; s2 += s1;
            s1 += buffer[5]; s2 += s1;
            s1 += buffer[6]; s2 += s1;
            s1 += buffer[7]; s2 += s1;
            buffer += 8;
        }
        for (; i < blocklen; ++i) {
            s1 += *buffer++; s2 += s1;
        }

        s1 %= ADLER_MOD; s2 %= ADLER_MOD;
        buflen -= (unsigned int)blocklen;
        blocklen = 5552;
    }
    return (unsigned int)(s2 << 16) + (unsigned int)s1;
}

UI_INTERN unsigned int
ui_decompress(unsigned char *output, unsigned char *i, unsigned int length)
{
    unsigned int olen;
    if (ui__in4(0) != 0x57bC0000) return 0;
    if (ui__in4(4) != 0)          return 0; /* error! stream is > 4GB */
    olen = ui_decompress_length(i);
    ui__barrier2 = i;
    ui__barrier3 = i+length;
    ui__barrier = output + olen;
    ui__barrier4 = output;
    i += 16;

    ui__dout = output;
    for (;;) {
        unsigned char *old_i = i;
        i = ui_decompress_token(i);
        if (i == old_i) {
            if (*i == 0x05 && i[1] == 0xfa) {
                UI_ASSERT(ui__dout == output + olen);
                if (ui__dout != output + olen) return 0;
                if (ui_adler32(1, output, olen) != (unsigned int) ui__in4(2))
                    return 0;
                return olen;
            } else {
                UI_ASSERT(0); /* NOTREACHED */
                return 0;
            }
        }
        UI_ASSERT(ui__dout <= output + olen);
        if (ui__dout > output + olen)
            return 0;
    }
}

UI_INTERN unsigned int
ui_decode_85_byte(char c)
{ return (unsigned int)((c >= '\\') ? c-36 : c-35); }

UI_INTERN void
ui_decode_85(unsigned char* dst, const unsigned char* src)
{
    while (*src)
    {
        unsigned int tmp =
            ui_decode_85_byte((char)src[0]) +
            85 * (ui_decode_85_byte((char)src[1]) +
            85 * (ui_decode_85_byte((char)src[2]) +
            85 * (ui_decode_85_byte((char)src[3]) +
            85 * ui_decode_85_byte((char)src[4]))));

        /* we can't assume little-endianess. */
        dst[0] = (unsigned char)((tmp >> 0) & 0xFF);
        dst[1] = (unsigned char)((tmp >> 8) & 0xFF);
        dst[2] = (unsigned char)((tmp >> 16) & 0xFF);
        dst[3] = (unsigned char)((tmp >> 24) & 0xFF);

        src += 5;
        dst += 4;
    }
}

/* -------------------------------------------------------------
 *
 *                          FONT ATLAS
 *
 * --------------------------------------------------------------*/
UI_API struct ui_font_config
ui_font_config(float pixel_height)
{
    struct ui_font_config cfg;
    ui_zero_struct(cfg);
    cfg.ttf_blob = 0;
    cfg.ttf_size = 0;
    cfg.ttf_data_owned_by_atlas = 0;
    cfg.size = pixel_height;
    cfg.oversample_h = 3;
    cfg.oversample_v = 1;
    cfg.pixel_snap = 0;
    cfg.coord_type = UI_COORD_UV;
    cfg.spacing = ui_vec2(0,0);
    cfg.range = ui_font_default_glyph_ranges();
    cfg.merge_mode = 0;
    cfg.fallback_glyph = '?';
    cfg.font = 0;
    return cfg;
}

#ifdef UI_INCLUDE_DEFAULT_ALLOCATOR
UI_API void
ui_font_atlas_init_default(struct ui_font_atlas *atlas)
{
    UI_ASSERT(atlas);
    if (!atlas) return;
    ui_zero_struct(*atlas);
    atlas->temporary.userdata.ptr = 0;
    atlas->temporary.alloc = ui_malloc;
    atlas->temporary.free = ui_mfree;
    atlas->permanent.userdata.ptr = 0;
    atlas->permanent.alloc = ui_malloc;
    atlas->permanent.free = ui_mfree;
}
#endif

UI_API void
ui_font_atlas_init(struct ui_font_atlas *atlas, struct ui_allocator *alloc)
{
    UI_ASSERT(atlas);
    UI_ASSERT(alloc);
    if (!atlas || !alloc) return;
    ui_zero_struct(*atlas);
    atlas->permanent = *alloc;
    atlas->temporary = *alloc;
}

UI_API void
ui_font_atlas_init_custom(struct ui_font_atlas *atlas,
    struct ui_allocator *permanent, struct ui_allocator *temporary)
{
    UI_ASSERT(atlas);
    UI_ASSERT(permanent);
    UI_ASSERT(temporary);
    if (!atlas || !permanent || !temporary) return;
    ui_zero_struct(*atlas);
    atlas->permanent = *permanent;
    atlas->temporary = *temporary;
}

UI_API void
ui_font_atlas_begin(struct ui_font_atlas *atlas)
{
    UI_ASSERT(atlas);
    UI_ASSERT(atlas->temporary.alloc && atlas->temporary.free);
    UI_ASSERT(atlas->permanent.alloc && atlas->permanent.free);
    if (!atlas || !atlas->permanent.alloc || !atlas->permanent.free ||
        !atlas->temporary.alloc || !atlas->temporary.free) return;
    if (atlas->glyphs) {
        atlas->permanent.free(atlas->permanent.userdata, atlas->glyphs);
        atlas->glyphs = 0;
    }
    if (atlas->pixel) {
        atlas->permanent.free(atlas->permanent.userdata, atlas->pixel);
        atlas->pixel = 0;
    }
}

UI_API struct ui_font*
ui_font_atlas_add(struct ui_font_atlas *atlas, const struct ui_font_config *config)
{
    struct ui_font *font = 0;
    struct ui_font_config *cfg;

    UI_ASSERT(atlas);
    UI_ASSERT(atlas->permanent.alloc);
    UI_ASSERT(atlas->permanent.free);
    UI_ASSERT(atlas->temporary.alloc);
    UI_ASSERT(atlas->temporary.free);

    UI_ASSERT(config);
    UI_ASSERT(config->ttf_blob);
    UI_ASSERT(config->ttf_size);
    UI_ASSERT(config->size > 0.0f);

    if (!atlas || !config || !config->ttf_blob || !config->ttf_size || config->size <= 0.0f||
        !atlas->permanent.alloc || !atlas->permanent.free ||
        !atlas->temporary.alloc || !atlas->temporary.free)
        return 0;

    /* allocate and insert font config into list */
    cfg = (struct ui_font_config*)
        atlas->permanent.alloc(atlas->permanent.userdata,0, sizeof(struct ui_font_config));
    UI_MEMCPY(cfg, config, sizeof(*config));
    if (!atlas->config) {
        atlas->config = cfg;
        cfg->next = 0;
    } else {
        cfg->next = atlas->config;
        atlas->config = cfg;
    }

    /* allocate new font */
    if (!config->merge_mode) {
        font = (struct ui_font*)
            atlas->permanent.alloc(atlas->permanent.userdata,0, sizeof(struct ui_font));
        UI_ASSERT(font);
        if (!font) return 0;
        font->config = cfg;
    } else {
        UI_ASSERT(atlas->font_num);
        font = atlas->fonts;
        font->config = cfg;
    }

    /* insert font into list */
    if (!config->merge_mode) {
        if (!atlas->fonts) {
            atlas->fonts = font;
            font->next = 0;
        } else {
            font->next = atlas->fonts;
            atlas->fonts = font;
        }
        cfg->font = &font->info;
    }

    /* create own copy of .TTF font blob */
    if (!config->ttf_data_owned_by_atlas) {
        cfg->ttf_blob = atlas->permanent.alloc(atlas->permanent.userdata,0, cfg->ttf_size);
        UI_ASSERT(cfg->ttf_blob);
        if (!cfg->ttf_blob) {
            atlas->font_num++;
            return 0;
        }
        UI_MEMCPY(cfg->ttf_blob, config->ttf_blob, cfg->ttf_size);
        cfg->ttf_data_owned_by_atlas = 1;
    }
    atlas->font_num++;
    return font;
}

UI_API struct ui_font*
ui_font_atlas_add_from_memory(struct ui_font_atlas *atlas, void *memory,
    ui_size size, float height, const struct ui_font_config *config)
{
    struct ui_font_config cfg;
    UI_ASSERT(memory);
    UI_ASSERT(size);

    UI_ASSERT(atlas);
    UI_ASSERT(atlas->temporary.alloc);
    UI_ASSERT(atlas->temporary.free);
    UI_ASSERT(atlas->permanent.alloc);
    UI_ASSERT(atlas->permanent.free);
    if (!atlas || !atlas->temporary.alloc || !atlas->temporary.free || !memory || !size ||
        !atlas->permanent.alloc || !atlas->permanent.free)
        return 0;

    cfg = (config) ? *config: ui_font_config(height);
    cfg.ttf_blob = memory;
    cfg.ttf_size = size;
    cfg.size = height;
    cfg.ttf_data_owned_by_atlas = 0;
    return ui_font_atlas_add(atlas, &cfg);
}

#ifdef UI_INCLUDE_STANDARD_IO
UI_API struct ui_font*
ui_font_atlas_add_from_file(struct ui_font_atlas *atlas, const char *file_path,
    float height, const struct ui_font_config *config)
{
    ui_size size;
    char *memory;
    struct ui_font_config cfg;

    UI_ASSERT(atlas);
    UI_ASSERT(atlas->temporary.alloc);
    UI_ASSERT(atlas->temporary.free);
    UI_ASSERT(atlas->permanent.alloc);
    UI_ASSERT(atlas->permanent.free);

    if (!atlas || !file_path) return 0;
    memory = ui_file_load(file_path, &size, &atlas->permanent);
    if (!memory) return 0;

    cfg = (config) ? *config: ui_font_config(height);
    cfg.ttf_blob = memory;
    cfg.ttf_size = size;
    cfg.size = height;
    cfg.ttf_data_owned_by_atlas = 1;
    return ui_font_atlas_add(atlas, &cfg);
}
#endif

UI_API struct ui_font*
ui_font_atlas_add_compressed(struct ui_font_atlas *atlas,
    void *compressed_data, ui_size compressed_size, float height,
    const struct ui_font_config *config)
{
    unsigned int decompressed_size;
    void *decompressed_data;
    struct ui_font_config cfg;

    UI_ASSERT(atlas);
    UI_ASSERT(atlas->temporary.alloc);
    UI_ASSERT(atlas->temporary.free);
    UI_ASSERT(atlas->permanent.alloc);
    UI_ASSERT(atlas->permanent.free);

    UI_ASSERT(compressed_data);
    UI_ASSERT(compressed_size);
    if (!atlas || !compressed_data || !atlas->temporary.alloc || !atlas->temporary.free ||
        !atlas->permanent.alloc || !atlas->permanent.free)
        return 0;

    decompressed_size = ui_decompress_length((unsigned char*)compressed_data);
    decompressed_data = atlas->permanent.alloc(atlas->permanent.userdata,0,decompressed_size);
    UI_ASSERT(decompressed_data);
    if (!decompressed_data) return 0;
    ui_decompress((unsigned char*)decompressed_data, (unsigned char*)compressed_data,
        (unsigned int)compressed_size);

    cfg = (config) ? *config: ui_font_config(height);
    cfg.ttf_blob = decompressed_data;
    cfg.ttf_size = decompressed_size;
    cfg.size = height;
    cfg.ttf_data_owned_by_atlas = 1;
    return ui_font_atlas_add(atlas, &cfg);
}

UI_API struct ui_font*
ui_font_atlas_add_compressed_base85(struct ui_font_atlas *atlas,
    const char *data_base85, float height, const struct ui_font_config *config)
{
    int compressed_size;
    void *compressed_data;
    struct ui_font *font;

    UI_ASSERT(atlas);
    UI_ASSERT(atlas->temporary.alloc);
    UI_ASSERT(atlas->temporary.free);
    UI_ASSERT(atlas->permanent.alloc);
    UI_ASSERT(atlas->permanent.free);

    UI_ASSERT(data_base85);
    if (!atlas || !data_base85 || !atlas->temporary.alloc || !atlas->temporary.free ||
        !atlas->permanent.alloc || !atlas->permanent.free)
        return 0;

    compressed_size = (((int)ui_strlen(data_base85) + 4) / 5) * 4;
    compressed_data = atlas->temporary.alloc(atlas->temporary.userdata,0, (ui_size)compressed_size);
    UI_ASSERT(compressed_data);
    if (!compressed_data) return 0;
    ui_decode_85((unsigned char*)compressed_data, (const unsigned char*)data_base85);
    font = ui_font_atlas_add_compressed(atlas, compressed_data,
                    (ui_size)compressed_size, height, config);
    atlas->temporary.free(atlas->temporary.userdata, compressed_data);
    return font;
}

#ifdef UI_INCLUDE_DEFAULT_FONT
UI_API struct ui_font*
ui_font_atlas_add_default(struct ui_font_atlas *atlas,
    float pixel_height, const struct ui_font_config *config)
{
    UI_ASSERT(atlas);
    UI_ASSERT(atlas->temporary.alloc);
    UI_ASSERT(atlas->temporary.free);
    UI_ASSERT(atlas->permanent.alloc);
    UI_ASSERT(atlas->permanent.free);
    return ui_font_atlas_add_compressed_base85(atlas,
        ui_proggy_clean_ttf_compressed_data_base85, pixel_height, config);
}
#endif

UI_API const void*
ui_font_atlas_bake(struct ui_font_atlas *atlas, int *width, int *height,
    enum ui_font_atlas_format fmt)
{
    int i = 0;
    void *tmp = 0;
    ui_size tmp_size, img_size;
    struct ui_font *font_iter;
    struct ui_font_baker *baker;

    UI_ASSERT(atlas);
    UI_ASSERT(atlas->temporary.alloc);
    UI_ASSERT(atlas->temporary.free);
    UI_ASSERT(atlas->permanent.alloc);
    UI_ASSERT(atlas->permanent.free);

    UI_ASSERT(width);
    UI_ASSERT(height);
    if (!atlas || !width || !height ||
        !atlas->temporary.alloc || !atlas->temporary.free ||
        !atlas->permanent.alloc || !atlas->permanent.free)
        return 0;

#ifdef UI_INCLUDE_DEFAULT_FONT
    /* no font added so just use default font */
    if (!atlas->font_num)
        atlas->default_font = ui_font_atlas_add_default(atlas, 13.0f, 0);
#endif
    UI_ASSERT(atlas->font_num);
    if (!atlas->font_num) return 0;

    /* allocate temporary baker memory required for the baking process */
    ui_font_baker_memory(&tmp_size, &atlas->glyph_count, atlas->config, atlas->font_num);
    tmp = atlas->temporary.alloc(atlas->temporary.userdata,0, tmp_size);
    UI_ASSERT(tmp);
    if (!tmp) goto failed;

    /* allocate glyph memory for all fonts */
    baker = ui_font_baker(tmp, atlas->glyph_count, atlas->font_num, &atlas->temporary);
    atlas->glyphs = (struct ui_font_glyph*)atlas->permanent.alloc(
        atlas->permanent.userdata,0, sizeof(struct ui_font_glyph)*(ui_size)atlas->glyph_count);
    UI_ASSERT(atlas->glyphs);
    if (!atlas->glyphs)
        goto failed;

    /* pack all glyphs into a tight fit space */
    atlas->custom.w = (UI_CURSOR_DATA_W*2)+1;
    atlas->custom.h = UI_CURSOR_DATA_H + 1;
    if (!ui_font_bake_pack(baker, &img_size, width, height, &atlas->custom,
        atlas->config, atlas->font_num, &atlas->temporary))
        goto failed;

    /* allocate memory for the baked image font atlas */
    atlas->pixel = atlas->temporary.alloc(atlas->temporary.userdata,0, img_size);
    UI_ASSERT(atlas->pixel);
    if (!atlas->pixel)
        goto failed;

    /* bake glyphs and custom white pixel into image */
    ui_font_bake(baker, atlas->pixel, *width, *height,
        atlas->glyphs, atlas->glyph_count, atlas->config, atlas->font_num);
    ui_font_bake_custom_data(atlas->pixel, *width, *height, atlas->custom,
            ui_custom_cursor_data, UI_CURSOR_DATA_W, UI_CURSOR_DATA_H, '.', 'X');

    if (fmt == UI_FONT_ATLAS_RGBA32) {
        /* convert alpha8 image into rgba32 image */
        void *img_rgba = atlas->temporary.alloc(atlas->temporary.userdata,0,
                            (ui_size)(*width * *height * 4));
        UI_ASSERT(img_rgba);
        if (!img_rgba) goto failed;
        ui_font_bake_convert(img_rgba, *width, *height, atlas->pixel);
        atlas->temporary.free(atlas->temporary.userdata, atlas->pixel);
        atlas->pixel = img_rgba;
    }
    atlas->tex_width = *width;
    atlas->tex_height = *height;

    /* initialize each font */
    for (font_iter = atlas->fonts; font_iter; font_iter = font_iter->next) {
        struct ui_font *font = font_iter;
        struct ui_font_config *config = font->config;
        ui_font_init(font, config->size, config->fallback_glyph, atlas->glyphs,
            config->font, ui_handle_ptr(0));
    }

    /* initialize each cursor */
    {UI_STORAGE const struct ui_vec2 ui_cursor_data[UI_CURSOR_COUNT][3] = {
        /* Pos ----- Size ------- Offset --*/
        {{ 0, 3},   {12,19},    { 0, 0}},
        {{13, 0},   { 7,16},    { 4, 8}},
        {{31, 0},   {23,23},    {11,11}},
        {{21, 0},   { 9, 23},   { 5,11}},
        {{55,18},   {23, 9},    {11, 5}},
        {{73, 0},   {17,17},    { 9, 9}},
        {{55, 0},   {17,17},    { 9, 9}}
    };
    for (i = 0; i < UI_CURSOR_COUNT; ++i) {
        struct ui_cursor *cursor = &atlas->cursors[i];
        cursor->img.w = (unsigned short)*width;
        cursor->img.h = (unsigned short)*height;
        cursor->img.region[0] = (unsigned short)(atlas->custom.x + ui_cursor_data[i][0].x);
        cursor->img.region[1] = (unsigned short)(atlas->custom.y + ui_cursor_data[i][0].y);
        cursor->img.region[2] = (unsigned short)ui_cursor_data[i][1].x;
        cursor->img.region[3] = (unsigned short)ui_cursor_data[i][1].y;
        cursor->size = ui_cursor_data[i][1];
        cursor->offset = ui_cursor_data[i][2];
    }}
    /* free temporary memory */
    atlas->temporary.free(atlas->temporary.userdata, tmp);
    return atlas->pixel;

failed:
    /* error so cleanup all memory */
    if (tmp) atlas->temporary.free(atlas->temporary.userdata, tmp);
    if (atlas->glyphs) {
        atlas->permanent.free(atlas->permanent.userdata, atlas->glyphs);
        atlas->glyphs = 0;
    }
    if (atlas->pixel) {
        atlas->temporary.free(atlas->temporary.userdata, atlas->pixel);
        atlas->pixel = 0;
    }
    return 0;
}

UI_API void
ui_font_atlas_end(struct ui_font_atlas *atlas, ui_handle texture,
    struct ui_draw_null_texture *null)
{
    int i = 0;
    struct ui_font *font_iter;
    UI_ASSERT(atlas);
    if (!atlas) {
        if (!null) return;
        null->texture = texture;
        null->uv = ui_vec2(0.5f,0.5f);
    }
    if (null) {
        null->texture = texture;
        null->uv.x = (atlas->custom.x + 0.5f)/(float)atlas->tex_width;
        null->uv.y = (atlas->custom.y + 0.5f)/(float)atlas->tex_height;
    }
    for (font_iter = atlas->fonts; font_iter; font_iter = font_iter->next) {
        font_iter->texture = texture;
#ifdef UI_INCLUDE_VERTEX_BUFFER_OUTPUT
        font_iter->handle.texture = texture;
#endif
    }
    for (i = 0; i < UI_CURSOR_COUNT; ++i)
        atlas->cursors[i].img.handle = texture;

    atlas->temporary.free(atlas->temporary.userdata, atlas->pixel);
    atlas->pixel = 0;
    atlas->tex_width = 0;
    atlas->tex_height = 0;
    atlas->custom.x = 0;
    atlas->custom.y = 0;
    atlas->custom.w = 0;
    atlas->custom.h = 0;
}

UI_API void
ui_font_atlas_cleanup(struct ui_font_atlas *atlas)
{
    UI_ASSERT(atlas);
    UI_ASSERT(atlas->temporary.alloc);
    UI_ASSERT(atlas->temporary.free);
    UI_ASSERT(atlas->permanent.alloc);
    UI_ASSERT(atlas->permanent.free);

    if (!atlas || !atlas->permanent.alloc || !atlas->permanent.free) return;
    if (atlas->config) {
        struct ui_font_config *iter, *next;
        for (iter = atlas->config; iter; iter = next) {
            next = iter->next;
            atlas->permanent.free(atlas->permanent.userdata, iter->ttf_blob);
            atlas->permanent.free(atlas->permanent.userdata, iter);
        }
    }
}

UI_API void
ui_font_atlas_clear(struct ui_font_atlas *atlas)
{
    UI_ASSERT(atlas);
    UI_ASSERT(atlas->temporary.alloc);
    UI_ASSERT(atlas->temporary.free);
    UI_ASSERT(atlas->permanent.alloc);
    UI_ASSERT(atlas->permanent.free);
    if (!atlas || !atlas->permanent.alloc || !atlas->permanent.free) return;

    ui_font_atlas_cleanup(atlas);
    if (atlas->fonts) {
        struct ui_font *iter, *next;
        for (iter = atlas->fonts; iter; iter = next) {
            next = iter->next;
            atlas->permanent.free(atlas->permanent.userdata, iter);
        }
        atlas->fonts = 0;
    }
    if (atlas->glyphs)
        atlas->permanent.free(atlas->permanent.userdata, atlas->glyphs);
    ui_zero_struct(*atlas);
}
#endif
/* ==============================================================
 *
 *                          INPUT
 *
 * ===============================================================*/
UI_API void
ui_input_begin(struct ui_context *ctx)
{
    int i;
    struct ui_input *in;
    UI_ASSERT(ctx);
    if (!ctx) return;
    in = &ctx->input;
    for (i = 0; i < UI_BUTTON_MAX; ++i)
        in->mouse.buttons[i].clicked = 0;

    in->keyboard.text_len = 0;
    in->mouse.scroll_delta = 0;
    in->mouse.prev.x = in->mouse.pos.x;
    in->mouse.prev.y = in->mouse.pos.y;
    in->mouse.delta.x = 0;
    in->mouse.delta.y = 0;
    for (i = 0; i < UI_KEY_MAX; i++)
        in->keyboard.keys[i].clicked = 0;
}

UI_API void
ui_input_end(struct ui_context *ctx)
{
    struct ui_input *in;
    UI_ASSERT(ctx);
    if (!ctx) return;
    in = &ctx->input;
    if (in->mouse.grab)
        in->mouse.grab = 0;
    if (in->mouse.ungrab) {
        in->mouse.grabbed = 0;
        in->mouse.ungrab = 0;
        in->mouse.grab = 0;
    }
}

UI_API void
ui_input_motion(struct ui_context *ctx, int x, int y)
{
    struct ui_input *in;
    UI_ASSERT(ctx);
    if (!ctx) return;
    in = &ctx->input;
    in->mouse.pos.x = (float)x;
    in->mouse.pos.y = (float)y;
    in->mouse.delta.x = in->mouse.pos.x - in->mouse.prev.x;
    in->mouse.delta.y = in->mouse.pos.y - in->mouse.prev.y;
}

UI_API void
ui_input_key(struct ui_context *ctx, enum ui_keys key, int down)
{
    struct ui_input *in;
    UI_ASSERT(ctx);
    if (!ctx) return;
    in = &ctx->input;
    in->keyboard.keys[key].down = down;
    in->keyboard.keys[key].clicked++;
}

UI_API void
ui_input_button(struct ui_context *ctx, enum ui_buttons id, int x, int y, int down)
{
    struct ui_mouse_button *btn;
    struct ui_input *in;
    UI_ASSERT(ctx);
    if (!ctx) return;
    in = &ctx->input;
    if (in->mouse.buttons[id].down == down) return;

    btn = &in->mouse.buttons[id];
    btn->clicked_pos.x = (float)x;
    btn->clicked_pos.y = (float)y;
    btn->down = down;
    btn->clicked++;
}

UI_API void
ui_input_scroll(struct ui_context *ctx, float y)
{
    UI_ASSERT(ctx);
    if (!ctx) return;
    ctx->input.mouse.scroll_delta += y;
}

UI_API void
ui_input_glyph(struct ui_context *ctx, const ui_glyph glyph)
{
    int len = 0;
    ui_rune unicode;
    struct ui_input *in;

    UI_ASSERT(ctx);
    if (!ctx) return;
    in = &ctx->input;

    len = ui_utf_decode(glyph, &unicode, UI_UTF_SIZE);
    if (len && ((in->keyboard.text_len + len) < UI_INPUT_MAX)) {
        ui_utf_encode(unicode, &in->keyboard.text[in->keyboard.text_len],
            UI_INPUT_MAX - in->keyboard.text_len);
        in->keyboard.text_len += len;
    }
}

UI_API void
ui_input_char(struct ui_context *ctx, char c)
{
    ui_glyph glyph;
    UI_ASSERT(ctx);
    if (!ctx) return;
    glyph[0] = c;
    ui_input_glyph(ctx, glyph);
}

UI_API void
ui_input_unicode(struct ui_context *ctx, ui_rune unicode)
{
    ui_glyph rune;
    UI_ASSERT(ctx);
    if (!ctx) return;
    ui_utf_encode(unicode, rune, UI_UTF_SIZE);
    ui_input_glyph(ctx, rune);
}

UI_API int
ui_input_has_mouse_click(const struct ui_input *i, enum ui_buttons id)
{
    const struct ui_mouse_button *btn;
    if (!i) return ui_false;
    btn = &i->mouse.buttons[id];
    return (btn->clicked && btn->down == ui_false) ? ui_true : ui_false;
}

UI_API int
ui_input_has_mouse_click_in_rect(const struct ui_input *i, enum ui_buttons id,
    struct ui_rect b)
{
    const struct ui_mouse_button *btn;
    if (!i) return ui_false;
    btn = &i->mouse.buttons[id];
    if (!UI_INBOX(btn->clicked_pos.x,btn->clicked_pos.y,b.x,b.y,b.w,b.h))
        return ui_false;
    return ui_true;
}

UI_API int
ui_input_has_mouse_click_down_in_rect(const struct ui_input *i, enum ui_buttons id,
    struct ui_rect b, int down)
{
    const struct ui_mouse_button *btn;
    if (!i) return ui_false;
    btn = &i->mouse.buttons[id];
    return ui_input_has_mouse_click_in_rect(i, id, b) && (btn->down == down);
}

UI_API int
ui_input_is_mouse_click_in_rect(const struct ui_input *i, enum ui_buttons id,
    struct ui_rect b)
{
    const struct ui_mouse_button *btn;
    if (!i) return ui_false;
    btn = &i->mouse.buttons[id];
    return (ui_input_has_mouse_click_down_in_rect(i, id, b, ui_false) &&
            btn->clicked) ? ui_true : ui_false;
}

UI_API int
ui_input_is_mouse_click_down_in_rect(const struct ui_input *i, enum ui_buttons id,
    struct ui_rect b, int down)
{
    const struct ui_mouse_button *btn;
    if (!i) return ui_false;
    btn = &i->mouse.buttons[id];
    return (ui_input_has_mouse_click_down_in_rect(i, id, b, down) &&
            btn->clicked) ? ui_true : ui_false;
}

UI_API int
ui_input_any_mouse_click_in_rect(const struct ui_input *in, struct ui_rect b)
{
    int i, down = 0;
    for (i = 0; i < UI_BUTTON_MAX; ++i)
        down = down || ui_input_is_mouse_click_in_rect(in, (enum ui_buttons)i, b);
    return down;
}

UI_API int
ui_input_is_mouse_hovering_rect(const struct ui_input *i, struct ui_rect rect)
{
    if (!i) return ui_false;
    return UI_INBOX(i->mouse.pos.x, i->mouse.pos.y, rect.x, rect.y, rect.w, rect.h);
}

UI_API int
ui_input_is_mouse_prev_hovering_rect(const struct ui_input *i, struct ui_rect rect)
{
    if (!i) return ui_false;
    return UI_INBOX(i->mouse.prev.x, i->mouse.prev.y, rect.x, rect.y, rect.w, rect.h);
}

UI_API int
ui_input_mouse_clicked(const struct ui_input *i, enum ui_buttons id, struct ui_rect rect)
{
    if (!i) return ui_false;
    if (!ui_input_is_mouse_hovering_rect(i, rect)) return ui_false;
    return ui_input_is_mouse_click_in_rect(i, id, rect);
}

UI_API int
ui_input_is_mouse_down(const struct ui_input *i, enum ui_buttons id)
{
    if (!i) return ui_false;
    return i->mouse.buttons[id].down;
}

UI_API int
ui_input_is_mouse_pressed(const struct ui_input *i, enum ui_buttons id)
{
    const struct ui_mouse_button *b;
    if (!i) return ui_false;
    b = &i->mouse.buttons[id];
    if (b->down && b->clicked)
        return ui_true;
    return ui_false;
}

UI_API int
ui_input_is_mouse_released(const struct ui_input *i, enum ui_buttons id)
{
    if (!i) return ui_false;
    return (!i->mouse.buttons[id].down && i->mouse.buttons[id].clicked);
}

UI_API int
ui_input_is_key_pressed(const struct ui_input *i, enum ui_keys key)
{
    const struct ui_key *k;
    if (!i) return ui_false;
    k = &i->keyboard.keys[key];
    if ((k->down && k->clicked) || (!k->down && k->clicked >= 2))
        return ui_true;
    return ui_false;
}

UI_API int
ui_input_is_key_released(const struct ui_input *i, enum ui_keys key)
{
    const struct ui_key *k;
    if (!i) return ui_false;
    k = &i->keyboard.keys[key];
    if ((!k->down && k->clicked) || (k->down && k->clicked >= 2))
        return ui_true;
    return ui_false;
}

UI_API int
ui_input_is_key_down(const struct ui_input *i, enum ui_keys key)
{
    const struct ui_key *k;
    if (!i) return ui_false;
    k = &i->keyboard.keys[key];
    if (k->down) return ui_true;
    return ui_false;
}

/*
 * ==============================================================
 *
 *                          TEXT EDITOR
 *
 * ===============================================================
 */
/* stb_textedit.h - v1.8  - public domain - Sean Barrett */
struct ui_text_find {
   float x,y;    /* position of n'th character */
   float height; /* height of line */
   int first_char, length; /* first char of row, and length */
   int prev_first;  /*_ first char of previous row */
};

struct ui_text_edit_row {
   float x0,x1;
   /* starting x location, end x location (allows for align=right, etc) */
   float baseline_y_delta;
   /* position of baseline relative to previous row's baseline*/
   float ymin,ymax;
   /* height of row above and below baseline */
   int num_chars;
};

/* forward declarations */
UI_INTERN void ui_textedit_makeundo_delete(struct ui_text_edit*, int, int);
UI_INTERN void ui_textedit_makeundo_insert(struct ui_text_edit*, int, int);
UI_INTERN void ui_textedit_makeundo_replace(struct ui_text_edit*, int, int, int);
#define UI_TEXT_HAS_SELECTION(s)   ((s)->select_start != (s)->select_end)

UI_INTERN float
ui_textedit_get_width(const struct ui_text_edit *edit, int line_start, int char_id,
    const struct ui_user_font *font)
{
    int len = 0;
    ui_rune unicode = 0;
    const char *str = ui_str_at_const(&edit->string, line_start + char_id, &unicode, &len);
    return font->width(font->userdata, font->height, str, len);
}

UI_INTERN void
ui_textedit_layout_row(struct ui_text_edit_row *r, struct ui_text_edit *edit,
    int line_start_id, float row_height, const struct ui_user_font *font)
{
    int l;
    int glyphs = 0;
    ui_rune unicode;
    const char *remaining;
    int len = ui_str_len_char(&edit->string);
    const char *end = ui_str_get_const(&edit->string) + len;
    const char *text = ui_str_at_const(&edit->string, line_start_id, &unicode, &l);
    const struct ui_vec2 size = ui_text_calculate_text_bounds(font,
        text, (int)(end - text), row_height, &remaining, 0, &glyphs, UI_STOP_ON_NEW_LINE);

    r->x0 = 0.0f;
    r->x1 = size.x;
    r->baseline_y_delta = size.y;
    r->ymin = 0.0f;
    r->ymax = size.y;
    r->num_chars = glyphs;
}

UI_INTERN int
ui_textedit_locate_coord(struct ui_text_edit *edit, float x, float y,
    const struct ui_user_font *font, float row_height)
{
    struct ui_text_edit_row r;
    int n = edit->string.len;
    float base_y = 0, prev_x;
    int i=0, k;

    r.x0 = r.x1 = 0;
    r.ymin = r.ymax = 0;
    r.num_chars = 0;

    /* search rows to find one that straddles 'y' */
    while (i < n) {
        ui_textedit_layout_row(&r, edit, i, row_height, font);
        if (r.num_chars <= 0)
            return n;

        if (i==0 && y < base_y + r.ymin)
            return 0;

        if (y < base_y + r.ymax)
            break;

        i += r.num_chars;
        base_y += r.baseline_y_delta;
    }

    /* below all text, return 'after' last character */
    if (i >= n)
        return n;

    /* check if it's before the beginning of the line */
    if (x < r.x0)
        return i;

    /* check if it's before the end of the line */
    if (x < r.x1) {
        /* search characters in row for one that straddles 'x' */
        k = i;
        prev_x = r.x0;
        for (i=0; i < r.num_chars; ++i) {
            float w = ui_textedit_get_width(edit, k, i, font);
            if (x < prev_x+w) {
                if (x < prev_x+w/2)
                    return k+i;
                else return k+i+1;
            }
            prev_x += w;
        }
        /* shouldn't happen, but if it does, fall through to end-of-line case */
    }

    /* if the last character is a newline, return that.
     * otherwise return 'after' the last character */
    if (ui_str_rune_at(&edit->string, i+r.num_chars-1) == '\n')
        return i+r.num_chars-1;
    else return i+r.num_chars;
}

UI_INTERN void
ui_textedit_click(struct ui_text_edit *state, float x, float y,
    const struct ui_user_font *font, float row_height)
{
    /* API click: on mouse down, move the cursor to the clicked location,
     * and reset the selection */
    state->cursor = ui_textedit_locate_coord(state, x, y, font, row_height);
    state->select_start = state->cursor;
    state->select_end = state->cursor;
    state->has_preferred_x = 0;
}

UI_INTERN void
ui_textedit_drag(struct ui_text_edit *state, float x, float y,
    const struct ui_user_font *font, float row_height)
{
    /* API drag: on mouse drag, move the cursor and selection endpoint
     * to the clicked location */
    int p = ui_textedit_locate_coord(state, x, y, font, row_height);
    if (state->select_start == state->select_end)
        state->select_start = state->cursor;
    state->cursor = state->select_end = p;
}

UI_INTERN void
ui_textedit_find_charpos(struct ui_text_find *find, struct ui_text_edit *state,
    int n, int single_line, const struct ui_user_font *font, float row_height)
{
    /* find the x/y location of a character, and remember info about the previous
     * row in case we get a move-up event (for page up, we'll have to rescan) */
    struct ui_text_edit_row r;
    int prev_start = 0;
    int z = state->string.len;
    int i=0, first;

    ui_zero_struct(r);
    if (n == z) {
        /* if it's at the end, then find the last line -- simpler than trying to
        explicitly handle this case in the regular code */
        ui_textedit_layout_row(&r, state, 0, row_height, font);
        if (single_line) {
            find->first_char = 0;
            find->length = z;
        } else {
            while (i < z) {
                prev_start = i;
                i += r.num_chars;
                ui_textedit_layout_row(&r, state, i, row_height, font);
            }

            find->first_char = i;
            find->length = r.num_chars;
        }
        find->x = r.x1;
        find->y = r.ymin;
        find->height = r.ymax - r.ymin;
        find->prev_first = prev_start;
        return;
    }

    /* search rows to find the one that straddles character n */
    find->y = 0;

    for(;;) {
        ui_textedit_layout_row(&r, state, i, row_height, font);
        if (n < i + r.num_chars) break;
        prev_start = i;
        i += r.num_chars;
        find->y += r.baseline_y_delta;
    }

    find->first_char = first = i;
    find->length = r.num_chars;
    find->height = r.ymax - r.ymin;
    find->prev_first = prev_start;

    /* now scan to find xpos */
    find->x = r.x0;
    for (i=0; first+i < n; ++i)
        find->x += ui_textedit_get_width(state, first, i, font);
}

UI_INTERN void
ui_textedit_clamp(struct ui_text_edit *state)
{
    /* make the selection/cursor state valid if client altered the string */
    int n = state->string.len;
    if (UI_TEXT_HAS_SELECTION(state)) {
        if (state->select_start > n) state->select_start = n;
        if (state->select_end   > n) state->select_end = n;
        /* if clamping forced them to be equal, move the cursor to match */
        if (state->select_start == state->select_end)
            state->cursor = state->select_start;
    }
    if (state->cursor > n) state->cursor = n;
}

UI_API void
ui_textedit_delete(struct ui_text_edit *state, int where, int len)
{
    /* delete characters while updating undo */
    ui_textedit_makeundo_delete(state, where, len);
    ui_str_delete_runes(&state->string, where, len);
    state->has_preferred_x = 0;
}

UI_API void
ui_textedit_delete_selection(struct ui_text_edit *state)
{
    /* delete the section */
    ui_textedit_clamp(state);
    if (UI_TEXT_HAS_SELECTION(state)) {
        if (state->select_start < state->select_end) {
            ui_textedit_delete(state, state->select_start,
                state->select_end - state->select_start);
            state->select_end = state->cursor = state->select_start;
        } else {
            ui_textedit_delete(state, state->select_end,
                state->select_start - state->select_end);
            state->select_start = state->cursor = state->select_end;
        }
        state->has_preferred_x = 0;
    }
}

UI_INTERN void
ui_textedit_sortselection(struct ui_text_edit *state)
{
    /* canonicalize the selection so start <= end */
    if (state->select_end < state->select_start) {
        int temp = state->select_end;
        state->select_end = state->select_start;
        state->select_start = temp;
    }
}

UI_INTERN void
ui_textedit_move_to_first(struct ui_text_edit *state)
{
    /* move cursor to first character of selection */
    if (UI_TEXT_HAS_SELECTION(state)) {
        ui_textedit_sortselection(state);
        state->cursor = state->select_start;
        state->select_end = state->select_start;
        state->has_preferred_x = 0;
    }
}

UI_INTERN void
ui_textedit_move_to_last(struct ui_text_edit *state)
{
    /* move cursor to last character of selection */
    if (UI_TEXT_HAS_SELECTION(state)) {
        ui_textedit_sortselection(state);
        ui_textedit_clamp(state);
        state->cursor = state->select_end;
        state->select_start = state->select_end;
        state->has_preferred_x = 0;
    }
}

UI_INTERN int
ui_is_word_boundary( struct ui_text_edit *state, int idx)
{
    int len;
    ui_rune c;
    if (idx <= 0) return 1;
    if (!ui_str_at_rune(&state->string, idx, &c, &len)) return 1;
    return (c == ' ' || c == '\t' ||c == 0x3000 || c == ',' || c == ';' ||
            c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' ||
            c == '|');
}

UI_INTERN int
ui_textedit_move_to_word_previous(struct ui_text_edit *state)
{
   int c = state->cursor - 1;
   while( c >= 0 && !ui_is_word_boundary(state, c))
      --c;

   if( c < 0 )
      c = 0;

   return c;
}

UI_INTERN int
ui_textedit_move_to_word_next(struct ui_text_edit *state)
{
   const int len = state->string.len;
   int c = state->cursor+1;
   while( c < len && !ui_is_word_boundary(state, c))
      ++c;

   if( c > len )
      c = len;

   return c;
}

UI_INTERN void
ui_textedit_prep_selection_at_cursor(struct ui_text_edit *state)
{
    /* update selection and cursor to match each other */
    if (!UI_TEXT_HAS_SELECTION(state))
        state->select_start = state->select_end = state->cursor;
    else state->cursor = state->select_end;
}

UI_API int
ui_textedit_cut(struct ui_text_edit *state)
{
    /* API cut: delete selection */
    if (state->mode == UI_TEXT_EDIT_MODE_VIEW)
        return 0;
    if (UI_TEXT_HAS_SELECTION(state)) {
        ui_textedit_delete_selection(state); /* implicitly clamps */
        state->has_preferred_x = 0;
        return 1;
    }
   return 0;
}

UI_API int
ui_textedit_paste(struct ui_text_edit *state, char const *ctext, int len)
{
    /* API paste: replace existing selection with passed-in text */
    int glyphs;
    const char *text = (const char *) ctext;
    if (state->mode == UI_TEXT_EDIT_MODE_VIEW) return 0;

    /* if there's a selection, the paste should delete it */
    ui_textedit_clamp(state);
    ui_textedit_delete_selection(state);

    /* try to insert the characters */
    glyphs = ui_utf_len(ctext, len);
    if (ui_str_insert_text_char(&state->string, state->cursor, text, len)) {
        ui_textedit_makeundo_insert(state, state->cursor, glyphs);
        state->cursor += len;
        state->has_preferred_x = 0;
        return 1;
    }
    /* remove the undo since we didn't actually insert the characters */
    if (state->undo.undo_point)
        --state->undo.undo_point;
    return 0;
}

UI_API void
ui_textedit_text(struct ui_text_edit *state, const char *text, int total_len)
{
    ui_rune unicode;
    int glyph_len;
    int text_len = 0;

    UI_ASSERT(state);
    UI_ASSERT(text);
    if (!text || !total_len || state->mode == UI_TEXT_EDIT_MODE_VIEW) return;

    glyph_len = ui_utf_decode(text, &unicode, total_len);
    if (!glyph_len) return;
    while ((text_len < total_len) && glyph_len)
    {
        /* don't insert a backward delete, just process the event */
        if (unicode == 127)
            break;

        /* can't add newline in single-line mode */
        if (unicode == '\n' && state->single_line)
            break;

        /* filter incoming text */
        if (state->filter && !state->filter(state, unicode)) {
            glyph_len = ui_utf_decode(text + text_len, &unicode, total_len-text_len);
            text_len += glyph_len;
            continue;
        }

        if (!UI_TEXT_HAS_SELECTION(state) &&
            state->cursor < state->string.len)
        {
            if (state->mode == UI_TEXT_EDIT_MODE_REPLACE) {
                ui_textedit_makeundo_replace(state, state->cursor, 1, 1);
                ui_str_delete_runes(&state->string, state->cursor, 1);
            }
            if (ui_str_insert_text_utf8(&state->string, state->cursor,
                                        text+text_len, 1))
            {
                ++state->cursor;
                state->has_preferred_x = 0;
            }
        } else {
            ui_textedit_delete_selection(state); /* implicitly clamps */
            if (ui_str_insert_text_utf8(&state->string, state->cursor,
                                        text+text_len, 1))
            {
                ui_textedit_makeundo_insert(state, state->cursor, 1);
                ++state->cursor;
                state->has_preferred_x = 0;
            }
        }
        glyph_len = ui_utf_decode(text + text_len, &unicode, total_len-text_len);
        text_len += glyph_len;
    }
}

UI_INTERN void
ui_textedit_key(struct ui_text_edit *state, enum ui_keys key, int shift_mod,
    const struct ui_user_font *font, float row_height)
{
retry:
    switch (key)
    {
    case UI_KEY_NONE:
    case UI_KEY_CTRL:
    case UI_KEY_ENTER:
    case UI_KEY_SHIFT:
    case UI_KEY_TAB:
    case UI_KEY_COPY:
    case UI_KEY_CUT:
    case UI_KEY_PASTE:
    case UI_KEY_MAX:
    default: break;
    case UI_KEY_TEXT_UNDO:
         ui_textedit_undo(state);
         state->has_preferred_x = 0;
         break;

    case UI_KEY_TEXT_REDO:
        ui_textedit_redo(state);
        state->has_preferred_x = 0;
        break;

    case UI_KEY_TEXT_SELECT_ALL:
        ui_textedit_select_all(state);
        state->has_preferred_x = 0;
        break;

    case UI_KEY_TEXT_INSERT_MODE:
        if (state->mode == UI_TEXT_EDIT_MODE_VIEW)
            state->mode = UI_TEXT_EDIT_MODE_INSERT;
        break;
    case UI_KEY_TEXT_REPLACE_MODE:
        if (state->mode == UI_TEXT_EDIT_MODE_VIEW)
            state->mode = UI_TEXT_EDIT_MODE_REPLACE;
        break;
    case UI_KEY_TEXT_RESET_MODE:
        if (state->mode == UI_TEXT_EDIT_MODE_INSERT ||
            state->mode == UI_TEXT_EDIT_MODE_REPLACE)
            state->mode = UI_TEXT_EDIT_MODE_VIEW;
        break;

    case UI_KEY_LEFT:
        if (shift_mod) {
            ui_textedit_clamp(state);
            ui_textedit_prep_selection_at_cursor(state);
            /* move selection left */
            if (state->select_end > 0)
                --state->select_end;
            state->cursor = state->select_end;
            state->has_preferred_x = 0;
        } else {
            /* if currently there's a selection,
             * move cursor to start of selection */
            if (UI_TEXT_HAS_SELECTION(state))
                ui_textedit_move_to_first(state);
            else if (state->cursor > 0)
               --state->cursor;
            state->has_preferred_x = 0;
        } break;

    case UI_KEY_RIGHT:
        if (shift_mod) {
            ui_textedit_prep_selection_at_cursor(state);
            /* move selection right */
            ++state->select_end;
            ui_textedit_clamp(state);
            state->cursor = state->select_end;
            state->has_preferred_x = 0;
        } else {
            /* if currently there's a selection,
             * move cursor to end of selection */
            if (UI_TEXT_HAS_SELECTION(state))
                ui_textedit_move_to_last(state);
            else ++state->cursor;
            ui_textedit_clamp(state);
            state->has_preferred_x = 0;
        } break;

    case UI_KEY_TEXT_WORD_LEFT:
        if (shift_mod) {
            if( !UI_TEXT_HAS_SELECTION( state ) )
            ui_textedit_prep_selection_at_cursor(state);
            state->cursor = ui_textedit_move_to_word_previous(state);
            state->select_end = state->cursor;
            ui_textedit_clamp(state );
        } else {
            if (UI_TEXT_HAS_SELECTION(state))
                ui_textedit_move_to_first(state);
            else {
                state->cursor = ui_textedit_move_to_word_previous(state);
                ui_textedit_clamp(state );
            }
        } break;

    case UI_KEY_TEXT_WORD_RIGHT:
        if (shift_mod) {
            if( !UI_TEXT_HAS_SELECTION( state ) )
                ui_textedit_prep_selection_at_cursor(state);
            state->cursor = ui_textedit_move_to_word_next(state);
            state->select_end = state->cursor;
            ui_textedit_clamp(state);
        } else {
            if (UI_TEXT_HAS_SELECTION(state))
                ui_textedit_move_to_last(state);
            else {
                state->cursor = ui_textedit_move_to_word_next(state);
                ui_textedit_clamp(state );
            }
        } break;

    case UI_KEY_DOWN: {
        struct ui_text_find find;
        struct ui_text_edit_row row;
        int i, sel = shift_mod;

        if (state->single_line) {
            /* on windows, up&down in single-line behave like left&right */
            key = UI_KEY_RIGHT;
            goto retry;
        }

        if (sel)
            ui_textedit_prep_selection_at_cursor(state);
        else if (UI_TEXT_HAS_SELECTION(state))
            ui_textedit_move_to_last(state);

        /* compute current position of cursor point */
        ui_textedit_clamp(state);
        ui_textedit_find_charpos(&find, state, state->cursor, state->single_line,
            font, row_height);

        /* now find character position down a row */
        if (find.length)
        {
            float x;
            float goal_x = state->has_preferred_x ? state->preferred_x : find.x;
            int start = find.first_char + find.length;

            state->cursor = start;
            ui_textedit_layout_row(&row, state, state->cursor, row_height, font);
            x = row.x0;

            for (i=0; i < row.num_chars && x < row.x1; ++i) {
                float dx = ui_textedit_get_width(state, start, i, font);
                x += dx;
                if (x > goal_x)
                    break;
                ++state->cursor;
            }
            ui_textedit_clamp(state);

            state->has_preferred_x = 1;
            state->preferred_x = goal_x;
            if (sel)
                state->select_end = state->cursor;
        }
    } break;

    case UI_KEY_UP: {
        struct ui_text_find find;
        struct ui_text_edit_row row;
        int i, sel = shift_mod;

        if (state->single_line) {
            /* on windows, up&down become left&right */
            key = UI_KEY_LEFT;
            goto retry;
        }

        if (sel)
            ui_textedit_prep_selection_at_cursor(state);
        else if (UI_TEXT_HAS_SELECTION(state))
            ui_textedit_move_to_first(state);

         /* compute current position of cursor point */
         ui_textedit_clamp(state);
         ui_textedit_find_charpos(&find, state, state->cursor, state->single_line,
                font, row_height);

         /* can only go up if there's a previous row */
         if (find.prev_first != find.first_char) {
            /* now find character position up a row */
            float x;
            float goal_x = state->has_preferred_x ? state->preferred_x : find.x;

            state->cursor = find.prev_first;
            ui_textedit_layout_row(&row, state, state->cursor, row_height, font);
            x = row.x0;

            for (i=0; i < row.num_chars && x < row.x1; ++i) {
                float dx = ui_textedit_get_width(state, find.prev_first, i, font);
                x += dx;
                if (x > goal_x)
                    break;
                ++state->cursor;
            }
            ui_textedit_clamp(state);

            state->has_preferred_x = 1;
            state->preferred_x = goal_x;
            if (sel) state->select_end = state->cursor;
         }
      } break;

    case UI_KEY_DEL:
        if (state->mode == UI_TEXT_EDIT_MODE_VIEW)
            break;
        if (UI_TEXT_HAS_SELECTION(state))
            ui_textedit_delete_selection(state);
        else {
            int n = state->string.len;
            if (state->cursor < n)
                ui_textedit_delete(state, state->cursor, 1);
         }
         state->has_preferred_x = 0;
         break;

    case UI_KEY_BACKSPACE:
        if (state->mode == UI_TEXT_EDIT_MODE_VIEW)
            break;
        if (UI_TEXT_HAS_SELECTION(state))
            ui_textedit_delete_selection(state);
        else {
            ui_textedit_clamp(state);
            if (state->cursor > 0) {
                ui_textedit_delete(state, state->cursor-1, 1);
                --state->cursor;
            }
         }
         state->has_preferred_x = 0;
         break;

    case UI_KEY_TEXT_START:
         if (shift_mod) {
            ui_textedit_prep_selection_at_cursor(state);
            state->cursor = state->select_end = 0;
            state->has_preferred_x = 0;
         } else {
            state->cursor = state->select_start = state->select_end = 0;
            state->has_preferred_x = 0;
         }
         break;

    case UI_KEY_TEXT_END:
         if (shift_mod) {
            ui_textedit_prep_selection_at_cursor(state);
            state->cursor = state->select_end = state->string.len;
            state->has_preferred_x = 0;
         } else {
            state->cursor = state->string.len;
            state->select_start = state->select_end = 0;
            state->has_preferred_x = 0;
         }
         break;

    case UI_KEY_TEXT_LINE_START: {
        if (shift_mod) {
            struct ui_text_find find;
           ui_textedit_clamp(state);
            ui_textedit_prep_selection_at_cursor(state);
            if (state->string.len && state->cursor == state->string.len)
                --state->cursor;
            ui_textedit_find_charpos(&find, state,state->cursor, state->single_line,
                font, row_height);
            state->cursor = state->select_end = find.first_char;
            state->has_preferred_x = 0;
        } else {
            struct ui_text_find find;
            if (state->string.len && state->cursor == state->string.len)
                --state->cursor;
            ui_textedit_clamp(state);
            ui_textedit_move_to_first(state);
            ui_textedit_find_charpos(&find, state, state->cursor, state->single_line,
                font, row_height);
            state->cursor = find.first_char;
            state->has_preferred_x = 0;
        }
      } break;

    case UI_KEY_TEXT_LINE_END: {
        if (shift_mod) {
            struct ui_text_find find;
            ui_textedit_clamp(state);
            ui_textedit_prep_selection_at_cursor(state);
            ui_textedit_find_charpos(&find, state, state->cursor, state->single_line,
                font, row_height);
            state->has_preferred_x = 0;
            state->cursor = find.first_char + find.length;
            if (find.length > 0 && ui_str_rune_at(&state->string, state->cursor-1) == '\n')
                --state->cursor;
            state->select_end = state->cursor;
        } else {
            struct ui_text_find find;
            ui_textedit_clamp(state);
            ui_textedit_move_to_first(state);
            ui_textedit_find_charpos(&find, state, state->cursor, state->single_line,
                font, row_height);

            state->has_preferred_x = 0;
            state->cursor = find.first_char + find.length;
            if (find.length > 0 && ui_str_rune_at(&state->string, state->cursor-1) == '\n')
                --state->cursor;
        }} break;
    }
}

UI_INTERN void
ui_textedit_flush_redo(struct ui_text_undo_state *state)
{
    state->redo_point = UI_TEXTEDIT_UNDOSTATECOUNT;
    state->redo_char_point = UI_TEXTEDIT_UNDOCHARCOUNT;
}

UI_INTERN void
ui_textedit_discard_undo(struct ui_text_undo_state *state)
{
    /* discard the oldest entry in the undo list */
    if (state->undo_point > 0) {
        /* if the 0th undo state has characters, clean those up */
        if (state->undo_rec[0].char_storage >= 0) {
            int n = state->undo_rec[0].insert_length, i;
            /* delete n characters from all other records */
            state->undo_char_point = (short)(state->undo_char_point - n);
            UI_MEMCPY(state->undo_char, state->undo_char + n,
                (ui_size)state->undo_char_point*sizeof(ui_rune));
            for (i=0; i < state->undo_point; ++i) {
                if (state->undo_rec[i].char_storage >= 0)
                state->undo_rec[i].char_storage = (short)
                    (state->undo_rec[i].char_storage - n);
            }
        }
        --state->undo_point;
        UI_MEMCPY(state->undo_rec, state->undo_rec+1,
            (ui_size)((ui_size)state->undo_point * sizeof(state->undo_rec[0])));
    }
}

UI_INTERN void
ui_textedit_discard_redo(struct ui_text_undo_state *state)
{
/*  discard the oldest entry in the redo list--it's bad if this
    ever happens, but because undo & redo have to store the actual
    characters in different cases, the redo character buffer can
    fill up even though the undo buffer didn't */
    ui_size num;
    int k = UI_TEXTEDIT_UNDOSTATECOUNT-1;
    if (state->redo_point <= k) {
        /* if the k'th undo state has characters, clean those up */
        if (state->undo_rec[k].char_storage >= 0) {
            int n = state->undo_rec[k].insert_length, i;
            /* delete n characters from all other records */
            state->redo_char_point = (short)(state->redo_char_point + n);
            num = (ui_size)(UI_TEXTEDIT_UNDOCHARCOUNT - state->redo_char_point);
            UI_MEMCPY(state->undo_char + state->redo_char_point,
                state->undo_char + state->redo_char_point-n, num * sizeof(char));
            for (i = state->redo_point; i < k; ++i) {
                if (state->undo_rec[i].char_storage >= 0) {
                    state->undo_rec[i].char_storage = (short)
                        (state->undo_rec[i].char_storage + n);
                }
            }
        }
        ++state->redo_point;
        num = (ui_size)(UI_TEXTEDIT_UNDOSTATECOUNT - state->redo_point);
        if (num) UI_MEMCPY(state->undo_rec + state->redo_point-1,
            state->undo_rec + state->redo_point, num * sizeof(state->undo_rec[0]));
    }
}

UI_INTERN struct ui_text_undo_record*
ui_textedit_create_undo_record(struct ui_text_undo_state *state, int numchars)
{
    /* any time we create a new undo record, we discard redo*/
    ui_textedit_flush_redo(state);

    /* if we have no free records, we have to make room,
     * by sliding the existing records down */
    if (state->undo_point == UI_TEXTEDIT_UNDOSTATECOUNT)
        ui_textedit_discard_undo(state);

    /* if the characters to store won't possibly fit in the buffer,
     * we can't undo */
    if (numchars > UI_TEXTEDIT_UNDOCHARCOUNT) {
        state->undo_point = 0;
        state->undo_char_point = 0;
        return 0;
    }

    /* if we don't have enough free characters in the buffer,
     * we have to make room */
    while (state->undo_char_point + numchars > UI_TEXTEDIT_UNDOCHARCOUNT)
        ui_textedit_discard_undo(state);
    return &state->undo_rec[state->undo_point++];
}

UI_INTERN ui_rune*
ui_textedit_createundo(struct ui_text_undo_state *state, int pos,
    int insert_len, int delete_len)
{
    struct ui_text_undo_record *r = ui_textedit_create_undo_record(state, insert_len);
    if (r == 0)
        return 0;

    r->where = pos;
    r->insert_length = (short) insert_len;
    r->delete_length = (short) delete_len;

    if (insert_len == 0) {
        r->char_storage = -1;
        return 0;
    } else {
        r->char_storage = state->undo_char_point;
        state->undo_char_point = (short)(state->undo_char_point +  insert_len);
        return &state->undo_char[r->char_storage];
    }
}

UI_API void
ui_textedit_undo(struct ui_text_edit *state)
{
    struct ui_text_undo_state *s = &state->undo;
    struct ui_text_undo_record u, *r;
    if (s->undo_point == 0)
        return;

    /* we need to do two things: apply the undo record, and create a redo record */
    u = s->undo_rec[s->undo_point-1];
    r = &s->undo_rec[s->redo_point-1];
    r->char_storage = -1;

    r->insert_length = u.delete_length;
    r->delete_length = u.insert_length;
    r->where = u.where;

    if (u.delete_length)
    {
       /*   if the undo record says to delete characters, then the redo record will
            need to re-insert the characters that get deleted, so we need to store
            them.
            there are three cases:
                - there's enough room to store the characters
                - characters stored for *redoing* don't leave room for redo
                - characters stored for *undoing* don't leave room for redo
            if the last is true, we have to bail */
        if (s->undo_char_point + u.delete_length >= UI_TEXTEDIT_UNDOCHARCOUNT) {
            /* the undo records take up too much character space; there's no space
            * to store the redo characters */
            r->insert_length = 0;
        } else {
            int i;
            /* there's definitely room to store the characters eventually */
            while (s->undo_char_point + u.delete_length > s->redo_char_point) {
                /* there's currently not enough room, so discard a redo record */
                ui_textedit_discard_redo(s);
                /* should never happen: */
                if (s->redo_point == UI_TEXTEDIT_UNDOSTATECOUNT)
                    return;
            }

            r = &s->undo_rec[s->redo_point-1];
            r->char_storage = (short)(s->redo_char_point - u.delete_length);
            s->redo_char_point = (short)(s->redo_char_point -  u.delete_length);

            /* now save the characters */
            for (i=0; i < u.delete_length; ++i)
                s->undo_char[r->char_storage + i] =
                    ui_str_rune_at(&state->string, u.where + i);
        }
        /* now we can carry out the deletion */
        ui_str_delete_runes(&state->string, u.where, u.delete_length);
    }

    /* check type of recorded action: */
    if (u.insert_length) {
        /* easy case: was a deletion, so we need to insert n characters */
        ui_str_insert_text_runes(&state->string, u.where,
            &s->undo_char[u.char_storage], u.insert_length);
        s->undo_char_point = (short)(s->undo_char_point - u.insert_length);
    }
    state->cursor = (short)(u.where + u.insert_length);

    s->undo_point--;
    s->redo_point--;
}

UI_API void
ui_textedit_redo(struct ui_text_edit *state)
{
    struct ui_text_undo_state *s = &state->undo;
    struct ui_text_undo_record *u, r;
    if (s->redo_point == UI_TEXTEDIT_UNDOSTATECOUNT)
        return;

    /* we need to do two things: apply the redo record, and create an undo record */
    u = &s->undo_rec[s->undo_point];
    r = s->undo_rec[s->redo_point];

    /* we KNOW there must be room for the undo record, because the redo record
    was derived from an undo record */
    u->delete_length = r.insert_length;
    u->insert_length = r.delete_length;
    u->where = r.where;
    u->char_storage = -1;

    if (r.delete_length) {
        /* the redo record requires us to delete characters, so the undo record
        needs to store the characters */
        if (s->undo_char_point + u->insert_length > s->redo_char_point) {
            u->insert_length = 0;
            u->delete_length = 0;
        } else {
            int i;
            u->char_storage = s->undo_char_point;
            s->undo_char_point = (short)(s->undo_char_point + u->insert_length);

            /* now save the characters */
            for (i=0; i < u->insert_length; ++i) {
                s->undo_char[u->char_storage + i] =
                    ui_str_rune_at(&state->string, u->where + i);
            }
        }
        ui_str_delete_runes(&state->string, r.where, r.delete_length);
    }

    if (r.insert_length) {
        /* easy case: need to insert n characters */
        ui_str_insert_text_runes(&state->string, r.where,
            &s->undo_char[r.char_storage], r.insert_length);
    }
    state->cursor = r.where + r.insert_length;

    s->undo_point++;
    s->redo_point++;
}

UI_INTERN void
ui_textedit_makeundo_insert(struct ui_text_edit *state, int where, int length)
{
    ui_textedit_createundo(&state->undo, where, 0, length);
}

UI_INTERN void
ui_textedit_makeundo_delete(struct ui_text_edit *state, int where, int length)
{
    int i;
    ui_rune *p = ui_textedit_createundo(&state->undo, where, length, 0);
    if (p) {
        for (i=0; i < length; ++i)
            p[i] = ui_str_rune_at(&state->string, where+i);
    }
}

UI_INTERN void
ui_textedit_makeundo_replace(struct ui_text_edit *state, int where,
    int old_length, int new_length)
{
    int i;
    ui_rune *p = ui_textedit_createundo(&state->undo, where, old_length, new_length);
    if (p) {
        for (i=0; i < old_length; ++i)
            p[i] = ui_str_rune_at(&state->string, where+i);
    }
}

UI_INTERN void
ui_textedit_clear_state(struct ui_text_edit *state, enum ui_text_edit_type type,
    ui_plugin_filter filter)
{
    /* reset the state to default */
   state->undo.undo_point = 0;
   state->undo.undo_char_point = 0;
   state->undo.redo_point = UI_TEXTEDIT_UNDOSTATECOUNT;
   state->undo.redo_char_point = UI_TEXTEDIT_UNDOCHARCOUNT;
   state->select_end = state->select_start = 0;
   state->cursor = 0;
   state->has_preferred_x = 0;
   state->preferred_x = 0;
   state->cursor_at_end_of_line = 0;
   state->initialized = 1;
   state->single_line = (unsigned char)(type == UI_TEXT_EDIT_SINGLE_LINE);
   state->mode = UI_TEXT_EDIT_MODE_VIEW;
   state->filter = filter;
   state->scrollbar = ui_vec2(0,0);
}

UI_API void
ui_textedit_init_fixed(struct ui_text_edit *state, void *memory, ui_size size)
{
    UI_ASSERT(state);
    UI_ASSERT(memory);
    if (!state || !memory || !size) return;
    ui_textedit_clear_state(state, UI_TEXT_EDIT_SINGLE_LINE, 0);
    ui_str_init_fixed(&state->string, memory, size);
}

UI_API void
ui_textedit_init(struct ui_text_edit *state, struct ui_allocator *alloc, ui_size size)
{
    UI_ASSERT(state);
    UI_ASSERT(alloc);
    if (!state || !alloc) return;
    ui_textedit_clear_state(state, UI_TEXT_EDIT_SINGLE_LINE, 0);
    ui_str_init(&state->string, alloc, size);
}

#ifdef UI_INCLUDE_DEFAULT_ALLOCATOR
UI_API void
ui_textedit_init_default(struct ui_text_edit *state)
{
    UI_ASSERT(state);
    if (!state) return;
    ui_textedit_clear_state(state, UI_TEXT_EDIT_SINGLE_LINE, 0);
    ui_str_init_default(&state->string);
}
#endif

UI_API void
ui_textedit_select_all(struct ui_text_edit *state)
{
    UI_ASSERT(state);
    state->select_start = 0;
    state->select_end = state->string.len;
}

UI_API void
ui_textedit_free(struct ui_text_edit *state)
{
    UI_ASSERT(state);
    if (!state) return;
    ui_str_free(&state->string);
}

/* ===============================================================
 *
 *                          TEXT WIDGET
 *
 * ===============================================================*/
#define ui_widget_state_reset(s)\
    if ((*(s)) & UI_WIDGET_STATE_MODIFIED)\
        (*(s)) = UI_WIDGET_STATE_INACTIVE|UI_WIDGET_STATE_MODIFIED;\
    else (*(s)) = UI_WIDGET_STATE_INACTIVE;

struct ui_text {
    struct ui_vec2 padding;
    struct ui_color background;
    struct ui_color text;
};

UI_INTERN void
ui_widget_text(struct ui_command_buffer *o, struct ui_rect b,
    const char *string, int len, const struct ui_text *t,
    ui_flags a, const struct ui_user_font *f)
{
    struct ui_rect label;
    float text_width;

    UI_ASSERT(o);
    UI_ASSERT(t);
    if (!o || !t) return;

    b.h = UI_MAX(b.h, 2 * t->padding.y);
    label.x = 0; label.w = 0;
    label.y = b.y + t->padding.y;
    label.h = UI_MIN(f->height, b.h - 2 * t->padding.y);

    text_width = f->width(f->userdata, f->height, (const char*)string, len);
    text_width += (2.0f * t->padding.x);

    /* align in x-axis */
    if (a & UI_TEXT_ALIGN_LEFT) {
        label.x = b.x + t->padding.x;
        label.w = UI_MAX(0, b.w - 2 * t->padding.x);
    } else if (a & UI_TEXT_ALIGN_CENTERED) {
        label.w = UI_MAX(1, 2 * t->padding.x + (float)text_width);
        label.x = (b.x + t->padding.x + ((b.w - 2 * t->padding.x) - label.w) / 2);
        label.x = UI_MAX(b.x + t->padding.x, label.x);
        label.w = UI_MIN(b.x + b.w, label.x + label.w);
        if (label.w >= label.x) label.w -= label.x;
    } else if (a & UI_TEXT_ALIGN_RIGHT) {
        label.x = UI_MAX(b.x + t->padding.x, (b.x + b.w) - (2 * t->padding.x + (float)text_width));
        label.w = (float)text_width + 2 * t->padding.x;
    } else return;

    /* align in y-axis */
    if (a & UI_TEXT_ALIGN_MIDDLE) {
        label.y = b.y + b.h/2.0f - (float)f->height/2.0f;
        label.h = UI_MAX(b.h/2.0f, b.h - (b.h/2.0f + f->height/2.0f));
    } else if (a & UI_TEXT_ALIGN_BOTTOM) {
        label.y = b.y + b.h - f->height;
        label.h = f->height;
    }
    ui_draw_text(o, label, (const char*)string,
        len, f, t->background, t->text);
}

UI_INTERN void
ui_widget_text_wrap(struct ui_command_buffer *o, struct ui_rect b,
    const char *string, int len, const struct ui_text *t,
    const struct ui_user_font *f)
{
    float width;
    int glyphs = 0;
    int fitting = 0;
    int done = 0;
    struct ui_rect line;
    struct ui_text text;
    UI_INTERN ui_rune seperator[] = {' '};

    UI_ASSERT(o);
    UI_ASSERT(t);
    if (!o || !t) return;

    text.padding = ui_vec2(0,0);
    text.background = t->background;
    text.text = t->text;

    b.w = UI_MAX(b.w, 2 * t->padding.x);
    b.h = UI_MAX(b.h, 2 * t->padding.y);
    b.h = b.h - 2 * t->padding.y;

    line.x = b.x + t->padding.x;
    line.y = b.y + t->padding.y;
    line.w = b.w - 2 * t->padding.x;
    line.h = 2 * t->padding.y + f->height;

    fitting = ui_text_clamp(f, string, len, line.w, &glyphs, &width, seperator,UI_LEN(seperator));
    while (done < len) {
        if (!fitting || line.y + line.h >= (b.y + b.h)) break;
        ui_widget_text(o, line, &string[done], fitting, &text, UI_TEXT_LEFT, f);
        done += fitting;
        line.y += f->height + 2 * t->padding.y;
        fitting = ui_text_clamp(f, &string[done], len - done, line.w, &glyphs, &width, seperator,UI_LEN(seperator));
    }
}

/* ===============================================================
 *
 *                          BUTTON
 *
 * ===============================================================*/
UI_INTERN void
ui_draw_symbol(struct ui_command_buffer *out, enum ui_symbol_type type,
    struct ui_rect content, struct ui_color background, struct ui_color foreground,
    float border_width, const struct ui_user_font *font)
{
    switch (type) {
    case UI_SYMBOL_X:
    case UI_SYMBOL_UNDERSCORE:
    case UI_SYMBOL_PLUS:
    case UI_SYMBOL_MINUS: {
        /* single character text symbol */
        const char *X = (type == UI_SYMBOL_X) ? "x":
            (type == UI_SYMBOL_UNDERSCORE) ? "_":
            (type == UI_SYMBOL_PLUS) ? "+": "-";
        struct ui_text text;
        text.padding = ui_vec2(0,0);
        text.background = background;
        text.text = foreground;
        ui_widget_text(out, content, X, 1, &text, UI_TEXT_CENTERED, font);
    } break;
    case UI_SYMBOL_CIRCLE_SOLID:
    case UI_SYMBOL_CIRCLE_OUTLINE:
    case UI_SYMBOL_RECT_SOLID:
    case UI_SYMBOL_RECT_OUTLINE: {
        /* simple empty/filled shapes */
        if (type == UI_SYMBOL_RECT_SOLID || type == UI_SYMBOL_RECT_OUTLINE) {
            ui_fill_rect(out, content,  0, foreground);
            if (type == UI_SYMBOL_RECT_OUTLINE)
                ui_fill_rect(out, ui_shriui_rect(content, border_width), 0, background);
        } else {
            ui_fill_circle(out, content, foreground);
            if (type == UI_SYMBOL_CIRCLE_OUTLINE)
                ui_fill_circle(out, ui_shriui_rect(content, 1), background);
        }
    } break;
    case UI_SYMBOL_TRIANGLE_UP:
    case UI_SYMBOL_TRIANGLE_DOWN:
    case UI_SYMBOL_TRIANGLE_LEFT:
    case UI_SYMBOL_TRIANGLE_RIGHT: {
        enum ui_heading heading;
        struct ui_vec2 points[3];
        heading = (type == UI_SYMBOL_TRIANGLE_RIGHT) ? UI_RIGHT :
            (type == UI_SYMBOL_TRIANGLE_LEFT) ? UI_LEFT:
            (type == UI_SYMBOL_TRIANGLE_UP) ? UI_UP: UI_DOWN;
        ui_triangle_from_direction(points, content, 0, 0, heading);
        ui_fill_triangle(out, points[0].x, points[0].y, points[1].x, points[1].y,
            points[2].x, points[2].y, foreground);
    } break;
    default:
    case UI_SYMBOL_NONE:
    case UI_SYMBOL_MAX: break;
    }
}

UI_INTERN int
ui_button_behavior(ui_flags *state, struct ui_rect r,
    const struct ui_input *i, enum ui_button_behavior behavior)
{
    int ret = 0;
    ui_widget_state_reset(state);
    if (!i) return 0;
    if (ui_input_is_mouse_hovering_rect(i, r)) {
        *state = UI_WIDGET_STATE_HOVERED;
        if (ui_input_is_mouse_down(i, UI_BUTTON_LEFT))
            *state = UI_WIDGET_STATE_ACTIVE;
        if (ui_input_has_mouse_click_in_rect(i, UI_BUTTON_LEFT, r)) {
            ret = (behavior != UI_BUTTON_DEFAULT) ?
                ui_input_is_mouse_down(i, UI_BUTTON_LEFT):
#ifdef UI_BUTTON_TRIGGER_ON_RELEASE
                ui_input_is_mouse_released(i, UI_BUTTON_LEFT);
#else
                ui_input_is_mouse_pressed(i, UI_BUTTON_LEFT);
#endif
        }
    }
    if (*state & UI_WIDGET_STATE_HOVER && !ui_input_is_mouse_prev_hovering_rect(i, r))
        *state |= UI_WIDGET_STATE_ENTERED;
    else if (ui_input_is_mouse_prev_hovering_rect(i, r))
        *state |= UI_WIDGET_STATE_LEFT;
    return ret;
}

UI_INTERN const struct ui_style_item*
ui_draw_button(struct ui_command_buffer *out,
    const struct ui_rect *bounds, ui_flags state,
    const struct ui_style_button *style)
{
    const struct ui_style_item *background;
    if (state & UI_WIDGET_STATE_HOVER)
        background = &style->hover;
    else if (state & UI_WIDGET_STATE_ACTIVED)
        background = &style->active;
    else background = &style->normal;

    if (background->type == UI_STYLE_ITEM_IMAGE) {
        ui_draw_image(out, *bounds, &background->data.image, ui_white);
    } else {
        ui_fill_rect(out, *bounds, style->rounding, background->data.color);
        ui_stroke_rect(out, *bounds, style->rounding, style->border, style->border_color);
    }
    return background;
}

UI_INTERN int
ui_do_button(ui_flags *state, struct ui_command_buffer *out, struct ui_rect r,
    const struct ui_style_button *style, const struct ui_input *in,
    enum ui_button_behavior behavior, struct ui_rect *content)
{
    struct ui_rect bounds;
    UI_ASSERT(style);
    UI_ASSERT(state);
    UI_ASSERT(out);
    if (!out || !style)
        return ui_false;

    /* calculate button content space */
    content->x = r.x + style->padding.x + style->border + style->rounding;
    content->y = r.y + style->padding.y + style->border + style->rounding;
    content->w = r.w - (2 * style->padding.x + style->border + style->rounding*2);
    content->h = r.h - (2 * style->padding.y + style->border + style->rounding*2);

    /* execute button behavior */
    bounds.x = r.x - style->touch_padding.x;
    bounds.y = r.y - style->touch_padding.y;
    bounds.w = r.w + 2 * style->touch_padding.x;
    bounds.h = r.h + 2 * style->touch_padding.y;
    return ui_button_behavior(state, bounds, in, behavior);
}

UI_INTERN void
ui_draw_button_text(struct ui_command_buffer *out,
    const struct ui_rect *bounds, const struct ui_rect *content, ui_flags state,
    const struct ui_style_button *style, const char *txt, int len,
    ui_flags text_alignment, const struct ui_user_font *font)
{
    struct ui_text text;
    const struct ui_style_item *background;
    background = ui_draw_button(out, bounds, state, style);

    /* select correct colors/images */
    if (background->type == UI_STYLE_ITEM_COLOR)
        text.background = background->data.color;
    else text.background = style->text_background;
    if (state & UI_WIDGET_STATE_HOVER)
        text.text = style->text_hover;
    else if (state & UI_WIDGET_STATE_ACTIVED)
        text.text = style->text_active;
    else text.text = style->text_normal;

    text.padding = ui_vec2(0,0);
    ui_widget_text(out, *content, txt, len, &text, text_alignment, font);
}

UI_INTERN int
ui_do_button_text(ui_flags *state,
    struct ui_command_buffer *out, struct ui_rect bounds,
    const char *string, int len, ui_flags align, enum ui_button_behavior behavior,
    const struct ui_style_button *style, const struct ui_input *in,
    const struct ui_user_font *font)
{
    struct ui_rect content;
    int ret = ui_false;

    UI_ASSERT(state);
    UI_ASSERT(style);
    UI_ASSERT(out);
    UI_ASSERT(string);
    UI_ASSERT(font);
    if (!out || !style || !font || !string)
        return ui_false;

    ret = ui_do_button(state, out, bounds, style, in, behavior, &content);
    if (style->draw_begin) style->draw_begin(out, style->userdata);
    ui_draw_button_text(out, &bounds, &content, *state, style, string, len, align, font);
    if (style->draw_end) style->draw_end(out, style->userdata);
    return ret;
}

UI_INTERN void
ui_draw_button_symbol(struct ui_command_buffer *out,
    const struct ui_rect *bounds, const struct ui_rect *content,
    ui_flags state, const struct ui_style_button *style,
    enum ui_symbol_type type, const struct ui_user_font *font)
{
    struct ui_color sym, bg;
    const struct ui_style_item *background;

    /* select correct colors/images */
    background = ui_draw_button(out, bounds, state, style);
    if (background->type == UI_STYLE_ITEM_COLOR)
        bg = background->data.color;
    else bg = style->text_background;

    if (state & UI_WIDGET_STATE_HOVER)
        sym = style->text_hover;
    else if (state & UI_WIDGET_STATE_ACTIVED)
        sym = style->text_active;
    else sym = style->text_normal;
    ui_draw_symbol(out, type, *content, bg, sym, 1, font);
}

UI_INTERN int
ui_do_button_symbol(ui_flags *state,
    struct ui_command_buffer *out, struct ui_rect bounds,
    enum ui_symbol_type symbol, enum ui_button_behavior behavior,
    const struct ui_style_button *style, const struct ui_input *in,
    const struct ui_user_font *font)
{
    int ret;
    struct ui_rect content;

    UI_ASSERT(state);
    UI_ASSERT(style);
    UI_ASSERT(font);
    UI_ASSERT(out);
    if (!out || !style || !font || !state)
        return ui_false;

    ret = ui_do_button(state, out, bounds, style, in, behavior, &content);
    if (style->draw_begin) style->draw_begin(out, style->userdata);
    ui_draw_button_symbol(out, &bounds, &content, *state, style, symbol, font);
    if (style->draw_end) style->draw_end(out, style->userdata);
    return ret;
}

UI_INTERN void
ui_draw_button_image(struct ui_command_buffer *out,
    const struct ui_rect *bounds, const struct ui_rect *content,
    ui_flags state, const struct ui_style_button *style, const struct ui_image *img)
{
    ui_draw_button(out, bounds, state, style);
    ui_draw_image(out, *content, img, ui_white);
}

UI_INTERN int
ui_do_button_image(ui_flags *state,
    struct ui_command_buffer *out, struct ui_rect bounds,
    struct ui_image img, enum ui_button_behavior b,
    const struct ui_style_button *style, const struct ui_input *in)
{
    int ret;
    struct ui_rect content;

    UI_ASSERT(state);
    UI_ASSERT(style);
    UI_ASSERT(out);
    if (!out || !style || !state)
        return ui_false;

    ret = ui_do_button(state, out, bounds, style, in, b, &content);
    content.x += style->image_padding.x;
    content.y += style->image_padding.y;
    content.w -= 2 * style->image_padding.x;
    content.h -= 2 * style->image_padding.y;

    if (style->draw_begin) style->draw_begin(out, style->userdata);
    ui_draw_button_image(out, &bounds, &content, *state, style, &img);
    if (style->draw_end) style->draw_end(out, style->userdata);
    return ret;
}

UI_INTERN void
ui_draw_button_text_symbol(struct ui_command_buffer *out,
    const struct ui_rect *bounds, const struct ui_rect *label,
    const struct ui_rect *symbol, ui_flags state, const struct ui_style_button *style,
    const char *str, int len, enum ui_symbol_type type,
    const struct ui_user_font *font)
{
    struct ui_color sym;
    struct ui_text text;
    const struct ui_style_item *background;

    /* select correct background colors/images */
    background = ui_draw_button(out, bounds, state, style);
    if (background->type == UI_STYLE_ITEM_COLOR)
        text.background = background->data.color;
    else text.background = style->text_background;

    /* select correct text colors */
    if (state & UI_WIDGET_STATE_HOVER) {
        sym = style->text_hover;
        text.text = style->text_hover;
    } else if (state & UI_WIDGET_STATE_ACTIVED) {
        sym = style->text_active;
        text.text = style->text_active;
    } else {
        sym = style->text_normal;
        text.text = style->text_normal;
    }

    text.padding = ui_vec2(0,0);
    ui_draw_symbol(out, type, *symbol, style->text_background, sym, 0, font);
    ui_widget_text(out, *label, str, len, &text, UI_TEXT_CENTERED, font);
}

UI_INTERN int
ui_do_button_text_symbol(ui_flags *state,
    struct ui_command_buffer *out, struct ui_rect bounds,
    enum ui_symbol_type symbol, const char *str, int len, ui_flags align,
    enum ui_button_behavior behavior, const struct ui_style_button *style,
    const struct ui_user_font *font, const struct ui_input *in)
{
    int ret;
    struct ui_rect tri = {0,0,0,0};
    struct ui_rect content;

    UI_ASSERT(style);
    UI_ASSERT(out);
    UI_ASSERT(font);
    if (!out || !style || !font)
        return ui_false;

    ret = ui_do_button(state, out, bounds, style, in, behavior, &content);
    tri.y = content.y + (content.h/2) - font->height/2;
    tri.w = font->height; tri.h = font->height;
    if (align & UI_TEXT_ALIGN_LEFT) {
        tri.x = (content.x + content.w) - (2 * style->padding.x + tri.w);
        tri.x = UI_MAX(tri.x, 0);
    } else tri.x = content.x + 2 * style->padding.x;

    /* draw button */
    if (style->draw_begin) style->draw_begin(out, style->userdata);
    ui_draw_button_text_symbol(out, &bounds, &content, &tri,
        *state, style, str, len, symbol, font);
    if (style->draw_end) style->draw_end(out, style->userdata);
    return ret;
}

UI_INTERN void
ui_draw_button_text_image(struct ui_command_buffer *out,
    const struct ui_rect *bounds, const struct ui_rect *label,
    const struct ui_rect *image, ui_flags state, const struct ui_style_button *style,
    const char *str, int len, const struct ui_user_font *font,
    const struct ui_image *img)
{
    struct ui_text text;
    const struct ui_style_item *background;
    background = ui_draw_button(out, bounds, state, style);

    /* select correct colors */
    if (background->type == UI_STYLE_ITEM_COLOR)
        text.background = background->data.color;
    else text.background = style->text_background;
    if (state & UI_WIDGET_STATE_HOVER)
        text.text = style->text_hover;
    else if (state & UI_WIDGET_STATE_ACTIVED)
        text.text = style->text_active;
    else text.text = style->text_normal;

    text.padding = ui_vec2(0,0);
    ui_widget_text(out, *label, str, len, &text, UI_TEXT_CENTERED, font);
    ui_draw_image(out, *image, img, ui_white);
}

UI_INTERN int
ui_do_button_text_image(ui_flags *state,
    struct ui_command_buffer *out, struct ui_rect bounds,
    struct ui_image img, const char* str, int len, ui_flags align,
    enum ui_button_behavior behavior, const struct ui_style_button *style,
    const struct ui_user_font *font, const struct ui_input *in)
{
    int ret;
    struct ui_rect icon;
    struct ui_rect content;

    UI_ASSERT(style);
    UI_ASSERT(state);
    UI_ASSERT(font);
    UI_ASSERT(out);
    if (!out || !font || !style || !str)
        return ui_false;

    ret = ui_do_button(state, out, bounds, style, in, behavior, &content);
    icon.y = bounds.y + style->padding.y;
    icon.w = icon.h = bounds.h - 2 * style->padding.y;
    if (align & UI_TEXT_ALIGN_LEFT) {
        icon.x = (bounds.x + bounds.w) - (2 * style->padding.x + icon.w);
        icon.x = UI_MAX(icon.x, 0);
    } else icon.x = bounds.x + 2 * style->padding.x;

    icon.x += style->image_padding.x;
    icon.y += style->image_padding.y;
    icon.w -= 2 * style->image_padding.x;
    icon.h -= 2 * style->image_padding.y;

    if (style->draw_begin) style->draw_begin(out, style->userdata);
    ui_draw_button_text_image(out, &bounds, &content, &icon, *state, style, str, len, font, &img);
    if (style->draw_end) style->draw_end(out, style->userdata);
    return ret;
}

/* ===============================================================
 *
 *                          TOGGLE
 *
 * ===============================================================*/
enum ui_toggle_type {
    UI_TOGGLE_CHECK,
    UI_TOGGLE_OPTION
};

UI_INTERN int
ui_toggle_behavior(const struct ui_input *in, struct ui_rect select,
    ui_flags *state, int active)
{
    ui_widget_state_reset(state);
    if (ui_button_behavior(state, select, in, UI_BUTTON_DEFAULT)) {
        *state = UI_WIDGET_STATE_ACTIVE;
        active = !active;
    }
    if (*state & UI_WIDGET_STATE_HOVER && !ui_input_is_mouse_prev_hovering_rect(in, select))
        *state |= UI_WIDGET_STATE_ENTERED;
    else if (ui_input_is_mouse_prev_hovering_rect(in, select))
        *state |= UI_WIDGET_STATE_LEFT;
    return active;
}

UI_INTERN void
ui_draw_checkbox(struct ui_command_buffer *out,
    ui_flags state, const struct ui_style_toggle *style, int active,
    const struct ui_rect *label, const struct ui_rect *selector,
    const struct ui_rect *cursors, const char *string, int len,
    const struct ui_user_font *font)
{
    const struct ui_style_item *background;
    const struct ui_style_item *cursor;
    struct ui_text text;

    /* select correct colors/images */
    if (state & UI_WIDGET_STATE_HOVER) {
        background = &style->hover;
        cursor = &style->cursor_hover;
        text.text = style->text_hover;
    } else if (state & UI_WIDGET_STATE_ACTIVED) {
        background = &style->hover;
        cursor = &style->cursor_hover;
        text.text = style->text_active;
    } else {
        background = &style->normal;
        cursor = &style->cursor_normal;
        text.text = style->text_normal;
    }

    /* draw background and cursor */
    if (background->type == UI_STYLE_ITEM_COLOR) {
        ui_fill_rect(out, *selector, 0, style->border_color);
        ui_fill_rect(out, ui_shriui_rect(*selector, style->border), 0, background->data.color);
    } else ui_draw_image(out, *selector, &background->data.image, ui_white);
    if (active) {
        if (cursor->type == UI_STYLE_ITEM_IMAGE)
            ui_draw_image(out, *cursors, &cursor->data.image, ui_white);
        else ui_fill_rect(out, *cursors, 0, cursor->data.color);
    }

    text.padding.x = 0;
    text.padding.y = 0;
    text.background = style->text_background;
    ui_widget_text(out, *label, string, len, &text, UI_TEXT_LEFT, font);
}

UI_INTERN void
ui_draw_option(struct ui_command_buffer *out,
    ui_flags state, const struct ui_style_toggle *style, int active,
    const struct ui_rect *label, const struct ui_rect *selector,
    const struct ui_rect *cursors, const char *string, int len,
    const struct ui_user_font *font)
{
    const struct ui_style_item *background;
    const struct ui_style_item *cursor;
    struct ui_text text;

    /* select correct colors/images */
    if (state & UI_WIDGET_STATE_HOVER) {
        background = &style->hover;
        cursor = &style->cursor_hover;
        text.text = style->text_hover;
    } else if (state & UI_WIDGET_STATE_ACTIVED) {
        background = &style->hover;
        cursor = &style->cursor_hover;
        text.text = style->text_active;
    } else {
        background = &style->normal;
        cursor = &style->cursor_normal;
        text.text = style->text_normal;
    }

    /* draw background and cursor */
    if (background->type == UI_STYLE_ITEM_COLOR) {
        ui_fill_circle(out, *selector, style->border_color);
        ui_fill_circle(out, ui_shriui_rect(*selector, style->border), background->data.color);
    } else ui_draw_image(out, *selector, &background->data.image, ui_white);
    if (active) {
        if (cursor->type == UI_STYLE_ITEM_IMAGE)
            ui_draw_image(out, *cursors, &cursor->data.image, ui_white);
        else ui_fill_circle(out, *cursors, cursor->data.color);
    }

    text.padding.x = 0;
    text.padding.y = 0;
    text.background = style->text_background;
    ui_widget_text(out, *label, string, len, &text, UI_TEXT_LEFT, font);
}

UI_INTERN int
ui_do_toggle(ui_flags *state,
    struct ui_command_buffer *out, struct ui_rect r,
    int *active, const char *str, int len, enum ui_toggle_type type,
    const struct ui_style_toggle *style, const struct ui_input *in,
    const struct ui_user_font *font)
{
    int was_active;
    struct ui_rect bounds;
    struct ui_rect select;
    struct ui_rect cursor;
    struct ui_rect label;

    UI_ASSERT(style);
    UI_ASSERT(out);
    UI_ASSERT(font);
    if (!out || !style || !font || !active)
        return 0;

    r.w = UI_MAX(r.w, font->height + 2 * style->padding.x);
    r.h = UI_MAX(r.h, font->height + 2 * style->padding.y);

    /* add additional touch padding for touch screen devices */
    bounds.x = r.x - style->touch_padding.x;
    bounds.y = r.y - style->touch_padding.y;
    bounds.w = r.w + 2 * style->touch_padding.x;
    bounds.h = r.h + 2 * style->touch_padding.y;

    /* calculate the selector space */
    select.w = font->height;
    select.h = select.w;
    select.y = r.y + r.h/2.0f - select.h/2.0f;
    select.x = r.x;

    /* calculate the bounds of the cursor inside the selector */
    cursor.x = select.x + style->padding.x + style->border;
    cursor.y = select.y + style->padding.y + style->border;
    cursor.w = select.w - (2 * style->padding.x + 2 * style->border);
    cursor.h = select.h - (2 * style->padding.y + 2 * style->border);

    /* label behind the selector */
    label.x = select.x + select.w + style->spacing;
    label.y = select.y;
    label.w = UI_MAX(r.x + r.w, label.x) - label.x;
    label.h = select.w;

    /* update selector */
    was_active = *active;
    *active = ui_toggle_behavior(in, bounds, state, *active);

    /* draw selector */
    if (style->draw_begin)
        style->draw_begin(out, style->userdata);
    if (type == UI_TOGGLE_CHECK) {
        ui_draw_checkbox(out, *state, style, *active, &label, &select, &cursor, str, len, font);
    } else {
        ui_draw_option(out, *state, style, *active, &label, &select, &cursor, str, len, font);
    }
    if (style->draw_end)
        style->draw_end(out, style->userdata);
    return (was_active != *active);
}

/* ===============================================================
 *
 *                          SELECTABLE
 *
 * ===============================================================*/
UI_INTERN void
ui_draw_selectable(struct ui_command_buffer *out,
    ui_flags state, const struct ui_style_selectable *style, int active,
    const struct ui_rect *bounds, const struct ui_rect *icon, const struct ui_image *img,
    const char *string, int len, ui_flags align, const struct ui_user_font *font)
{
    const struct ui_style_item *background;
    struct ui_text text;
    text.padding = style->padding;

    /* select correct colors/images */
    if (!active) {
        if (state & UI_WIDGET_STATE_ACTIVED) {
            background = &style->pressed;
            text.text = style->text_pressed;
        } else if (state & UI_WIDGET_STATE_HOVER) {
            background = &style->hover;
            text.text = style->text_hover;
        } else {
            background = &style->normal;
            text.text = style->text_normal;
        }
    } else {
        if (state & UI_WIDGET_STATE_ACTIVED) {
            background = &style->pressed_active;
            text.text = style->text_pressed_active;
        } else if (state & UI_WIDGET_STATE_HOVER) {
            background = &style->hover_active;
            text.text = style->text_hover_active;
        } else {
            background = &style->normal_active;
            text.text = style->text_normal_active;
        }
    }


    /* draw selectable background and text */
    if (background->type == UI_STYLE_ITEM_IMAGE) {
        ui_draw_image(out, *bounds, &background->data.image, ui_white);
        text.background = ui_rgba(0,0,0,0);
    } else {
        ui_fill_rect(out, *bounds, style->rounding, background->data.color);
        text.background = background->data.color;
    }
    if (img && icon) ui_draw_image(out, *icon, img, ui_white);
    ui_widget_text(out, *bounds, string, len, &text, align, font);
}

UI_INTERN int
ui_do_selectable(ui_flags *state, struct ui_command_buffer *out,
    struct ui_rect bounds, const char *str, int len, ui_flags align, int *value,
    const struct ui_style_selectable *style, const struct ui_input *in,
    const struct ui_user_font *font)
{
    int old_value;
    struct ui_rect touch;

    UI_ASSERT(state);
    UI_ASSERT(out);
    UI_ASSERT(str);
    UI_ASSERT(len);
    UI_ASSERT(value);
    UI_ASSERT(style);
    UI_ASSERT(font);

    if (!state || !out || !str || !len || !value || !style || !font) return 0;
    old_value = *value;

    /* remove padding */
    touch.x = bounds.x - style->touch_padding.x;
    touch.y = bounds.y - style->touch_padding.y;
    touch.w = bounds.w + style->touch_padding.x * 2;
    touch.h = bounds.h + style->touch_padding.y * 2;

    /* update button */
    if (ui_button_behavior(state, touch, in, UI_BUTTON_DEFAULT))
        *value = !(*value);

    /* draw selectable */
    if (style->draw_begin) style->draw_begin(out, style->userdata);
    ui_draw_selectable(out, *state, style, *value, &bounds, 0,0, str, len, align, font);
    if (style->draw_end) style->draw_end(out, style->userdata);
    return old_value != *value;
}

UI_INTERN int
ui_do_selectable_image(ui_flags *state, struct ui_command_buffer *out,
    struct ui_rect bounds, const char *str, int len, ui_flags align, int *value,
    const struct ui_image *img, const struct ui_style_selectable *style,
    const struct ui_input *in, const struct ui_user_font *font)
{
    int old_value;
    struct ui_rect touch;
    struct ui_rect icon;

    UI_ASSERT(state);
    UI_ASSERT(out);
    UI_ASSERT(str);
    UI_ASSERT(len);
    UI_ASSERT(value);
    UI_ASSERT(style);
    UI_ASSERT(font);

    if (!state || !out || !str || !len || !value || !style || !font) return 0;
    old_value = *value;

    /* toggle behavior */
    touch.x = bounds.x - style->touch_padding.x;
    touch.y = bounds.y - style->touch_padding.y;
    touch.w = bounds.w + style->touch_padding.x * 2;
    touch.h = bounds.h + style->touch_padding.y * 2;
    if (ui_button_behavior(state, touch, in, UI_BUTTON_DEFAULT))
        *value = !(*value);

    icon.y = bounds.y + style->padding.y;
    icon.w = icon.h = bounds.h - 2 * style->padding.y;
    if (align & UI_TEXT_ALIGN_LEFT) {
        icon.x = (bounds.x + bounds.w) - (2 * style->padding.x + icon.w);
        icon.x = UI_MAX(icon.x, 0);
    } else icon.x = bounds.x + 2 * style->padding.x;

    icon.x += style->image_padding.x;
    icon.y += style->image_padding.y;
    icon.w -= 2 * style->image_padding.x;
    icon.h -= 2 * style->image_padding.y;

    /* draw selectable */
    if (style->draw_begin) style->draw_begin(out, style->userdata);
    ui_draw_selectable(out, *state, style, *value, &bounds, &icon, img, str, len, align, font);
    if (style->draw_end) style->draw_end(out, style->userdata);
    return old_value != *value;
}


/* ===============================================================
 *
 *                          SLIDER
 *
 * ===============================================================*/
UI_INTERN float
ui_slider_behavior(ui_flags *state, struct ui_rect *logical_cursor,
    struct ui_rect *visual_cursor, struct ui_input *in,
    const struct ui_style_slider *style, struct ui_rect bounds,
    float slider_min, float slider_max, float slider_value,
    float slider_step, float slider_steps)
{
    int left_mouse_down;
    int left_mouse_click_in_cursor;

    /* check if visual cursor is being dragged */
    ui_widget_state_reset(state);
    left_mouse_down = in && in->mouse.buttons[UI_BUTTON_LEFT].down;
    left_mouse_click_in_cursor = in && ui_input_has_mouse_click_down_in_rect(in,
            UI_BUTTON_LEFT, *visual_cursor, ui_true);

    if (left_mouse_down && left_mouse_click_in_cursor)
    {
        float ratio = 0;
        const float d = in->mouse.pos.x - (visual_cursor->x+visual_cursor->w*0.5f);
        const float pxstep = bounds.w / slider_steps;

        /* only update value if the next slider step is reached */
        *state = UI_WIDGET_STATE_ACTIVE;
        if (UI_ABS(d) >= pxstep) {
            const float steps = (float)((int)(UI_ABS(d) / pxstep));
            slider_value += (d > 0) ? (slider_step*steps) : -(slider_step*steps);
            slider_value = UI_CLAMP(slider_min, slider_value, slider_max);
            ratio = (slider_value - slider_min)/slider_step;
            logical_cursor->x = bounds.x + (logical_cursor->w * ratio);
            in->mouse.buttons[UI_BUTTON_LEFT].clicked_pos.x = logical_cursor->x;
        }
    }

    /* slider widget state */
    if (ui_input_is_mouse_hovering_rect(in, bounds))
        *state = UI_WIDGET_STATE_HOVERED;
    if (*state & UI_WIDGET_STATE_HOVER &&
        !ui_input_is_mouse_prev_hovering_rect(in, bounds))
        *state |= UI_WIDGET_STATE_ENTERED;
    else if (ui_input_is_mouse_prev_hovering_rect(in, bounds))
        *state |= UI_WIDGET_STATE_LEFT;
    return slider_value;
}

UI_INTERN void
ui_draw_slider(struct ui_command_buffer *out, ui_flags state,
    const struct ui_style_slider *style, const struct ui_rect *bounds,
    const struct ui_rect *visual_cursor, float min, float value, float max)
{
    struct ui_rect fill;
    struct ui_rect bar;
    const struct ui_style_item *background;

    /* select correct slider images/colors */
    struct ui_color bar_color;
    const struct ui_style_item *cursor;

    UI_UNUSED(min);
    UI_UNUSED(max);
    UI_UNUSED(value);

    if (state & UI_WIDGET_STATE_ACTIVED) {
        background = &style->active;
        bar_color = style->bar_active;
        cursor = &style->cursor_active;
    } else if (state & UI_WIDGET_STATE_HOVER) {
        background = &style->hover;
        bar_color = style->bar_hover;
        cursor = &style->cursor_hover;
    } else {
        background = &style->normal;
        bar_color = style->bar_normal;
        cursor = &style->cursor_normal;
    }

    /* calculate slider background bar */
    bar.x = bounds->x;
    bar.y = (visual_cursor->y + visual_cursor->h/2) - bounds->h/12;
    bar.w = bounds->w;
    bar.h = bounds->h/6;

    /* filled background bar style */
    fill.w = (visual_cursor->x + (visual_cursor->w/2.0f)) - bar.x;
    fill.x = bar.x;
    fill.y = bar.y;
    fill.h = bar.h;

    /* draw background */
    if (background->type == UI_STYLE_ITEM_IMAGE) {
        ui_draw_image(out, *bounds, &background->data.image, ui_white);
    } else {
        ui_fill_rect(out, *bounds, style->rounding, background->data.color);
        ui_stroke_rect(out, *bounds, style->rounding, style->border, style->border_color);
    }

    /* draw slider bar */
    ui_fill_rect(out, bar, style->rounding, bar_color);
    ui_fill_rect(out, fill, style->rounding, style->bar_filled);

    /* draw cursor */
    if (cursor->type == UI_STYLE_ITEM_IMAGE)
        ui_draw_image(out, *visual_cursor, &cursor->data.image, ui_white);
    else ui_fill_circle(out, *visual_cursor, cursor->data.color);
}

UI_INTERN float
ui_do_slider(ui_flags *state,
    struct ui_command_buffer *out, struct ui_rect bounds,
    float min, float val, float max, float step,
    const struct ui_style_slider *style, struct ui_input *in,
    const struct ui_user_font *font)
{
    float slider_range;
    float slider_min;
    float slider_max;
    float slider_value;
    float slider_steps;
    float cursor_offset;

    struct ui_rect visual_cursor;
    struct ui_rect logical_cursor;

    UI_ASSERT(style);
    UI_ASSERT(out);
    if (!out || !style)
        return 0;

    /* remove padding from slider bounds */
    bounds.x = bounds.x + style->padding.x;
    bounds.y = bounds.y + style->padding.y;
    bounds.h = UI_MAX(bounds.h, 2*style->padding.y);
    bounds.w = UI_MAX(bounds.w, 2*style->padding.x + style->cursor_size.x);
    bounds.w -= 2 * style->padding.x;
    bounds.h -= 2 * style->padding.y;

    /* optional buttons */
    if (style->show_buttons) {
        ui_flags ws;
        struct ui_rect button;
        button.y = bounds.y;
        button.w = bounds.h;
        button.h = bounds.h;

        /* decrement button */
        button.x = bounds.x;
        if (ui_do_button_symbol(&ws, out, button, style->dec_symbol, UI_BUTTON_DEFAULT,
            &style->dec_button, in, font))
            val -= step;

        /* increment button */
        button.x = (bounds.x + bounds.w) - button.w;
        if (ui_do_button_symbol(&ws, out, button, style->inc_symbol, UI_BUTTON_DEFAULT,
            &style->inc_button, in, font))
            val += step;

        bounds.x = bounds.x + button.w + style->spacing.x;
        bounds.w = bounds.w - (2*button.w + 2*style->spacing.x);
    }

    /* remove one cursor size to support visual cursor */
    bounds.x += style->cursor_size.x*0.5f;
    bounds.w -= style->cursor_size.x;

    /* make sure the provided values are correct */
    slider_max = UI_MAX(min, max);
    slider_min = UI_MIN(min, max);
    slider_value = UI_CLAMP(slider_min, val, slider_max);
    slider_range = slider_max - slider_min;
    slider_steps = slider_range / step;
    cursor_offset = (slider_value - slider_min) / step;

    /* calculate cursor
    Basically you have two cursors. One for visual representation and interaction
    and one for updating the actual cursor value. */
    logical_cursor.h = bounds.h;
    logical_cursor.w = bounds.w / slider_steps;
    logical_cursor.x = bounds.x + (logical_cursor.w * cursor_offset);
    logical_cursor.y = bounds.y;

    visual_cursor.h = style->cursor_size.y;
    visual_cursor.w = style->cursor_size.x;
    visual_cursor.y = (bounds.y + bounds.h*0.5f) - visual_cursor.h*0.5f;
    visual_cursor.x = logical_cursor.x - visual_cursor.w*0.5f;

    slider_value = ui_slider_behavior(state, &logical_cursor, &visual_cursor,
        in, style, bounds, slider_min, slider_max, slider_value, step, slider_steps);
    visual_cursor.x = logical_cursor.x - visual_cursor.w*0.5f;

    /* draw slider */
    if (style->draw_begin) style->draw_begin(out, style->userdata);
    ui_draw_slider(out, *state, style, &bounds, &visual_cursor, slider_min, slider_value, slider_max);
    if (style->draw_end) style->draw_end(out, style->userdata);
    return slider_value;
}

/* ===============================================================
 *
 *                          PROGRESSBAR
 *
 * ===============================================================*/
UI_INTERN ui_size
ui_progress_behavior(ui_flags *state, const struct ui_input *in,
    struct ui_rect r, ui_size max, ui_size value, int modifiable)
{
    ui_widget_state_reset(state);
    if (in && modifiable && ui_input_is_mouse_hovering_rect(in, r)) {
        int left_mouse_down = in->mouse.buttons[UI_BUTTON_LEFT].down;
        int left_mouse_click_in_cursor = ui_input_has_mouse_click_down_in_rect(in,
            UI_BUTTON_LEFT, r, ui_true);

        if (left_mouse_down && left_mouse_click_in_cursor) {
            float ratio = UI_MAX(0, (float)(in->mouse.pos.x - r.x)) / (float)r.w;
            value = (ui_size)UI_MAX(0,((float)max * ratio));
            *state = UI_WIDGET_STATE_ACTIVE;
        } else *state = UI_WIDGET_STATE_HOVERED;
    }

    /* set progressbar widget state */
    if (*state & UI_WIDGET_STATE_HOVER && !ui_input_is_mouse_prev_hovering_rect(in, r))
        *state |= UI_WIDGET_STATE_ENTERED;
    else if (ui_input_is_mouse_prev_hovering_rect(in, r))
        *state |= UI_WIDGET_STATE_LEFT;

    if (!max) return value;
    value = UI_MIN(value, max);
    return value;
}

UI_INTERN void
ui_draw_progress(struct ui_command_buffer *out, ui_flags state,
    const struct ui_style_progress *style, const struct ui_rect *bounds,
    const struct ui_rect *scursor, ui_size value, ui_size max)
{
    const struct ui_style_item *background;
    const struct ui_style_item *cursor;

    UI_UNUSED(max);
    UI_UNUSED(value);

    /* select correct colors/images to draw */
    if (state & UI_WIDGET_STATE_ACTIVED) {
        background = &style->active;
        cursor = &style->cursor_active;
    } else if (state & UI_WIDGET_STATE_HOVER){
        background = &style->hover;
        cursor = &style->cursor_hover;
    } else {
        background = &style->normal;
        cursor = &style->cursor_normal;
    }

    /* draw background */
    if (background->type == UI_STYLE_ITEM_COLOR) {
        ui_fill_rect(out, *bounds, style->rounding, background->data.color);
        ui_stroke_rect(out, *bounds, style->rounding, style->border, style->border_color);
    } else ui_draw_image(out, *bounds, &background->data.image, ui_white);

    /* draw cursor */
    if (background->type == UI_STYLE_ITEM_COLOR) {
        ui_fill_rect(out, *scursor, style->rounding, cursor->data.color);
        ui_stroke_rect(out, *scursor, style->rounding, style->border, style->border_color);
    } else ui_draw_image(out, *scursor, &cursor->data.image, ui_white);
}

UI_INTERN ui_size
ui_do_progress(ui_flags *state,
    struct ui_command_buffer *out, struct ui_rect bounds,
    ui_size value, ui_size max, int modifiable,
    const struct ui_style_progress *style, const struct ui_input *in)
{
    float prog_scale;
    ui_size prog_value;
    struct ui_rect cursor;

    UI_ASSERT(style);
    UI_ASSERT(out);
    if (!out || !style) return 0;

    /* calculate progressbar cursor */
    cursor.w = UI_MAX(bounds.w, 2 * style->padding.x + 2 * style->border);
    cursor.h = UI_MAX(bounds.h, 2 * style->padding.y + 2 * style->border);
    cursor = ui_pad_rect(bounds, ui_vec2(style->padding.x + style->border, style->padding.y + style->border));
    prog_scale = (float)value / (float)max;
    cursor.w = (bounds.w - 2) * prog_scale;

    /* update progressbar */
    prog_value = UI_MIN(value, max);
    prog_value = ui_progress_behavior(state, in, bounds, max, prog_value, modifiable);

    /* draw progressbar */
    if (style->draw_begin) style->draw_begin(out, style->userdata);
    ui_draw_progress(out, *state, style, &bounds, &cursor, value, max);
    if (style->draw_end) style->draw_end(out, style->userdata);
    return prog_value;
}

/* ===============================================================
 *
 *                          SCROLLBAR
 *
 * ===============================================================*/
UI_INTERN float
ui_scrollbar_behavior(ui_flags *state, struct ui_input *in,
    int has_scrolling, const struct ui_rect *scroll,
    const struct ui_rect *cursor, const struct ui_rect *empty0,
    const struct ui_rect *empty1, float scroll_offset,
    float target, float scroll_step, enum ui_orientation o)
{
    ui_flags ws;
    int left_mouse_down;
    int left_mouse_click_in_cursor;

    ui_widget_state_reset(state);
    if (!in) return scroll_offset;

    left_mouse_down = in->mouse.buttons[UI_BUTTON_LEFT].down;
    left_mouse_click_in_cursor = ui_input_has_mouse_click_down_in_rect(in,
        UI_BUTTON_LEFT, *cursor, ui_true);
    if (ui_input_is_mouse_hovering_rect(in, *scroll))
        *state = UI_WIDGET_STATE_HOVERED;

    if (left_mouse_down && left_mouse_click_in_cursor) {
        /* update cursor by mouse dragging */
        float pixel, delta;
        *state = UI_WIDGET_STATE_ACTIVE;
        if (o == UI_VERTICAL) {
            float cursor_y;
            pixel = in->mouse.delta.y;
            delta = (pixel / scroll->h) * target;
            scroll_offset = UI_CLAMP(0, scroll_offset + delta, target - scroll->h);
            cursor_y = scroll->y + ((scroll_offset/target) * scroll->h);
            in->mouse.buttons[UI_BUTTON_LEFT].clicked_pos.y = cursor_y + cursor->h/2.0f;
        } else {
            float cursor_x;
            pixel = in->mouse.delta.x;
            delta = (pixel / scroll->w) * target;
            scroll_offset = UI_CLAMP(0, scroll_offset + delta, target - scroll->w);
            cursor_x = scroll->x + ((scroll_offset/target) * scroll->w);
            in->mouse.buttons[UI_BUTTON_LEFT].clicked_pos.x = cursor_x + cursor->w/2.0f;
        }
    } else if ((ui_input_is_key_pressed(in, UI_KEY_SCROLL_UP) && o == UI_VERTICAL && has_scrolling)||
            ui_button_behavior(&ws, *empty0, in, UI_BUTTON_DEFAULT)) {
        /* scroll page up by click on empty space or shortcut */
        if (o == UI_VERTICAL)
            scroll_offset = UI_MAX(0, scroll_offset - scroll->h);
        else scroll_offset = UI_MAX(0, scroll_offset - scroll->w);
    } else if ((ui_input_is_key_pressed(in, UI_KEY_SCROLL_DOWN) && o == UI_VERTICAL && has_scrolling) ||
        ui_button_behavior(&ws, *empty1, in, UI_BUTTON_DEFAULT)) {
        /* scroll page down by click on empty space or shortcut */
        if (o == UI_VERTICAL)
            scroll_offset = UI_MIN(scroll_offset + scroll->h, target - scroll->h);
        else scroll_offset = UI_MIN(scroll_offset + scroll->w, target - scroll->w);
    } else if (has_scrolling) {
        if ((in->mouse.scroll_delta<0 || (in->mouse.scroll_delta>0))) {
            /* update cursor by mouse scrolling */
            scroll_offset = scroll_offset + scroll_step * (-in->mouse.scroll_delta);
            if (o == UI_VERTICAL)
                scroll_offset = UI_CLAMP(0, scroll_offset, target - scroll->h);
            else scroll_offset = UI_CLAMP(0, scroll_offset, target - scroll->w);
        } else if (ui_input_is_key_pressed(in, UI_KEY_SCROLL_START)) {
            /* update cursor to the beginning  */
            if (o == UI_VERTICAL) scroll_offset = 0;
        } else if (ui_input_is_key_pressed(in, UI_KEY_SCROLL_END)) {
            /* update cursor to the end */
            if (o == UI_VERTICAL) scroll_offset = target - scroll->h;
        }
    }
    if (*state & UI_WIDGET_STATE_HOVER && !ui_input_is_mouse_prev_hovering_rect(in, *scroll))
        *state |= UI_WIDGET_STATE_ENTERED;
    else if (ui_input_is_mouse_prev_hovering_rect(in, *scroll))
        *state |= UI_WIDGET_STATE_LEFT;
    return scroll_offset;
}

UI_INTERN void
ui_draw_scrollbar(struct ui_command_buffer *out, ui_flags state,
    const struct ui_style_scrollbar *style, const struct ui_rect *bounds,
    const struct ui_rect *scroll)
{
    const struct ui_style_item *background;
    const struct ui_style_item *cursor;

    /* select correct colors/images to draw */
    if (state & UI_WIDGET_STATE_ACTIVED) {
        background = &style->active;
        cursor = &style->cursor_active;
    } else if (state & UI_WIDGET_STATE_HOVER) {
        background = &style->hover;
        cursor = &style->cursor_hover;
    } else {
        background = &style->normal;
        cursor = &style->cursor_normal;
    }

    /* draw background */
    if (background->type == UI_STYLE_ITEM_COLOR) {
        ui_fill_rect(out, *bounds, style->rounding, background->data.color);
        ui_stroke_rect(out, *bounds, style->rounding, style->border, style->border_color);
    } else {
        ui_draw_image(out, *bounds, &background->data.image, ui_white);
    }

    /* draw cursor */
    if (background->type == UI_STYLE_ITEM_COLOR) {
        ui_fill_rect(out, *scroll, style->rounding_cursor, cursor->data.color);
        ui_stroke_rect(out, *scroll, style->rounding_cursor, style->border_cursor, style->cursor_border_color);
    } else ui_draw_image(out, *scroll, &cursor->data.image, ui_white);
}

UI_INTERN float
ui_do_scrollbarv(ui_flags *state,
    struct ui_command_buffer *out, struct ui_rect scroll, int has_scrolling,
    float offset, float target, float step, float button_pixel_inc,
    const struct ui_style_scrollbar *style, struct ui_input *in,
    const struct ui_user_font *font)
{
    struct ui_rect empty_north;
    struct ui_rect empty_south;
    struct ui_rect cursor;

    float scroll_step;
    float scroll_offset;
    float scroll_off;
    float scroll_ratio;

    UI_ASSERT(out);
    UI_ASSERT(style);
    UI_ASSERT(state);
    if (!out || !style) return 0;

    scroll.w = UI_MAX(scroll.w, 1);
    scroll.h = UI_MAX(scroll.h, 2 * scroll.w);
    if (target <= scroll.h) return 0;

    /* optional scrollbar buttons */
    if (style->show_buttons) {
        ui_flags ws;
        float scroll_h;
        struct ui_rect button;

        button.x = scroll.x;
        button.w = scroll.w;
        button.h = scroll.w;

        scroll_h = scroll.h - 2 * button.h;
        scroll_step = UI_MIN(step, button_pixel_inc);

        /* decrement button */
        button.y = scroll.y;
        if (ui_do_button_symbol(&ws, out, button, style->dec_symbol,
            UI_BUTTON_REPEATER, &style->dec_button, in, font))
            offset = offset - scroll_step;

        /* increment button */
        button.y = scroll.y + scroll.h - button.h;
        if (ui_do_button_symbol(&ws, out, button, style->inc_symbol,
            UI_BUTTON_REPEATER, &style->inc_button, in, font))
            offset = offset + scroll_step;

        scroll.y = scroll.y + button.h;
        scroll.h = scroll_h;
    }

    /* calculate scrollbar constants */
    scroll_step = UI_MIN(step, scroll.h);
    scroll_offset = UI_CLAMP(0, offset, target - scroll.h);
    scroll_ratio = scroll.h / target;
    scroll_off = scroll_offset / target;

    /* calculate scrollbar cursor bounds */
    cursor.h = (scroll_ratio * scroll.h) - (2*style->border + 2*style->padding.y);
    cursor.y = scroll.y + (scroll_off * scroll.h) + style->border + style->padding.y;
    cursor.w = scroll.w - (2 * style->border + 2 * style->padding.x);
    cursor.x = scroll.x + style->border + style->padding.x;

    /* calculate empty space around cursor */
    empty_north.x = scroll.x;
    empty_north.y = scroll.y;
    empty_north.w = scroll.w;
    empty_north.h = cursor.y - scroll.y;

    empty_south.x = scroll.x;
    empty_south.y = cursor.y + cursor.h;
    empty_south.w = scroll.w;
    empty_south.h = (scroll.y + scroll.h) - (cursor.y + cursor.h);

    /* update scrollbar */
    scroll_offset = ui_scrollbar_behavior(state, in, has_scrolling, &scroll, &cursor,
        &empty_north, &empty_south, scroll_offset, target, scroll_step, UI_VERTICAL);
    scroll_off = scroll_offset / target;
    cursor.y = scroll.y + (scroll_off * scroll.h) + style->border_cursor + style->padding.y;

    /* draw scrollbar */
    if (style->draw_begin) style->draw_begin(out, style->userdata);
    ui_draw_scrollbar(out, *state, style, &scroll, &cursor);
    if (style->draw_end) style->draw_end(out, style->userdata);
    return scroll_offset;
}

UI_INTERN float
ui_do_scrollbarh(ui_flags *state,
    struct ui_command_buffer *out, struct ui_rect scroll, int has_scrolling,
    float offset, float target, float step, float button_pixel_inc,
    const struct ui_style_scrollbar *style, struct ui_input *in,
    const struct ui_user_font *font)
{
    struct ui_rect cursor;
    struct ui_rect empty_west;
    struct ui_rect empty_east;

    float scroll_step;
    float scroll_offset;
    float scroll_off;
    float scroll_ratio;

    UI_ASSERT(out);
    UI_ASSERT(style);
    if (!out || !style) return 0;

    /* scrollbar background */
    scroll.h = UI_MAX(scroll.h, 1);
    scroll.w = UI_MAX(scroll.w, 2 * scroll.h);
    if (target <= scroll.w) return 0;

    /* optional scrollbar buttons */
    if (style->show_buttons) {
        ui_flags ws;
        float scroll_w;
        struct ui_rect button;
        button.y = scroll.y;
        button.w = scroll.h;
        button.h = scroll.h;

        scroll_w = scroll.w - 2 * button.w;
        scroll_step = UI_MIN(step, button_pixel_inc);

        /* decrement button */
        button.x = scroll.x;
        if (ui_do_button_symbol(&ws, out, button, style->dec_symbol,
            UI_BUTTON_REPEATER, &style->dec_button, in, font))
            offset = offset - scroll_step;

        /* increment button */
        button.x = scroll.x + scroll.w - button.w;
        if (ui_do_button_symbol(&ws, out, button, style->inc_symbol,
            UI_BUTTON_REPEATER, &style->inc_button, in, font))
            offset = offset + scroll_step;

        scroll.x = scroll.x + button.w;
        scroll.w = scroll_w;
    }

    /* calculate scrollbar constants */
    scroll_step = UI_MIN(step, scroll.w);
    scroll_offset = UI_CLAMP(0, offset, target - scroll.w);
    scroll_ratio = scroll.w / target;
    scroll_off = scroll_offset / target;

    /* calculate cursor bounds */
    cursor.w = (scroll_ratio * scroll.w) - (2*style->border + 2*style->padding.x);
    cursor.x = scroll.x + (scroll_off * scroll.w) + style->border + style->padding.x;
    cursor.h = scroll.h - (2 * style->border + 2 * style->padding.y);
    cursor.y = scroll.y + style->border + style->padding.y;

    /* calculate empty space around cursor */
    empty_west.x = scroll.x;
    empty_west.y = scroll.y;
    empty_west.w = cursor.x - scroll.x;
    empty_west.h = scroll.h;

    empty_east.x = cursor.x + cursor.w;
    empty_east.y = scroll.y;
    empty_east.w = (scroll.x + scroll.w) - (cursor.x + cursor.w);
    empty_east.h = scroll.h;

    /* update scrollbar */
    scroll_offset = ui_scrollbar_behavior(state, in, has_scrolling, &scroll, &cursor,
        &empty_west, &empty_east, scroll_offset, target, scroll_step, UI_HORIZONTAL);
    scroll_off = scroll_offset / target;
    cursor.x = scroll.x + (scroll_off * scroll.w);

    /* draw scrollbar */
    if (style->draw_begin) style->draw_begin(out, style->userdata);
    ui_draw_scrollbar(out, *state, style, &scroll, &cursor);
    if (style->draw_end) style->draw_end(out, style->userdata);
    return scroll_offset;
}

/* ===============================================================
 *
 *                          FILTER
 *
 * ===============================================================*/
UI_API int ui_filter_default(const struct ui_text_edit *box, ui_rune unicode)
{(void)unicode;UI_UNUSED(box);return ui_true;}

UI_API int
ui_filter_ascii(const struct ui_text_edit *box, ui_rune unicode)
{
    UI_UNUSED(box);
    if (unicode > 128) return ui_false;
    else return ui_true;
}

UI_API int
ui_filter_float(const struct ui_text_edit *box, ui_rune unicode)
{
    UI_UNUSED(box);
    if ((unicode < '0' || unicode > '9') && unicode != '.' && unicode != '-')
        return ui_false;
    else return ui_true;
}

UI_API int
ui_filter_decimal(const struct ui_text_edit *box, ui_rune unicode)
{
    UI_UNUSED(box);
    if ((unicode < '0' || unicode > '9') && unicode != '-')
        return ui_false;
    else return ui_true;
}

UI_API int
ui_filter_hex(const struct ui_text_edit *box, ui_rune unicode)
{
    UI_UNUSED(box);
    if ((unicode < '0' || unicode > '9') &&
        (unicode < 'a' || unicode > 'f') &&
        (unicode < 'A' || unicode > 'F'))
        return ui_false;
    else return ui_true;
}

UI_API int
ui_filter_oct(const struct ui_text_edit *box, ui_rune unicode)
{
    UI_UNUSED(box);
    if (unicode < '0' || unicode > '7')
        return ui_false;
    else return ui_true;
}

UI_API int
ui_filter_binary(const struct ui_text_edit *box, ui_rune unicode)
{
    UI_UNUSED(box);
    if (unicode != '0' && unicode != '1')
        return ui_false;
    else return ui_true;
}

/* ===============================================================
 *
 *                          EDIT
 *
 * ===============================================================*/
UI_INTERN void
ui_edit_draw_text(struct ui_command_buffer *out,
    const struct ui_style_edit *style, float pos_x, float pos_y,
    float x_offset, const char *text, int byte_len, float row_height,
    const struct ui_user_font *font, struct ui_color background,
    struct ui_color foreground, int is_selected)
{
    UI_ASSERT(out);
    UI_ASSERT(font);
    UI_ASSERT(style);
    if (!text || !byte_len || !out || !style) return;

    {int glyph_len = 0;
    ui_rune unicode = 0;
    int text_len = 0;
    float line_width = 0;
    float glyph_width;
    const char *line = text;
    float line_offset = 0;
    int line_count = 0;

    struct ui_text txt;
    txt.padding = ui_vec2(0,0);
    txt.background = background;
    txt.text = foreground;

    glyph_len = ui_utf_decode(text+text_len, &unicode, byte_len-text_len);
    if (!glyph_len) return;
    while ((text_len < byte_len) && glyph_len)
    {
        if (unicode == '\n') {
            /* new line sepeator so draw previous line */
            struct ui_rect label;
            label.y = pos_y + line_offset;
            label.h = row_height;
            label.w = line_width;
            label.x = pos_x;
            if (!line_count)
                label.x += x_offset;

            if (is_selected) /* selection needs to draw different background color */
                ui_fill_rect(out, label, 0, background);
            ui_widget_text(out, label, line, (int)((text + text_len) - line),
                &txt, UI_TEXT_CENTERED, font);

            text_len++;
            line_count++;
            line_width = 0;
            line = text + text_len;
            line_offset += row_height;
            glyph_len = ui_utf_decode(text + text_len, &unicode, (int)(byte_len-text_len));
            continue;
        }
        if (unicode == '\r') {
            text_len++;
            glyph_len = ui_utf_decode(text + text_len, &unicode, byte_len-text_len);
            continue;
        }
        glyph_width = font->width(font->userdata, font->height, text+text_len, glyph_len);
        line_width += (float)glyph_width;
        text_len += glyph_len;
        glyph_len = ui_utf_decode(text + text_len, &unicode, byte_len-text_len);
        continue;
    }
    if (line_width > 0) {
        /* draw last line */
        struct ui_rect label;
        label.y = pos_y + line_offset;
        label.h = row_height;
        label.w = line_width;
        label.x = pos_x;
        if (!line_count)
            label.x += x_offset;

        if (is_selected)
            ui_fill_rect(out, label, 0, background);
        ui_widget_text(out, label, line, (int)((text + text_len) - line),
            &txt, UI_TEXT_LEFT, font);
    }}
}

UI_INTERN ui_flags
ui_do_edit(ui_flags *state, struct ui_command_buffer *out,
    struct ui_rect bounds, ui_flags flags, ui_plugin_filter filter,
    struct ui_text_edit *edit, const struct ui_style_edit *style,
    struct ui_input *in, const struct ui_user_font *font)
{
    struct ui_rect area;
    ui_flags ret = 0;
    float row_height;
    char prev_state = 0;
    char is_hovered = 0;
    char select_all = 0;
    char cursor_follow = 0;
    struct ui_rect old_clip;
    struct ui_rect clip;

    UI_ASSERT(state);
    UI_ASSERT(out);
    UI_ASSERT(style);
    if (!state || !out || !style)
        return ret;

    /* visible text area calculation */
    area.x = bounds.x + style->padding.x + style->border;
    area.y = bounds.y + style->padding.y + style->border;
    area.w = bounds.w - (2.0f * style->padding.x + 2 * style->border);
    area.h = bounds.h - (2.0f * style->padding.y + 2 * style->border);
    if (flags & UI_EDIT_MULTILINE)
        area.w = UI_MAX(0, area.w - style->scrollbar_size.x);
    row_height = (flags & UI_EDIT_MULTILINE)? font->height + style->row_padding: area.h;

    /* calculate clipping rectangle */
    old_clip = out->clip;
    ui_unify(&clip, &old_clip, area.x, area.y, area.x + area.w, area.y + area.h);

    /* update edit state */
    prev_state = (char)edit->active;
    is_hovered = (char)ui_input_is_mouse_hovering_rect(in, bounds);
    if (in && in->mouse.buttons[UI_BUTTON_LEFT].clicked && in->mouse.buttons[UI_BUTTON_LEFT].down) {
        edit->active = UI_INBOX(in->mouse.pos.x, in->mouse.pos.y,
                                bounds.x, bounds.y, bounds.w, bounds.h);
    }

    /* (de)activate text editor */
    if (!prev_state && edit->active) {
        const enum ui_text_edit_type type = (flags & UI_EDIT_MULTILINE) ?
            UI_TEXT_EDIT_MULTI_LINE: UI_TEXT_EDIT_SINGLE_LINE;
        ui_textedit_clear_state(edit, type, filter);
        if (flags & UI_EDIT_ALWAYS_INSERT_MODE)
            edit->mode = UI_TEXT_EDIT_MODE_INSERT;
        if (flags & UI_EDIT_AUTO_SELECT)
            select_all = ui_true;
        if (flags & UI_EDIT_GOTO_END_ON_ACTIVATE) {
            edit->cursor = edit->string.len;
            in = 0;
        }
    } else if (!edit->active) edit->mode = UI_TEXT_EDIT_MODE_VIEW;
    if (flags & UI_EDIT_READ_ONLY)
        edit->mode = UI_TEXT_EDIT_MODE_VIEW;

    ret = (edit->active) ? UI_EDIT_ACTIVE: UI_EDIT_INACTIVE;
    if (prev_state != edit->active)
        ret |= (edit->active) ? UI_EDIT_ACTIVATED: UI_EDIT_DEACTIVATED;

    /* handle user input */
    if (edit->active && in)
    {
        int shift_mod = in->keyboard.keys[UI_KEY_SHIFT].down;
        const float mouse_x = (in->mouse.pos.x - area.x) + edit->scrollbar.x;
        const float mouse_y = (in->mouse.pos.y - area.y) + edit->scrollbar.y;

        /* mouse click handler */
        is_hovered = (char)ui_input_is_mouse_hovering_rect(in, area);
        if (select_all) {
            ui_textedit_select_all(edit);
        } else if (is_hovered && in->mouse.buttons[UI_BUTTON_LEFT].down &&
            in->mouse.buttons[UI_BUTTON_LEFT].clicked) {
            ui_textedit_click(edit, mouse_x, mouse_y, font, row_height);
        } else if (is_hovered && in->mouse.buttons[UI_BUTTON_LEFT].down &&
            (in->mouse.delta.x != 0.0f || in->mouse.delta.y != 0.0f)) {
            ui_textedit_drag(edit, mouse_x, mouse_y, font, row_height);
            cursor_follow = ui_true;
        } else if (is_hovered && in->mouse.buttons[UI_BUTTON_RIGHT].clicked &&
            in->mouse.buttons[UI_BUTTON_RIGHT].down) {
            ui_textedit_key(edit, UI_KEY_TEXT_WORD_LEFT, ui_false, font, row_height);
            ui_textedit_key(edit, UI_KEY_TEXT_WORD_RIGHT, ui_true, font, row_height);
            cursor_follow = ui_true;
        }

        {int i; /* keyboard input */
        int old_mode = edit->mode;
        for (i = 0; i < UI_KEY_MAX; ++i) {
            if (i == UI_KEY_ENTER || i == UI_KEY_TAB) continue; /* special case */
            if (ui_input_is_key_pressed(in, (enum ui_keys)i)) {
                ui_textedit_key(edit, (enum ui_keys)i, shift_mod, font, row_height);
                cursor_follow = ui_true;
            }
        }
        if (old_mode != edit->mode) {
            in->keyboard.text_len = 0;
        }}

        /* text input */
        edit->filter = filter;
        if (in->keyboard.text_len) {
            ui_textedit_text(edit, in->keyboard.text, in->keyboard.text_len);
            cursor_follow = ui_true;
            in->keyboard.text_len = 0;
        }

        /* enter key handler */
        if (ui_input_is_key_pressed(in, UI_KEY_ENTER)) {
            cursor_follow = ui_true;
            if (flags & UI_EDIT_CTRL_ENTER_NEWLINE && shift_mod)
                ui_textedit_text(edit, "\n", 1);
            else if (flags & UI_EDIT_SIG_ENTER)
                ret |= UI_EDIT_COMMITED;
            else ui_textedit_text(edit, "\n", 1);
        }

        /* cut & copy handler */
        {int copy= ui_input_is_key_pressed(in, UI_KEY_COPY);
        int cut = ui_input_is_key_pressed(in, UI_KEY_CUT);
        if ((copy || cut) && (flags & UI_EDIT_CLIPBOARD))
        {
            int glyph_len;
            ui_rune unicode;
            const char *text;
            int b = edit->select_start;
            int e = edit->select_end;

            int begin = UI_MIN(b, e);
            int end = UI_MAX(b, e);
            text = ui_str_at_const(&edit->string, begin, &unicode, &glyph_len);
            if (edit->clip.copy)
                edit->clip.copy(edit->clip.userdata, text, end - begin);
            if (cut && !(flags & UI_EDIT_READ_ONLY)){
                ui_textedit_cut(edit);
                cursor_follow = ui_true;
            }
        }}

        /* paste handler */
        {int paste = ui_input_is_key_pressed(in, UI_KEY_PASTE);
        if (paste && (flags & UI_EDIT_CLIPBOARD) && edit->clip.paste) {
            edit->clip.paste(edit->clip.userdata, edit);
            cursor_follow = ui_true;
        }}

        /* tab handler */
        {int tab = ui_input_is_key_pressed(in, UI_KEY_TAB);
        if (tab && (flags & UI_EDIT_ALLOW_TAB)) {
            ui_textedit_text(edit, "    ", 4);
            cursor_follow = ui_true;
        }}
    }

    /* set widget state */
    if (edit->active)
        *state = UI_WIDGET_STATE_ACTIVE;
    else ui_widget_state_reset(state);

    if (is_hovered)
        *state |= UI_WIDGET_STATE_HOVERED;

    /* DRAW EDIT */
    {const char *text = ui_str_get_const(&edit->string);
    int len = ui_str_len_char(&edit->string);

    {/* select background colors/images  */
    const struct ui_style_item *background;
    if (*state & UI_WIDGET_STATE_ACTIVED)
        background = &style->active;
    else if (*state & UI_WIDGET_STATE_HOVER)
        background = &style->hover;
    else background = &style->normal;

    /* draw background frame */
    if (background->type == UI_STYLE_ITEM_COLOR) {
        ui_stroke_rect(out, bounds, style->rounding, style->border, style->border_color);
        ui_fill_rect(out, bounds, style->rounding, background->data.color);
    } else ui_draw_image(out, bounds, &background->data.image, ui_white);}

    area.w = UI_MAX(0, area.w - style->cursor_size);
    if (edit->active)
    {
        int total_lines = 1;
        struct ui_vec2 text_size = ui_vec2(0,0);

        /* text pointer positions */
        const char *cursor_ptr = 0;
        const char *select_begin_ptr = 0;
        const char *select_end_ptr = 0;

        /* 2D pixel positions */
        struct ui_vec2 cursor_pos = ui_vec2(0,0);
        struct ui_vec2 selection_offset_start = ui_vec2(0,0);
        struct ui_vec2 selection_offset_end = ui_vec2(0,0);

        int selection_begin = UI_MIN(edit->select_start, edit->select_end);
        int selection_end = UI_MAX(edit->select_start, edit->select_end);

        /* calculate total line count + total space + cursor/selection position */
        float line_width = 0.0f;
        if (text && len)
        {
            /* utf8 encoding */
            float glyph_width;
            int glyph_len = 0;
            ui_rune unicode = 0;
            int text_len = 0;
            int glyphs = 0;
            int row_begin = 0;

            glyph_len = ui_utf_decode(text, &unicode, len);
            glyph_width = font->width(font->userdata, font->height, text, glyph_len);
            line_width = 0;

            /* iterate all lines */
            while ((text_len < len) && glyph_len)
            {
                /* set cursor 2D position and line */
                if (!cursor_ptr && glyphs == edit->cursor)
                {
                    int glyph_offset;
                    struct ui_vec2 out_offset;
                    struct ui_vec2 row_size;
                    const char *remaining;

                    /* calculate 2d position */
                    cursor_pos.y = (float)(total_lines-1) * row_height;
                    row_size = ui_text_calculate_text_bounds(font, text+row_begin,
                                text_len-row_begin, row_height, &remaining,
                                &out_offset, &glyph_offset, UI_STOP_ON_NEW_LINE);
                    cursor_pos.x = row_size.x;
                    cursor_ptr = text + text_len;
                }

                /* set start selection 2D position and line */
                if (!select_begin_ptr && edit->select_start != edit->select_end &&
                    glyphs == selection_begin)
                {
                    int glyph_offset;
                    struct ui_vec2 out_offset;
                    struct ui_vec2 row_size;
                    const char *remaining;

                    /* calculate 2d position */
                    selection_offset_start.y = (float)(UI_MAX(total_lines-1,0)) * row_height;
                    row_size = ui_text_calculate_text_bounds(font, text+row_begin,
                                text_len-row_begin, row_height, &remaining,
                                &out_offset, &glyph_offset, UI_STOP_ON_NEW_LINE);
                    selection_offset_start.x = row_size.x;
                    select_begin_ptr = text + text_len;
                }

                /* set end selection 2D position and line */
                if (!select_end_ptr && edit->select_start != edit->select_end &&
                    glyphs == selection_end)
                {
                    int glyph_offset;
                    struct ui_vec2 out_offset;
                    struct ui_vec2 row_size;
                    const char *remaining;

                    /* calculate 2d position */
                    selection_offset_end.y = (float)(total_lines-1) * row_height;
                    row_size = ui_text_calculate_text_bounds(font, text+row_begin,
                                text_len-row_begin, row_height, &remaining,
                                &out_offset, &glyph_offset, UI_STOP_ON_NEW_LINE);
                    selection_offset_end.x = row_size.x;
                    select_end_ptr = text + text_len;
                }
                if (unicode == '\n') {
                    text_size.x = UI_MAX(text_size.x, line_width);
                    total_lines++;
                    line_width = 0;
                    text_len++;
                    glyphs++;
                    row_begin = text_len;
                    glyph_len = ui_utf_decode(text + text_len, &unicode, len-text_len);
                    glyph_width = font->width(font->userdata, font->height, text+text_len, glyph_len);
                    continue;
                }

                glyphs++;
                text_len += glyph_len;
                line_width += (float)glyph_width;

                glyph_len = ui_utf_decode(text + text_len, &unicode, len-text_len);
                glyph_width = font->width(font->userdata, font->height,
                    text+text_len, glyph_len);
                continue;
            }
            text_size.y = (float)total_lines * row_height;

            /* handle case when cursor is at end of text buffer */
            if (!cursor_ptr && edit->cursor == edit->string.len) {
                cursor_pos.x = line_width;
                cursor_pos.y = text_size.y - row_height;
            }
        }
        {
            /* scrollbar */
            if (cursor_follow)
            {
                /* update scrollbar to follow cursor */
                if (!(flags & UI_EDIT_NO_HORIZONTAL_SCROLL)) {
                    /* horizontal scroll */
                    const float scroll_increment = area.w * 0.25f;
                    if (cursor_pos.x < edit->scrollbar.x)
                        edit->scrollbar.x = (float)(int)UI_MAX(0.0f, cursor_pos.x - scroll_increment);
                    if (cursor_pos.x >= edit->scrollbar.x + area.w)
                        edit->scrollbar.x = (float)(int)UI_MAX(0.0f, cursor_pos.x);
                } else edit->scrollbar.x = 0;

                if (flags & UI_EDIT_MULTILINE) {
                    /* vertical scroll */
                    if (cursor_pos.y < edit->scrollbar.y)
                        edit->scrollbar.y = UI_MAX(0.0f, cursor_pos.y - row_height);
                    if (cursor_pos.y >= edit->scrollbar.y + area.h)
                        edit->scrollbar.y = edit->scrollbar.y + row_height;
                } else edit->scrollbar.y = 0;
            }

            /* scrollbar widget */
            if (flags & UI_EDIT_MULTILINE)
            {
                ui_flags ws;
                struct ui_rect scroll;
                float scroll_target;
                float scroll_offset;
                float scroll_step;
                float scroll_inc;

                scroll = area;
                scroll.x = (bounds.x + bounds.w - style->border) - style->scrollbar_size.x;
                scroll.w = style->scrollbar_size.x;

                scroll_offset = edit->scrollbar.y;
                scroll_step = scroll.h * 0.10f;
                scroll_inc = scroll.h * 0.01f;
                scroll_target = text_size.y;
                edit->scrollbar.y = ui_do_scrollbarv(&ws, out, scroll, 0,
                        scroll_offset, scroll_target, scroll_step, scroll_inc,
                        &style->scrollbar, in, font);
            }
        }

        /* draw text */
        {struct ui_color background_color;
        struct ui_color text_color;
        struct ui_color sel_background_color;
        struct ui_color sel_text_color;
        struct ui_color cursor_color;
        struct ui_color cursor_text_color;
        const struct ui_style_item *background;
        ui_push_scissor(out, clip);

        /* select correct colors to draw */
        if (*state & UI_WIDGET_STATE_ACTIVED) {
            background = &style->active;
            text_color = style->text_active;
            sel_text_color = style->selected_text_hover;
            sel_background_color = style->selected_hover;
            cursor_color = style->cursor_hover;
            cursor_text_color = style->cursor_text_hover;
        } else if (*state & UI_WIDGET_STATE_HOVER) {
            background = &style->hover;
            text_color = style->text_hover;
            sel_text_color = style->selected_text_hover;
            sel_background_color = style->selected_hover;
            cursor_text_color = style->cursor_text_hover;
            cursor_color = style->cursor_hover;
        } else {
            background = &style->normal;
            text_color = style->text_normal;
            sel_text_color = style->selected_text_normal;
            sel_background_color = style->selected_normal;
            cursor_color = style->cursor_normal;
            cursor_text_color = style->cursor_text_normal;
        }
        if (background->type == UI_STYLE_ITEM_IMAGE)
            background_color = ui_rgba(0,0,0,0);
        else background_color = background->data.color;


        if (edit->select_start == edit->select_end) {
            /* no selection so just draw the complete text */
            const char *begin = ui_str_get_const(&edit->string);
            int l = ui_str_len_char(&edit->string);
            ui_edit_draw_text(out, style, area.x - edit->scrollbar.x,
                area.y - edit->scrollbar.y, 0, begin, l, row_height, font, 
                background_color, text_color, ui_false);
        } else {
            /* edit has selection so draw 1-3 text chuuis */
            if (edit->select_start != edit->select_end && selection_begin > 0){
                /* draw unselected text before selection */
                const char *begin = ui_str_get_const(&edit->string);
                UI_ASSERT(select_begin_ptr);
                ui_edit_draw_text(out, style, area.x - edit->scrollbar.x,
                    area.y - edit->scrollbar.y, 0, begin, (int)(select_begin_ptr - begin),
                    row_height, font, background_color, text_color, ui_false);
            }
            if (edit->select_start != edit->select_end) {
                /* draw selected text */
                UI_ASSERT(select_begin_ptr);
                if (!select_end_ptr) {
                    const char *begin = ui_str_get_const(&edit->string);
                    select_end_ptr = begin + ui_str_len_char(&edit->string);
                }
                ui_edit_draw_text(out, style,
                    area.x - edit->scrollbar.x,
                    area.y + selection_offset_start.y - edit->scrollbar.y,
                    selection_offset_start.x,
                    select_begin_ptr, (int)(select_end_ptr - select_begin_ptr),
                    row_height, font, sel_background_color, sel_text_color, ui_true);
            }
            if ((edit->select_start != edit->select_end &&
                selection_end < edit->string.len))
            {
                /* draw unselected text after selected text */
                const char *begin = select_end_ptr;
                const char *end = ui_str_get_const(&edit->string) +
                                    ui_str_len_char(&edit->string);
                UI_ASSERT(select_end_ptr);
                ui_edit_draw_text(out, style,
                    area.x - edit->scrollbar.x,
                    area.y + selection_offset_end.y - edit->scrollbar.y,
                    selection_offset_end.x,
                    begin, (int)(end - begin), row_height, font,
                    background_color, text_color, ui_true);
            }
        }

        /* cursor */
        if (edit->select_start == edit->select_end)
        {
            if (edit->cursor >= ui_str_len(&edit->string) ||
                (cursor_ptr && *cursor_ptr == '\n')) {
                /* draw cursor at end of line */
                struct ui_rect cursor;
                cursor.w = style->cursor_size;
                cursor.h = font->height;
                cursor.x = area.x + cursor_pos.x - edit->scrollbar.x;
                cursor.y = area.y + cursor_pos.y + row_height/2.0f - cursor.h/2.0f;
                cursor.y -= edit->scrollbar.y;
                ui_fill_rect(out, cursor, 0, cursor_color);
            } else {
                /* draw cursor inside text */
                int glyph_len;
                struct ui_rect label;
                struct ui_text txt;

                ui_rune unicode;
                UI_ASSERT(cursor_ptr);
                glyph_len = ui_utf_decode(cursor_ptr, &unicode, 4);

                label.x = area.x + cursor_pos.x - edit->scrollbar.x;
                label.y = area.y + cursor_pos.y - edit->scrollbar.y;
                label.w = font->width(font->userdata, font->height, cursor_ptr, glyph_len);
                label.h = row_height;

                txt.padding = ui_vec2(0,0);
                txt.background = cursor_color;;
                txt.text = cursor_text_color;
                ui_fill_rect(out, label, 0, cursor_color);
                ui_widget_text(out, label, cursor_ptr, glyph_len, &txt, UI_TEXT_LEFT, font);
            }
        }}
    } else {
        /* not active so just draw text */
        int l = ui_str_len_char(&edit->string);
        const char *begin = ui_str_get_const(&edit->string);

        const struct ui_style_item *background;
        struct ui_color background_color;
        struct ui_color text_color;
        ui_push_scissor(out, clip);
        if (*state & UI_WIDGET_STATE_ACTIVED) {
            background = &style->active;
            text_color = style->text_active;
        } else if (*state & UI_WIDGET_STATE_HOVER) {
            background = &style->hover;
            text_color = style->text_hover;
        } else {
            background = &style->normal;
            text_color = style->text_normal;
        }
        if (background->type == UI_STYLE_ITEM_IMAGE)
            background_color = ui_rgba(0,0,0,0);
        else background_color = background->data.color;
        ui_edit_draw_text(out, style, area.x - edit->scrollbar.x,
            area.y - edit->scrollbar.y, 0, begin, l, row_height, font,
            background_color, text_color, ui_false);
    }
    ui_push_scissor(out, old_clip);}
    return ret;
}

/* ===============================================================
 *
 *                          PROPERTY
 *
 * ===============================================================*/
enum ui_property_status {
    UI_PROPERTY_DEFAULT,
    UI_PROPERTY_EDIT,
    UI_PROPERTY_DRAG
};
enum ui_property_filter {
    UI_FILTER_INT,
    UI_FILTER_FLOAT
};
enum ui_property_kind {
    UI_PROPERTY_INT,
    UI_PROPERTY_FLOAT,
    UI_PROPERTY_DOUBLE
};
union ui_property {
    int i;
    float f;
    double d;
};
struct ui_property_variant {
    enum ui_property_kind kind;
    union ui_property value;
    union ui_property min_value;
    union ui_property max_value;
    union ui_property step;
};

UI_INTERN void
ui_drag_behavior(ui_flags *state, const struct ui_input *in,
    struct ui_rect drag, struct ui_property_variant *variant,
    float inc_per_pixel)
{
    int left_mouse_down = in && in->mouse.buttons[UI_BUTTON_LEFT].down;
    int left_mouse_click_in_cursor = in &&
        ui_input_has_mouse_click_down_in_rect(in, UI_BUTTON_LEFT, drag, ui_true);

    ui_widget_state_reset(state);
    if (ui_input_is_mouse_hovering_rect(in, drag))
        *state = UI_WIDGET_STATE_HOVERED;

    if (left_mouse_down && left_mouse_click_in_cursor) {
        float delta, pixels;
        pixels = in->mouse.delta.x;
        delta = pixels * inc_per_pixel;
        switch (variant->kind) {
        default: break;
        case UI_PROPERTY_INT:
            variant->value.i = variant->value.i + (int)delta;
            variant->value.i = UI_CLAMP(variant->min_value.i, variant->value.i, variant->max_value.i);
            break;
        case UI_PROPERTY_FLOAT:
            variant->value.f = variant->value.f + (float)delta;
            variant->value.f = UI_CLAMP(variant->min_value.f, variant->value.f, variant->max_value.f);
            break;
        case UI_PROPERTY_DOUBLE:
            variant->value.d = variant->value.d + (double)delta;
            variant->value.d = UI_CLAMP(variant->min_value.d, variant->value.d, variant->max_value.d);
            break;
        }
        *state = UI_WIDGET_STATE_ACTIVE;
    }
    if (*state & UI_WIDGET_STATE_HOVER && !ui_input_is_mouse_prev_hovering_rect(in, drag))
        *state |= UI_WIDGET_STATE_ENTERED;
    else if (ui_input_is_mouse_prev_hovering_rect(in, drag))
        *state |= UI_WIDGET_STATE_LEFT;
}

UI_INTERN void
ui_property_behavior(ui_flags *ws, const struct ui_input *in,
    struct ui_rect property,  struct ui_rect label, struct ui_rect edit,
    struct ui_rect empty, int *state, struct ui_property_variant *variant,
    float inc_per_pixel)
{
    if (in && *state == UI_PROPERTY_DEFAULT) {
        if (ui_button_behavior(ws, edit, in, UI_BUTTON_DEFAULT))
            *state = UI_PROPERTY_EDIT;
        else if (ui_input_is_mouse_click_down_in_rect(in, UI_BUTTON_LEFT, label, ui_true))
            *state = UI_PROPERTY_DRAG;
        else if (ui_input_is_mouse_click_down_in_rect(in, UI_BUTTON_LEFT, empty, ui_true))
            *state = UI_PROPERTY_DRAG;
    }
    if (*state == UI_PROPERTY_DRAG) {
        ui_drag_behavior(ws, in, property, variant, inc_per_pixel);
        if (!(*ws & UI_WIDGET_STATE_ACTIVED)) *state = UI_PROPERTY_DEFAULT;
    }
}

UI_INTERN void
ui_draw_property(struct ui_command_buffer *out, const struct ui_style_property *style,
    const struct ui_rect *bounds, const struct ui_rect *label, ui_flags state,
    const char *name, int len, const struct ui_user_font *font)
{
    struct ui_text text;
    const struct ui_style_item *background;

    /* select correct background and text color */
    if (state & UI_WIDGET_STATE_ACTIVED) {
        background = &style->active;
        text.text = style->label_active;
    } else if (state & UI_WIDGET_STATE_HOVER) {
        background = &style->hover;
        text.text = style->label_hover;
    } else {
        background = &style->normal;
        text.text = style->label_normal;
    }

    /* draw background */
    if (background->type == UI_STYLE_ITEM_IMAGE) {
        ui_draw_image(out, *bounds, &background->data.image, ui_white);
        text.background = ui_rgba(0,0,0,0);
    } else {
        text.background = background->data.color;
        ui_fill_rect(out, *bounds, style->rounding, background->data.color);
        ui_stroke_rect(out, *bounds, style->rounding, style->border, background->data.color);
    }

    /* draw label */
    text.padding = ui_vec2(0,0);
    ui_widget_text(out, *label, name, len, &text, UI_TEXT_CENTERED, font);
}

UI_INTERN void
ui_do_property(ui_flags *ws,
    struct ui_command_buffer *out, struct ui_rect property,
    const char *name, struct ui_property_variant *variant,
    float inc_per_pixel, char *buffer, int *len,
    int *state, int *cursor, const struct ui_style_property *style,
    enum ui_property_filter filter, struct ui_input *in,
    const struct ui_user_font *font, struct ui_text_edit *text_edit)
{
    const ui_plugin_filter filters[] = {
        ui_filter_decimal,
        ui_filter_float
    };
    int active, old;
    int num_len, name_len;
    char string[UI_MAX_NUMBER_BUFFER];
    float size;

    char *dst = 0;
    int *length;

    struct ui_rect left;
    struct ui_rect right;
    struct ui_rect label;
    struct ui_rect edit;
    struct ui_rect empty;

    /* left decrement button */
    left.h = font->height/2;
    left.w = left.h;
    left.x = property.x + style->border + style->padding.x;
    left.y = property.y + style->border + property.h/2.0f - left.h/2;

    /* text label */
    name_len = ui_strlen(name);
    size = font->width(font->userdata, font->height, name, name_len);
    label.x = left.x + left.w + style->padding.x;
    label.w = (float)size + 2 * style->padding.x;
    label.y = property.y + style->border + style->padding.y;
    label.h = property.h - (2 * style->border + 2 * style->padding.y);

    /* right increment button */
    right.y = left.y;
    right.w = left.w;
    right.h = left.h;
    right.x = property.x + property.w - (right.w + style->padding.x);

    /* edit */
    if (*state == UI_PROPERTY_EDIT) {
        size = font->width(font->userdata, font->height, buffer, *len);
        size += style->edit.cursor_size;
        length = len;
        dst = buffer;
    } else {
        switch (variant->kind) {
        default: break;
        case UI_PROPERTY_INT:
            ui_itoa(string, variant->value.i);
            num_len = ui_strlen(string);
            break;
        case UI_PROPERTY_FLOAT:
            ui_dtoa(string, (double)variant->value.f);
            num_len = ui_string_float_limit(string, UI_MAX_FLOAT_PRECISION);
            break;
        case UI_PROPERTY_DOUBLE:
            ui_dtoa(string, variant->value.d);
            num_len = ui_string_float_limit(string, UI_MAX_FLOAT_PRECISION);
            break;
        }
        size = font->width(font->userdata, font->height, string, num_len);
        dst = string;
        length = &num_len;
    }

    edit.w =  (float)size + 2 * style->padding.x;
    edit.w = UI_MIN(edit.w, right.x - (label.x + label.w));
    edit.x = right.x - (edit.w + style->padding.x);
    edit.y = property.y + style->border;
    edit.h = property.h - (2 * style->border);

    /* empty left space activator */
    empty.w = edit.x - (label.x + label.w);
    empty.x = label.x + label.w;
    empty.y = property.y;
    empty.h = property.h;

    /* update property */
    old = (*state == UI_PROPERTY_EDIT);
    ui_property_behavior(ws, in, property, label, edit, empty, state, variant, inc_per_pixel);

    /* draw property */
    if (style->draw_begin) style->draw_begin(out, style->userdata);
    ui_draw_property(out, style, &property, &label, *ws, name, name_len, font);
    if (style->draw_end) style->draw_end(out, style->userdata);

    /* execute right button  */
    if (ui_do_button_symbol(ws, out, left, style->sym_left, UI_BUTTON_DEFAULT, &style->dec_button, in, font)) {
        switch (variant->kind) {
        default: break;
        case UI_PROPERTY_INT:
            variant->value.i = UI_CLAMP(variant->min_value.i, variant->value.i - variant->step.i, variant->max_value.i); break;
        case UI_PROPERTY_FLOAT:
            variant->value.f = UI_CLAMP(variant->min_value.f, variant->value.f - variant->step.f, variant->max_value.f); break;
        case UI_PROPERTY_DOUBLE:
            variant->value.d = UI_CLAMP(variant->min_value.d, variant->value.d - variant->step.d, variant->max_value.d); break;
        }
    }

    /* execute left button  */
    if (ui_do_button_symbol(ws, out, right, style->sym_right, UI_BUTTON_DEFAULT, &style->inc_button, in, font)) {
        switch (variant->kind) {
        default: break;
        case UI_PROPERTY_INT:
            variant->value.i = UI_CLAMP(variant->min_value.i, variant->value.i + variant->step.i, variant->max_value.i); break;
        case UI_PROPERTY_FLOAT:
            variant->value.f = UI_CLAMP(variant->min_value.f, variant->value.f + variant->step.f, variant->max_value.f); break;
        case UI_PROPERTY_DOUBLE:
            variant->value.d = UI_CLAMP(variant->min_value.d, variant->value.d + variant->step.d, variant->max_value.d); break;
        }
    }

    active = (*state == UI_PROPERTY_EDIT);
    if (old != UI_PROPERTY_EDIT && active) {
        /* property has been activated so setup buffer */
        UI_MEMCPY(buffer, dst, (ui_size)*length);
        *cursor = ui_utf_len(buffer, *length);
        *len = *length;
        length = len;
        dst = buffer;
    }

    /* execute and run text edit field */
    ui_textedit_clear_state(text_edit, UI_TEXT_EDIT_SINGLE_LINE, filters[filter]);
    text_edit->active = (unsigned char)active;
    text_edit->string.len = *length;
    text_edit->cursor = UI_CLAMP(0, *cursor, *length);
    text_edit->string.buffer.allocated = (ui_size)*length;
    text_edit->string.buffer.memory.size = UI_MAX_NUMBER_BUFFER;
    text_edit->string.buffer.memory.ptr = dst;
    text_edit->string.buffer.size = UI_MAX_NUMBER_BUFFER;
    text_edit->mode = UI_TEXT_EDIT_MODE_INSERT;
    ui_do_edit(ws, out, edit, UI_EDIT_ALWAYS_INSERT_MODE, filters[filter],
        text_edit, &style->edit, (*state == UI_PROPERTY_EDIT) ? in: 0, font);

    *length = text_edit->string.len;
    active = text_edit->active;
    *cursor = text_edit->cursor;

    if (active && ui_input_is_key_pressed(in, UI_KEY_ENTER))
        active = !active;

    if (old && !active) {
        /* property is now not active so convert edit text to value*/
        *state = UI_PROPERTY_DEFAULT;
        buffer[*len] = '\0';
        switch (variant->kind) {
        default: break;
        case UI_PROPERTY_INT:
            variant->value.i = ui_strtoi(buffer, 0);
            variant->value.i = UI_CLAMP(variant->min_value.i, variant->value.i, variant->max_value.i);
            break;
        case UI_PROPERTY_FLOAT:
            ui_string_float_limit(buffer, UI_MAX_FLOAT_PRECISION);
            variant->value.f = ui_strtof(buffer, 0);
            variant->value.f = UI_CLAMP(variant->min_value.f, variant->value.f, variant->max_value.f);
            break;
        case UI_PROPERTY_DOUBLE:
            ui_string_float_limit(buffer, UI_MAX_FLOAT_PRECISION);
            variant->value.d = ui_strtod(buffer, 0);
            variant->value.d = UI_CLAMP(variant->min_value.d, variant->value.d, variant->max_value.d);
            break;
        }
    }
}
/* ===============================================================
 *
 *                          COLOR PICKER
 *
 * ===============================================================*/
UI_INTERN int
ui_color_picker_behavior(ui_flags *state,
    const struct ui_rect *bounds, const struct ui_rect *matrix,
    const struct ui_rect *hue_bar, const struct ui_rect *alpha_bar,
    struct ui_color *color, const struct ui_input *in)
{
    float hsva[4];
    int value_changed = 0;
    int hsv_changed = 0;

    UI_ASSERT(state);
    UI_ASSERT(matrix);
    UI_ASSERT(hue_bar);
    UI_ASSERT(color);

    /* color matrix */
    ui_color_hsva_fv(hsva, *color);
    if (ui_button_behavior(state, *matrix, in, UI_BUTTON_REPEATER)) {
        hsva[1] = UI_SATURATE((in->mouse.pos.x - matrix->x) / (matrix->w-1));
        hsva[2] = 1.0f - UI_SATURATE((in->mouse.pos.y - matrix->y) / (matrix->h-1));
        value_changed = hsv_changed = 1;
    }

    /* hue bar */
    if (ui_button_behavior(state, *hue_bar, in, UI_BUTTON_REPEATER)) {
        hsva[0] = UI_SATURATE((in->mouse.pos.y - hue_bar->y) / (hue_bar->h-1));
        value_changed = hsv_changed = 1;
    }

    /* alpha bar */
    if (alpha_bar) {
        if (ui_button_behavior(state, *alpha_bar, in, UI_BUTTON_REPEATER)) {
            hsva[3] = 1.0f - UI_SATURATE((in->mouse.pos.y - alpha_bar->y) / (alpha_bar->h-1));
            value_changed = 1;
        }
    }
    ui_widget_state_reset(state);
    if (hsv_changed) {
        *color = ui_hsva_fv(hsva);
        *state = UI_WIDGET_STATE_ACTIVE;
    }
    if (value_changed) {
        color->a = (ui_byte)(hsva[3] * 255.0f);
        *state = UI_WIDGET_STATE_ACTIVE;
    }

    /* set color picker widget state */
    if (ui_input_is_mouse_hovering_rect(in, *bounds))
        *state = UI_WIDGET_STATE_HOVERED;
    if (*state & UI_WIDGET_STATE_HOVER && !ui_input_is_mouse_prev_hovering_rect(in, *bounds))
        *state |= UI_WIDGET_STATE_ENTERED;
    else if (ui_input_is_mouse_prev_hovering_rect(in, *bounds))
        *state |= UI_WIDGET_STATE_LEFT;
    return value_changed;
}

UI_INTERN void
ui_draw_color_picker(struct ui_command_buffer *o, const struct ui_rect *matrix,
    const struct ui_rect *hue_bar, const struct ui_rect *alpha_bar,
    struct ui_color color)
{
    UI_STORAGE const struct ui_color black = {0,0,0,255};
    UI_STORAGE const struct ui_color white = {255, 255, 255, 255};
    UI_STORAGE const struct ui_color black_trans = {0,0,0,0};

    const float crosshair_size = 7.0f;
    struct ui_color temp;
    float hsva[4];
    float line_y;
    int i;

    UI_ASSERT(o);
    UI_ASSERT(matrix);
    UI_ASSERT(hue_bar);

    /* draw hue bar */
    ui_color_hsv_fv(hsva, color);
    for (i = 0; i < 6; ++i) {
        UI_GLOBAL const struct ui_color hue_colors[] = {
            {255, 0, 0, 255},
            {255,255,0,255},
            {0,255,0,255},
            {0, 255,255,255},
            {0,0,255,255},
            {255, 0, 255, 255},
            {255, 0, 0, 255}
        };
        ui_fill_rect_multi_color(o,
            ui_rect(hue_bar->x, hue_bar->y + (float)i * (hue_bar->h/6.0f) + 0.5f,
                hue_bar->w, (hue_bar->h/6.0f) + 0.5f), hue_colors[i], hue_colors[i],
                hue_colors[i+1], hue_colors[i+1]);
    }
    line_y = (float)(int)(hue_bar->y + hsva[0] * matrix->h + 0.5f);
    ui_stroke_line(o, hue_bar->x-1, line_y, hue_bar->x + hue_bar->w + 2,
        line_y, 1, ui_rgb(255,255,255));

    /* draw alpha bar */
    if (alpha_bar) {
        float alpha = UI_SATURATE((float)color.a/255.0f);
        line_y = (float)(int)(alpha_bar->y +  (1.0f - alpha) * matrix->h + 0.5f);

        ui_fill_rect_multi_color(o, *alpha_bar, white, white, black, black);
        ui_stroke_line(o, alpha_bar->x-1, line_y, alpha_bar->x + alpha_bar->w + 2,
            line_y, 1, ui_rgb(255,255,255));
    }

    /* draw color matrix */
    temp = ui_hsv_f(hsva[0], 1.0f, 1.0f);
    ui_fill_rect_multi_color(o, *matrix, white, temp, temp, white);
    ui_fill_rect_multi_color(o, *matrix, black_trans, black_trans, black, black);

    /* draw cross-hair */
    {struct ui_vec2 p; float S = hsva[1]; float V = hsva[2];
    p.x = (float)(int)(matrix->x + S * matrix->w);
    p.y = (float)(int)(matrix->y + (1.0f - V) * matrix->h);
    ui_stroke_line(o, p.x - crosshair_size, p.y, p.x-2, p.y, 1.0f, white);
    ui_stroke_line(o, p.x + crosshair_size + 1, p.y, p.x+3, p.y, 1.0f, white);
    ui_stroke_line(o, p.x, p.y + crosshair_size + 1, p.x, p.y+3, 1.0f, white);
    ui_stroke_line(o, p.x, p.y - crosshair_size, p.x, p.y-2, 1.0f, white);}
}

UI_INTERN int
ui_do_color_picker(ui_flags *state,
    struct ui_command_buffer *out, struct ui_color *color,
    enum ui_color_format fmt, struct ui_rect bounds,
    struct ui_vec2 padding, const struct ui_input *in,
    const struct ui_user_font *font)
{
    int ret = 0;
    struct ui_rect matrix;
    struct ui_rect hue_bar;
    struct ui_rect alpha_bar;
    float bar_w;

    UI_ASSERT(out);
    UI_ASSERT(color);
    UI_ASSERT(state);
    UI_ASSERT(font);
    if (!out || !color || !state || !font)
        return ret;

    bar_w = font->height;
    bounds.x += padding.x;
    bounds.y += padding.x;
    bounds.w -= 2 * padding.x;
    bounds.h -= 2 * padding.y;

    matrix.x = bounds.x;
    matrix.y = bounds.y;
    matrix.h = bounds.h;
    matrix.w = bounds.w - (3 * padding.x + 2 * bar_w);

    hue_bar.w = bar_w;
    hue_bar.y = bounds.y;
    hue_bar.h = matrix.h;
    hue_bar.x = matrix.x + matrix.w + padding.x;

    alpha_bar.x = hue_bar.x + hue_bar.w + padding.x;
    alpha_bar.y = bounds.y;
    alpha_bar.w = bar_w;
    alpha_bar.h = matrix.h;

    ret = ui_color_picker_behavior(state, &bounds, &matrix, &hue_bar,
        (fmt == UI_RGBA) ? &alpha_bar:0, color, in);
    ui_draw_color_picker(out, &matrix, &hue_bar, (fmt == UI_RGBA) ? &alpha_bar:0, *color);
    return ret;
}

/* ==============================================================
 *
 *                          STYLE
 *
 * ===============================================================*/
UI_API void ui_style_default(struct ui_context *ctx){ui_style_from_table(ctx, 0);}
#define UI_COLOR_MAP(UI_COLOR)\
    UI_COLOR(UI_COLOR_TEXT,                 175,175,175,255) \
    UI_COLOR(UI_COLOR_WINDOW,               45, 45, 45, 255) \
    UI_COLOR(UI_COLOR_HEADER,               40, 40, 40, 255) \
    UI_COLOR(UI_COLOR_BORDER,               65, 65, 65, 255) \
    UI_COLOR(UI_COLOR_BUTTON,               50, 50, 50, 255) \
    UI_COLOR(UI_COLOR_BUTTON_HOVER,         40, 40, 40, 255) \
    UI_COLOR(UI_COLOR_BUTTON_ACTIVE,        35, 35, 35, 255) \
    UI_COLOR(UI_COLOR_TOGGLE,               100,100,100,255) \
    UI_COLOR(UI_COLOR_TOGGLE_HOVER,         120,120,120,255) \
    UI_COLOR(UI_COLOR_TOGGLE_CURSOR,        45, 45, 45, 255) \
    UI_COLOR(UI_COLOR_SELECT,               45, 45, 45, 255) \
    UI_COLOR(UI_COLOR_SELECT_ACTIVE,        35, 35, 35,255) \
    UI_COLOR(UI_COLOR_SLIDER,               38, 38, 38, 255) \
    UI_COLOR(UI_COLOR_SLIDER_CURSOR,        100,100,100,255) \
    UI_COLOR(UI_COLOR_SLIDER_CURSOR_HOVER,  120,120,120,255) \
    UI_COLOR(UI_COLOR_SLIDER_CURSOR_ACTIVE, 150,150,150,255) \
    UI_COLOR(UI_COLOR_PROPERTY,             38, 38, 38, 255) \
    UI_COLOR(UI_COLOR_EDIT,                 38, 38, 38, 255)  \
    UI_COLOR(UI_COLOR_EDIT_CURSOR,          175,175,175,255) \
    UI_COLOR(UI_COLOR_COMBO,                45, 45, 45, 255) \
    UI_COLOR(UI_COLOR_CHART,                120,120,120,255) \
    UI_COLOR(UI_COLOR_CHART_COLOR,          45, 45, 45, 255) \
    UI_COLOR(UI_COLOR_CHART_COLOR_HIGHLIGHT,255, 0,  0, 255) \
    UI_COLOR(UI_COLOR_SCROLLBAR,            40, 40, 40, 255) \
    UI_COLOR(UI_COLOR_SCROLLBAR_CURSOR,     100,100,100,255) \
    UI_COLOR(UI_COLOR_SCROLLBAR_CURSOR_HOVER,120,120,120,255) \
    UI_COLOR(UI_COLOR_SCROLLBAR_CURSOR_ACTIVE,150,150,150,255) \
    UI_COLOR(UI_COLOR_TAB_HEADER,           40, 40, 40,255)

UI_GLOBAL const struct ui_color
ui_default_color_style[UI_COLOR_COUNT] = {
#define UI_COLOR(a,b,c,d,e) {b,c,d,e},
    UI_COLOR_MAP(UI_COLOR)
#undef UI_COLOR
};

UI_GLOBAL const char *ui_color_names[UI_COLOR_COUNT] = {
#define UI_COLOR(a,b,c,d,e) #a,
    UI_COLOR_MAP(UI_COLOR)
#undef UI_COLOR
};

UI_API const char *ui_style_get_color_by_name(enum ui_style_colors c)
{return ui_color_names[c];}

UI_API struct ui_style_item ui_style_item_image(struct ui_image img)
{struct ui_style_item i; i.type = UI_STYLE_ITEM_IMAGE; i.data.image = img; return i;}

UI_API struct ui_style_item ui_style_item_color(struct ui_color col)
{struct ui_style_item i; i.type = UI_STYLE_ITEM_COLOR; i.data.color = col; return i;}

UI_API struct ui_style_item ui_style_item_hide(void)
{struct ui_style_item i; i.type = UI_STYLE_ITEM_COLOR; i.data.color = ui_rgba(0,0,0,0); return i;}

UI_API void
ui_style_from_table(struct ui_context *ctx, const struct ui_color *table)
{
    struct ui_style *style;
    struct ui_style_text *text;
    struct ui_style_button *button;
    struct ui_style_toggle *toggle;
    struct ui_style_selectable *select;
    struct ui_style_slider *slider;
    struct ui_style_progress *prog;
    struct ui_style_scrollbar *scroll;
    struct ui_style_edit *edit;
    struct ui_style_property *property;
    struct ui_style_combo *combo;
    struct ui_style_chart *chart;
    struct ui_style_tab *tab;
    struct ui_style_window *win;

    UI_ASSERT(ctx);
    if (!ctx) return;
    style = &ctx->style;
    table = (!table) ? ui_default_color_style: table;

    /* default text */
    text = &style->text;
    text->color = table[UI_COLOR_TEXT];
    text->padding = ui_vec2(0,0);

    /* default button */
    button = &style->button;
    ui_zero_struct(*button);
    button->normal          = ui_style_item_color(table[UI_COLOR_BUTTON]);
    button->hover           = ui_style_item_color(table[UI_COLOR_BUTTON_HOVER]);
    button->active          = ui_style_item_color(table[UI_COLOR_BUTTON_ACTIVE]);
    button->border_color    = table[UI_COLOR_BORDER];
    button->text_background = table[UI_COLOR_BUTTON];
    button->text_normal     = table[UI_COLOR_TEXT];
    button->text_hover      = table[UI_COLOR_TEXT];
    button->text_active     = table[UI_COLOR_TEXT];
    button->padding         = ui_vec2(2.0f,2.0f);
    button->image_padding   = ui_vec2(0.0f,0.0f);
    button->touch_padding   = ui_vec2(0.0f, 0.0f);
    button->userdata        = ui_handle_ptr(0);
    button->text_alignment  = UI_TEXT_CENTERED;
    button->border          = 1.0f;
    button->rounding        = 4.0f;
    button->draw_begin      = 0;
    button->draw_end        = 0;

    /* contextual button */
    button = &style->contextual_button;
    ui_zero_struct(*button);
    button->normal          = ui_style_item_color(table[UI_COLOR_WINDOW]);
    button->hover           = ui_style_item_color(table[UI_COLOR_BUTTON_HOVER]);
    button->active          = ui_style_item_color(table[UI_COLOR_BUTTON_ACTIVE]);
    button->border_color    = table[UI_COLOR_WINDOW];
    button->text_background = table[UI_COLOR_WINDOW];
    button->text_normal     = table[UI_COLOR_TEXT];
    button->text_hover      = table[UI_COLOR_TEXT];
    button->text_active     = table[UI_COLOR_TEXT];
    button->padding         = ui_vec2(2.0f,2.0f);
    button->touch_padding   = ui_vec2(0.0f,0.0f);
    button->userdata        = ui_handle_ptr(0);
    button->text_alignment  = UI_TEXT_CENTERED;
    button->border          = 0.0f;
    button->rounding        = 0.0f;
    button->draw_begin      = 0;
    button->draw_end        = 0;

    /* menu button */
    button = &style->menu_button;
    ui_zero_struct(*button);
    button->normal          = ui_style_item_color(table[UI_COLOR_WINDOW]);
    button->hover           = ui_style_item_color(table[UI_COLOR_WINDOW]);
    button->active          = ui_style_item_color(table[UI_COLOR_WINDOW]);
    button->border_color    = table[UI_COLOR_WINDOW];
    button->text_background = table[UI_COLOR_WINDOW];
    button->text_normal     = table[UI_COLOR_TEXT];
    button->text_hover      = table[UI_COLOR_TEXT];
    button->text_active     = table[UI_COLOR_TEXT];
    button->padding         = ui_vec2(2.0f,2.0f);
    button->touch_padding   = ui_vec2(0.0f,0.0f);
    button->userdata        = ui_handle_ptr(0);
    button->text_alignment  = UI_TEXT_CENTERED;
    button->border          = 0.0f;
    button->rounding        = 1.0f;
    button->draw_begin      = 0;
    button->draw_end        = 0;

    /* checkbox toggle */
    toggle = &style->checkbox;
    ui_zero_struct(*toggle);
    toggle->normal          = ui_style_item_color(table[UI_COLOR_TOGGLE]);
    toggle->hover           = ui_style_item_color(table[UI_COLOR_TOGGLE_HOVER]);
    toggle->active          = ui_style_item_color(table[UI_COLOR_TOGGLE_HOVER]);
    toggle->cursor_normal   = ui_style_item_color(table[UI_COLOR_TOGGLE_CURSOR]);
    toggle->cursor_hover    = ui_style_item_color(table[UI_COLOR_TOGGLE_CURSOR]);
    toggle->userdata        = ui_handle_ptr(0);
    toggle->text_background = table[UI_COLOR_WINDOW];
    toggle->text_normal     = table[UI_COLOR_TEXT];
    toggle->text_hover      = table[UI_COLOR_TEXT];
    toggle->text_active     = table[UI_COLOR_TEXT];
    toggle->padding         = ui_vec2(2.0f, 2.0f);
    toggle->touch_padding   = ui_vec2(0,0);
    toggle->border_color    = ui_rgba(0,0,0,0);
    toggle->border          = 0.0f;
    toggle->spacing         = 4;

    /* option toggle */
    toggle = &style->option;
    ui_zero_struct(*toggle);
    toggle->normal          = ui_style_item_color(table[UI_COLOR_TOGGLE]);
    toggle->hover           = ui_style_item_color(table[UI_COLOR_TOGGLE_HOVER]);
    toggle->active          = ui_style_item_color(table[UI_COLOR_TOGGLE_HOVER]);
    toggle->cursor_normal   = ui_style_item_color(table[UI_COLOR_TOGGLE_CURSOR]);
    toggle->cursor_hover    = ui_style_item_color(table[UI_COLOR_TOGGLE_CURSOR]);
    toggle->userdata        = ui_handle_ptr(0);
    toggle->text_background = table[UI_COLOR_WINDOW];
    toggle->text_normal     = table[UI_COLOR_TEXT];
    toggle->text_hover      = table[UI_COLOR_TEXT];
    toggle->text_active     = table[UI_COLOR_TEXT];
    toggle->padding         = ui_vec2(3.0f, 3.0f);
    toggle->touch_padding   = ui_vec2(0,0);
    toggle->border_color    = ui_rgba(0,0,0,0);
    toggle->border          = 0.0f;
    toggle->spacing         = 4;

    /* selectable */
    select = &style->selectable;
    ui_zero_struct(*select);
    select->normal          = ui_style_item_color(table[UI_COLOR_SELECT]);
    select->hover           = ui_style_item_color(table[UI_COLOR_SELECT]);
    select->pressed         = ui_style_item_color(table[UI_COLOR_SELECT]);
    select->normal_active   = ui_style_item_color(table[UI_COLOR_SELECT_ACTIVE]);
    select->hover_active    = ui_style_item_color(table[UI_COLOR_SELECT_ACTIVE]);
    select->pressed_active  = ui_style_item_color(table[UI_COLOR_SELECT_ACTIVE]);
    select->text_normal     = table[UI_COLOR_TEXT];
    select->text_hover      = table[UI_COLOR_TEXT];
    select->text_pressed    = table[UI_COLOR_TEXT];
    select->text_normal_active  = table[UI_COLOR_TEXT];
    select->text_hover_active   = table[UI_COLOR_TEXT];
    select->text_pressed_active = table[UI_COLOR_TEXT];
    select->padding         = ui_vec2(2.0f,2.0f);
    select->touch_padding   = ui_vec2(0,0);
    select->userdata        = ui_handle_ptr(0);
    select->rounding        = 0.0f;
    select->draw_begin      = 0;
    select->draw_end        = 0;

    /* slider */
    slider = &style->slider;
    ui_zero_struct(*slider);
    slider->normal          = ui_style_item_hide();
    slider->hover           = ui_style_item_hide();
    slider->active          = ui_style_item_hide();
    slider->bar_normal      = table[UI_COLOR_SLIDER];
    slider->bar_hover       = table[UI_COLOR_SLIDER];
    slider->bar_active      = table[UI_COLOR_SLIDER];
    slider->bar_filled      = table[UI_COLOR_SLIDER_CURSOR];
    slider->cursor_normal   = ui_style_item_color(table[UI_COLOR_SLIDER_CURSOR]);
    slider->cursor_hover    = ui_style_item_color(table[UI_COLOR_SLIDER_CURSOR_HOVER]);
    slider->cursor_active   = ui_style_item_color(table[UI_COLOR_SLIDER_CURSOR_ACTIVE]);
    slider->inc_symbol      = UI_SYMBOL_TRIANGLE_RIGHT;
    slider->dec_symbol      = UI_SYMBOL_TRIANGLE_LEFT;
    slider->cursor_size     = ui_vec2(16,16);
    slider->padding         = ui_vec2(2,2);
    slider->spacing         = ui_vec2(2,2);
    slider->userdata        = ui_handle_ptr(0);
    slider->show_buttons    = ui_false;
    slider->bar_height      = 8;
    slider->rounding        = 0;
    slider->draw_begin      = 0;
    slider->draw_end        = 0;

    /* slider buttons */
    button = &style->slider.inc_button;
    button->normal          = ui_style_item_color(ui_rgb(40,40,40));
    button->hover           = ui_style_item_color(ui_rgb(42,42,42));
    button->active          = ui_style_item_color(ui_rgb(44,44,44));
    button->border_color    = ui_rgb(65,65,65);
    button->text_background = ui_rgb(40,40,40);
    button->text_normal     = ui_rgb(175,175,175);
    button->text_hover      = ui_rgb(175,175,175);
    button->text_active     = ui_rgb(175,175,175);
    button->padding         = ui_vec2(8.0f,8.0f);
    button->touch_padding   = ui_vec2(0.0f,0.0f);
    button->userdata        = ui_handle_ptr(0);
    button->text_alignment  = UI_TEXT_CENTERED;
    button->border          = 1.0f;
    button->rounding        = 0.0f;
    button->draw_begin      = 0;
    button->draw_end        = 0;
    style->slider.dec_button = style->slider.inc_button;

    /* progressbar */
    prog = &style->progress;
    ui_zero_struct(*prog);
    prog->normal            = ui_style_item_color(table[UI_COLOR_SLIDER]);
    prog->hover             = ui_style_item_color(table[UI_COLOR_SLIDER]);
    prog->active            = ui_style_item_color(table[UI_COLOR_SLIDER]);
    prog->cursor_normal     = ui_style_item_color(table[UI_COLOR_SLIDER_CURSOR]);
    prog->cursor_hover      = ui_style_item_color(table[UI_COLOR_SLIDER_CURSOR_HOVER]);
    prog->cursor_active     = ui_style_item_color(table[UI_COLOR_SLIDER_CURSOR_ACTIVE]);
    prog->border_color      = ui_rgba(0,0,0,0);
    prog->cursor_border_color = ui_rgba(0,0,0,0);
    prog->userdata          = ui_handle_ptr(0);
    prog->padding           = ui_vec2(4,4);
    prog->rounding          = 0;
    prog->border            = 0;
    prog->cursor_rounding   = 0;
    prog->cursor_border     = 0;
    prog->draw_begin        = 0;
    prog->draw_end          = 0;

    /* scrollbars */
    scroll = &style->scrollh;
    ui_zero_struct(*scroll);
    scroll->normal          = ui_style_item_color(table[UI_COLOR_SCROLLBAR]);
    scroll->hover           = ui_style_item_color(table[UI_COLOR_SCROLLBAR]);
    scroll->active          = ui_style_item_color(table[UI_COLOR_SCROLLBAR]);
    scroll->cursor_normal   = ui_style_item_color(table[UI_COLOR_SCROLLBAR_CURSOR]);
    scroll->cursor_hover    = ui_style_item_color(table[UI_COLOR_SCROLLBAR_CURSOR_HOVER]);
    scroll->cursor_active   = ui_style_item_color(table[UI_COLOR_SCROLLBAR_CURSOR_ACTIVE]);
    scroll->dec_symbol      = UI_SYMBOL_CIRCLE_SOLID;
    scroll->inc_symbol      = UI_SYMBOL_CIRCLE_SOLID;
    scroll->userdata        = ui_handle_ptr(0);
    scroll->border_color    = table[UI_COLOR_SCROLLBAR];
    scroll->cursor_border_color = table[UI_COLOR_SCROLLBAR];
    scroll->padding         = ui_vec2(0,0);
    scroll->show_buttons    = ui_false;
    scroll->border          = 0;
    scroll->rounding        = 0;
    scroll->border_cursor   = 0;
    scroll->rounding_cursor = 0;
    scroll->draw_begin      = 0;
    scroll->draw_end        = 0;
    style->scrollv = style->scrollh;

    /* scrollbars buttons */
    button = &style->scrollh.inc_button;
    button->normal          = ui_style_item_color(ui_rgb(40,40,40));
    button->hover           = ui_style_item_color(ui_rgb(42,42,42));
    button->active          = ui_style_item_color(ui_rgb(44,44,44));
    button->border_color    = ui_rgb(65,65,65);
    button->text_background = ui_rgb(40,40,40);
    button->text_normal     = ui_rgb(175,175,175);
    button->text_hover      = ui_rgb(175,175,175);
    button->text_active     = ui_rgb(175,175,175);
    button->padding         = ui_vec2(4.0f,4.0f);
    button->touch_padding   = ui_vec2(0.0f,0.0f);
    button->userdata        = ui_handle_ptr(0);
    button->text_alignment  = UI_TEXT_CENTERED;
    button->border          = 1.0f;
    button->rounding        = 0.0f;
    button->draw_begin      = 0;
    button->draw_end        = 0;
    style->scrollh.dec_button = style->scrollh.inc_button;
    style->scrollv.inc_button = style->scrollh.inc_button;
    style->scrollv.dec_button = style->scrollh.inc_button;

    /* edit */
    edit = &style->edit;
    ui_zero_struct(*edit);
    edit->normal            = ui_style_item_color(table[UI_COLOR_EDIT]);
    edit->hover             = ui_style_item_color(table[UI_COLOR_EDIT]);
    edit->active            = ui_style_item_color(table[UI_COLOR_EDIT]);
    edit->cursor_normal     = table[UI_COLOR_TEXT];
    edit->cursor_hover      = table[UI_COLOR_TEXT];
    edit->cursor_text_normal= table[UI_COLOR_EDIT];
    edit->cursor_text_hover = table[UI_COLOR_EDIT];
    edit->border_color      = table[UI_COLOR_BORDER];
    edit->text_normal       = table[UI_COLOR_TEXT];
    edit->text_hover        = table[UI_COLOR_TEXT];
    edit->text_active       = table[UI_COLOR_TEXT];
    edit->selected_normal   = table[UI_COLOR_TEXT];
    edit->selected_hover    = table[UI_COLOR_TEXT];
    edit->selected_text_normal  = table[UI_COLOR_EDIT];
    edit->selected_text_hover   = table[UI_COLOR_EDIT];
    edit->scrollbar_size    = ui_vec2(10,10);
    edit->scrollbar         = style->scrollv;
    edit->padding           = ui_vec2(4,4);
    edit->row_padding       = 2;
    edit->cursor_size       = 4;
    edit->border            = 1;
    edit->rounding          = 0;

    /* property */
    property = &style->property;
    ui_zero_struct(*property);
    property->normal        = ui_style_item_color(table[UI_COLOR_PROPERTY]);
    property->hover         = ui_style_item_color(table[UI_COLOR_PROPERTY]);
    property->active        = ui_style_item_color(table[UI_COLOR_PROPERTY]);
    property->border_color  = table[UI_COLOR_BORDER];
    property->label_normal  = table[UI_COLOR_TEXT];
    property->label_hover   = table[UI_COLOR_TEXT];
    property->label_active  = table[UI_COLOR_TEXT];
    property->sym_left      = UI_SYMBOL_TRIANGLE_LEFT;
    property->sym_right     = UI_SYMBOL_TRIANGLE_RIGHT;
    property->userdata      = ui_handle_ptr(0);
    property->padding       = ui_vec2(4,4);
    property->border        = 1;
    property->rounding      = 10;
    property->draw_begin    = 0;
    property->draw_end      = 0;

    /* property buttons */
    button = &style->property.dec_button;
    ui_zero_struct(*button);
    button->normal          = ui_style_item_color(table[UI_COLOR_PROPERTY]);
    button->hover           = ui_style_item_color(table[UI_COLOR_PROPERTY]);
    button->active          = ui_style_item_color(table[UI_COLOR_PROPERTY]);
    button->border_color    = ui_rgba(0,0,0,0);
    button->text_background = table[UI_COLOR_PROPERTY];
    button->text_normal     = table[UI_COLOR_TEXT];
    button->text_hover      = table[UI_COLOR_TEXT];
    button->text_active     = table[UI_COLOR_TEXT];
    button->padding         = ui_vec2(0.0f,0.0f);
    button->touch_padding   = ui_vec2(0.0f,0.0f);
    button->userdata        = ui_handle_ptr(0);
    button->text_alignment  = UI_TEXT_CENTERED;
    button->border          = 0.0f;
    button->rounding        = 0.0f;
    button->draw_begin      = 0;
    button->draw_end        = 0;
    style->property.inc_button = style->property.dec_button;

    /* property edit */
    edit = &style->property.edit;
    ui_zero_struct(*edit);
    edit->normal            = ui_style_item_color(table[UI_COLOR_PROPERTY]);
    edit->hover             = ui_style_item_color(table[UI_COLOR_PROPERTY]);
    edit->active            = ui_style_item_color(table[UI_COLOR_PROPERTY]);
    edit->border_color      = ui_rgba(0,0,0,0);
    edit->cursor_normal     = table[UI_COLOR_TEXT];
    edit->cursor_hover      = table[UI_COLOR_TEXT];
    edit->cursor_text_normal= table[UI_COLOR_EDIT];
    edit->cursor_text_hover = table[UI_COLOR_EDIT];
    edit->text_normal       = table[UI_COLOR_TEXT];
    edit->text_hover        = table[UI_COLOR_TEXT];
    edit->text_active       = table[UI_COLOR_TEXT];
    edit->selected_normal   = table[UI_COLOR_TEXT];
    edit->selected_hover    = table[UI_COLOR_TEXT];
    edit->selected_text_normal  = table[UI_COLOR_EDIT];
    edit->selected_text_hover   = table[UI_COLOR_EDIT];
    edit->padding           = ui_vec2(0,0);
    edit->cursor_size       = 8;
    edit->border            = 0;
    edit->rounding          = 0;

    /* chart */
    chart = &style->chart;
    ui_zero_struct(*chart);
    chart->background       = ui_style_item_color(table[UI_COLOR_CHART]);
    chart->border_color     = table[UI_COLOR_BORDER];
    chart->selected_color   = table[UI_COLOR_CHART_COLOR_HIGHLIGHT];
    chart->color            = table[UI_COLOR_CHART_COLOR];
    chart->padding          = ui_vec2(4,4);
    chart->border           = 0;
    chart->rounding         = 0;

    /* combo */
    combo = &style->combo;
    combo->normal           = ui_style_item_color(table[UI_COLOR_COMBO]);
    combo->hover            = ui_style_item_color(table[UI_COLOR_COMBO]);
    combo->active           = ui_style_item_color(table[UI_COLOR_COMBO]);
    combo->border_color     = table[UI_COLOR_BORDER];
    combo->label_normal     = table[UI_COLOR_TEXT];
    combo->label_hover      = table[UI_COLOR_TEXT];
    combo->label_active     = table[UI_COLOR_TEXT];
    combo->sym_normal       = UI_SYMBOL_TRIANGLE_DOWN;
    combo->sym_hover        = UI_SYMBOL_TRIANGLE_DOWN;
    combo->sym_active       = UI_SYMBOL_TRIANGLE_DOWN;
    combo->content_padding  = ui_vec2(4,4);
    combo->button_padding   = ui_vec2(0,4);
    combo->spacing          = ui_vec2(4,0);
    combo->border           = 1;
    combo->rounding         = 0;

    /* combo button */
    button = &style->combo.button;
    ui_zero_struct(*button);
    button->normal          = ui_style_item_color(table[UI_COLOR_COMBO]);
    button->hover           = ui_style_item_color(table[UI_COLOR_COMBO]);
    button->active          = ui_style_item_color(table[UI_COLOR_COMBO]);
    button->border_color    = ui_rgba(0,0,0,0);
    button->text_background = table[UI_COLOR_COMBO];
    button->text_normal     = table[UI_COLOR_TEXT];
    button->text_hover      = table[UI_COLOR_TEXT];
    button->text_active     = table[UI_COLOR_TEXT];
    button->padding         = ui_vec2(2.0f,2.0f);
    button->touch_padding   = ui_vec2(0.0f,0.0f);
    button->userdata        = ui_handle_ptr(0);
    button->text_alignment  = UI_TEXT_CENTERED;
    button->border          = 0.0f;
    button->rounding        = 0.0f;
    button->draw_begin      = 0;
    button->draw_end        = 0;

    /* tab */
    tab = &style->tab;
    tab->background         = ui_style_item_color(table[UI_COLOR_TAB_HEADER]);
    tab->border_color       = table[UI_COLOR_BORDER];
    tab->text               = table[UI_COLOR_TEXT];
    tab->sym_minimize       = UI_SYMBOL_TRIANGLE_RIGHT;
    tab->sym_maximize       = UI_SYMBOL_TRIANGLE_DOWN;
    tab->padding            = ui_vec2(4,4);
    tab->spacing            = ui_vec2(4,4);
    tab->indent             = 10.0f;
    tab->border             = 1;
    tab->rounding           = 0;

    /* tab button */
    button = &style->tab.tab_minimize_button;
    ui_zero_struct(*button);
    button->normal          = ui_style_item_color(table[UI_COLOR_TAB_HEADER]);
    button->hover           = ui_style_item_color(table[UI_COLOR_TAB_HEADER]);
    button->active          = ui_style_item_color(table[UI_COLOR_TAB_HEADER]);
    button->border_color    = ui_rgba(0,0,0,0);
    button->text_background = table[UI_COLOR_TAB_HEADER];
    button->text_normal     = table[UI_COLOR_TEXT];
    button->text_hover      = table[UI_COLOR_TEXT];
    button->text_active     = table[UI_COLOR_TEXT];
    button->padding         = ui_vec2(2.0f,2.0f);
    button->touch_padding   = ui_vec2(0.0f,0.0f);
    button->userdata        = ui_handle_ptr(0);
    button->text_alignment  = UI_TEXT_CENTERED;
    button->border          = 0.0f;
    button->rounding        = 0.0f;
    button->draw_begin      = 0;
    button->draw_end        = 0;
    style->tab.tab_maximize_button =*button;

    /* node button */
    button = &style->tab.node_minimize_button;
    ui_zero_struct(*button);
    button->normal          = ui_style_item_color(table[UI_COLOR_WINDOW]);
    button->hover           = ui_style_item_color(table[UI_COLOR_WINDOW]);
    button->active          = ui_style_item_color(table[UI_COLOR_WINDOW]);
    button->border_color    = ui_rgba(0,0,0,0);
    button->text_background = table[UI_COLOR_TAB_HEADER];
    button->text_normal     = table[UI_COLOR_TEXT];
    button->text_hover      = table[UI_COLOR_TEXT];
    button->text_active     = table[UI_COLOR_TEXT];
    button->padding         = ui_vec2(2.0f,2.0f);
    button->touch_padding   = ui_vec2(0.0f,0.0f);
    button->userdata        = ui_handle_ptr(0);
    button->text_alignment  = UI_TEXT_CENTERED;
    button->border          = 0.0f;
    button->rounding        = 0.0f;
    button->draw_begin      = 0;
    button->draw_end        = 0;
    style->tab.node_maximize_button =*button;

    /* window header */
    win = &style->window;
    win->header.align = UI_HEADER_RIGHT;
    win->header.close_symbol = UI_SYMBOL_X;
    win->header.minimize_symbol = UI_SYMBOL_MINUS;
    win->header.maximize_symbol = UI_SYMBOL_PLUS;
    win->header.normal = ui_style_item_color(table[UI_COLOR_HEADER]);
    win->header.hover = ui_style_item_color(table[UI_COLOR_HEADER]);
    win->header.active = ui_style_item_color(table[UI_COLOR_HEADER]);
    win->header.label_normal = table[UI_COLOR_TEXT];
    win->header.label_hover = table[UI_COLOR_TEXT];
    win->header.label_active = table[UI_COLOR_TEXT];
    win->header.label_padding = ui_vec2(4,4);
    win->header.padding = ui_vec2(4,4);
    win->header.spacing = ui_vec2(0,0);

    /* window header close button */
    button = &style->window.header.close_button;
    ui_zero_struct(*button);
    button->normal          = ui_style_item_color(table[UI_COLOR_HEADER]);
    button->hover           = ui_style_item_color(table[UI_COLOR_HEADER]);
    button->active          = ui_style_item_color(table[UI_COLOR_HEADER]);
    button->border_color    = ui_rgba(0,0,0,0);
    button->text_background = table[UI_COLOR_HEADER];
    button->text_normal     = table[UI_COLOR_TEXT];
    button->text_hover      = table[UI_COLOR_TEXT];
    button->text_active     = table[UI_COLOR_TEXT];
    button->padding         = ui_vec2(0.0f,0.0f);
    button->touch_padding   = ui_vec2(0.0f,0.0f);
    button->userdata        = ui_handle_ptr(0);
    button->text_alignment  = UI_TEXT_CENTERED;
    button->border          = 0.0f;
    button->rounding        = 0.0f;
    button->draw_begin      = 0;
    button->draw_end        = 0;

    /* window header minimize button */
    button = &style->window.header.minimize_button;
    ui_zero_struct(*button);
    button->normal          = ui_style_item_color(table[UI_COLOR_HEADER]);
    button->hover           = ui_style_item_color(table[UI_COLOR_HEADER]);
    button->active          = ui_style_item_color(table[UI_COLOR_HEADER]);
    button->border_color    = ui_rgba(0,0,0,0);
    button->text_background = table[UI_COLOR_HEADER];
    button->text_normal     = table[UI_COLOR_TEXT];
    button->text_hover      = table[UI_COLOR_TEXT];
    button->text_active     = table[UI_COLOR_TEXT];
    button->padding         = ui_vec2(0.0f,0.0f);
    button->touch_padding   = ui_vec2(0.0f,0.0f);
    button->userdata        = ui_handle_ptr(0);
    button->text_alignment  = UI_TEXT_CENTERED;
    button->border          = 0.0f;
    button->rounding        = 0.0f;
    button->draw_begin      = 0;
    button->draw_end        = 0;

    /* window */
    win->background = table[UI_COLOR_WINDOW];
    win->fixed_background = ui_style_item_color(table[UI_COLOR_WINDOW]);
    win->border_color = table[UI_COLOR_BORDER];
    win->popup_border_color = table[UI_COLOR_BORDER];
    win->combo_border_color = table[UI_COLOR_BORDER];
    win->contextual_border_color = table[UI_COLOR_BORDER];
    win->menu_border_color = table[UI_COLOR_BORDER];
    win->group_border_color = table[UI_COLOR_BORDER];
    win->tooltip_border_color = table[UI_COLOR_BORDER];
    win->scaler = ui_style_item_color(table[UI_COLOR_TEXT]);

    win->rounding = 0.0f;
    win->spacing = ui_vec2(4,4);
    win->scrollbar_size = ui_vec2(10,10);
    win->min_size = ui_vec2(64,64);

    win->combo_border = 1.0f;
    win->contextual_border = 1.0f;
    win->menu_border = 1.0f;
    win->group_border = 1.0f;
    win->tooltip_border = 1.0f;
    win->popup_border = 1.0f;
    win->border = 2.0f;

    win->padding = ui_vec2(4,4);
    win->group_padding = ui_vec2(4,4);
    win->popup_padding = ui_vec2(4,4);
    win->combo_padding = ui_vec2(4,4);
    win->contextual_padding = ui_vec2(4,4);
    win->menu_padding = ui_vec2(4,4);
    win->tooltip_padding = ui_vec2(4,4);
}

UI_API void
ui_style_set_font(struct ui_context *ctx, const struct ui_user_font *font)
{
    struct ui_style *style;
    UI_ASSERT(ctx);
    if (!ctx) return;
    style = &ctx->style;
    style->font = font;
    ctx->stacks.fonts.head = 0;
}

UI_API int
ui_style_push_font(struct ui_context *ctx, struct ui_user_font *font)
{
    struct ui_config_stack_user_font *font_stack;
    struct ui_config_stack_user_font_element *element;

    UI_ASSERT(ctx);
    if (!ctx) return 0;

    font_stack = &ctx->stacks.fonts;
    UI_ASSERT(font_stack->head < (int)UI_LEN(font_stack->elements));
    if (font_stack->head >= (int)UI_LEN(font_stack->elements))
        return 0;

    element = &font_stack->elements[font_stack->head++];
    element->address = &ctx->style.font;
    element->old_value = ctx->style.font;
    ctx->style.font = font;
    return 1;
}

UI_API int
ui_style_pop_font(struct ui_context *ctx)
{
    struct ui_config_stack_user_font *font_stack;
    struct ui_config_stack_user_font_element *element;

    UI_ASSERT(ctx);
    if (!ctx) return 0;

    font_stack = &ctx->stacks.fonts;
    UI_ASSERT(font_stack->head > 0);
    if (font_stack->head < 1)
        return 0;

    element = &font_stack->elements[--font_stack->head];
    *element->address = element->old_value;
    return 1;
}

#define UI_STYLE_PUSH_IMPLEMENATION(prefix, type, stack) \
ui_style_push_##type(struct ui_context *ctx, prefix##_##type *address, prefix##_##type value)\
{\
    struct ui_config_stack_##type * type_stack;\
    struct ui_config_stack_##type##_element *element;\
    UI_ASSERT(ctx);\
    if (!ctx) return 0;\
    type_stack = &ctx->stacks.stack;\
    UI_ASSERT(type_stack->head < (int)UI_LEN(type_stack->elements));\
    if (type_stack->head >= (int)UI_LEN(type_stack->elements))\
        return 0;\
    element = &type_stack->elements[type_stack->head++];\
    element->address = address;\
    element->old_value = *address;\
    *address = value;\
    return 1;\
}

#define UI_STYLE_POP_IMPLEMENATION(type, stack) \
ui_style_pop_##type(struct ui_context *ctx)\
{\
    struct ui_config_stack_##type *type_stack;\
    struct ui_config_stack_##type##_element *element;\
    UI_ASSERT(ctx);\
    if (!ctx) return 0;\
    type_stack = &ctx->stacks.stack;\
    UI_ASSERT(type_stack->head > 0);\
    if (type_stack->head < 1)\
        return 0;\
    element = &type_stack->elements[--type_stack->head];\
    *element->address = element->old_value;\
    return 1;\
}

UI_API int UI_STYLE_PUSH_IMPLEMENATION(struct ui, style_item, style_items)
UI_API int UI_STYLE_PUSH_IMPLEMENATION(ui,float, floats)
UI_API int UI_STYLE_PUSH_IMPLEMENATION(struct ui, vec2, vectors)
UI_API int UI_STYLE_PUSH_IMPLEMENATION(ui,flags, flags)
UI_API int UI_STYLE_PUSH_IMPLEMENATION(struct ui,color, colors)

UI_API int UI_STYLE_POP_IMPLEMENATION(style_item, style_items)
UI_API int UI_STYLE_POP_IMPLEMENATION(float,floats)
UI_API int UI_STYLE_POP_IMPLEMENATION(vec2, vectors)
UI_API int UI_STYLE_POP_IMPLEMENATION(flags,flags)
UI_API int UI_STYLE_POP_IMPLEMENATION(color,colors)

UI_API int
ui_style_set_cursor(struct ui_context *ctx, enum ui_style_cursor c)
{
    struct ui_style *style;
    UI_ASSERT(ctx);
    if (!ctx) return 0;
    style = &ctx->style;
    if (style->cursors[c]) {
        style->cursor_active = style->cursors[c];
        return 1;
    }
    return 0;
}

UI_API void
ui_style_show_cursor(struct ui_context *ctx)
{
    ctx->style.cursor_visible = ui_true;
}

UI_API void
ui_style_hide_cursor(struct ui_context *ctx)
{
    ctx->style.cursor_visible = ui_false;
}

UI_API void
ui_style_load_cursor(struct ui_context *ctx, enum ui_style_cursor cursor,
    const struct ui_cursor *c)
{
    struct ui_style *style;
    UI_ASSERT(ctx);
    if (!ctx) return;
    style = &ctx->style;
    style->cursors[cursor] = c;
}

UI_API void
ui_style_load_all_cursors(struct ui_context *ctx, struct ui_cursor *cursors)
{
    int i = 0;
    struct ui_style *style;
    UI_ASSERT(ctx);
    if (!ctx) return;
    style = &ctx->style;
    for (i = 0; i < UI_CURSOR_COUNT; ++i)
        style->cursors[i] = &cursors[i];
    style->cursor_visible = ui_true;
}

/* ===============================================================
 *
 *                          POOL
 *
 * ===============================================================*/
UI_INTERN void
ui_pool_init(struct ui_pool *pool, struct ui_allocator *alloc,
            unsigned int capacity)
{
    ui_zero(pool, sizeof(*pool));
    pool->alloc = *alloc;
    pool->capacity = capacity;
    pool->type = UI_BUFFER_DYNAMIC;
    pool->pages = 0;
}

UI_INTERN void
ui_pool_free(struct ui_pool *pool)
{
    struct ui_page *next;
    struct ui_page *iter = pool->pages;
    if (!pool) return;
    if (pool->type == UI_BUFFER_FIXED) return;
    while (iter) {
        next = iter->next;
        pool->alloc.free(pool->alloc.userdata, iter);
        iter = next;
    }
}

UI_INTERN void
ui_pool_init_fixed(struct ui_pool *pool, void *memory, ui_size size)
{
    ui_zero(pool, sizeof(*pool));
    UI_ASSERT(size >= sizeof(struct ui_page));
    if (size < sizeof(struct ui_page)) return;
    pool->capacity = (unsigned)(size - sizeof(struct ui_page)) / sizeof(struct ui_page_element);
    pool->pages = (struct ui_page*)memory;
    pool->type = UI_BUFFER_FIXED;
    pool->size = size;
}

UI_INTERN struct ui_page_element*
ui_pool_alloc(struct ui_pool *pool)
{
    if (!pool->pages || pool->pages->size >= pool->capacity) {
        /* allocate new page */
        struct ui_page *page;
        if (pool->type == UI_BUFFER_FIXED) {
            if (!pool->pages) {
                UI_ASSERT(pool->pages);
                return 0;
            }
            UI_ASSERT(pool->pages->size < pool->capacity);
            return 0;
        } else {
            ui_size size = sizeof(struct ui_page);
            size += UI_POOL_DEFAULT_CAPACITY * sizeof(union ui_page_data);
            page = (struct ui_page*)pool->alloc.alloc(pool->alloc.userdata,0, size);
            page->next = pool->pages;
            pool->pages = page;
            page->size = 0;
        }
    }
    return &pool->pages->win[pool->pages->size++];
}

/* ===============================================================
 *
 *                          CONTEXT
 *
 * ===============================================================*/
UI_INTERN void* ui_create_window(struct ui_context *ctx);
UI_INTERN void ui_remove_window(struct ui_context*, struct ui_window*);
UI_INTERN void ui_free_window(struct ui_context *ctx, struct ui_window *win);
UI_INTERN void ui_free_table(struct ui_context *ctx, struct ui_table *tbl);
UI_INTERN void ui_remove_table(struct ui_window *win, struct ui_table *tbl);
UI_INTERN void* ui_create_panel(struct ui_context *ctx);
UI_INTERN void ui_free_panel(struct ui_context*, struct ui_panel *pan);

UI_INTERN void
ui_setup(struct ui_context *ctx, const struct ui_user_font *font)
{
    UI_ASSERT(ctx);
    if (!ctx) return;
    ui_zero_struct(*ctx);
    ui_style_default(ctx);
    ctx->seq = 1;
    if (font) ctx->style.font = font;
#ifdef UI_INCLUDE_VERTEX_BUFFER_OUTPUT
    ui_draw_list_init(&ctx->draw_list);
#endif
}

#ifdef UI_INCLUDE_DEFAULT_ALLOCATOR
UI_API int
ui_init_default(struct ui_context *ctx, const struct ui_user_font *font)
{
    struct ui_allocator alloc;
    alloc.userdata.ptr = 0;
    alloc.alloc = ui_malloc;
    alloc.free = ui_mfree;
    return ui_init(ctx, &alloc, font);
}
#endif

UI_API int
ui_init_fixed(struct ui_context *ctx, void *memory, ui_size size,
    const struct ui_user_font *font)
{
    UI_ASSERT(memory);
    if (!memory) return 0;
    ui_setup(ctx, font);
    ui_buffer_init_fixed(&ctx->memory, memory, size);
    ctx->use_pool = ui_false;
    return 1;
}

UI_API int
ui_init_custom(struct ui_context *ctx, struct ui_buffer *cmds,
    struct ui_buffer *pool, const struct ui_user_font *font)
{
    UI_ASSERT(cmds);
    UI_ASSERT(pool);
    if (!cmds || !pool) return 0;

    ui_setup(ctx, font);
    ctx->memory = *cmds;
    if (pool->type == UI_BUFFER_FIXED) {
        /* take memory from buffer and alloc fixed pool */
        ui_pool_init_fixed(&ctx->pool, pool->memory.ptr, pool->memory.size);
    } else {
        /* create dynamic pool from buffer allocator */
        struct ui_allocator *alloc = &pool->pool;
        ui_pool_init(&ctx->pool, alloc, UI_POOL_DEFAULT_CAPACITY);
    }
    ctx->use_pool = ui_true;
    return 1;
}

UI_API int
ui_init(struct ui_context *ctx, struct ui_allocator *alloc,
    const struct ui_user_font *font)
{
    UI_ASSERT(alloc);
    if (!alloc) return 0;
    ui_setup(ctx, font);
    ui_buffer_init(&ctx->memory, alloc, UI_DEFAULT_COMMAND_BUFFER_SIZE);
    ui_pool_init(&ctx->pool, alloc, UI_POOL_DEFAULT_CAPACITY);
    ctx->use_pool = ui_true;
    return 1;
}

#ifdef UI_INCLUDE_COMMAND_USERDATA
UI_API void
ui_set_user_data(struct ui_context *ctx, ui_handle handle)
{
    if (!ctx) return;
    ctx->userdata = handle;
    if (ctx->current)
        ctx->current->buffer.userdata = handle;
}
#endif

UI_API void
ui_free(struct ui_context *ctx)
{
    UI_ASSERT(ctx);
    if (!ctx) return;
    ui_buffer_free(&ctx->memory);
    if (ctx->use_pool)
        ui_pool_free(&ctx->pool);

    ui_zero(&ctx->input, sizeof(ctx->input));
    ui_zero(&ctx->style, sizeof(ctx->style));
    ui_zero(&ctx->memory, sizeof(ctx->memory));

    ctx->seq = 0;
    ctx->build = 0;
    ctx->begin = 0;
    ctx->end = 0;
    ctx->active = 0;
    ctx->current = 0;
    ctx->freelist = 0;
    ctx->count = 0;
}

UI_API void
ui_clear(struct ui_context *ctx)
{
    struct ui_window *iter;
    struct ui_window *next;
    UI_ASSERT(ctx);

    if (!ctx) return;
    if (ctx->use_pool)
        ui_buffer_clear(&ctx->memory);
    else ui_buffer_reset(&ctx->memory, UI_BUFFER_FRONT);

    ctx->build = 0;
    ctx->memory.calls = 0;
    ctx->last_widget_state = 0;
    ctx->style.cursor_active = ctx->style.cursors[UI_CURSOR_ARROW];
    UI_MEMSET(&ctx->overlay, 0, sizeof(ctx->overlay));
#ifdef UI_INCLUDE_VERTEX_BUFFER_OUTPUT
    ui_draw_list_clear(&ctx->draw_list);
#endif

    /* garbage collector */
    iter = ctx->begin;
    while (iter) {
        /* make sure minimized windows do not get removed */
        if ((iter->flags & UI_WINDOW_MINIMIZED) &&
            !(iter->flags & UI_WINDOW_CLOSED)) {
            iter = iter->next;
            continue;
        }

        /* remove hotness from hidden or closed windows*/
        if (((iter->flags & UI_WINDOW_HIDDEN) ||
            (iter->flags & UI_WINDOW_CLOSED)) &&
            iter == ctx->active)
            ctx->active = iter->next;

        /* free unused popup windows */
        if (iter->popup.win && iter->popup.win->seq != ctx->seq) {
            ui_free_window(ctx, iter->popup.win);
            iter->popup.win = 0;
        }

        /* remove unused window state tables */
        {struct ui_table *n, *it = iter->tables;
        while (it) {
            n = it->next;
            if (it->seq != ctx->seq) {
                ui_remove_table(iter, it);
                ui_zero(it, sizeof(union ui_page_data));
                ui_free_table(ctx, it);
                if (it == iter->tables)
                    iter->tables = n;
            }
            it = n;
        }}

        /* window itself is not used anymore so free */
        if (iter->seq != ctx->seq || iter->flags & UI_WINDOW_CLOSED) {
            next = iter->next;
            ui_remove_window(ctx, iter);
            ui_free_window(ctx, iter);
            iter = next;
        } else iter = iter->next;
    }
    ctx->seq++;
}

/* ----------------------------------------------------------------
 *
 *                          BUFFERING
 *
 * ---------------------------------------------------------------*/
UI_INTERN void
ui_start_buffer(struct ui_context *ctx, struct ui_command_buffer *buffer)
{
    UI_ASSERT(ctx);
    UI_ASSERT(buffer);
    if (!ctx || !buffer) return;
    buffer->begin = ctx->memory.allocated;
    buffer->end = buffer->begin;
    buffer->last = buffer->begin;
    buffer->clip = ui_null_rect;
}

UI_INTERN void
ui_start(struct ui_context *ctx, struct ui_window *win)
{
    UI_ASSERT(ctx);
    UI_ASSERT(win);
    ui_start_buffer(ctx, &win->buffer);
}

UI_INTERN void
ui_start_popup(struct ui_context *ctx, struct ui_window *win)
{
    struct ui_popup_buffer *buf;
    struct ui_panel *iter;
    UI_ASSERT(ctx);
    UI_ASSERT(win);
    if (!ctx || !win) return;

    /* make sure to use the correct popup buffer*/
    iter = win->layout;
    while (iter->parent)
        iter = iter->parent;

    /* save buffer fill state for popup */
    buf = &iter->popup_buffer;
    buf->begin = win->buffer.end;
    buf->end = win->buffer.end;
    buf->parent = win->buffer.last;
    buf->last = buf->begin;
    buf->active = ui_true;
}

UI_INTERN void
ui_finish_popup(struct ui_context *ctx, struct ui_window *win)
{
    struct ui_popup_buffer *buf;
    struct ui_panel *iter;
    UI_ASSERT(ctx);
    UI_ASSERT(win);
    if (!ctx || !win) return;

    /* make sure to use the correct popup buffer*/
    iter = win->layout;
    while (iter->parent)
        iter = iter->parent;

    buf = &iter->popup_buffer;
    buf->last = win->buffer.last;
    buf->end = win->buffer.end;
}

UI_INTERN void
ui_finish_buffer(struct ui_context *ctx, struct ui_command_buffer *buffer)
{
    UI_ASSERT(ctx);
    UI_ASSERT(buffer);
    if (!ctx || !buffer) return;
    buffer->end = ctx->memory.allocated;
}

UI_INTERN void
ui_finish(struct ui_context *ctx, struct ui_window *win)
{
    struct ui_popup_buffer *buf;
    struct ui_command *parent_last;
    struct ui_command *sublast;
    struct ui_command *last;
    void *memory;

    UI_ASSERT(ctx);
    UI_ASSERT(win);
    if (!ctx || !win) return;
    ui_finish_buffer(ctx, &win->buffer);
    if (!win->layout->popup_buffer.active) return;

    /* from here on is for popup window buffer handling */
    /*--------------------------------------------------*/
    buf = &win->layout->popup_buffer;
    memory = ctx->memory.memory.ptr;

    /* redirect the sub-window buffer to the end of the window command buffer */
    parent_last = ui_ptr_add(struct ui_command, memory, buf->parent);
    sublast = ui_ptr_add(struct ui_command, memory, buf->last);
    last = ui_ptr_add(struct ui_command, memory, win->buffer.last);

    parent_last->next = buf->end;
    sublast->next = last->next;
    last->next = buf->begin;
    win->buffer.last = buf->last;
    win->buffer.end = buf->end;
    buf->active = ui_false;
}

UI_INTERN void
ui_build(struct ui_context *ctx)
{
    struct ui_window *iter;
    struct ui_window *next;
    struct ui_command *cmd;
    ui_byte *buffer;

    /* draw cursor overlay */
    if (!ctx->style.cursor_active)
        ctx->style.cursor_active = ctx->style.cursors[UI_CURSOR_ARROW];
    if (ctx->style.cursor_active && !ctx->input.mouse.grabbed && ctx->style.cursor_visible) {
        struct ui_rect mouse_bounds;
        const struct ui_cursor *cursor = ctx->style.cursor_active;
        ui_command_buffer_init(&ctx->overlay, &ctx->memory, UI_CLIPPING_OFF);
        ui_start_buffer(ctx, &ctx->overlay);

        mouse_bounds.x = ctx->input.mouse.pos.x - cursor->offset.x;
        mouse_bounds.y = ctx->input.mouse.pos.y - cursor->offset.y;
        mouse_bounds.w = cursor->size.x;
        mouse_bounds.h = cursor->size.y;

        ui_draw_image(&ctx->overlay, mouse_bounds, &cursor->img, ui_white);
        ui_finish_buffer(ctx, &ctx->overlay);
    }

    /* build one big draw command list out of all buffers */
    iter = ctx->begin;
    buffer = (ui_byte*)ctx->memory.memory.ptr;
    while (iter != 0) {
        next = iter->next;
        if (iter->buffer.last == iter->buffer.begin || (iter->flags & UI_WINDOW_HIDDEN)) {
            iter = next;
            continue;
        }
        cmd = ui_ptr_add(struct ui_command, buffer, iter->buffer.last);
        while (next && ((next->buffer.last == next->buffer.begin) ||
            (next->flags & UI_WINDOW_HIDDEN)))
            next = next->next; /* skip empty command buffers */

        if (next) {
            cmd->next = next->buffer.begin;
        } else {
            if (ctx->overlay.end != ctx->overlay.begin)
                cmd->next = ctx->overlay.begin;
            else cmd->next = ctx->memory.allocated;
        }
        iter = next;
    }
}

UI_API const struct ui_command*
ui__begin(struct ui_context *ctx)
{
    struct ui_window *iter;
    ui_byte *buffer;
    UI_ASSERT(ctx);
    if (!ctx) return 0;
    if (!ctx->count) return 0;

    buffer = (ui_byte*)ctx->memory.memory.ptr;
    if (!ctx->build) {
        ui_build(ctx);
        ctx->build = ui_true;
    }

    iter = ctx->begin;
    while (iter && ((iter->buffer.begin == iter->buffer.end) || (iter->flags & UI_WINDOW_HIDDEN)))
        iter = iter->next;
    if (!iter) return 0;
    return ui_ptr_add_const(struct ui_command, buffer, iter->buffer.begin);
}

UI_API const struct ui_command*
ui__next(struct ui_context *ctx, const struct ui_command *cmd)
{
    ui_byte *buffer;
    const struct ui_command *next;
    UI_ASSERT(ctx);
    if (!ctx || !cmd || !ctx->count) return 0;
    if (cmd->next >= ctx->memory.allocated) return 0;
    buffer = (ui_byte*)ctx->memory.memory.ptr;
    next = ui_ptr_add_const(struct ui_command, buffer, cmd->next);
    return next;
}

/* ----------------------------------------------------------------
 *
 *                          PANEL
 *
 * ---------------------------------------------------------------*/
static int
ui_panel_has_header(ui_flags flags, const char *title)
{
    /* window header state */
    int active = 0;
    active = (flags & (UI_WINDOW_CLOSABLE|UI_WINDOW_MINIMIZABLE));
    active = active || (flags & UI_WINDOW_TITLE);
    active = active && !(flags & UI_WINDOW_HIDDEN) && title;
    return active;
}

UI_INTERN struct ui_vec2
ui_panel_get_padding(const struct ui_style *style, enum ui_panel_type type)
{
    switch (type) {
    default:
    case UI_PANEL_WINDOW: return style->window.padding;
    case UI_PANEL_GROUP: return style->window.group_padding;
    case UI_PANEL_POPUP: return style->window.popup_padding;
    case UI_PANEL_CONTEXTUAL: return style->window.contextual_padding;
    case UI_PANEL_COMBO: return style->window.combo_padding;
    case UI_PANEL_MENU: return style->window.menu_padding;
    case UI_PANEL_TOOLTIP: return style->window.menu_padding;
    }
}

UI_INTERN float
ui_panel_get_border(const struct ui_style *style, ui_flags flags,
    enum ui_panel_type type)
{
    if (flags & UI_WINDOW_BORDER) {
        switch (type) {
        default:
        case UI_PANEL_WINDOW: return style->window.border;
        case UI_PANEL_GROUP: return style->window.group_border;
        case UI_PANEL_POPUP: return style->window.popup_border;
        case UI_PANEL_CONTEXTUAL: return style->window.contextual_border;
        case UI_PANEL_COMBO: return style->window.combo_border;
        case UI_PANEL_MENU: return style->window.menu_border;
        case UI_PANEL_TOOLTIP: return style->window.menu_border;
    }} else return 0;
}

UI_INTERN struct ui_color
ui_panel_get_border_color(const struct ui_style *style, enum ui_panel_type type)
{
    switch (type) {
    default:
    case UI_PANEL_WINDOW: return style->window.border_color;
    case UI_PANEL_GROUP: return style->window.group_border_color;
    case UI_PANEL_POPUP: return style->window.popup_border_color;
    case UI_PANEL_CONTEXTUAL: return style->window.contextual_border_color;
    case UI_PANEL_COMBO: return style->window.combo_border_color;
    case UI_PANEL_MENU: return style->window.menu_border_color;
    case UI_PANEL_TOOLTIP: return style->window.menu_border_color;
    }
}

UI_INTERN int
ui_panel_is_sub(enum ui_panel_type type)
{
    return (type & UI_PANEL_SET_SUB)?1:0;
}

UI_INTERN int
ui_panel_is_nonblock(enum ui_panel_type type)
{
    return (type & UI_PANEL_SET_NONBLOCK)?1:0;
}

UI_INTERN int
ui_panel_begin(struct ui_context *ctx, const char *title, enum ui_panel_type panel_type)
{
    struct ui_input *in;
    struct ui_window *win;
    struct ui_panel *layout;
    struct ui_command_buffer *out;
    const struct ui_style *style;
    const struct ui_user_font *font;

    struct ui_vec2 scrollbar_size;
    struct ui_vec2 panel_padding;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout) return 0;
    ui_zero(ctx->current->layout, sizeof(*ctx->current->layout));
    if (ctx->current->flags & UI_WINDOW_HIDDEN)
        return 0;

    /* pull state into local stack */
    style = &ctx->style;
    font = style->font;
    in = &ctx->input;
    win = ctx->current;
    layout = win->layout;
    out = &win->buffer;
#ifdef UI_INCLUDE_COMMAND_USERDATA
    win->buffer.userdata = ctx->userdata;
#endif

    /* pull style configuration into local stack */
    scrollbar_size = style->window.scrollbar_size;
    panel_padding = ui_panel_get_padding(style, panel_type);

    /* window movement */
    if ((win->flags & UI_WINDOW_MOVABLE) && !(win->flags & UI_WINDOW_ROM)) {
        int left_mouse_down;
        int left_mouse_click_in_cursor;

        /* calculate draggable window space */
        struct ui_rect header;
        header.x = win->bounds.x;
        header.y = win->bounds.y;
        header.w = win->bounds.w;
        if (ui_panel_has_header(win->flags, title)) {
            header.h = font->height + 2.0f * style->window.header.padding.y;
            header.h += 2.0f * style->window.header.label_padding.y;
        } else header.h = panel_padding.y;

        /* window movement by dragging */
        left_mouse_down = in->mouse.buttons[UI_BUTTON_LEFT].down;
        left_mouse_click_in_cursor = ui_input_has_mouse_click_down_in_rect(in,
            UI_BUTTON_LEFT, header, ui_true);
        if (left_mouse_down && left_mouse_click_in_cursor) {
            win->bounds.x = win->bounds.x + in->mouse.delta.x;
            win->bounds.y = win->bounds.y + in->mouse.delta.y;
            in->mouse.buttons[UI_BUTTON_LEFT].clicked_pos.x += in->mouse.delta.x;
            in->mouse.buttons[UI_BUTTON_LEFT].clicked_pos.y += in->mouse.delta.y;
            ctx->style.cursor_active = ctx->style.cursors[UI_CURSOR_MOVE];
        }
    }

    /* setup panel */
    layout->type = panel_type;
    layout->flags = win->flags;
    layout->bounds = win->bounds;
    layout->bounds.x += panel_padding.x;
    layout->bounds.w -= 2*panel_padding.x;
    if (win->flags & UI_WINDOW_BORDER) {
        layout->border = ui_panel_get_border(style, win->flags, panel_type);
        layout->bounds = ui_shriui_rect(layout->bounds, layout->border);
    } else layout->border = 0;
    layout->at_y = layout->bounds.y;
    layout->at_x = layout->bounds.x;
    layout->max_x = 0;
    layout->header_height = 0;
    layout->footer_height = 0;
    layout->row.index = 0;
    layout->row.columns = 0;
    layout->row.ratio = 0;
    layout->row.item_width = 0;
    layout->row.tree_depth = 0;
    layout->row.height = panel_padding.y;
    layout->has_scrolling = ui_true;
    if (!(win->flags & UI_WINDOW_NO_SCROLLBAR))
        layout->bounds.w -= scrollbar_size.x;
    if (!ui_panel_is_nonblock(panel_type)) {
        layout->footer_height = 0;
        if (!(win->flags & UI_WINDOW_NO_SCROLLBAR) || win->flags & UI_WINDOW_SCALABLE)
            layout->footer_height = scrollbar_size.y;
        layout->bounds.h -= layout->footer_height;
    }

    /* panel header */
    if (ui_panel_has_header(win->flags, title))
    {
        struct ui_text text;
        struct ui_rect header;
        const struct ui_style_item *background = 0;

        /* calculate header bounds */
        header.x = win->bounds.x;
        header.y = win->bounds.y;
        header.w = win->bounds.w;
        header.h = font->height + 2.0f * style->window.header.padding.y;
        header.h += (2.0f * style->window.header.label_padding.y);

        /* shriui panel by header */
        layout->header_height = header.h;
        layout->bounds.y += header.h;
        layout->bounds.h -= header.h;
        layout->at_y += header.h;

        /* select correct header background and text color */
        if (ctx->active == win) {
            background = &style->window.header.active;
            text.text = style->window.header.label_active;
        } else if (ui_input_is_mouse_hovering_rect(&ctx->input, header)) {
            background = &style->window.header.hover;
            text.text = style->window.header.label_hover;
        } else {
            background = &style->window.header.normal;
            text.text = style->window.header.label_normal;
        }

        /* draw header background */
        header.h += 1.0f;
        if (background->type == UI_STYLE_ITEM_IMAGE) {
            text.background = ui_rgba(0,0,0,0);
            ui_draw_image(&win->buffer, header, &background->data.image, ui_white);
        } else {
            text.background = background->data.color;
            ui_fill_rect(out, header, 0, background->data.color);
        }

        /* window close button */
        {struct ui_rect button;
        button.y = header.y + style->window.header.padding.y;
        button.h = header.h - 2 * style->window.header.padding.y;
        button.w = button.h;
        if (win->flags & UI_WINDOW_CLOSABLE) {
            ui_flags ws = 0;
            if (style->window.header.align == UI_HEADER_RIGHT) {
                button.x = (header.w + header.x) - (button.w + style->window.header.padding.x);
                header.w -= button.w + style->window.header.spacing.x + style->window.header.padding.x;
            } else {
                button.x = header.x + style->window.header.padding.x;
                header.x += button.w + style->window.header.spacing.x + style->window.header.padding.x;
            }

            if (ui_do_button_symbol(&ws, &win->buffer, button,
                style->window.header.close_symbol, UI_BUTTON_DEFAULT,
                &style->window.header.close_button, in, style->font) && !(win->flags & UI_WINDOW_ROM))
            {
                layout->flags |= UI_WINDOW_HIDDEN;
                layout->flags |= UI_WINDOW_CLOSED;
                layout->flags &= (ui_flags)~UI_WINDOW_MINIMIZED;
            }
        }

        /* window minimize button */
        if (win->flags & UI_WINDOW_MINIMIZABLE) {
            ui_flags ws = 0;
            if (style->window.header.align == UI_HEADER_RIGHT) {
                button.x = (header.w + header.x) - button.w;
                if (!(win->flags & UI_WINDOW_CLOSABLE)) {
                    button.x -= style->window.header.padding.x;
                    header.w -= style->window.header.padding.x;
                }
                header.w -= button.w + style->window.header.spacing.x;
            } else {
                button.x = header.x;
                header.x += button.w + style->window.header.spacing.x + style->window.header.padding.x;
            }
            if (ui_do_button_symbol(&ws, &win->buffer, button, (layout->flags & UI_WINDOW_MINIMIZED)?
                style->window.header.maximize_symbol: style->window.header.minimize_symbol,
                UI_BUTTON_DEFAULT, &style->window.header.minimize_button, in, style->font) && !(win->flags & UI_WINDOW_ROM))
                layout->flags = (layout->flags & UI_WINDOW_MINIMIZED) ?
                    layout->flags & (ui_flags)~UI_WINDOW_MINIMIZED:
                    layout->flags | UI_WINDOW_MINIMIZED;
        }}

        {/* window header title */
        int text_len = ui_strlen(title);
        struct ui_rect label = {0,0,0,0};
        float t = font->width(font->userdata, font->height, title, text_len);
        text.padding = ui_vec2(0,0);

        label.x = header.x + style->window.header.padding.x;
        label.x += style->window.header.label_padding.x;
        label.y = header.y + style->window.header.label_padding.y;
        label.h = font->height + 2 * style->window.header.label_padding.y;
        label.w = t + 2 * style->window.header.spacing.x;
        ui_widget_text(out, label,(const char*)title, text_len, &text, UI_TEXT_LEFT, font);}
    }

    /* draw window background */
    if (!(layout->flags & UI_WINDOW_MINIMIZED) && !(layout->flags & UI_WINDOW_DYNAMIC)) {
        struct ui_rect body;
        body.x = win->bounds.x;
        body.w = win->bounds.w;
        body.y = (win->bounds.y + layout->header_height);
        body.h = (win->bounds.h - layout->header_height);
        if (style->window.fixed_background.type == UI_STYLE_ITEM_IMAGE)
            ui_draw_image(out, body, &style->window.fixed_background.data.image, ui_white);
        else ui_fill_rect(out, body, 0, style->window.fixed_background.data.color);
    }

    /* set clipping rectangle */
    {struct ui_rect clip;
    layout->clip = layout->bounds;
    ui_unify(&clip, &win->buffer.clip, layout->clip.x, layout->clip.y,
        layout->clip.x + layout->clip.w, layout->clip.y + layout->clip.h);
    ui_push_scissor(out, clip);
    layout->clip = clip;}
    return !(layout->flags & UI_WINDOW_HIDDEN) && !(layout->flags & UI_WINDOW_MINIMIZED);
}

UI_INTERN void
ui_panel_end(struct ui_context *ctx)
{
    struct ui_input *in;
    struct ui_window *window;
    struct ui_panel *layout;
    const struct ui_style *style;
    struct ui_command_buffer *out;

    struct ui_vec2 scrollbar_size;
    struct ui_vec2 panel_padding;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return;

    window = ctx->current;
    layout = window->layout;
    style = &ctx->style;
    out = &window->buffer;
    in = (layout->flags & UI_WINDOW_ROM) ? 0 :&ctx->input;
    if (!ui_panel_is_sub(layout->type))
        ui_push_scissor(out, ui_null_rect);

    /* cache configuration data */
    scrollbar_size = style->window.scrollbar_size;
    panel_padding = ui_panel_get_padding(style, layout->type);

    /* update the current cursor Y-position to point over the last added widget */
    layout->at_y += layout->row.height;

    /* dynamic panels */
    if (layout->flags & UI_WINDOW_DYNAMIC && !(layout->flags & UI_WINDOW_MINIMIZED))
    {
        /* update panel height to fit dynamic growth */
        struct ui_rect empty_space;
        if (layout->at_y < (layout->bounds.y + layout->bounds.h))
            layout->bounds.h = layout->at_y - layout->bounds.y;

        /* fill top empty space */
        empty_space.x = window->bounds.x;
        empty_space.y = layout->bounds.y;
        empty_space.h = panel_padding.y;
        empty_space.w = window->bounds.w;
        ui_fill_rect(out, empty_space, 0, style->window.background);

        /* fill left empty space */
        empty_space.x = window->bounds.x;
        empty_space.y = layout->bounds.y;
        empty_space.w = panel_padding.x + layout->border;
        empty_space.h = layout->bounds.h;
        ui_fill_rect(out, empty_space, 0, style->window.background);

        /* fill right empty space */
        empty_space.x = layout->bounds.x + layout->bounds.w - layout->border;
        empty_space.y = layout->bounds.y;
        empty_space.w = panel_padding.x + layout->border;
        empty_space.h = layout->bounds.h;
        if (*layout->offset_y == 0 && !(layout->flags & UI_WINDOW_NO_SCROLLBAR))
            empty_space.w += scrollbar_size.x;
        ui_fill_rect(out, empty_space, 0, style->window.background);

        /* fill bottom empty space */
        if (*layout->offset_x != 0 && !(layout->flags & UI_WINDOW_NO_SCROLLBAR)) {
            empty_space.x = window->bounds.x;
            empty_space.y = layout->bounds.y + layout->bounds.h;
            empty_space.w = window->bounds.w;
            empty_space.h = scrollbar_size.y;
            ui_fill_rect(out, empty_space, 0, style->window.background);
        }
    }

    /* scrollbars */
    if (!(layout->flags & UI_WINDOW_NO_SCROLLBAR) &&
        !(layout->flags & UI_WINDOW_MINIMIZED) &&
        window->scrollbar_hiding_timer < UI_SCROLLBAR_HIDING_TIMEOUT)
    {
        struct ui_rect scroll;
        int scroll_has_scrolling;
        float scroll_target;
        float scroll_offset;
        float scroll_step;
        float scroll_inc;
        {
            /* vertical scrollbar */
            ui_flags state = 0;
            scroll.x = layout->bounds.x + layout->bounds.w + panel_padding.x;
            scroll.y = layout->bounds.y;
            scroll.w = scrollbar_size.x;
            scroll.h = layout->bounds.h;

            scroll_offset = (float)*layout->offset_y;
            scroll_step = scroll.h * 0.10f;
            scroll_inc = scroll.h * 0.01f;
            scroll_target = (float)(int)(layout->at_y - scroll.y);

            /* scrolling by mouse wheel */
            if (ui_panel_is_sub(layout->type))
            {
                /* sub-window scrollbar wheel scrolling */
                struct ui_window *root_window = window;
                struct ui_panel *root_panel = window->layout;
                while (root_panel->parent)
                    root_panel = root_panel->parent;
                while (root_window->parent)
                    root_window = root_window->parent;

                /* only allow scrolling if parent window is active */
                scroll_has_scrolling = 0;
                if ((root_window == ctx->active) && layout->has_scrolling) {
                    /* and panel is being hovered and inside clip rect*/
                    if (ui_input_is_mouse_hovering_rect(in, layout->bounds) &&
                        UI_INTERSECT(layout->bounds.x, layout->bounds.y, layout->bounds.w, layout->bounds.h,
                            root_panel->clip.x, root_panel->clip.y, root_panel->clip.w, root_panel->clip.h))
                    {
                        /* deactivate all parent scrolling */
                        root_panel = window->layout;
                        while (root_panel->parent) {
                            root_panel->has_scrolling = ui_false;
                            root_panel = root_panel->parent;
                        }
                        root_panel->has_scrolling = ui_false;
                        scroll_has_scrolling = ui_true;
                    }
                }
            } else if (!ui_panel_is_sub(layout->type)) {
                /* window scrollbar wheel scrolling */
                scroll_has_scrolling = (window == ctx->active) && layout->has_scrolling;
                if (in && in->mouse.scroll_delta > 0 && scroll_has_scrolling)
                    window->scrolled = ui_true;
                else window->scrolled = ui_false;
            } else scroll_has_scrolling = ui_false;

            /* execute scrollbar */
            scroll_offset = ui_do_scrollbarv(&state, out, scroll, scroll_has_scrolling,
                                scroll_offset, scroll_target, scroll_step, scroll_inc,
                                &ctx->style.scrollv, in, style->font);
            *layout->offset_y = (ui_uint)scroll_offset;
            if (in && scroll_has_scrolling)
                in->mouse.scroll_delta = 0;
        }
        {
            /* horizontal scrollbar */
            ui_flags state = 0;
            scroll.x = layout->bounds.x;
            scroll.y = layout->bounds.y + layout->bounds.h;
            scroll.w = layout->bounds.w;
            scroll.h = scrollbar_size.y;

            scroll_offset = (float)*layout->offset_x;
            scroll_target = (float)(int)(layout->max_x - scroll.x);
            scroll_step = layout->max_x * 0.05f;
            scroll_inc = layout->max_x * 0.005f;
            scroll_has_scrolling = ui_false;
            scroll_offset = ui_do_scrollbarh(&state, out, scroll, scroll_has_scrolling,
                                scroll_offset, scroll_target, scroll_step, scroll_inc,
                                &ctx->style.scrollh, in, style->font);
            *layout->offset_x = (ui_uint)scroll_offset;
        }
    }

    /* hide scroll if no user input */
    if (window->flags & UI_WINDOW_SCROLL_AUTO_HIDE) {
        int has_input = ctx->input.mouse.delta.x != 0 || ctx->input.mouse.delta.y != 0 || ctx->input.mouse.scroll_delta != 0;
        int is_window_hovered = ui_window_is_hovered(ctx);
        int any_item_active = (ctx->last_widget_state & UI_WIDGET_STATE_MODIFIED);
        if ((!has_input && is_window_hovered) || (!is_window_hovered && !any_item_active))
            window->scrollbar_hiding_timer += ctx->delta_time_seconds;
        else window->scrollbar_hiding_timer = 0;
    } else window->scrollbar_hiding_timer = 0;

    /* window border */
    if (layout->flags & UI_WINDOW_BORDER)
    {
        struct ui_color border_color = ui_panel_get_border_color(style, layout->type);
        const float padding_y = (layout->flags & UI_WINDOW_MINIMIZED) ?
            style->window.border + window->bounds.y + layout->header_height:
            (layout->flags & UI_WINDOW_DYNAMIC)?
            layout->bounds.y + layout->bounds.h + layout->footer_height:
            window->bounds.y + window->bounds.h;

        /* draw border top */
        ui_stroke_line(out,window->bounds.x,window->bounds.y,
            window->bounds.x + window->bounds.w, window->bounds.y,
            layout->border, border_color);

        /* draw bottom border */
        ui_stroke_line(out, window->bounds.x, padding_y,
            window->bounds.x + window->bounds.w, padding_y, layout->border, border_color);

        /* draw left border */
        ui_stroke_line(out, window->bounds.x + layout->border*0.5f,
            window->bounds.y, window->bounds.x + layout->border*0.5f,
            padding_y, layout->border, border_color);

        /* draw right border */
        ui_stroke_line(out, window->bounds.x + window->bounds.w - layout->border*0.5f,
            window->bounds.y, window->bounds.x + window->bounds.w - layout->border*0.5f,
            padding_y, layout->border, border_color);
    }

    /* scaler */
    if ((layout->flags & UI_WINDOW_SCALABLE) && in && !(layout->flags & UI_WINDOW_MINIMIZED))
    {
        /* calculate scaler bounds */
        struct ui_rect scaler;
        scaler.w = scrollbar_size.x;
        scaler.h = scrollbar_size.y;
        scaler.y = layout->bounds.y + layout->bounds.h;
        if (layout->flags & UI_WINDOW_SCALE_LEFT)
            scaler.x = layout->bounds.x - panel_padding.x * 0.5f;
        else scaler.x = layout->bounds.x + layout->bounds.w + panel_padding.x;
        if (layout->flags & UI_WINDOW_NO_SCROLLBAR)
            scaler.x -= scaler.w;

        /* draw scaler */
        {const struct ui_style_item *item = &style->window.scaler;
        if (item->type == UI_STYLE_ITEM_IMAGE)
            ui_draw_image(out, scaler, &item->data.image, ui_white);
        else {
            if (layout->flags & UI_WINDOW_SCALE_LEFT) {
                ui_fill_triangle(out, scaler.x, scaler.y, scaler.x,
                    scaler.y + scaler.h, scaler.x + scaler.w,
                    scaler.y + scaler.h, item->data.color);
            } else {
                ui_fill_triangle(out, scaler.x + scaler.w, scaler.y, scaler.x + scaler.w,
                    scaler.y + scaler.h, scaler.x, scaler.y + scaler.h, item->data.color);
            }
        }}

        /* do window scaling */
        if (!(window->flags & UI_WINDOW_ROM)) {
            struct ui_vec2 window_size = style->window.min_size;
            int left_mouse_down = in->mouse.buttons[UI_BUTTON_LEFT].down;
            int left_mouse_click_in_scaler = ui_input_has_mouse_click_down_in_rect(in,
                    UI_BUTTON_LEFT, scaler, ui_true);

            if (ui_input_is_mouse_down(in, UI_BUTTON_LEFT) && left_mouse_down && left_mouse_click_in_scaler) {
                float delta_x = in->mouse.delta.x;
                if (layout->flags & UI_WINDOW_SCALE_LEFT) {
                    delta_x =- delta_x;
                    window->bounds.x += in->mouse.delta.x;
                }
                window->bounds.w = UI_MAX(window_size.x, window->bounds.w + delta_x);

                /* dragging in y-direction is only possible if static window */
                if (!(layout->flags & UI_WINDOW_DYNAMIC))
                    window->bounds.h = UI_MAX(window_size.y, window->bounds.h + in->mouse.delta.y);
                ctx->style.cursor_active = ctx->style.cursors[UI_CURSOR_RESIZE_TOP_RIGHT_DOWN_LEFT];
                in->mouse.buttons[UI_BUTTON_LEFT].clicked_pos.x = scaler.x + in->mouse.delta.x + scaler.w/2.0f;
                in->mouse.buttons[UI_BUTTON_LEFT].clicked_pos.y = scaler.y + in->mouse.delta.y + scaler.h/2.0f;
            }
        }
    }

    if (!ui_panel_is_sub(layout->type)) {
        /* window is hidden so clear command buffer  */
        if (layout->flags & UI_WINDOW_HIDDEN)
            ui_command_buffer_reset(&window->buffer);
        /* window is visible and not tab */
        else ui_finish(ctx, window);
    }

    /* UI_WINDOW_REMOVE_ROM flag was set so remove UI_WINDOW_ROM */
    if (layout->flags & UI_WINDOW_REMOVE_ROM) {
        layout->flags &= ~(ui_flags)UI_WINDOW_ROM;
        layout->flags &= ~(ui_flags)UI_WINDOW_REMOVE_ROM;
    }
    window->flags = layout->flags;

    /* property garbage collector */
    if (window->property.active && window->property.old != window->property.seq &&
        window->property.active == window->property.prev) {
        ui_zero(&window->property, sizeof(window->property));
    } else {
        window->property.old = window->property.seq;
        window->property.prev = window->property.active;
        window->property.seq = 0;
    }

    /* edit garbage collector */
    if (window->edit.active && window->edit.old != window->edit.seq &&
       window->edit.active == window->edit.prev) {
        ui_zero(&window->edit, sizeof(window->edit));
    } else {
        window->edit.old = window->edit.seq;
        window->edit.prev = window->edit.active;
        window->edit.seq = 0;
    }

    /* contextual garbage collector */
    if (window->popup.active_con && window->popup.con_old != window->popup.con_count) {
        window->popup.con_count = 0;
        window->popup.con_old = 0;
        window->popup.active_con = 0;
    } else {
        window->popup.con_old = window->popup.con_count;
        window->popup.con_count = 0;
    }
    window->popup.combo_count = 0;

    /* helper to make sure you have a 'ui_tree_push' * for every 'ui_tree_pop' */
    UI_ASSERT(!layout->row.tree_depth);
}

/* ----------------------------------------------------------------
 *
 *                          PAGE ELEMENT
 *
 * ---------------------------------------------------------------*/
UI_INTERN struct ui_page_element*
ui_create_page_element(struct ui_context *ctx)
{
    struct ui_page_element *elem;
    if (ctx->freelist) {
        /* unliui page element from free list */
        elem = ctx->freelist;
        ctx->freelist = elem->next;
    } else if (ctx->use_pool) {
        /* allocate page element from memory pool */
        elem = ui_pool_alloc(&ctx->pool);
        UI_ASSERT(elem);
        if (!elem) return 0;
    } else {
        /* allocate new page element from the back of the fixed size memory buffer */
        UI_STORAGE const ui_size size = sizeof(struct ui_page_element);
        UI_STORAGE const ui_size align = UI_ALIGNOF(struct ui_page_element);
        elem = (struct ui_page_element*)ui_buffer_alloc(&ctx->memory, UI_BUFFER_BACK, size, align);
        UI_ASSERT(elem);
        if (!elem) return 0;
    }
    ui_zero_struct(*elem);
    elem->next = 0;
    elem->prev = 0;
    return elem;
}

UI_INTERN void
ui_liui_page_element_into_freelist(struct ui_context *ctx,
    struct ui_page_element *elem)
{
    /* liui table into freelist */
    if (!ctx->freelist) {
        ctx->freelist = elem;
    } else {
        elem->next = ctx->freelist;
        ctx->freelist = elem;
    }
}

UI_INTERN void
ui_free_page_element(struct ui_context *ctx, struct ui_page_element *elem)
{
    /* we have a pool so just add to free list */
    if (ctx->use_pool) {
        ui_liui_page_element_into_freelist(ctx, elem);
        return;
    }

    /* if possible remove last element from back of fixed memory buffer */
    {void *elem_end = (void*)(elem + 1);
    void *buffer_end = (ui_byte*)ctx->memory.memory.ptr + ctx->memory.size;
    if (elem_end == buffer_end)
        ctx->memory.size -= sizeof(struct ui_page_element);
    else ui_liui_page_element_into_freelist(ctx, elem);}
}

/* ----------------------------------------------------------------
 *
 *                          PANEL
 *
 * ---------------------------------------------------------------*/
UI_INTERN void*
ui_create_panel(struct ui_context *ctx)
{
    struct ui_page_element *elem;
    elem = ui_create_page_element(ctx);
    if (!elem) return 0;
    ui_zero_struct(*elem);
    return &elem->data.pan;
}

UI_INTERN void
ui_free_panel(struct ui_context *ctx, struct ui_panel *pan)
{
    union ui_page_data *pd = UI_CONTAINER_OF(pan, union ui_page_data, pan);
    struct ui_page_element *pe = UI_CONTAINER_OF(pd, struct ui_page_element, data);
    ui_free_page_element(ctx, pe);
}

/* ----------------------------------------------------------------
 *
 *                          TABLES
 *
 * ---------------------------------------------------------------*/
UI_INTERN struct ui_table*
ui_create_table(struct ui_context *ctx)
{
    struct ui_page_element *elem;
    elem = ui_create_page_element(ctx);
    if (!elem) return 0;
    ui_zero_struct(*elem);
    return &elem->data.tbl;
}

UI_INTERN void
ui_free_table(struct ui_context *ctx, struct ui_table *tbl)
{
    union ui_page_data *pd = UI_CONTAINER_OF(tbl, union ui_page_data, tbl);
    struct ui_page_element *pe = UI_CONTAINER_OF(pd, struct ui_page_element, data);
    ui_free_page_element(ctx, pe);
}

UI_INTERN void
ui_push_table(struct ui_window *win, struct ui_table *tbl)
{
    if (!win->tables) {
        win->tables = tbl;
        tbl->next = 0;
        tbl->prev = 0;
        win->table_count = 1;
        win->table_size = 0;
        return;
    }
    win->tables->prev = tbl;
    tbl->next = win->tables;
    tbl->prev = 0;
    win->tables = tbl;
    win->table_count++;
    win->table_size = 0;
}

UI_INTERN void
ui_remove_table(struct ui_window *win, struct ui_table *tbl)
{
    if (win->tables == tbl)
        win->tables = tbl->next;
    if (tbl->next)
        tbl->next->prev = tbl->prev;
    if (tbl->prev)
        tbl->prev->next = tbl->next;
    tbl->next = 0;
    tbl->prev = 0;
}

UI_INTERN ui_uint*
ui_add_value(struct ui_context *ctx, struct ui_window *win,
            ui_hash name, ui_uint value)
{
    UI_ASSERT(ctx);
    UI_ASSERT(win);
    if (!win || !ctx) return 0;
    if (!win->tables || win->table_size >= UI_VALUE_PAGE_CAPACITY) {
        struct ui_table *tbl = ui_create_table(ctx);
        UI_ASSERT(tbl);
        if (!tbl) return 0;
        ui_push_table(win, tbl);
    }
    win->tables->seq = win->seq;
    win->tables->keys[win->table_size] = name;
    win->tables->values[win->table_size] = value;
    return &win->tables->values[win->table_size++];
}

UI_INTERN ui_uint*
ui_find_value(struct ui_window *win, ui_hash name)
{
    ui_ushort size = win->table_size;
    struct ui_table *iter = win->tables;
    while (iter) {
        ui_ushort i = 0;
        for (i = 0; i < size; ++i) {
            if (iter->keys[i] == name) {
                iter->seq = win->seq;
                return &iter->values[i];
            }
        }
        size = UI_VALUE_PAGE_CAPACITY;
        iter = iter->next;
    }
    return 0;
}

/* ----------------------------------------------------------------
 *
 *                          WINDOW
 *
 * ---------------------------------------------------------------*/
UI_INTERN void*
ui_create_window(struct ui_context *ctx)
{
    struct ui_page_element *elem;
    elem = ui_create_page_element(ctx);
    if (!elem) return 0;
    elem->data.win.seq = ctx->seq;
    return &elem->data.win;
}

UI_INTERN void
ui_free_window(struct ui_context *ctx, struct ui_window *win)
{
    /* unliui windows from list */
    struct ui_table *n, *it = win->tables;
    if (win->popup.win) {
        ui_free_window(ctx, win->popup.win);
        win->popup.win = 0;
    }
    win->next = 0;
    win->prev = 0;

    while (it) {
        /*free window state tables */
        n = it->next;
        ui_remove_table(win, it);
        ui_free_table(ctx, it);
        if (it == win->tables)
            win->tables = n;
        it = n;
    }

    /* liui windows into freelist */
    {union ui_page_data *pd = UI_CONTAINER_OF(win, union ui_page_data, win);
    struct ui_page_element *pe = UI_CONTAINER_OF(pd, struct ui_page_element, data);
    ui_free_page_element(ctx, pe);}
}

UI_INTERN struct ui_window*
ui_find_window(struct ui_context *ctx, ui_hash hash, const char *name)
{
    struct ui_window *iter;
    iter = ctx->begin;
    while (iter) {
        UI_ASSERT(iter != iter->next);
        if (iter->name == hash) {
            int max_len = ui_strlen(iter->name_string);
            if (!ui_stricmpn(iter->name_string, name, max_len))
                return iter;
        }
        iter = iter->next;
    }
    return 0;
}

enum ui_window_insert_location {
    UI_INSERT_BACK, /* inserts window into the back of list (front of screen) */
    UI_INSERT_FRONT /* inserts window into the front of list (back of screen) */
};
UI_INTERN void
ui_insert_window(struct ui_context *ctx, struct ui_window *win,
    enum ui_window_insert_location loc)
{
    const struct ui_window *iter;
    UI_ASSERT(ctx);
    UI_ASSERT(win);
    if (!win || !ctx) return;

    iter = ctx->begin;
    while (iter) {
        UI_ASSERT(iter != iter->next);
        UI_ASSERT(iter != win);
        if (iter == win) return;
        iter = iter->next;
    }

    if (!ctx->begin) {
        win->next = 0;
        win->prev = 0;
        ctx->begin = win;
        ctx->end = win;
        ctx->count = 1;
        return;
    }
    if (loc == UI_INSERT_BACK) {
        struct ui_window *end;
        end = ctx->end;
        end->flags |= UI_WINDOW_ROM;
        end->next = win;
        win->prev = ctx->end;
        win->next = 0;
        ctx->end = win;
        ctx->active = ctx->end;
        ctx->end->flags &= ~(ui_flags)UI_WINDOW_ROM;
    } else {
        ctx->end->flags |= UI_WINDOW_ROM;
        ctx->begin->prev = win;
        win->next = ctx->begin;
        win->prev = 0;
        ctx->begin = win;
        ctx->begin->flags &= ~(ui_flags)UI_WINDOW_ROM;
    }
    ctx->count++;
}

UI_INTERN void
ui_remove_window(struct ui_context *ctx, struct ui_window *win)
{
    if (win == ctx->begin || win == ctx->end) {
        if (win == ctx->begin) {
            ctx->begin = win->next;
            if (win->next)
                win->next->prev = 0;
        }
        if (win == ctx->end) {
            ctx->end = win->prev;
            if (win->prev)
                win->prev->next = 0;
        }
    } else {
        if (win->next)
            win->next->prev = win->prev;
        if (win->prev)
            win->prev->next = win->next;
    }
    if (win == ctx->active || !ctx->active) {
        ctx->active = ctx->end;
        if (ctx->end)
            ctx->end->flags &= ~(ui_flags)UI_WINDOW_ROM;
    }
    win->next = 0;
    win->prev = 0;
    ctx->count--;
}

UI_API int
ui_begin(struct ui_context *ctx, const char *title,
    struct ui_rect bounds, ui_flags flags)
{
    return ui_begin_titled(ctx, title, title, bounds, flags);
}

UI_API int
ui_begin_titled(struct ui_context *ctx, const char *name, const char *title,
    struct ui_rect bounds, ui_flags flags)
{
    struct ui_window *win;
    struct ui_style *style;
    ui_hash title_hash;
    int title_len;
    int ret = 0;

    UI_ASSERT(ctx);
    UI_ASSERT(name);
    UI_ASSERT(title);
    UI_ASSERT(ctx->style.font && ctx->style.font->width && "if this triggers you forgot to add a font");
    UI_ASSERT(!ctx->current && "if this triggers you missed a `ui_end` call");
    if (!ctx || ctx->current || !title || !name)
        return 0;

    /* find or create window */
    style = &ctx->style;
    title_len = (int)ui_strlen(name);
    title_hash = ui_murmur_hash(name, (int)title_len, UI_WINDOW_TITLE);
    win = ui_find_window(ctx, title_hash, name);
    if (!win) {
        /* create new window */
        ui_size name_length = (ui_size)ui_strlen(name);
        win = (struct ui_window*)ui_create_window(ctx);
        UI_ASSERT(win);
        if (!win) return 0;

        if (flags & UI_WINDOW_BACKGROUND)
            ui_insert_window(ctx, win, UI_INSERT_FRONT);
        else ui_insert_window(ctx, win, UI_INSERT_BACK);
        ui_command_buffer_init(&win->buffer, &ctx->memory, UI_CLIPPING_ON);

        win->flags = flags;
        win->bounds = bounds;
        win->name = title_hash;
        name_length = UI_MIN(name_length, UI_WINDOW_MAX_NAME-1);
        UI_MEMCPY(win->name_string, name, name_length);
        win->name_string[name_length] = 0;
        win->popup.win = 0;
        if (!ctx->active)
            ctx->active = win;
    } else {
        /* update window */
        win->flags &= ~(ui_flags)(UI_WINDOW_PRIVATE-1);
        win->flags |= flags;
        if (!(win->flags & (UI_WINDOW_MOVABLE | UI_WINDOW_SCALABLE)))
            win->bounds = bounds;
        /* If this assert triggers you either:
         *
         * I.) Have more than one window with the same name or
         * II.) You forgot to actually draw the window.
         *      More specific you did not call `ui_clear` (ui_clear will be
         *      automatically called for you if you are using one of the
         *      provided demo backends). */
        UI_ASSERT(win->seq != ctx->seq);
        win->seq = ctx->seq;
        if (!ctx->active && !(win->flags & UI_WINDOW_HIDDEN))
            ctx->active = win;
    }
    if (win->flags & UI_WINDOW_HIDDEN) {
        ctx->current = win;
        return 0;
    }

    /* window overlapping */
    if (!(win->flags & UI_WINDOW_HIDDEN))
    {
        int inpanel, ishovered;
        const struct ui_window *iter = win;
        float h = ctx->style.font->height + 2.0f * style->window.header.padding.y +
            (2.0f * style->window.header.label_padding.y);
        struct ui_rect win_bounds = (!(win->flags & UI_WINDOW_MINIMIZED))?
            win->bounds: ui_rect(win->bounds.x, win->bounds.y, win->bounds.w, h);

        /* activate window if hovered and no other window is overlapping this window */
        ui_start(ctx, win);
        inpanel = ui_input_has_mouse_click_down_in_rect(&ctx->input, UI_BUTTON_LEFT, win_bounds, ui_true);
        inpanel = inpanel && ctx->input.mouse.buttons[UI_BUTTON_LEFT].clicked;
        ishovered = ui_input_is_mouse_hovering_rect(&ctx->input, win_bounds);
        if ((win != ctx->active) && ishovered && !ctx->input.mouse.buttons[UI_BUTTON_LEFT].down) {
            iter = win->next;
            while (iter) {
                struct ui_rect iter_bounds = (!(iter->flags & UI_WINDOW_MINIMIZED))?
                    iter->bounds: ui_rect(iter->bounds.x, iter->bounds.y, iter->bounds.w, h);
                if (UI_INTERSECT(win_bounds.x, win_bounds.y, win_bounds.w, win_bounds.h,
                    iter_bounds.x, iter_bounds.y, iter_bounds.w, iter_bounds.h) &&
                    (!(iter->flags & UI_WINDOW_HIDDEN) || !(iter->flags & UI_WINDOW_BACKGROUND)))
                    break;

                if (iter->popup.win && iter->popup.active && !(iter->flags & UI_WINDOW_HIDDEN) &&
                    UI_INTERSECT(win->bounds.x, win_bounds.y, win_bounds.w, win_bounds.h,
                    iter->popup.win->bounds.x, iter->popup.win->bounds.y,
                    iter->popup.win->bounds.w, iter->popup.win->bounds.h))
                    break;
                iter = iter->next;
            }
        }

        /* activate window if clicked */
        if (iter && inpanel && (win != ctx->end) && !(iter->flags & UI_WINDOW_BACKGROUND)) {
            iter = win->next;
            while (iter) {
                /* try to find a panel with higher priority in the same position */
                struct ui_rect iter_bounds = (!(iter->flags & UI_WINDOW_MINIMIZED))?
                iter->bounds: ui_rect(iter->bounds.x, iter->bounds.y, iter->bounds.w, h);
                if (UI_INBOX(ctx->input.mouse.pos.x, ctx->input.mouse.pos.y,
                    iter_bounds.x, iter_bounds.y, iter_bounds.w, iter_bounds.h) &&
                    !(iter->flags & UI_WINDOW_HIDDEN))
                    break;
                if (iter->popup.win && iter->popup.active && !(iter->flags & UI_WINDOW_HIDDEN) &&
                    UI_INTERSECT(win_bounds.x, win_bounds.y, win_bounds.w, win_bounds.h,
                    iter->popup.win->bounds.x, iter->popup.win->bounds.y,
                    iter->popup.win->bounds.w, iter->popup.win->bounds.h))
                    break;
                iter = iter->next;
            }
        }

        if (!iter && ctx->end != win) {
            if (!(win->flags & UI_WINDOW_BACKGROUND)) {
                /* current window is active in that position so transfer to top
                 * at the highest priority in stack */
                ui_remove_window(ctx, win);
                ui_insert_window(ctx, win, UI_INSERT_BACK);
            }
            win->flags &= ~(ui_flags)UI_WINDOW_ROM;
            ctx->active = win;
        }
        if (ctx->end != win && !(win->flags & UI_WINDOW_BACKGROUND))
            win->flags |= UI_WINDOW_ROM;
    }

    win->layout = (struct ui_panel*)ui_create_panel(ctx);
    ctx->current = win;
    ret = ui_panel_begin(ctx, title, UI_PANEL_WINDOW);
    win->layout->offset_x = &win->scrollbar.x;
    win->layout->offset_y = &win->scrollbar.y;
    return ret;
}

UI_API void
ui_end(struct ui_context *ctx)
{
    struct ui_panel *layout;
    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current && "if this triggers you forgot to call `ui_begin`");
    UI_ASSERT(ctx->current->layout);

    layout = ctx->current->layout;
    if (!ctx || !ctx->current) return;
    if (layout->type == UI_PANEL_WINDOW && (ctx->current->flags & UI_WINDOW_HIDDEN)) {
        ctx->current = 0;
        return;
    }
    ui_panel_end(ctx);
    ui_free_panel(ctx, ctx->current->layout);
    ctx->current = 0;
}

UI_API struct ui_rect
ui_window_get_bounds(const struct ui_context *ctx)
{
    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    if (!ctx || !ctx->current) return ui_rect(0,0,0,0);
    return ctx->current->bounds;
}

UI_API struct ui_vec2
ui_window_get_position(const struct ui_context *ctx)
{
    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    if (!ctx || !ctx->current) return ui_vec2(0,0);
    return ui_vec2(ctx->current->bounds.x, ctx->current->bounds.y);
}

UI_API struct ui_vec2
ui_window_get_size(const struct ui_context *ctx)
{
    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    if (!ctx || !ctx->current) return ui_vec2(0,0);
    return ui_vec2(ctx->current->bounds.w, ctx->current->bounds.h);
}

UI_API float
ui_window_get_width(const struct ui_context *ctx)
{
    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    if (!ctx || !ctx->current) return 0;
    return ctx->current->bounds.w;
}

UI_API float
ui_window_get_height(const struct ui_context *ctx)
{
    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    if (!ctx || !ctx->current) return 0;
    return ctx->current->bounds.h;
}

UI_API struct ui_rect
ui_window_get_content_region(struct ui_context *ctx)
{
    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    if (!ctx || !ctx->current) return ui_rect(0,0,0,0);
    return ctx->current->layout->clip;
}

UI_API struct ui_vec2
ui_window_get_content_region_min(struct ui_context *ctx)
{
    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current) return ui_vec2(0,0);
    return ui_vec2(ctx->current->layout->clip.x, ctx->current->layout->clip.y);
}

UI_API struct ui_vec2
ui_window_get_content_region_max(struct ui_context *ctx)
{
    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current) return ui_vec2(0,0);
    return ui_vec2(ctx->current->layout->clip.x + ctx->current->layout->clip.w,
        ctx->current->layout->clip.y + ctx->current->layout->clip.h);
}

UI_API struct ui_vec2
ui_window_get_content_region_size(struct ui_context *ctx)
{
    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current) return ui_vec2(0,0);
    return ui_vec2(ctx->current->layout->clip.w, ctx->current->layout->clip.h);
}

UI_API struct ui_command_buffer*
ui_window_get_canvas(struct ui_context *ctx)
{
    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current) return 0;
    return &ctx->current->buffer;
}

UI_API struct ui_panel*
ui_window_get_panel(struct ui_context *ctx)
{
    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    if (!ctx || !ctx->current) return 0;
    return ctx->current->layout;
}

UI_API int
ui_window_has_focus(const struct ui_context *ctx)
{
    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current) return 0;
    return ctx->current == ctx->active;
}

UI_API int
ui_window_is_hovered(struct ui_context *ctx)
{
    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    if (!ctx || !ctx->current) return 0;
    return ui_input_is_mouse_hovering_rect(&ctx->input, ctx->current->bounds);
}

UI_API int
ui_window_is_any_hovered(struct ui_context *ctx)
{
    struct ui_window *iter;
    UI_ASSERT(ctx);
    if (!ctx) return 0;
    iter = ctx->begin;
    while (iter) {
        /* check if window is being hovered */
        if (iter->flags & UI_WINDOW_MINIMIZED) {
            struct ui_rect header = iter->bounds;
            header.h = ctx->style.font->height + 2 * ctx->style.window.header.padding.y;
            if (ui_input_is_mouse_hovering_rect(&ctx->input, header))
                return 1;
        } else if (ui_input_is_mouse_hovering_rect(&ctx->input, iter->bounds)) {
            return 1;
        }
        /* check if window popup is being hovered */
        if (iter->popup.active && iter->popup.win && ui_input_is_mouse_hovering_rect(&ctx->input, iter->popup.win->bounds))
            return 1;
        iter = iter->next;
    }
    return 0;
}

UI_API int
ui_item_is_any_active(struct ui_context *ctx)
{
    int any_hovered = ui_window_is_any_hovered(ctx);
    int any_active = (ctx->last_widget_state & UI_WIDGET_STATE_MODIFIED);
    return any_hovered || any_active;
}

UI_API int
ui_window_is_collapsed(struct ui_context *ctx, const char *name)
{
    int title_len;
    ui_hash title_hash;
    struct ui_window *win;
    UI_ASSERT(ctx);
    if (!ctx) return 0;

    title_len = (int)ui_strlen(name);
    title_hash = ui_murmur_hash(name, (int)title_len, UI_WINDOW_TITLE);
    win = ui_find_window(ctx, title_hash, name);
    if (!win) return 0;
    return win->flags & UI_WINDOW_MINIMIZED;
}

UI_API int
ui_window_is_closed(struct ui_context *ctx, const char *name)
{
    int title_len;
    ui_hash title_hash;
    struct ui_window *win;
    UI_ASSERT(ctx);
    if (!ctx) return 1;

    title_len = (int)ui_strlen(name);
    title_hash = ui_murmur_hash(name, (int)title_len, UI_WINDOW_TITLE);
    win = ui_find_window(ctx, title_hash, name);
    if (!win) return 1;
    return (win->flags & UI_WINDOW_CLOSED);
}

UI_API int
ui_window_is_hidden(struct ui_context *ctx, const char *name)
{
    int title_len;
    ui_hash title_hash;
    struct ui_window *win;
    UI_ASSERT(ctx);
    if (!ctx) return 1;

    title_len = (int)ui_strlen(name);
    title_hash = ui_murmur_hash(name, (int)title_len, UI_WINDOW_TITLE);
    win = ui_find_window(ctx, title_hash, name);
    if (!win) return 1;
    return (win->flags & UI_WINDOW_HIDDEN);
}

UI_API int
ui_window_is_active(struct ui_context *ctx, const char *name)
{
    int title_len;
    ui_hash title_hash;
    struct ui_window *win;
    UI_ASSERT(ctx);
    if (!ctx) return 0;

    title_len = (int)ui_strlen(name);
    title_hash = ui_murmur_hash(name, (int)title_len, UI_WINDOW_TITLE);
    win = ui_find_window(ctx, title_hash, name);
    if (!win) return 0;
    return win == ctx->active;
}

UI_API struct ui_window*
ui_window_find(struct ui_context *ctx, const char *name)
{
    int title_len;
    ui_hash title_hash;
    title_len = (int)ui_strlen(name);
    title_hash = ui_murmur_hash(name, (int)title_len, UI_WINDOW_TITLE);
    return ui_find_window(ctx, title_hash, name);
}

UI_API void
ui_window_close(struct ui_context *ctx, const char *name)
{
    struct ui_window *win;
    UI_ASSERT(ctx);
    if (!ctx) return;
    win = ui_window_find(ctx, name);
    if (!win) return;
    UI_ASSERT(ctx->current != win && "You cannot close a currently active window");
    if (ctx->current == win) return;
    win->flags |= UI_WINDOW_HIDDEN;
    win->flags |= UI_WINDOW_CLOSED;
}

UI_API void
ui_window_set_bounds(struct ui_context *ctx, struct ui_rect bounds)
{
    UI_ASSERT(ctx); UI_ASSERT(ctx->current);
    if (!ctx || !ctx->current) return;
    ctx->current->bounds = bounds;
}

UI_API void
ui_window_set_position(struct ui_context *ctx, struct ui_vec2 pos)
{
    UI_ASSERT(ctx); UI_ASSERT(ctx->current);
    if (!ctx || !ctx->current) return;
    ctx->current->bounds.x = pos.x;
    ctx->current->bounds.y = pos.y;
}

UI_API void
ui_window_set_size(struct ui_context *ctx, struct ui_vec2 size)
{
    UI_ASSERT(ctx); UI_ASSERT(ctx->current);
    if (!ctx || !ctx->current) return;
    ctx->current->bounds.w = size.x;
    ctx->current->bounds.h = size.y;
}

UI_API void
ui_window_collapse(struct ui_context *ctx, const char *name,
                    enum ui_collapse_states c)
{
    int title_len;
    ui_hash title_hash;
    struct ui_window *win;
    UI_ASSERT(ctx);
    if (!ctx) return;

    title_len = (int)ui_strlen(name);
    title_hash = ui_murmur_hash(name, (int)title_len, UI_WINDOW_TITLE);
    win = ui_find_window(ctx, title_hash, name);
    if (!win) return;
    if (c == UI_MINIMIZED)
        win->flags |= UI_WINDOW_MINIMIZED;
    else win->flags &= ~(ui_flags)UI_WINDOW_MINIMIZED;
}

UI_API void
ui_window_collapse_if(struct ui_context *ctx, const char *name,
    enum ui_collapse_states c, int cond)
{
    UI_ASSERT(ctx);
    if (!ctx || !cond) return;
    ui_window_collapse(ctx, name, c);
}

UI_API void
ui_window_show(struct ui_context *ctx, const char *name, enum ui_show_states s)
{
    int title_len;
    ui_hash title_hash;
    struct ui_window *win;
    UI_ASSERT(ctx);
    if (!ctx) return;

    title_len = (int)ui_strlen(name);
    title_hash = ui_murmur_hash(name, (int)title_len, UI_WINDOW_TITLE);
    win = ui_find_window(ctx, title_hash, name);
    if (!win) return;
    if (s == UI_HIDDEN) {
        win->flags |= UI_WINDOW_HIDDEN;
    } else win->flags &= ~(ui_flags)UI_WINDOW_HIDDEN;
}

UI_API void
ui_window_show_if(struct ui_context *ctx, const char *name,
    enum ui_show_states s, int cond)
{
    UI_ASSERT(ctx);
    if (!ctx || !cond) return;
    ui_window_show(ctx, name, s);
}

UI_API void
ui_window_set_focus(struct ui_context *ctx, const char *name)
{
    int title_len;
    ui_hash title_hash;
    struct ui_window *win;
    UI_ASSERT(ctx);
    if (!ctx) return;

    title_len = (int)ui_strlen(name);
    title_hash = ui_murmur_hash(name, (int)title_len, UI_WINDOW_TITLE);
    win = ui_find_window(ctx, title_hash, name);
    if (win && ctx->end != win) {
        ui_remove_window(ctx, win);
        ui_insert_window(ctx, win, UI_INSERT_BACK);
    }
    ctx->active = win;
}

/*----------------------------------------------------------------
 *
 *                          MENUBAR
 *
 * --------------------------------------------------------------*/
UI_API void
ui_menubar_begin(struct ui_context *ctx)
{
    struct ui_panel *layout;
    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return;

    layout = ctx->current->layout;
    UI_ASSERT(layout->at_y == layout->bounds.y);
    /* if this assert triggers you allocated space between ui_begin and ui_menubar_begin.
    If you want a menubar the first nuklear function after `ui_begin` has to be a
    `ui_menubar_begin` call. Inside the menubar you then have to allocate space for
    widgets (also supports multiple rows).
    Example:
        if (ui_begin(...)) {
            ui_menubar_begin(...);
                ui_layout_xxxx(...);
                ui_button(...);
                ui_layout_xxxx(...);
                ui_button(...);
            ui_menubar_end(...);
        }
        ui_end(...);
    */
    if (layout->flags & UI_WINDOW_HIDDEN || layout->flags & UI_WINDOW_MINIMIZED)
        return;

    layout->menu.x = layout->at_x;
    layout->menu.y = layout->at_y + layout->row.height;
    layout->menu.w = layout->bounds.w;
    layout->menu.offset.x = *layout->offset_x;
    layout->menu.offset.y = *layout->offset_y;
    *layout->offset_y = 0;
}

UI_API void
ui_menubar_end(struct ui_context *ctx)
{
    struct ui_window *win;
    struct ui_panel *layout;
    struct ui_command_buffer *out;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return;

    win = ctx->current;
    out = &win->buffer;
    layout = win->layout;
    if (layout->flags & UI_WINDOW_HIDDEN || layout->flags & UI_WINDOW_MINIMIZED)
        return;

    layout->menu.h = layout->at_y - layout->menu.y;
    layout->bounds.y += layout->menu.h + ctx->style.window.spacing.y + layout->row.height;
    layout->bounds.h -= layout->menu.h + ctx->style.window.spacing.y + layout->row.height;

    *layout->offset_x = layout->menu.offset.x;
    *layout->offset_y = layout->menu.offset.y;
    layout->at_y = layout->bounds.y - layout->row.height;

    layout->clip.y = layout->bounds.y;
    layout->clip.h = layout->bounds.h;
    ui_push_scissor(out, layout->clip);
}
/* -------------------------------------------------------------
 *
 *                          LAYOUT
 *
 * --------------------------------------------------------------*/
UI_INTERN float
ui_layout_row_calculate_usable_space(const struct ui_style *style, enum ui_panel_type type,
    float total_space, int columns)
{
    float panel_padding;
    float panel_spacing;
    float panel_space;

    struct ui_vec2 spacing;
    struct ui_vec2 padding;

    spacing = style->window.spacing;
    padding = ui_panel_get_padding(style, type);

    /* calculate the usable panel space */
    panel_padding = 2 * padding.x;
    panel_spacing = (float)UI_MAX(columns - 1, 0) * spacing.x;
    panel_space  = total_space - panel_padding - panel_spacing;
    return panel_space;
}

UI_INTERN void
ui_panel_layout(const struct ui_context *ctx, struct ui_window *win,
    float height, int cols)
{
    struct ui_panel *layout;
    const struct ui_style *style;
    struct ui_command_buffer *out;

    struct ui_vec2 item_spacing;
    struct ui_color color;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return;

    /* prefetch some configuration data */
    layout = win->layout;
    style = &ctx->style;
    out = &win->buffer;
    color = style->window.background;
    item_spacing = style->window.spacing;

    /*  if one of these triggers you forgot to add an `if` condition around either
        a window, group, popup, combobox or contextual menu `begin` and `end` block.
        Example:
            if (ui_begin(...) {...} ui_end(...); or
            if (ui_group_begin(...) { ui_group_end(...);} */
    UI_ASSERT(!(layout->flags & UI_WINDOW_MINIMIZED));
    UI_ASSERT(!(layout->flags & UI_WINDOW_HIDDEN));
    UI_ASSERT(!(layout->flags & UI_WINDOW_CLOSED));

    /* update the current row and set the current row layout */
    layout->row.index = 0;
    layout->at_y += layout->row.height;
    layout->row.columns = cols;
    layout->row.height = height + item_spacing.y;
    layout->row.item_offset = 0;
    if (layout->flags & UI_WINDOW_DYNAMIC) {
        /* draw background for dynamic panels */
        struct ui_rect background;
        background.x = win->bounds.x;
        background.w = win->bounds.w;
        background.y = layout->at_y - 1.0f;
        background.h = layout->row.height + 1.0f;
        ui_fill_rect(out, background, 0, color);
    }
}

UI_INTERN void
ui_row_layout(struct ui_context *ctx, enum ui_layout_format fmt,
    float height, int cols, int width)
{
    /* update the current row and set the current row layout */
    struct ui_window *win;
    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return;

    win = ctx->current;
    ui_panel_layout(ctx, win, height, cols);
    if (fmt == UI_DYNAMIC)
        win->layout->row.type = UI_LAYOUT_DYNAMIC_FIXED;
    else win->layout->row.type = UI_LAYOUT_STATIC_FIXED;

    win->layout->row.ratio = 0;
    win->layout->row.filled = 0;
    win->layout->row.item_offset = 0;
    win->layout->row.item_width = (float)width;
}

UI_API float
ui_layout_ratio_from_pixel(struct ui_context *ctx, float pixel_width)
{
    struct ui_window *win;
    UI_ASSERT(ctx);
    UI_ASSERT(pixel_width);
    if (!ctx || !ctx->current || !ctx->current->layout) return 0;
    win = ctx->current;
    return UI_CLAMP(0.0f, pixel_width/win->bounds.x, 1.0f);
}

UI_API void
ui_layout_row_dynamic(struct ui_context *ctx, float height, int cols)
{
    ui_row_layout(ctx, UI_DYNAMIC, height, cols, 0);
}

UI_API void
ui_layout_row_static(struct ui_context *ctx, float height, int item_width, int cols)
{
    ui_row_layout(ctx, UI_STATIC, height, cols, item_width);
}

UI_API void
ui_layout_row_begin(struct ui_context *ctx, enum ui_layout_format fmt,
    float row_height, int cols)
{
    struct ui_window *win;
    struct ui_panel *layout;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return;

    win = ctx->current;
    layout = win->layout;
    ui_panel_layout(ctx, win, row_height, cols);
    if (fmt == UI_DYNAMIC)
        layout->row.type = UI_LAYOUT_DYNAMIC_ROW;
    else layout->row.type = UI_LAYOUT_STATIC_ROW;

    layout->row.ratio = 0;
    layout->row.filled = 0;
    layout->row.item_width = 0;
    layout->row.item_offset = 0;
    layout->row.columns = cols;
}

UI_API void
ui_layout_row_push(struct ui_context *ctx, float ratio_or_width)
{
    struct ui_window *win;
    struct ui_panel *layout;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return;

    win = ctx->current;
    layout = win->layout;
    UI_ASSERT(layout->row.type == UI_LAYOUT_STATIC_ROW || layout->row.type == UI_LAYOUT_DYNAMIC_ROW);
    if (layout->row.type != UI_LAYOUT_STATIC_ROW && layout->row.type != UI_LAYOUT_DYNAMIC_ROW)
        return;

    if (layout->row.type == UI_LAYOUT_DYNAMIC_ROW) {
        float ratio = ratio_or_width;
        if ((ratio + layout->row.filled) > 1.0f) return;
        if (ratio > 0.0f)
            layout->row.item_width = UI_SATURATE(ratio);
        else layout->row.item_width = 1.0f - layout->row.filled;
    } else layout->row.item_width = ratio_or_width;
}

UI_API void
ui_layout_row_end(struct ui_context *ctx)
{
    struct ui_window *win;
    struct ui_panel *layout;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return;

    win = ctx->current;
    layout = win->layout;
    UI_ASSERT(layout->row.type == UI_LAYOUT_STATIC_ROW || layout->row.type == UI_LAYOUT_DYNAMIC_ROW);
    if (layout->row.type != UI_LAYOUT_STATIC_ROW && layout->row.type != UI_LAYOUT_DYNAMIC_ROW)
        return;
    layout->row.item_width = 0;
    layout->row.item_offset = 0;
}

UI_API void
ui_layout_row(struct ui_context *ctx, enum ui_layout_format fmt,
    float height, int cols, const float *ratio)
{
    int i;
    int n_undef = 0;
    struct ui_window *win;
    struct ui_panel *layout;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return;

    win = ctx->current;
    layout = win->layout;
    ui_panel_layout(ctx, win, height, cols);
    if (fmt == UI_DYNAMIC) {
        /* calculate width of undefined widget ratios */
        float r = 0;
        layout->row.ratio = ratio;
        for (i = 0; i < cols; ++i) {
            if (ratio[i] < 0.0f)
                n_undef++;
            else r += ratio[i];
        }
        r = UI_SATURATE(1.0f - r);
        layout->row.type = UI_LAYOUT_DYNAMIC;
        layout->row.item_width = (r > 0 && n_undef > 0) ? (r / (float)n_undef):0;
    } else {
        layout->row.ratio = ratio;
        layout->row.type = UI_LAYOUT_STATIC;
        layout->row.item_width = 0;
        layout->row.item_offset = 0;
    }
    layout->row.item_offset = 0;
    layout->row.filled = 0;
}

UI_API void
ui_layout_row_template_begin(struct ui_context *ctx, float height)
{
    struct ui_window *win;
    struct ui_panel *layout;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return;

    win = ctx->current;
    layout = win->layout;
    ui_panel_layout(ctx, win, height, 1);
    layout->row.type = UI_LAYOUT_TEMPLATE;
    layout->row.columns = 0;
    layout->row.ratio = 0;
    layout->row.item_width = 0;
    layout->row.item_height = 0;
    layout->row.item_offset = 0;
    layout->row.filled = 0;
    layout->row.item.x = 0;
    layout->row.item.y = 0;
    layout->row.item.w = 0;
    layout->row.item.h = 0;
}

UI_API void
ui_layout_row_template_push_dynamic(struct ui_context *ctx)
{
    struct ui_window *win;
    struct ui_panel *layout;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return;

    win = ctx->current;
    layout = win->layout;
    UI_ASSERT(layout->row.type == UI_LAYOUT_TEMPLATE);
    UI_ASSERT(layout->row.columns < UI_MAX_LAYOUT_ROW_TEMPLATE_COLUMNS);
    if (layout->row.type != UI_LAYOUT_TEMPLATE) return;
    if (layout->row.columns >= UI_MAX_LAYOUT_ROW_TEMPLATE_COLUMNS) return;
    layout->row.templates[layout->row.columns++] = -1.0f;
}

UI_API void
ui_layout_row_template_push_variable(struct ui_context *ctx, float min_width)
{
    struct ui_window *win;
    struct ui_panel *layout;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return;

    win = ctx->current;
    layout = win->layout;
    UI_ASSERT(layout->row.type == UI_LAYOUT_TEMPLATE);
    UI_ASSERT(layout->row.columns < UI_MAX_LAYOUT_ROW_TEMPLATE_COLUMNS);
    if (layout->row.type != UI_LAYOUT_TEMPLATE) return;
    if (layout->row.columns >= UI_MAX_LAYOUT_ROW_TEMPLATE_COLUMNS) return;
    layout->row.templates[layout->row.columns++] = -min_width;
}

UI_API void
ui_layout_row_template_push_static(struct ui_context *ctx, float width)
{
    struct ui_window *win;
    struct ui_panel *layout;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return;

    win = ctx->current;
    layout = win->layout;
    UI_ASSERT(layout->row.type == UI_LAYOUT_TEMPLATE);
    UI_ASSERT(layout->row.columns < UI_MAX_LAYOUT_ROW_TEMPLATE_COLUMNS);
    if (layout->row.type != UI_LAYOUT_TEMPLATE) return;
    if (layout->row.columns >= UI_MAX_LAYOUT_ROW_TEMPLATE_COLUMNS) return;
    layout->row.templates[layout->row.columns++] = width;
}

UI_API void
ui_layout_row_template_end(struct ui_context *ctx)
{
    struct ui_window *win;
    struct ui_panel *layout;

    int i = 0;
    int variable_count = 0;
    int min_variable_count = 0;
    float min_fixed_width = 0.0f;
    float total_fixed_width = 0.0f;
    float max_variable_width = 0.0f;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return;

    win = ctx->current;
    layout = win->layout;
    UI_ASSERT(layout->row.type == UI_LAYOUT_TEMPLATE);
    if (layout->row.type != UI_LAYOUT_TEMPLATE) return;
    for (i = 0; i < layout->row.columns; ++i) {
        float width = layout->row.templates[i];
        if (width >= 0.0f) {
            total_fixed_width += width;
            min_fixed_width += width;
        } else if (width < -1.0f) {
            width = -width;
            total_fixed_width += width;
            max_variable_width = UI_MAX(max_variable_width, width);
            variable_count++;
        } else {
            min_variable_count++;
            variable_count++;
        }
    }
    if (variable_count) {
        float space = ui_layout_row_calculate_usable_space(&ctx->style, layout->type,
                            layout->bounds.w, layout->row.columns);
        float var_width = (UI_MAX(space-min_fixed_width,0.0f)) / (float)variable_count;
        int enough_space = var_width >= max_variable_width;
        if (!enough_space)
            var_width = (UI_MAX(space-total_fixed_width,0)) / (float)min_variable_count;
        for (i = 0; i < layout->row.columns; ++i) {
            float *width = &layout->row.templates[i];
            *width = (*width >= 0.0f)? *width: (*width < -1.0f && !enough_space)? -(*width): var_width;
        }
    }
}

UI_API void
ui_layout_space_begin(struct ui_context *ctx, enum ui_layout_format fmt,
    float height, int widget_count)
{
    struct ui_window *win;
    struct ui_panel *layout;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return;

    win = ctx->current;
    layout = win->layout;
    ui_panel_layout(ctx, win, height, widget_count);
    if (fmt == UI_STATIC)
        layout->row.type = UI_LAYOUT_STATIC_FREE;
    else layout->row.type = UI_LAYOUT_DYNAMIC_FREE;

    layout->row.ratio = 0;
    layout->row.filled = 0;
    layout->row.item_width = 0;
    layout->row.item_offset = 0;
}

UI_API void
ui_layout_space_end(struct ui_context *ctx)
{
    struct ui_window *win;
    struct ui_panel *layout;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return;

    win = ctx->current;
    layout = win->layout;
    layout->row.item_width = 0;
    layout->row.item_height = 0;
    layout->row.item_offset = 0;
    ui_zero(&layout->row.item, sizeof(layout->row.item));
}

UI_API void
ui_layout_space_push(struct ui_context *ctx, struct ui_rect rect)
{
    struct ui_window *win;
    struct ui_panel *layout;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return;

    win = ctx->current;
    layout = win->layout;
    layout->row.item = rect;
}

UI_API struct ui_rect
ui_layout_space_bounds(struct ui_context *ctx)
{
    struct ui_rect ret;
    struct ui_window *win;
    struct ui_panel *layout;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    win = ctx->current;
    layout = win->layout;

    ret.x = layout->clip.x;
    ret.y = layout->clip.y;
    ret.w = layout->clip.w;
    ret.h = layout->row.height;
    return ret;
}

UI_API struct ui_vec2
ui_layout_space_to_screen(struct ui_context *ctx, struct ui_vec2 ret)
{
    struct ui_window *win;
    struct ui_panel *layout;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    win = ctx->current;
    layout = win->layout;

    ret.x += layout->at_x - (float)*layout->offset_x;
    ret.y += layout->at_y - (float)*layout->offset_y;
    return ret;
}

UI_API struct ui_vec2
ui_layout_space_to_local(struct ui_context *ctx, struct ui_vec2 ret)
{
    struct ui_window *win;
    struct ui_panel *layout;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    win = ctx->current;
    layout = win->layout;

    ret.x += -layout->at_x + (float)*layout->offset_x;
    ret.y += -layout->at_y + (float)*layout->offset_y;
    return ret;
}

UI_API struct ui_rect
ui_layout_space_rect_to_screen(struct ui_context *ctx, struct ui_rect ret)
{
    struct ui_window *win;
    struct ui_panel *layout;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    win = ctx->current;
    layout = win->layout;

    ret.x += layout->at_x - (float)*layout->offset_x;
    ret.y += layout->at_y - (float)*layout->offset_y;
    return ret;
}

UI_API struct ui_rect
ui_layout_space_rect_to_local(struct ui_context *ctx, struct ui_rect ret)
{
    struct ui_window *win;
    struct ui_panel *layout;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    win = ctx->current;
    layout = win->layout;

    ret.x += -layout->at_x + (float)*layout->offset_x;
    ret.y += -layout->at_y + (float)*layout->offset_y;
    return ret;
}

UI_INTERN void
ui_panel_alloc_row(const struct ui_context *ctx, struct ui_window *win)
{
    struct ui_panel *layout = win->layout;
    struct ui_vec2 spacing = ctx->style.window.spacing;
    const float row_height = layout->row.height - spacing.y;
    ui_panel_layout(ctx, win, row_height, layout->row.columns);
}

UI_INTERN void
ui_layout_widget_space(struct ui_rect *bounds, const struct ui_context *ctx,
    struct ui_window *win, int modify)
{
    struct ui_panel *layout;
    const struct ui_style *style;

    struct ui_vec2 spacing;
    struct ui_vec2 padding;

    float item_offset = 0;
    float item_width = 0;
    float item_spacing = 0;
    float panel_space = 0;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return;

    win = ctx->current;
    layout = win->layout;
    style = &ctx->style;
    UI_ASSERT(bounds);

    spacing = style->window.spacing;
    padding = ui_panel_get_padding(style, layout->type);
    panel_space = ui_layout_row_calculate_usable_space(&ctx->style, layout->type,
                                            layout->bounds.w, layout->row.columns);

    /* calculate the width of one item inside the current layout space */
    switch (layout->row.type) {
    case UI_LAYOUT_DYNAMIC_FIXED: {
        /* scaling fixed size widgets item width */
        item_width = panel_space / (float)layout->row.columns;
        item_offset = (float)layout->row.index * item_width;
        item_spacing = (float)layout->row.index * spacing.x;
    } break;
    case UI_LAYOUT_DYNAMIC_ROW: {
        /* scaling single ratio widget width */
        item_width = layout->row.item_width * panel_space;
        item_offset = layout->row.item_offset;
        item_spacing = 0;

        if (modify) {
            layout->row.item_offset += item_width + spacing.x;
            layout->row.filled += layout->row.item_width;
            layout->row.index = 0;
        }
    } break;
    case UI_LAYOUT_DYNAMIC_FREE: {
        /* panel width depended free widget placing */
        bounds->x = layout->at_x + (layout->bounds.w * layout->row.item.x);
        bounds->x -= (float)*layout->offset_x;
        bounds->y = layout->at_y + (layout->row.height * layout->row.item.y);
        bounds->y -= (float)*layout->offset_y;
        bounds->w = layout->bounds.w  * layout->row.item.w;
        bounds->h = layout->row.height * layout->row.item.h;
        return;
    } break;
    case UI_LAYOUT_DYNAMIC: {
        /* scaling arrays of panel width ratios for every widget */
        float ratio;
        UI_ASSERT(layout->row.ratio);
        ratio = (layout->row.ratio[layout->row.index] < 0) ?
            layout->row.item_width : layout->row.ratio[layout->row.index];

        item_spacing = (float)layout->row.index * spacing.x;
        item_width = (ratio * panel_space);
        item_offset = layout->row.item_offset;

        if (modify) {
            layout->row.item_offset += item_width;
            layout->row.filled += ratio;
        }
    } break;
    case UI_LAYOUT_STATIC_FIXED: {
        /* non-scaling fixed widgets item width */
        item_width = layout->row.item_width;
        item_offset = (float)layout->row.index * item_width;
        item_spacing = (float)layout->row.index * spacing.x;
    } break;
    case UI_LAYOUT_STATIC_ROW: {
        /* scaling single ratio widget width */
        item_width = layout->row.item_width;
        item_offset = layout->row.item_offset;
        item_spacing = (float)layout->row.index * spacing.x;
        if (modify) layout->row.item_offset += item_width;
    } break;
    case UI_LAYOUT_STATIC_FREE: {
        /* free widget placing */
        bounds->x = layout->at_x + layout->row.item.x;
        bounds->w = layout->row.item.w;
        if (((bounds->x + bounds->w) > layout->max_x) && modify)
            layout->max_x = (bounds->x + bounds->w);
        bounds->x -= (float)*layout->offset_x;
        bounds->y = layout->at_y + layout->row.item.y;
        bounds->y -= (float)*layout->offset_y;
        bounds->h = layout->row.item.h;
        return;
    } break;
    case UI_LAYOUT_STATIC: {
        /* non-scaling array of panel pixel width for every widget */
        item_spacing = (float)layout->row.index * spacing.x;
        item_width = layout->row.ratio[layout->row.index];
        item_offset = layout->row.item_offset;
        if (modify) layout->row.item_offset += item_width;
    } break;
    case UI_LAYOUT_TEMPLATE: {
        /* stretchy row layout with combined dynamic/static widget width*/
        UI_ASSERT(layout->row.index < layout->row.columns);
        UI_ASSERT(layout->row.index < UI_MAX_LAYOUT_ROW_TEMPLATE_COLUMNS);
        item_width = layout->row.templates[layout->row.index];
        item_offset = layout->row.item_offset;
        item_spacing = (float)layout->row.index * spacing.x;
        if (modify) layout->row.item_offset += item_width;
    } break;
    default: UI_ASSERT(0); break;
    };

    /* set the bounds of the newly allocated widget */
    bounds->w = item_width;
    bounds->h = layout->row.height - spacing.y;
    bounds->y = layout->at_y - (float)*layout->offset_y;
    bounds->x = layout->at_x + item_offset + item_spacing + padding.x;
    if (((bounds->x + bounds->w) > layout->max_x) && modify)
        layout->max_x = bounds->x + bounds->w;
    bounds->x -= (float)*layout->offset_x;
}

UI_INTERN void
ui_panel_alloc_space(struct ui_rect *bounds, const struct ui_context *ctx)
{
    struct ui_window *win;
    struct ui_panel *layout;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return;

    /* check if the end of the row has been hit and begin new row if so */
    win = ctx->current;
    layout = win->layout;
    if (layout->row.index >= layout->row.columns)
        ui_panel_alloc_row(ctx, win);

    /* calculate widget position and size */
    ui_layout_widget_space(bounds, ctx, win, ui_true);
    layout->row.index++;
}

UI_INTERN void
ui_layout_peek(struct ui_rect *bounds, struct ui_context *ctx)
{
    float y;
    int index;
    struct ui_window *win;
    struct ui_panel *layout;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return;

    win = ctx->current;
    layout = win->layout;
    y = layout->at_y;
    index = layout->row.index;
    if (layout->row.index >= layout->row.columns) {
        layout->at_y += layout->row.height;
        layout->row.index = 0;
    }
    ui_layout_widget_space(bounds, ctx, win, ui_false);
    layout->at_y = y;
    layout->row.index = index;
}

UI_INTERN int
ui_tree_state_base(struct ui_context *ctx, enum ui_tree_type type,
    struct ui_image *img, const char *title, enum ui_collapse_states *state)
{
    struct ui_window *win;
    struct ui_panel *layout;
    const struct ui_style *style;
    struct ui_command_buffer *out;
    const struct ui_input *in;
    const struct ui_style_button *button;
    enum ui_symbol_type symbol;

    struct ui_vec2 item_spacing;
    struct ui_rect header = {0,0,0,0};
    struct ui_rect sym = {0,0,0,0};
    struct ui_text text;

    ui_flags ws = 0;
    enum ui_widget_layout_states widget_state;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return 0;

    /* cache some data */
    win = ctx->current;
    layout = win->layout;
    out = &win->buffer;
    style = &ctx->style;
    item_spacing = style->window.spacing;

    /* calculate header bounds and draw background */
    ui_layout_row_dynamic(ctx, style->font->height + 2 * style->tab.padding.y, 1);
    widget_state = ui_widget(&header, ctx);
    if (type == UI_TREE_TAB) {
        const struct ui_style_item *background = &style->tab.background;
        if (background->type == UI_STYLE_ITEM_IMAGE) {
            ui_draw_image(out, header, &background->data.image, ui_white);
            text.background = ui_rgba(0,0,0,0);
        } else {
            text.background = background->data.color;
            ui_fill_rect(out, header, 0, style->tab.border_color);
            ui_fill_rect(out, ui_shriui_rect(header, style->tab.border),
                style->tab.rounding, background->data.color);
        }
    } else text.background = style->window.background;

    /* update node state */
    in = (!(layout->flags & UI_WINDOW_ROM)) ? &ctx->input: 0;
    in = (in && widget_state == UI_WIDGET_VALID) ? &ctx->input : 0;
    if (ui_button_behavior(&ws, header, in, UI_BUTTON_DEFAULT))
        *state = (*state == UI_MAXIMIZED) ? UI_MINIMIZED : UI_MAXIMIZED;

    /* select correct button style */
    if (*state == UI_MAXIMIZED) {
        symbol = style->tab.sym_maximize;
        if (type == UI_TREE_TAB)
            button = &style->tab.tab_maximize_button;
        else button = &style->tab.node_maximize_button;
    } else {
        symbol = style->tab.sym_minimize;
        if (type == UI_TREE_TAB)
            button = &style->tab.tab_minimize_button;
        else button = &style->tab.node_minimize_button;
    }

    {/* draw triangle button */
    sym.w = sym.h = style->font->height;
    sym.y = header.y + style->tab.padding.y;
    sym.x = header.x + style->tab.padding.x;
    ui_do_button_symbol(&ws, &win->buffer, sym, symbol, UI_BUTTON_DEFAULT,
        button, 0, style->font);

    if (img) {
        /* draw optional image icon */
        sym.x = sym.x + sym.w + 4 * item_spacing.x;
        ui_draw_image(&win->buffer, sym, img, ui_white);
        sym.w = style->font->height + style->tab.spacing.x;}
    }

    {/* draw label */
    struct ui_rect label;
    header.w = UI_MAX(header.w, sym.w + item_spacing.x);
    label.x = sym.x + sym.w + item_spacing.x;
    label.y = sym.y;
    label.w = header.w - (sym.w + item_spacing.y + style->tab.indent);
    label.h = style->font->height;
    text.text = style->tab.text;
    text.padding = ui_vec2(0,0);
    ui_widget_text(out, label, title, ui_strlen(title), &text,
        UI_TEXT_LEFT, style->font);}

    /* increase x-axis cursor widget position pointer */
    if (*state == UI_MAXIMIZED) {
        layout->at_x = header.x + (float)*layout->offset_x + style->tab.indent;
        layout->bounds.w = UI_MAX(layout->bounds.w, style->tab.indent);
        layout->bounds.w -= (style->tab.indent + style->window.padding.x);
        layout->row.tree_depth++;
        return ui_true;
    } else return ui_false;
}

UI_INTERN int
ui_tree_base(struct ui_context *ctx, enum ui_tree_type type,
    struct ui_image *img, const char *title, enum ui_collapse_states initial_state,
    const char *hash, int len, int line)
{
    struct ui_window *win = ctx->current;
    int title_len = 0;
    ui_hash tree_hash = 0;
    ui_uint *state = 0;

    /* retrieve tree state from internal widget state tables */
    if (!hash) {
        title_len = (int)ui_strlen(title);
        tree_hash = ui_murmur_hash(title, (int)title_len, (ui_hash)line);
    } else tree_hash = ui_murmur_hash(hash, len, (ui_hash)line);
    state = ui_find_value(win, tree_hash);
    if (!state) {
        state = ui_add_value(ctx, win, tree_hash, 0);
        *state = initial_state;
    }
    return ui_tree_state_base(ctx, type, img, title, (enum ui_collapse_states*)state);
}

UI_API int
ui_tree_state_push(struct ui_context *ctx, enum ui_tree_type type,
    const char *title, enum ui_collapse_states *state)
{return ui_tree_state_base(ctx, type, 0, title, state);}

UI_API int
ui_tree_state_image_push(struct ui_context *ctx, enum ui_tree_type type,
    struct ui_image img, const char *title, enum ui_collapse_states *state)
{return ui_tree_state_base(ctx, type, &img, title, state);}

UI_API void
ui_tree_state_pop(struct ui_context *ctx)
{
    struct ui_window *win = 0;
    struct ui_panel *layout = 0;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return;

    win = ctx->current;
    layout = win->layout;
    layout->at_x -= ctx->style.tab.indent + ctx->style.window.padding.x;
    layout->bounds.w += ctx->style.tab.indent + ctx->style.window.padding.x;
    UI_ASSERT(layout->row.tree_depth);
    layout->row.tree_depth--;
}

UI_API int
ui_tree_push_hashed(struct ui_context *ctx, enum ui_tree_type type,
    const char *title, enum ui_collapse_states initial_state,
    const char *hash, int len, int line)
{return ui_tree_base(ctx, type, 0, title, initial_state, hash, len, line);}

UI_API int
ui_tree_image_push_hashed(struct ui_context *ctx, enum ui_tree_type type,
    struct ui_image img, const char *title, enum ui_collapse_states initial_state,
    const char *hash, int len,int seed)
{return ui_tree_base(ctx, type, &img, title, initial_state, hash, len, seed);}

UI_API void
ui_tree_pop(struct ui_context *ctx)
{ui_tree_state_pop(ctx);}

/*----------------------------------------------------------------
 *
 *                          WIDGETS
 *
 * --------------------------------------------------------------*/
UI_API struct ui_rect
ui_widget_bounds(struct ui_context *ctx)
{
    struct ui_rect bounds;
    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    if (!ctx || !ctx->current)
        return ui_rect(0,0,0,0);
    ui_layout_peek(&bounds, ctx);
    return bounds;
}

UI_API struct ui_vec2
ui_widget_position(struct ui_context *ctx)
{
    struct ui_rect bounds;
    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    if (!ctx || !ctx->current)
        return ui_vec2(0,0);

    ui_layout_peek(&bounds, ctx);
    return ui_vec2(bounds.x, bounds.y);
}

UI_API struct ui_vec2
ui_widget_size(struct ui_context *ctx)
{
    struct ui_rect bounds;
    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    if (!ctx || !ctx->current)
        return ui_vec2(0,0);

    ui_layout_peek(&bounds, ctx);
    return ui_vec2(bounds.w, bounds.h);
}

UI_API float
ui_widget_width(struct ui_context *ctx)
{
    struct ui_rect bounds;
    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    if (!ctx || !ctx->current)
        return 0;

    ui_layout_peek(&bounds, ctx);
    return bounds.w;
}

UI_API float
ui_widget_height(struct ui_context *ctx)
{
    struct ui_rect bounds;
    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    if (!ctx || !ctx->current)
        return 0;

    ui_layout_peek(&bounds, ctx);
    return bounds.h;
}

UI_API int
ui_widget_is_hovered(struct ui_context *ctx)
{
    int ret;
    struct ui_rect bounds;
    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    if (!ctx || !ctx->current)
        return 0;

    ui_layout_peek(&bounds, ctx);
    ret = (ctx->active == ctx->current);
    ret = ret && ui_input_is_mouse_hovering_rect(&ctx->input, bounds);
    return ret;
}

UI_API int
ui_widget_is_mouse_clicked(struct ui_context *ctx, enum ui_buttons btn)
{
    int ret;
    struct ui_rect bounds;
    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    if (!ctx || !ctx->current)
        return 0;

    ui_layout_peek(&bounds, ctx);
    ret = (ctx->active == ctx->current);
    ret = ret && ui_input_mouse_clicked(&ctx->input, btn, bounds);
    return ret;
}

UI_API int
ui_widget_has_mouse_click_down(struct ui_context *ctx, enum ui_buttons btn, int down)
{
    int ret;
    struct ui_rect bounds;
    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    if (!ctx || !ctx->current)
        return 0;

    ui_layout_peek(&bounds, ctx);
    ret = (ctx->active == ctx->current);
    ret = ret && ui_input_has_mouse_click_down_in_rect(&ctx->input, btn, bounds, down);
    return ret;
}

UI_API enum ui_widget_layout_states
ui_widget(struct ui_rect *bounds, const struct ui_context *ctx)
{
    struct ui_rect *c = 0;
    struct ui_window *win;
    struct ui_panel *layout;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return UI_WIDGET_INVALID;

    /* allocate space  and check if the widget needs to be updated and drawn */
    ui_panel_alloc_space(bounds, ctx);
    win = ctx->current;
    layout = win->layout;
    c = &layout->clip;

    /*  if one of these triggers you forgot to add an `if` condition around either
        a window, group, popup, combobox or contextual menu `begin` and `end` block.
        Example:
            if (ui_begin(...) {...} ui_end(...); or
            if (ui_group_begin(...) { ui_group_end(...);} */
    UI_ASSERT(!(layout->flags & UI_WINDOW_MINIMIZED));
    UI_ASSERT(!(layout->flags & UI_WINDOW_HIDDEN));
    UI_ASSERT(!(layout->flags & UI_WINDOW_CLOSED));

    /* need to convert to int here to remove floating point error */
    bounds->x = (float)((int)bounds->x);
    bounds->y = (float)((int)bounds->y);
    bounds->w = (float)((int)bounds->w);
    bounds->h = (float)((int)bounds->h);

    if (!UI_INTERSECT(c->x, c->y, c->w, c->h, bounds->x, bounds->y, bounds->w, bounds->h))
        return UI_WIDGET_INVALID;
    if (!UI_CONTAINS(bounds->x, bounds->y, bounds->w, bounds->h, c->x, c->y, c->w, c->h))
        return UI_WIDGET_ROM;
    return UI_WIDGET_VALID;
}

UI_API enum ui_widget_layout_states
ui_widget_fitting(struct ui_rect *bounds, struct ui_context *ctx,
    struct ui_vec2 item_padding)
{
    /* update the bounds to stand without padding  */
    struct ui_window *win;
    struct ui_style *style;
    struct ui_panel *layout;
    enum ui_widget_layout_states state;
    struct ui_vec2 panel_padding;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return UI_WIDGET_INVALID;

    win = ctx->current;
    style = &ctx->style;
    layout = win->layout;
    state = ui_widget(bounds, ctx);

    panel_padding = ui_panel_get_padding(style, layout->type);
    if (layout->row.index == 1) {
        bounds->w += panel_padding.x;
        bounds->x -= panel_padding.x;
    } else bounds->x -= item_padding.x;

    if (layout->row.index == layout->row.columns)
        bounds->w += panel_padding.x;
    else bounds->w += item_padding.x;
    return state;
}

/*----------------------------------------------------------------
 *
 *                          MISC
 *
 * --------------------------------------------------------------*/
UI_API void
ui_spacing(struct ui_context *ctx, int cols)
{
    struct ui_window *win;
    struct ui_panel *layout;
    struct ui_rect none;
    int i, index, rows;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return;

    /* spacing over row boundaries */
    win = ctx->current;
    layout = win->layout;
    index = (layout->row.index + cols) % layout->row.columns;
    rows = (layout->row.index + cols) / layout->row.columns;
    if (rows) {
        for (i = 0; i < rows; ++i)
            ui_panel_alloc_row(ctx, win);
        cols = index;
    }

    /* non table layout need to allocate space */
    if (layout->row.type != UI_LAYOUT_DYNAMIC_FIXED &&
        layout->row.type != UI_LAYOUT_STATIC_FIXED) {
        for (i = 0; i < cols; ++i)
            ui_panel_alloc_space(&none, ctx);
    }
    layout->row.index = index;
}

/*----------------------------------------------------------------
 *
 *                          TEXT
 *
 * --------------------------------------------------------------*/
UI_API void
ui_text_colored(struct ui_context *ctx, const char *str, int len,
    ui_flags alignment, struct ui_color color)
{
    struct ui_window *win;
    const struct ui_style *style;

    struct ui_vec2 item_padding;
    struct ui_rect bounds;
    struct ui_text text;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout) return;

    win = ctx->current;
    style = &ctx->style;
    ui_panel_alloc_space(&bounds, ctx);
    item_padding = style->text.padding;

    text.padding.x = item_padding.x;
    text.padding.y = item_padding.y;
    text.background = style->window.background;
    text.text = color;
    ui_widget_text(&win->buffer, bounds, str, len, &text, alignment, style->font);
}

UI_API void
ui_text_wrap_colored(struct ui_context *ctx, const char *str,
    int len, struct ui_color color)
{
    struct ui_window *win;
    const struct ui_style *style;

    struct ui_vec2 item_padding;
    struct ui_rect bounds;
    struct ui_text text;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout) return;

    win = ctx->current;
    style = &ctx->style;
    ui_panel_alloc_space(&bounds, ctx);
    item_padding = style->text.padding;

    text.padding.x = item_padding.x;
    text.padding.y = item_padding.y;
    text.background = style->window.background;
    text.text = color;
    ui_widget_text_wrap(&win->buffer, bounds, str, len, &text, style->font);
}

#ifdef UI_INCLUDE_STANDARD_VARARGS
UI_API void
ui_labelf_colored(struct ui_context *ctx, ui_flags flags,
    struct ui_color color, const char *fmt, ...)
{
    char buf[256];
    va_list args;
    va_start(args, fmt);
    ui_strfmt(buf, UI_LEN(buf), fmt, args);
    ui_label_colored(ctx, buf, flags, color);
    va_end(args);
}

UI_API void
ui_labelf_colored_wrap(struct ui_context *ctx, struct ui_color color,
    const char *fmt, ...)
{
    char buf[256];
    va_list args;
    va_start(args, fmt);
    ui_strfmt(buf, UI_LEN(buf), fmt, args);
    ui_label_colored_wrap(ctx, buf, color);
    va_end(args);
}

UI_API void
ui_labelf(struct ui_context *ctx, ui_flags flags, const char *fmt, ...)
{
    char buf[256];
    va_list args;
    va_start(args, fmt);
    ui_strfmt(buf, UI_LEN(buf), fmt, args);
    ui_label(ctx, buf, flags);
    va_end(args);
}

UI_API void
ui_labelf_wrap(struct ui_context *ctx, const char *fmt,...)
{
    char buf[256];
    va_list args;
    va_start(args, fmt);
    ui_strfmt(buf, UI_LEN(buf), fmt, args);
    ui_label_wrap(ctx, buf);
    va_end(args);
}

UI_API void
ui_value_bool(struct ui_context *ctx, const char *prefix, int value)
{ui_labelf(ctx, UI_TEXT_LEFT, "%s: %s", prefix, ((value) ? "true": "false"));}

UI_API void
ui_value_int(struct ui_context *ctx, const char *prefix, int value)
{ui_labelf(ctx, UI_TEXT_LEFT, "%s: %d", prefix, value);}

UI_API void
ui_value_uint(struct ui_context *ctx, const char *prefix, unsigned int value)
{ui_labelf(ctx, UI_TEXT_LEFT, "%s: %u", prefix, value);}

UI_API void
ui_value_float(struct ui_context *ctx, const char *prefix, float value)
{
    double double_value = (double)value;
    ui_labelf(ctx, UI_TEXT_LEFT, "%s: %.3f", prefix, double_value);
}

UI_API void
ui_value_color_byte(struct ui_context *ctx, const char *p, struct ui_color c)
{ui_labelf(ctx, UI_TEXT_LEFT, "%s: (%d, %d, %d, %d)", p, c.r, c.g, c.b, c.a);}

UI_API void
ui_value_color_float(struct ui_context *ctx, const char *p, struct ui_color color)
{
    double c[4]; ui_color_dv(c, color);
    ui_labelf(ctx, UI_TEXT_LEFT, "%s: (%.2f, %.2f, %.2f, %.2f)",
        p, c[0], c[1], c[2], c[3]);
}

UI_API void
ui_value_color_hex(struct ui_context *ctx, const char *prefix, struct ui_color color)
{
    char hex[16];
    ui_color_hex_rgba(hex, color);
    ui_labelf(ctx, UI_TEXT_LEFT, "%s: %s", prefix, hex);
}
#endif

UI_API void
ui_text(struct ui_context *ctx, const char *str, int len, ui_flags alignment)
{
    UI_ASSERT(ctx);
    if (!ctx) return;
    ui_text_colored(ctx, str, len, alignment, ctx->style.text.color);
}

UI_API void
ui_text_wrap(struct ui_context *ctx, const char *str, int len)
{
    UI_ASSERT(ctx);
    if (!ctx) return;
    ui_text_wrap_colored(ctx, str, len, ctx->style.text.color);
}

UI_API void
ui_label(struct ui_context *ctx, const char *str, ui_flags alignment)
{ui_text(ctx, str, ui_strlen(str), alignment);}

UI_API void
ui_label_colored(struct ui_context *ctx, const char *str, ui_flags align,
    struct ui_color color)
{ui_text_colored(ctx, str, ui_strlen(str), align, color);}

UI_API void
ui_label_wrap(struct ui_context *ctx, const char *str)
{ui_text_wrap(ctx, str, ui_strlen(str));}

UI_API void
ui_label_colored_wrap(struct ui_context *ctx, const char *str, struct ui_color color)
{ui_text_wrap_colored(ctx, str, ui_strlen(str), color);}

UI_API void
ui_image(struct ui_context *ctx, struct ui_image img)
{
    struct ui_window *win;
    struct ui_rect bounds;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout) return;

    win = ctx->current;
    if (!ui_widget(&bounds, ctx)) return;
    ui_draw_image(&win->buffer, bounds, &img, ui_white);
}

/*----------------------------------------------------------------
 *
 *                          BUTTON
 *
 * --------------------------------------------------------------*/
UI_API void
ui_button_set_behavior(struct ui_context *ctx, enum ui_button_behavior behavior)
{
    UI_ASSERT(ctx);
    if (!ctx) return;
    ctx->button_behavior = behavior;
}

UI_API int
ui_button_push_behavior(struct ui_context *ctx, enum ui_button_behavior behavior)
{
    struct ui_config_stack_button_behavior *button_stack;
    struct ui_config_stack_button_behavior_element *element;

    UI_ASSERT(ctx);
    if (!ctx) return 0;

    button_stack = &ctx->stacks.button_behaviors;
    UI_ASSERT(button_stack->head < (int)UI_LEN(button_stack->elements));
    if (button_stack->head >= (int)UI_LEN(button_stack->elements))
        return 0;

    element = &button_stack->elements[button_stack->head++];
    element->address = &ctx->button_behavior;
    element->old_value = ctx->button_behavior;
    ctx->button_behavior = behavior;
    return 1;
}

UI_API int
ui_button_pop_behavior(struct ui_context *ctx)
{
    struct ui_config_stack_button_behavior *button_stack;
    struct ui_config_stack_button_behavior_element *element;

    UI_ASSERT(ctx);
    if (!ctx) return 0;

    button_stack = &ctx->stacks.button_behaviors;
    UI_ASSERT(button_stack->head > 0);
    if (button_stack->head < 1)
        return 0;

    element = &button_stack->elements[--button_stack->head];
    *element->address = element->old_value;
    return 1;
}

UI_API int
ui_button_text_styled(struct ui_context *ctx,
    const struct ui_style_button *style, const char *title, int len)
{
    struct ui_window *win;
    struct ui_panel *layout;
    const struct ui_input *in;

    struct ui_rect bounds;
    enum ui_widget_layout_states state;

    UI_ASSERT(ctx);
    UI_ASSERT(style);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!style || !ctx || !ctx->current || !ctx->current->layout) return 0;

    win = ctx->current;
    layout = win->layout;
    state = ui_widget(&bounds, ctx);

    if (!state) return 0;
    in = (state == UI_WIDGET_ROM || layout->flags & UI_WINDOW_ROM) ? 0 : &ctx->input;
    return ui_do_button_text(&ctx->last_widget_state, &win->buffer, bounds,
                    title, len, style->text_alignment, ctx->button_behavior,
                    style, in, ctx->style.font);
}

UI_API int
ui_button_text(struct ui_context *ctx, const char *title, int len)
{
    UI_ASSERT(ctx);
    if (!ctx) return 0;
    return ui_button_text_styled(ctx, &ctx->style.button, title, len);
}

UI_API int ui_button_label_styled(struct ui_context *ctx,
    const struct ui_style_button *style, const char *title)
{return ui_button_text_styled(ctx, style, title, ui_strlen(title));}

UI_API int ui_button_label(struct ui_context *ctx, const char *title)
{return ui_button_text(ctx, title, ui_strlen(title));}

UI_API int
ui_button_color(struct ui_context *ctx, struct ui_color color)
{
    struct ui_window *win;
    struct ui_panel *layout;
    const struct ui_input *in;
    struct ui_style_button button;

    int ret = 0;
    struct ui_rect bounds;
    struct ui_rect content;
    enum ui_widget_layout_states state;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return 0;

    win = ctx->current;
    layout = win->layout;

    state = ui_widget(&bounds, ctx);
    if (!state) return 0;
    in = (state == UI_WIDGET_ROM || layout->flags & UI_WINDOW_ROM) ? 0 : &ctx->input;

    button = ctx->style.button;
    button.normal = ui_style_item_color(color);
    button.hover = ui_style_item_color(color);
    button.active = ui_style_item_color(color);
    ret = ui_do_button(&ctx->last_widget_state, &win->buffer, bounds,
                &button, in, ctx->button_behavior, &content);
    ui_draw_button(&win->buffer, &bounds, ctx->last_widget_state, &button);
    return ret;
}

UI_API int
ui_button_symbol_styled(struct ui_context *ctx,
    const struct ui_style_button *style, enum ui_symbol_type symbol)
{
    struct ui_window *win;
    struct ui_panel *layout;
    const struct ui_input *in;

    struct ui_rect bounds;
    enum ui_widget_layout_states state;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return 0;

    win = ctx->current;
    layout = win->layout;
    state = ui_widget(&bounds, ctx);
    if (!state) return 0;
    in = (state == UI_WIDGET_ROM || layout->flags & UI_WINDOW_ROM) ? 0 : &ctx->input;
    return ui_do_button_symbol(&ctx->last_widget_state, &win->buffer, bounds,
            symbol, ctx->button_behavior, style, in, ctx->style.font);
}

UI_API int
ui_button_symbol(struct ui_context *ctx, enum ui_symbol_type symbol)
{
    UI_ASSERT(ctx);
    if (!ctx) return 0;
    return ui_button_symbol_styled(ctx, &ctx->style.button, symbol);
}

UI_API int
ui_button_image_styled(struct ui_context *ctx, const struct ui_style_button *style,
    struct ui_image img)
{
    struct ui_window *win;
    struct ui_panel *layout;
    const struct ui_input *in;

    struct ui_rect bounds;
    enum ui_widget_layout_states state;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return 0;

    win = ctx->current;
    layout = win->layout;

    state = ui_widget(&bounds, ctx);
    if (!state) return 0;
    in = (state == UI_WIDGET_ROM || layout->flags & UI_WINDOW_ROM) ? 0 : &ctx->input;
    return ui_do_button_image(&ctx->last_widget_state, &win->buffer, bounds,
                img, ctx->button_behavior, style, in);
}

UI_API int
ui_button_image(struct ui_context *ctx, struct ui_image img)
{
    UI_ASSERT(ctx);
    if (!ctx) return 0;
    return ui_button_image_styled(ctx, &ctx->style.button, img);
}

UI_API int
ui_button_symbol_text_styled(struct ui_context *ctx,
    const struct ui_style_button *style, enum ui_symbol_type symbol,
    const char *text, int len, ui_flags align)
{
    struct ui_window *win;
    struct ui_panel *layout;
    const struct ui_input *in;

    struct ui_rect bounds;
    enum ui_widget_layout_states state;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return 0;

    win = ctx->current;
    layout = win->layout;

    state = ui_widget(&bounds, ctx);
    if (!state) return 0;
    in = (state == UI_WIDGET_ROM || layout->flags & UI_WINDOW_ROM) ? 0 : &ctx->input;
    return ui_do_button_text_symbol(&ctx->last_widget_state, &win->buffer, bounds,
                symbol, text, len, align, ctx->button_behavior,
                style, ctx->style.font, in);
}

UI_API int
ui_button_symbol_text(struct ui_context *ctx, enum ui_symbol_type symbol,
    const char* text, int len, ui_flags align)
{
    UI_ASSERT(ctx);
    if (!ctx) return 0;
    return ui_button_symbol_text_styled(ctx, &ctx->style.button, symbol, text, len, align);
}

UI_API int ui_button_symbol_label(struct ui_context *ctx, enum ui_symbol_type symbol,
    const char *label, ui_flags align)
{return ui_button_symbol_text(ctx, symbol, label, ui_strlen(label), align);}

UI_API int ui_button_symbol_label_styled(struct ui_context *ctx,
    const struct ui_style_button *style, enum ui_symbol_type symbol,
    const char *title, ui_flags align)
{return ui_button_symbol_text_styled(ctx, style, symbol, title, ui_strlen(title), align);}

UI_API int
ui_button_image_text_styled(struct ui_context *ctx,
    const struct ui_style_button *style, struct ui_image img, const char *text,
    int len, ui_flags align)
{
    struct ui_window *win;
    struct ui_panel *layout;
    const struct ui_input *in;

    struct ui_rect bounds;
    enum ui_widget_layout_states state;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return 0;

    win = ctx->current;
    layout = win->layout;

    state = ui_widget(&bounds, ctx);
    if (!state) return 0;
    in = (state == UI_WIDGET_ROM || layout->flags & UI_WINDOW_ROM) ? 0 : &ctx->input;
    return ui_do_button_text_image(&ctx->last_widget_state, &win->buffer,
            bounds, img, text, len, align, ctx->button_behavior,
            style, ctx->style.font, in);
}

UI_API int
ui_button_image_text(struct ui_context *ctx, struct ui_image img,
    const char *text, int len, ui_flags align)
{return ui_button_image_text_styled(ctx, &ctx->style.button,img, text, len, align);}


UI_API int ui_button_image_label(struct ui_context *ctx, struct ui_image img,
    const char *label, ui_flags align)
{return ui_button_image_text(ctx, img, label, ui_strlen(label), align);}

UI_API int ui_button_image_label_styled(struct ui_context *ctx,
    const struct ui_style_button *style, struct ui_image img,
    const char *label, ui_flags text_alignment)
{return ui_button_image_text_styled(ctx, style, img, label, ui_strlen(label), text_alignment);}

/*----------------------------------------------------------------
 *
 *                          SELECTABLE
 *
 * --------------------------------------------------------------*/
UI_API int
ui_selectable_text(struct ui_context *ctx, const char *str, int len,
    ui_flags align, int *value)
{
    struct ui_window *win;
    struct ui_panel *layout;
    const struct ui_input *in;
    const struct ui_style *style;

    enum ui_widget_layout_states state;
    struct ui_rect bounds;

    UI_ASSERT(ctx);
    UI_ASSERT(value);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout || !value)
        return 0;

    win = ctx->current;
    layout = win->layout;
    style = &ctx->style;

    state = ui_widget(&bounds, ctx);
    if (!state) return 0;
    in = (state == UI_WIDGET_ROM || layout->flags & UI_WINDOW_ROM) ? 0 : &ctx->input;
    return ui_do_selectable(&ctx->last_widget_state, &win->buffer, bounds,
                str, len, align, value, &style->selectable, in, style->font);
}

UI_API int
ui_selectable_image_text(struct ui_context *ctx, struct ui_image img,
    const char *str, int len, ui_flags align, int *value)
{
    struct ui_window *win;
    struct ui_panel *layout;
    const struct ui_input *in;
    const struct ui_style *style;

    enum ui_widget_layout_states state;
    struct ui_rect bounds;

    UI_ASSERT(ctx);
    UI_ASSERT(value);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout || !value)
        return 0;

    win = ctx->current;
    layout = win->layout;
    style = &ctx->style;

    state = ui_widget(&bounds, ctx);
    if (!state) return 0;
    in = (state == UI_WIDGET_ROM || layout->flags & UI_WINDOW_ROM) ? 0 : &ctx->input;
    return ui_do_selectable_image(&ctx->last_widget_state, &win->buffer, bounds,
                str, len, align, value, &img, &style->selectable, in, style->font);
}

UI_API int ui_select_text(struct ui_context *ctx, const char *str, int len,
    ui_flags align, int value)
{ui_selectable_text(ctx, str, len, align, &value);return value;}

UI_API int ui_selectable_label(struct ui_context *ctx, const char *str, ui_flags align, int *value)
{return ui_selectable_text(ctx, str, ui_strlen(str), align, value);}

UI_API int ui_selectable_image_label(struct ui_context *ctx,struct ui_image img,
    const char *str, ui_flags align, int *value)
{return ui_selectable_image_text(ctx, img, str, ui_strlen(str), align, value);}

UI_API int ui_select_label(struct ui_context *ctx, const char *str, ui_flags align, int value)
{ui_selectable_text(ctx, str, ui_strlen(str), align, &value);return value;}

UI_API int ui_select_image_label(struct ui_context *ctx, struct ui_image img,
    const char *str, ui_flags align, int value)
{ui_selectable_image_text(ctx, img, str, ui_strlen(str), align, &value);return value;}

UI_API int ui_select_image_text(struct ui_context *ctx, struct ui_image img,
    const char *str, int len, ui_flags align, int value)
{ui_selectable_image_text(ctx, img, str, len, align, &value);return value;}

/*----------------------------------------------------------------
 *
 *                          CHECKBOX
 *
 * --------------------------------------------------------------*/
UI_API int
ui_check_text(struct ui_context *ctx, const char *text, int len, int active)
{
    struct ui_window *win;
    struct ui_panel *layout;
    const struct ui_input *in;
    const struct ui_style *style;

    struct ui_rect bounds;
    enum ui_widget_layout_states state;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return active;

    win = ctx->current;
    style = &ctx->style;
    layout = win->layout;

    state = ui_widget(&bounds, ctx);
    if (!state) return active;
    in = (state == UI_WIDGET_ROM || layout->flags & UI_WINDOW_ROM) ? 0 : &ctx->input;
    ui_do_toggle(&ctx->last_widget_state, &win->buffer, bounds, &active,
        text, len, UI_TOGGLE_CHECK, &style->checkbox, in, style->font);
    return active;
}

UI_API unsigned int
ui_check_flags_text(struct ui_context *ctx, const char *text, int len,
    unsigned int flags, unsigned int value)
{
    int old_active;
    UI_ASSERT(ctx);
    UI_ASSERT(text);
    if (!ctx || !text) return flags;
    old_active = (int)((flags & value) & value);
    if (ui_check_text(ctx, text, len, old_active))
        flags |= value;
    else flags &= ~value;
    return flags;
}

UI_API int
ui_checkbox_text(struct ui_context *ctx, const char *text, int len, int *active)
{
    int old_val;
    UI_ASSERT(ctx);
    UI_ASSERT(text);
    UI_ASSERT(active);
    if (!ctx || !text || !active) return 0;
    old_val = *active;
    *active = ui_check_text(ctx, text, len, *active);
    return old_val != *active;
}

UI_API int
ui_checkbox_flags_text(struct ui_context *ctx, const char *text, int len,
    unsigned int *flags, unsigned int value)
{
    int active;
    UI_ASSERT(ctx);
    UI_ASSERT(text);
    UI_ASSERT(flags);
    if (!ctx || !text || !flags) return 0;

    active = (int)((*flags & value) & value);
    if (ui_checkbox_text(ctx, text, len, &active)) {
        if (active) *flags |= value;
        else *flags &= ~value;
        return 1;
    }
    return 0;
}

UI_API int ui_check_label(struct ui_context *ctx, const char *label, int active)
{return ui_check_text(ctx, label, ui_strlen(label), active);}

UI_API unsigned int ui_check_flags_label(struct ui_context *ctx, const char *label,
    unsigned int flags, unsigned int value)
{return ui_check_flags_text(ctx, label, ui_strlen(label), flags, value);}

UI_API int ui_checkbox_label(struct ui_context *ctx, const char *label, int *active)
{return ui_checkbox_text(ctx, label, ui_strlen(label), active);}

UI_API int ui_checkbox_flags_label(struct ui_context *ctx, const char *label,
    unsigned int *flags, unsigned int value)
{return ui_checkbox_flags_text(ctx, label, ui_strlen(label), flags, value);}

/*----------------------------------------------------------------
 *
 *                          OPTION
 *
 * --------------------------------------------------------------*/
UI_API int
ui_option_text(struct ui_context *ctx, const char *text, int len, int is_active)
{
    struct ui_window *win;
    struct ui_panel *layout;
    const struct ui_input *in;
    const struct ui_style *style;

    struct ui_rect bounds;
    enum ui_widget_layout_states state;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return is_active;

    win = ctx->current;
    style = &ctx->style;
    layout = win->layout;

    state = ui_widget(&bounds, ctx);
    if (!state) return state;
    in = (state == UI_WIDGET_ROM || layout->flags & UI_WINDOW_ROM) ? 0 : &ctx->input;
    ui_do_toggle(&ctx->last_widget_state, &win->buffer, bounds, &is_active,
        text, len, UI_TOGGLE_OPTION, &style->option, in, style->font);
    return is_active;
}

UI_API int
ui_radio_text(struct ui_context *ctx, const char *text, int len, int *active)
{
    int old_value;
    UI_ASSERT(ctx);
    UI_ASSERT(text);
    UI_ASSERT(active);
    if (!ctx || !text || !active) return 0;
    old_value = *active;
    *active = ui_option_text(ctx, text, len, old_value);
    return old_value != *active;
}

UI_API int
ui_option_label(struct ui_context *ctx, const char *label, int active)
{return ui_option_text(ctx, label, ui_strlen(label), active);}

UI_API int
ui_radio_label(struct ui_context *ctx, const char *label, int *active)
{return ui_radio_text(ctx, label, ui_strlen(label), active);}

/*----------------------------------------------------------------
 *
 *                          SLIDER
 *
 * --------------------------------------------------------------*/
UI_API int
ui_slider_float(struct ui_context *ctx, float min_value, float *value, float max_value,
    float value_step)
{
    struct ui_window *win;
    struct ui_panel *layout;
    struct ui_input *in;
    const struct ui_style *style;

    int ret = 0;
    float old_value;
    struct ui_rect bounds;
    enum ui_widget_layout_states state;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    UI_ASSERT(value);
    if (!ctx || !ctx->current || !ctx->current->layout || !value)
        return ret;

    win = ctx->current;
    style = &ctx->style;
    layout = win->layout;

    state = ui_widget(&bounds, ctx);
    if (!state) return ret;
    in = (state == UI_WIDGET_ROM || layout->flags & UI_WINDOW_ROM) ? 0 : &ctx->input;

    old_value = *value;
    *value = ui_do_slider(&ctx->last_widget_state, &win->buffer, bounds, min_value,
                old_value, max_value, value_step, &style->slider, in, style->font);
    return (old_value > *value || old_value < *value);
}

UI_API float
ui_slide_float(struct ui_context *ctx, float min, float val, float max, float step)
{
    ui_slider_float(ctx, min, &val, max, step); return val;
}

UI_API int
ui_slide_int(struct ui_context *ctx, int min, int val, int max, int step)
{
    float value = (float)val;
    ui_slider_float(ctx, (float)min, &value, (float)max, (float)step);
    return (int)value;
}

UI_API int
ui_slider_int(struct ui_context *ctx, int min, int *val, int max, int step)
{
    int ret;
    float value = (float)*val;
    ret = ui_slider_float(ctx, (float)min, &value, (float)max, (float)step);
    *val =  (int)value;
    return ret;
}

/*----------------------------------------------------------------
 *
 *                          PROGRESSBAR
 *
 * --------------------------------------------------------------*/
UI_API int
ui_progress(struct ui_context *ctx, ui_size *cur, ui_size max, int is_modifyable)
{
    struct ui_window *win;
    struct ui_panel *layout;
    const struct ui_style *style;
    const struct ui_input *in;

    struct ui_rect bounds;
    enum ui_widget_layout_states state;
    ui_size old_value;

    UI_ASSERT(ctx);
    UI_ASSERT(cur);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout || !cur)
        return 0;

    win = ctx->current;
    style = &ctx->style;
    layout = win->layout;
    state = ui_widget(&bounds, ctx);
    if (!state) return 0;

    in = (state == UI_WIDGET_ROM || layout->flags & UI_WINDOW_ROM) ? 0 : &ctx->input;
    old_value = *cur;
    *cur = ui_do_progress(&ctx->last_widget_state, &win->buffer, bounds,
            *cur, max, is_modifyable, &style->progress, in);
    return (*cur != old_value);
}

UI_API ui_size ui_prog(struct ui_context *ctx, ui_size cur, ui_size max, int modifyable)
{ui_progress(ctx, &cur, max, modifyable);return cur;}

/*----------------------------------------------------------------
 *
 *                          EDIT
 *
 * --------------------------------------------------------------*/
UI_API void
ui_edit_focus(struct ui_context *ctx, ui_flags flags)
{
    ui_hash hash;
    struct ui_window *win;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    if (!ctx || !ctx->current) return;

    win = ctx->current;
    hash = win->edit.seq;
    win->edit.active = ui_true;
    win->edit.name = hash;
    if (flags & UI_EDIT_ALWAYS_INSERT_MODE)
        win->edit.mode = UI_TEXT_EDIT_MODE_INSERT;
}

UI_API void
ui_edit_unfocus(struct ui_context *ctx)
{
    struct ui_window *win;
    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    if (!ctx || !ctx->current) return;

    win = ctx->current;
    win->edit.active = ui_false;
    win->edit.name = 0;
}

UI_API ui_flags
ui_edit_string(struct ui_context *ctx, ui_flags flags,
    char *memory, int *len, int max, ui_plugin_filter filter)
{
    ui_hash hash;
    ui_flags state;
    struct ui_text_edit *edit;
    struct ui_window *win;

    UI_ASSERT(ctx);
    UI_ASSERT(memory);
    UI_ASSERT(len);
    if (!ctx || !memory || !len)
        return 0;

    filter = (!filter) ? ui_filter_default: filter;
    win = ctx->current;
    hash = win->edit.seq;
    edit = &ctx->text_edit;
    ui_textedit_clear_state(&ctx->text_edit, (flags & UI_EDIT_MULTILINE)?
        UI_TEXT_EDIT_MULTI_LINE: UI_TEXT_EDIT_SINGLE_LINE, filter);

    if (win->edit.active && hash == win->edit.name) {
        if (flags & UI_EDIT_NO_CURSOR)
            edit->cursor = ui_utf_len(memory, *len);
        else edit->cursor = win->edit.cursor;
        if (!(flags & UI_EDIT_SELECTABLE)) {
            edit->select_start = win->edit.cursor;
            edit->select_end = win->edit.cursor;
        } else {
            edit->select_start = win->edit.sel_start;
            edit->select_end = win->edit.sel_end;
        }
        edit->mode = win->edit.mode;
        edit->scrollbar.x = (float)win->edit.scrollbar.x;
        edit->scrollbar.y = (float)win->edit.scrollbar.y;
        edit->active = ui_true;
    } else edit->active = ui_false;

    max = UI_MAX(1, max);
    *len = UI_MIN(*len, max-1);
    ui_str_init_fixed(&edit->string, memory, (ui_size)max);
    edit->string.buffer.allocated = (ui_size)*len;
    edit->string.len = ui_utf_len(memory, *len);
    state = ui_edit_buffer(ctx, flags, edit, filter);
    *len = (int)edit->string.buffer.allocated;

    if (edit->active) {
        win->edit.cursor = edit->cursor;
        win->edit.sel_start = edit->select_start;
        win->edit.sel_end = edit->select_end;
        win->edit.mode = edit->mode;
        win->edit.scrollbar.x = (ui_ushort)edit->scrollbar.x;
        win->edit.scrollbar.y = (ui_ushort)edit->scrollbar.y;
    }
    return state;
}

UI_API ui_flags
ui_edit_buffer(struct ui_context *ctx, ui_flags flags,
    struct ui_text_edit *edit, ui_plugin_filter filter)
{
    struct ui_window *win;
    struct ui_style *style;
    struct ui_input *in;

    enum ui_widget_layout_states state;
    struct ui_rect bounds;

    ui_flags ret_flags = 0;
    unsigned char prev_state;
    ui_hash hash;

    /* make sure correct values */
    UI_ASSERT(ctx);
    UI_ASSERT(edit);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return 0;

    win = ctx->current;
    style = &ctx->style;
    state = ui_widget(&bounds, ctx);
    if (!state) return state;
    in = (win->layout->flags & UI_WINDOW_ROM) ? 0 : &ctx->input;

    /* check if edit is currently hot item */
    hash = win->edit.seq++;
    if (win->edit.active && hash == win->edit.name) {
        if (flags & UI_EDIT_NO_CURSOR)
            edit->cursor = edit->string.len;
        if (!(flags & UI_EDIT_SELECTABLE)) {
            edit->select_start = edit->cursor;
            edit->select_end = edit->cursor;
        }
        if (flags & UI_EDIT_CLIPBOARD)
            edit->clip = ctx->clip;
    }

    filter = (!filter) ? ui_filter_default: filter;
    prev_state = (unsigned char)edit->active;
    in = (flags & UI_EDIT_READ_ONLY) ? 0: in;
    ret_flags = ui_do_edit(&ctx->last_widget_state, &win->buffer, bounds, flags,
                    filter, edit, &style->edit, in, style->font);

    if (ctx->last_widget_state & UI_WIDGET_STATE_HOVER)
        ctx->style.cursor_active = ctx->style.cursors[UI_CURSOR_TEXT];
    if (edit->active && prev_state != edit->active) {
        /* current edit is now hot */
        win->edit.active = ui_true;
        win->edit.name = hash;
    } else if (prev_state && !edit->active) {
        /* current edit is now cold */
        win->edit.active = ui_false;
    }
    return ret_flags;
}

UI_API ui_flags
ui_edit_string_zero_terminated(struct ui_context *ctx, ui_flags flags,
    char *buffer, int max, ui_plugin_filter filter)
{
    ui_flags result;
    int len = ui_strlen(buffer);
    result = ui_edit_string(ctx, flags, buffer, &len, max, filter);
    buffer[UI_MIN(UI_MAX(max-1,0), len)] = '\0';
    return result;
}

/*----------------------------------------------------------------
 *
 *                          PROPERTY
 *
 * --------------------------------------------------------------*/
UI_INTERN struct ui_property_variant
ui_property_variant_int(int value, int min_value, int max_value, int step)
{
    struct ui_property_variant result;
    result.kind = UI_PROPERTY_INT;
    result.value.i = value;
    result.min_value.i = min_value;
    result.max_value.i = max_value;
    result.step.i = step;
    return result;
}

UI_INTERN struct ui_property_variant
ui_property_variant_float(float value, float min_value, float max_value, float step)
{
    struct ui_property_variant result;
    result.kind = UI_PROPERTY_FLOAT;
    result.value.f = value;
    result.min_value.f = min_value;
    result.max_value.f = max_value;
    result.step.f = step;
    return result;
}

UI_INTERN struct ui_property_variant
ui_property_variant_double(double value, double min_value, double max_value,
    double step)
{
    struct ui_property_variant result;
    result.kind = UI_PROPERTY_DOUBLE;
    result.value.d = value;
    result.min_value.d = min_value;
    result.max_value.d = max_value;
    result.step.d = step;
    return result;
}

UI_INTERN void
ui_property(struct ui_context *ctx, const char *name, struct ui_property_variant *variant,
    float inc_per_pixel, const enum ui_property_filter filter)
{
    struct ui_window *win;
    struct ui_panel *layout;
    struct ui_input *in;
    const struct ui_style *style;

    struct ui_rect bounds;
    enum ui_widget_layout_states s;

    int *state = 0;
    ui_hash hash = 0;
    char *buffer = 0;
    int *len = 0;
    int *cursor = 0;
    int old_state;

    char dummy_buffer[UI_MAX_NUMBER_BUFFER];
    int dummy_state = UI_PROPERTY_DEFAULT;
    int dummy_length = 0;
    int dummy_cursor = 0;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return;

    win = ctx->current;
    layout = win->layout;
    style = &ctx->style;
    s = ui_widget(&bounds, ctx);
    if (!s) return;
    in = (s == UI_WIDGET_ROM || layout->flags & UI_WINDOW_ROM) ? 0 : &ctx->input;

    /* calculate hash from name */
    if (name[0] == '#') {
        hash = ui_murmur_hash(name, (int)ui_strlen(name), win->property.seq++);
        name++; /* special number hash */
    } else hash = ui_murmur_hash(name, (int)ui_strlen(name), 42);

    /* check if property is currently hot item */
    if (win->property.active && hash == win->property.name) {
        buffer = win->property.buffer;
        len = &win->property.length;
        cursor = &win->property.cursor;
        state = &win->property.state;
    } else {
        buffer = dummy_buffer;
        len = &dummy_length;
        cursor = &dummy_cursor;
        state = &dummy_state;
    }

    /* execute property widget */
    old_state = *state;
    ui_do_property(&ctx->last_widget_state, &win->buffer, bounds, name,
        variant, inc_per_pixel, buffer, len, state, cursor,
        &style->property, filter, in, style->font, &ctx->text_edit);

    if (in && *state != UI_PROPERTY_DEFAULT && !win->property.active) {
        /* current property is now hot */
        win->property.active = 1;
        UI_MEMCPY(win->property.buffer, buffer, (ui_size)*len);
        win->property.length = *len;
        win->property.cursor = *cursor;
        win->property.state = *state;
        win->property.name = hash;
        if (*state == UI_PROPERTY_DRAG) {
            ctx->input.mouse.grab = ui_true;
            ctx->input.mouse.grabbed = ui_true;
        }
    }

    /* check if previously active property is now inactive */
    if (*state == UI_PROPERTY_DEFAULT && old_state != UI_PROPERTY_DEFAULT) {
        if (old_state == UI_PROPERTY_DRAG) {
            ctx->input.mouse.grab = ui_false;
            ctx->input.mouse.grabbed = ui_false;
            ctx->input.mouse.ungrab = ui_true;
        }
        win->property.active = 0;
    }
}

UI_API void
ui_property_int(struct ui_context *ctx, const char *name,
    int min, int *val, int max, int step, float inc_per_pixel)
{
    struct ui_property_variant variant;
    UI_ASSERT(ctx);
    UI_ASSERT(name);
    UI_ASSERT(val);

    if (!ctx || !ctx->current || !name || !val) return;
    variant = ui_property_variant_int(*val, min, max, step);
    ui_property(ctx, name, &variant, inc_per_pixel, UI_FILTER_INT);
    *val = variant.value.i;
}

UI_API void
ui_property_float(struct ui_context *ctx, const char *name,
    float min, float *val, float max, float step, float inc_per_pixel)
{
    struct ui_property_variant variant;
    UI_ASSERT(ctx);
    UI_ASSERT(name);
    UI_ASSERT(val);

    if (!ctx || !ctx->current || !name || !val) return;
    variant = ui_property_variant_float(*val, min, max, step);
    ui_property(ctx, name, &variant, inc_per_pixel, UI_FILTER_FLOAT);
    *val = variant.value.f;
}

UI_API void
ui_property_double(struct ui_context *ctx, const char *name,
    double min, double *val, double max, double step, float inc_per_pixel)
{
    struct ui_property_variant variant;
    UI_ASSERT(ctx);
    UI_ASSERT(name);
    UI_ASSERT(val);

    if (!ctx || !ctx->current || !name || !val) return;
    variant = ui_property_variant_double(*val, min, max, step);
    ui_property(ctx, name, &variant, inc_per_pixel, UI_FILTER_FLOAT);
    *val = variant.value.d;
}

UI_API int
ui_propertyi(struct ui_context *ctx, const char *name, int min, int val,
    int max, int step, float inc_per_pixel)
{
    struct ui_property_variant variant;
    UI_ASSERT(ctx);
    UI_ASSERT(name);

    if (!ctx || !ctx->current || !name) return val;
    variant = ui_property_variant_int(val, min, max, step);
    ui_property(ctx, name, &variant, inc_per_pixel, UI_FILTER_INT);
    val = variant.value.i;
    return val;
}

UI_API float
ui_propertyf(struct ui_context *ctx, const char *name, float min,
    float val, float max, float step, float inc_per_pixel)
{
    struct ui_property_variant variant;
    UI_ASSERT(ctx);
    UI_ASSERT(name);

    if (!ctx || !ctx->current || !name) return val;
    variant = ui_property_variant_float(val, min, max, step);
    ui_property(ctx, name, &variant, inc_per_pixel, UI_FILTER_FLOAT);
    val = variant.value.f;
    return val;
}

UI_API double
ui_propertyd(struct ui_context *ctx, const char *name, double min,
    double val, double max, double step, float inc_per_pixel)
{
    struct ui_property_variant variant;
    UI_ASSERT(ctx);
    UI_ASSERT(name);

    if (!ctx || !ctx->current || !name) return val;
    variant = ui_property_variant_double(val, min, max, step);
    ui_property(ctx, name, &variant, inc_per_pixel, UI_FILTER_FLOAT);
    val = variant.value.d;
    return val;
}

/*----------------------------------------------------------------
 *
 *                          COLOR PICKER
 *
 * --------------------------------------------------------------*/
UI_API int
ui_color_pick(struct ui_context * ctx, struct ui_color *color,
    enum ui_color_format fmt)
{
    struct ui_window *win;
    struct ui_panel *layout;
    const struct ui_style *config;
    const struct ui_input *in;

    enum ui_widget_layout_states state;
    struct ui_rect bounds;

    UI_ASSERT(ctx);
    UI_ASSERT(color);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout || !color)
        return 0;

    win = ctx->current;
    config = &ctx->style;
    layout = win->layout;
    state = ui_widget(&bounds, ctx);
    if (!state) return 0;
    in = (state == UI_WIDGET_ROM || layout->flags & UI_WINDOW_ROM) ? 0 : &ctx->input;
    return ui_do_color_picker(&ctx->last_widget_state, &win->buffer, color, fmt, bounds,
                ui_vec2(0,0), in, config->font);
}

UI_API struct ui_color
ui_color_picker(struct ui_context *ctx, struct ui_color color,
    enum ui_color_format fmt)
{
    ui_color_pick(ctx, &color, fmt);
    return color;
}

/* -------------------------------------------------------------
 *
 *                          CHART
 *
 * --------------------------------------------------------------*/
UI_API int
ui_chart_begin_colored(struct ui_context *ctx, enum ui_chart_type type,
    struct ui_color color, struct ui_color highlight,
    int count, float min_value, float max_value)
{
    struct ui_window *win;
    struct ui_chart *chart;
    const struct ui_style *config;
    const struct ui_style_chart *style;

    const struct ui_style_item *background;
    struct ui_rect bounds = {0, 0, 0, 0};

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);

    if (!ctx || !ctx->current || !ctx->current->layout) return 0;
    if (!ui_widget(&bounds, ctx)) {
        chart = &ctx->current->layout->chart;
        ui_zero(chart, sizeof(*chart));
        return 0;
    }

    win = ctx->current;
    config = &ctx->style;
    chart = &win->layout->chart;
    style = &config->chart;

    /* setup basic generic chart  */
    ui_zero(chart, sizeof(*chart));
    chart->x = bounds.x + style->padding.x;
    chart->y = bounds.y + style->padding.y;
    chart->w = bounds.w - 2 * style->padding.x;
    chart->h = bounds.h - 2 * style->padding.y;
    chart->w = UI_MAX(chart->w, 2 * style->padding.x);
    chart->h = UI_MAX(chart->h, 2 * style->padding.y);

    /* add first slot into chart */
    {struct ui_chart_slot *slot = &chart->slots[chart->slot++];
    slot->type = type;
    slot->count = count;
    slot->color = color;
    slot->highlight = highlight;
    slot->min = UI_MIN(min_value, max_value);
    slot->max = UI_MAX(min_value, max_value);
    slot->range = slot->max - slot->min;}

    /* draw chart background */
    background = &style->background;
    if (background->type == UI_STYLE_ITEM_IMAGE) {
        ui_draw_image(&win->buffer, bounds, &background->data.image, ui_white);
    } else {
        ui_fill_rect(&win->buffer, bounds, style->rounding, style->border_color);
        ui_fill_rect(&win->buffer, ui_shriui_rect(bounds, style->border),
            style->rounding, style->background.data.color);
    }
    return 1;
}

UI_API int
ui_chart_begin(struct ui_context *ctx, const enum ui_chart_type type,
    int count, float min_value, float max_value)
{return ui_chart_begin_colored(ctx, type, ctx->style.chart.color, ctx->style.chart.selected_color, count, min_value, max_value);}

UI_API void
ui_chart_add_slot_colored(struct ui_context *ctx, const enum ui_chart_type type,
    struct ui_color color, struct ui_color highlight,
    int count, float min_value, float max_value)
{
    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    UI_ASSERT(ctx->current->layout->chart.slot < UI_CHART_MAX_SLOT);
    if (!ctx || !ctx->current || !ctx->current->layout) return;
    if (ctx->current->layout->chart.slot >= UI_CHART_MAX_SLOT) return;

    /* add another slot into the graph */
    {struct ui_chart *chart = &ctx->current->layout->chart;
    struct ui_chart_slot *slot = &chart->slots[chart->slot++];
    slot->type = type;
    slot->count = count;
    slot->color = color;
    slot->highlight = highlight;
    slot->min = UI_MIN(min_value, max_value);
    slot->max = UI_MAX(min_value, max_value);
    slot->range = slot->max - slot->min;}
}

UI_API void
ui_chart_add_slot(struct ui_context *ctx, const enum ui_chart_type type,
    int count, float min_value, float max_value)
{ui_chart_add_slot_colored(ctx, type, ctx->style.chart.color, ctx->style.chart.selected_color, count, min_value, max_value);}

UI_INTERN ui_flags
ui_chart_push_line(struct ui_context *ctx, struct ui_window *win,
    struct ui_chart *g, float value, int slot)
{
    struct ui_panel *layout = win->layout;
    const struct ui_input *i = &ctx->input;
    struct ui_command_buffer *out = &win->buffer;

    ui_flags ret = 0;
    struct ui_vec2 cur;
    struct ui_rect bounds;
    struct ui_color color;
    float step;
    float range;
    float ratio;

    UI_ASSERT(slot >= 0 && slot < UI_CHART_MAX_SLOT);
    step = g->w / (float)g->slots[slot].count;
    range = g->slots[slot].max - g->slots[slot].min;
    ratio = (value - g->slots[slot].min) / range;

    if (g->slots[slot].index == 0) {
        /* first data point does not have a connection */
        g->slots[slot].last.x = g->x;
        g->slots[slot].last.y = (g->y + g->h) - ratio * (float)g->h;

        bounds.x = g->slots[slot].last.x - 2;
        bounds.y = g->slots[slot].last.y - 2;
        bounds.w = bounds.h = 4;

        color = g->slots[slot].color;
        if (!(layout->flags & UI_WINDOW_ROM) &&
            UI_INBOX(i->mouse.pos.x,i->mouse.pos.y, g->slots[slot].last.x-3, g->slots[slot].last.y-3, 6, 6)){
            ret = ui_input_is_mouse_hovering_rect(i, bounds) ? UI_CHART_HOVERING : 0;
            ret |= (i->mouse.buttons[UI_BUTTON_LEFT].down &&
                i->mouse.buttons[UI_BUTTON_LEFT].clicked) ? UI_CHART_CLICKED: 0;
            color = g->slots[slot].highlight;
        }
        ui_fill_rect(out, bounds, 0, color);
        g->slots[slot].index += 1;
        return ret;
    }

    /* draw a line between the last data point and the new one */
    color = g->slots[slot].color;
    cur.x = g->x + (float)(step * (float)g->slots[slot].index);
    cur.y = (g->y + g->h) - (ratio * (float)g->h);
    ui_stroke_line(out, g->slots[slot].last.x, g->slots[slot].last.y, cur.x, cur.y, 1.0f, color);

    bounds.x = cur.x - 3;
    bounds.y = cur.y - 3;
    bounds.w = bounds.h = 6;

    /* user selection of current data point */
    if (!(layout->flags & UI_WINDOW_ROM)) {
        if (ui_input_is_mouse_hovering_rect(i, bounds)) {
            ret = UI_CHART_HOVERING;
            ret |= (!i->mouse.buttons[UI_BUTTON_LEFT].down &&
                i->mouse.buttons[UI_BUTTON_LEFT].clicked) ? UI_CHART_CLICKED: 0;
            color = g->slots[slot].highlight;
        }
    }
    ui_fill_rect(out, ui_rect(cur.x - 2, cur.y - 2, 4, 4), 0, color);

    /* save current data point position */
    g->slots[slot].last.x = cur.x;
    g->slots[slot].last.y = cur.y;
    g->slots[slot].index  += 1;
    return ret;
}

UI_INTERN ui_flags
ui_chart_push_column(const struct ui_context *ctx, struct ui_window *win,
    struct ui_chart *chart, float value, int slot)
{
    struct ui_command_buffer *out = &win->buffer;
    const struct ui_input *in = &ctx->input;
    struct ui_panel *layout = win->layout;

    float ratio;
    ui_flags ret = 0;
    struct ui_color color;
    struct ui_rect item = {0,0,0,0};

    UI_ASSERT(slot >= 0 && slot < UI_CHART_MAX_SLOT);
    if (chart->slots[slot].index  >= chart->slots[slot].count)
        return ui_false;
    if (chart->slots[slot].count) {
        float padding = (float)(chart->slots[slot].count-1);
        item.w = (chart->w - padding) / (float)(chart->slots[slot].count);
    }

    /* calculate bounds of current bar chart entry */
    color = chart->slots[slot].color;;
    item.h = chart->h * UI_ABS((value/chart->slots[slot].range));
    if (value >= 0) {
        ratio = (value + UI_ABS(chart->slots[slot].min)) / UI_ABS(chart->slots[slot].range);
        item.y = (chart->y + chart->h) - chart->h * ratio;
    } else {
        ratio = (value - chart->slots[slot].max) / chart->slots[slot].range;
        item.y = chart->y + (chart->h * UI_ABS(ratio)) - item.h;
    }
    item.x = chart->x + ((float)chart->slots[slot].index * item.w);
    item.x = item.x + ((float)chart->slots[slot].index);

    /* user chart bar selection */
    if (!(layout->flags & UI_WINDOW_ROM) &&
        UI_INBOX(in->mouse.pos.x,in->mouse.pos.y,item.x,item.y,item.w,item.h)) {
        ret = UI_CHART_HOVERING;
        ret |= (!in->mouse.buttons[UI_BUTTON_LEFT].down &&
                in->mouse.buttons[UI_BUTTON_LEFT].clicked) ? UI_CHART_CLICKED: 0;
        color = chart->slots[slot].highlight;
    }
    ui_fill_rect(out, item, 0, color);
    chart->slots[slot].index += 1;
    return ret;
}

UI_API ui_flags
ui_chart_push_slot(struct ui_context *ctx, float value, int slot)
{
    ui_flags flags;
    struct ui_window *win;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(slot >= 0 && slot < UI_CHART_MAX_SLOT);
    UI_ASSERT(slot < ctx->current->layout->chart.slot);
    if (!ctx || !ctx->current || slot >= UI_CHART_MAX_SLOT) return ui_false;
    if (slot >= ctx->current->layout->chart.slot) return ui_false;

    win = ctx->current;
    if (win->layout->chart.slot < slot) return ui_false;
    switch (win->layout->chart.slots[slot].type) {
    case UI_CHART_LINES:
        flags = ui_chart_push_line(ctx, win, &win->layout->chart, value, slot); break;
    case UI_CHART_COLUMN:
        flags = ui_chart_push_column(ctx, win, &win->layout->chart, value, slot); break;
    default:
    case UI_CHART_MAX:
        flags = 0;
    }
    return flags;
}

UI_API ui_flags
ui_chart_push(struct ui_context *ctx, float value)
{return ui_chart_push_slot(ctx, value, 0);}

UI_API void
ui_chart_end(struct ui_context *ctx)
{
    struct ui_window *win;
    struct ui_chart *chart;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    if (!ctx || !ctx->current)
        return;

    win = ctx->current;
    chart = &win->layout->chart;
    UI_MEMSET(chart, 0, sizeof(*chart));
    return;
}

UI_API void
ui_plot(struct ui_context *ctx, enum ui_chart_type type, const float *values,
    int count, int offset)
{
    int i = 0;
    float min_value;
    float max_value;

    UI_ASSERT(ctx);
    UI_ASSERT(values);
    if (!ctx || !values || !count) return;

    min_value = values[offset];
    max_value = values[offset];
    for (i = 0; i < count; ++i) {
        min_value = UI_MIN(values[i + offset], min_value);
        max_value = UI_MAX(values[i + offset], max_value);
    }
    ui_chart_begin(ctx, type, count, min_value, max_value);
    for (i = 0; i < count; ++i)
        ui_chart_push(ctx, values[i + offset]);
    ui_chart_end(ctx);
}

UI_API void
ui_plot_function(struct ui_context *ctx, enum ui_chart_type type, void *userdata,
    float(*value_getter)(void* user, int index), int count, int offset)
{
    int i = 0;
    float min_value;
    float max_value;

    UI_ASSERT(ctx);
    UI_ASSERT(value_getter);
    if (!ctx || !value_getter || !count) return;

    max_value = min_value = value_getter(userdata, offset);
    for (i = 0; i < count; ++i) {
        float value = value_getter(userdata, i + offset);
        min_value = UI_MIN(value, min_value);
        max_value = UI_MAX(value, max_value);
    }
    ui_chart_begin(ctx, type, count, min_value, max_value);
    for (i = 0; i < count; ++i)
        ui_chart_push(ctx, value_getter(userdata, i + offset));
    ui_chart_end(ctx);
}

/* -------------------------------------------------------------
 *
 *                          GROUP
 *
 * --------------------------------------------------------------*/
UI_API int
ui_group_scrolled_offset_begin(struct ui_context *ctx,
    ui_uint *x_offset, ui_uint *y_offset, const char *title, ui_flags flags)
{
    struct ui_rect bounds;
    struct ui_window panel;
    struct ui_window *win;

    win = ctx->current;
    ui_panel_alloc_space(&bounds, ctx);

    {const struct ui_rect *c = &win->layout->clip;
    if (!UI_INTERSECT(c->x, c->y, c->w, c->h, bounds.x, bounds.y, bounds.w, bounds.h) &&
        !(flags & UI_WINDOW_MOVABLE)) {
        return 0;
    }}
    if (win->flags & UI_WINDOW_ROM)
        flags |= UI_WINDOW_ROM;

    /* initialize a fake window to create the layout from */
    ui_zero(&panel, sizeof(panel));
    panel.bounds = bounds;
    panel.flags = flags;
    panel.scrollbar.x = *x_offset;
    panel.scrollbar.y = *y_offset;
    panel.buffer = win->buffer;
    panel.layout = (struct ui_panel*)ui_create_panel(ctx);
    ctx->current = &panel;
    ui_panel_begin(ctx, (flags & UI_WINDOW_TITLE) ? title: 0, UI_PANEL_GROUP);

    win->buffer = panel.buffer;
    win->buffer.clip = panel.layout->clip;
    panel.layout->offset_x = x_offset;
    panel.layout->offset_y = y_offset;
    panel.layout->parent = win->layout;
    win->layout = panel.layout;
    ctx->current = win;
    return 1;
}

UI_API void
ui_group_scrolled_end(struct ui_context *ctx)
{
    struct ui_window *win;
    struct ui_panel *parent;
    struct ui_panel *g;

    struct ui_rect clip;
    struct ui_window pan;
    struct ui_vec2 panel_padding;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    if (!ctx || !ctx->current)
        return;

    /* make sure ui_group_begin was called correctly */
    UI_ASSERT(ctx->current);
    win = ctx->current;
    UI_ASSERT(win->layout);
    g = win->layout;
    UI_ASSERT(g->parent);
    parent = g->parent;

    /* dummy window */
    ui_zero_struct(pan);
    panel_padding = ui_panel_get_padding(&ctx->style, UI_PANEL_GROUP);
    pan.bounds.y = g->bounds.y - (g->header_height + g->menu.h);
    pan.bounds.x = g->bounds.x - panel_padding.x;
    pan.bounds.w = g->bounds.w + 2 * panel_padding.x;
    pan.bounds.h = g->bounds.h + g->header_height + g->menu.h;
    if (g->flags & UI_WINDOW_BORDER) {
        pan.bounds.x -= g->border;
        pan.bounds.y -= g->border;
        pan.bounds.w += 2*g->border;
        pan.bounds.h += 2*g->border;
    }
    if (!(g->flags & UI_WINDOW_NO_SCROLLBAR)) {
        pan.bounds.w += ctx->style.window.scrollbar_size.x;
        pan.bounds.h += ctx->style.window.scrollbar_size.y;
    }
    pan.scrollbar.x = *g->offset_x;
    pan.scrollbar.y = *g->offset_y;
    pan.flags = g->flags;
    pan.buffer = win->buffer;
    pan.layout = g;
    pan.parent = win;
    ctx->current = &pan;

    /* make sure group has correct clipping rectangle */
    ui_unify(&clip, &parent->clip, pan.bounds.x, pan.bounds.y,
        pan.bounds.x + pan.bounds.w, pan.bounds.y + pan.bounds.h + panel_padding.x);
    ui_push_scissor(&pan.buffer, clip);
    ui_end(ctx);

    win->buffer = pan.buffer;
    ui_push_scissor(&win->buffer, parent->clip);
    ctx->current = win;
    win->layout = parent;
    g->bounds = pan.bounds;
    return;
}

UI_API int
ui_group_scrolled_begin(struct ui_context *ctx,
    struct ui_scroll *scroll, const char *title, ui_flags flags)
{return ui_group_scrolled_offset_begin(ctx, &scroll->x, &scroll->y, title, flags);}

UI_API int
ui_group_begin(struct ui_context *ctx, const char *title, ui_flags flags)
{
    int title_len;
    ui_hash title_hash;
    struct ui_window *win;
    ui_uint *x_offset;
    ui_uint *y_offset;

    UI_ASSERT(ctx);
    UI_ASSERT(title);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout || !title)
        return 0;

    /* find persistent group scrollbar value */
    win = ctx->current;
    title_len = (int)ui_strlen(title);
    title_hash = ui_murmur_hash(title, (int)title_len, UI_PANEL_GROUP);
    x_offset = ui_find_value(win, title_hash);
    if (!x_offset) {
        x_offset = ui_add_value(ctx, win, title_hash, 0);
        y_offset = ui_add_value(ctx, win, title_hash+1, 0);

        UI_ASSERT(x_offset);
        UI_ASSERT(y_offset);
        if (!x_offset || !y_offset) return 0;
        *x_offset = *y_offset = 0;
    } else y_offset = ui_find_value(win, title_hash+1);
    return ui_group_scrolled_offset_begin(ctx, x_offset, y_offset, title, flags);
}

UI_API void
ui_group_end(struct ui_context *ctx)
{ui_group_scrolled_end(ctx);}

UI_API int
ui_list_view_begin(struct ui_context *ctx, struct ui_list_view *view,
    const char *title, ui_flags flags, int row_height, int row_count)
{
    int title_len;
    ui_hash title_hash;
    ui_uint *x_offset;
    ui_uint *y_offset;

    int result;
    struct ui_window *win;
    struct ui_panel *layout;
    const struct ui_style *style;
    struct ui_vec2 item_spacing;

    UI_ASSERT(ctx);
    UI_ASSERT(view);
    UI_ASSERT(title);
    if (!ctx || !view || !title) return 0;

    win = ctx->current;
    style = &ctx->style;
    item_spacing = style->window.spacing;
    row_height += UI_MAX(0, (int)item_spacing.y);

    /* find persistent list view scrollbar offset */
    title_len = (int)ui_strlen(title);
    title_hash = ui_murmur_hash(title, (int)title_len, UI_PANEL_GROUP);
    x_offset = ui_find_value(win, title_hash);
    if (!x_offset) {
        x_offset = ui_add_value(ctx, win, title_hash, 0);
        y_offset = ui_add_value(ctx, win, title_hash+1, 0);

        UI_ASSERT(x_offset);
        UI_ASSERT(y_offset);
        if (!x_offset || !y_offset) return 0;
        *x_offset = *y_offset = 0;
    } else y_offset = ui_find_value(win, title_hash+1);
    view->scroll_value = *y_offset;
    view->scroll_pointer = y_offset;

    *y_offset = 0;
    result = ui_group_scrolled_offset_begin(ctx, x_offset, y_offset, title, flags);
    win = ctx->current;
    layout = win->layout;
 
    view->total_height = row_height * UI_MAX(row_count,1);
    view->begin = (int)UI_MAX(((float)view->scroll_value / (float)row_height), 0.0f);
    view->count = (int)UI_MAX(ui_iceilf((layout->clip.h)/(float)row_height), 0);
    view->end = view->begin + view->count;
    view->ctx = ctx;
    return result;
}

UI_API void
ui_list_view_end(struct ui_list_view *view)
{
    struct ui_context *ctx;
    struct ui_window *win;
    struct ui_panel *layout;

    UI_ASSERT(view);
    UI_ASSERT(view->ctx);
    UI_ASSERT(view->scroll_pointer);
    if (!view || !view->ctx) return;

    ctx = view->ctx;
    win = ctx->current;
    layout = win->layout;
    layout->at_y = layout->bounds.y + (float)view->total_height;
    *view->scroll_pointer = *view->scroll_pointer + view->scroll_value;
    ui_group_end(view->ctx);
}

/* --------------------------------------------------------------
 *
 *                          POPUP
 *
 * --------------------------------------------------------------*/
UI_API int
ui_popup_begin(struct ui_context *ctx, enum ui_popup_type type,
    const char *title, ui_flags flags, struct ui_rect rect)
{
    struct ui_window *popup;
    struct ui_window *win;
    struct ui_panel *panel;

    int title_len;
    ui_hash title_hash;
    ui_size allocated;

    UI_ASSERT(ctx);
    UI_ASSERT(title);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return 0;

    win = ctx->current;
    panel = win->layout;
    UI_ASSERT(!(panel->type & UI_PANEL_SET_POPUP) && "popups are not allowed to have popups");
    (void)panel;
    title_len = (int)ui_strlen(title);
    title_hash = ui_murmur_hash(title, (int)title_len, UI_PANEL_POPUP);

    popup = win->popup.win;
    if (!popup) {
        popup = (struct ui_window*)ui_create_window(ctx);
        popup->parent = win;
        win->popup.win = popup;
        win->popup.active = 0;
        win->popup.type = UI_PANEL_POPUP;
    }

    /* make sure we have to correct popup */
    if (win->popup.name != title_hash) {
        if (!win->popup.active) {
            ui_zero(popup, sizeof(*popup));
            win->popup.name = title_hash;
            win->popup.active = 1;
            win->popup.type = UI_PANEL_POPUP;
        } else return 0;
    }

    /* popup position is local to window */
    ctx->current = popup;
    rect.x += win->layout->clip.x;
    rect.y += win->layout->clip.y;

    /* setup popup data */
    popup->parent = win;
    popup->bounds = rect;
    popup->seq = ctx->seq;
    popup->layout = (struct ui_panel*)ui_create_panel(ctx);
    popup->flags = flags;
    popup->flags |= UI_WINDOW_BORDER;
    if (type == UI_POPUP_DYNAMIC)
        popup->flags |= UI_WINDOW_DYNAMIC;

    popup->buffer = win->buffer;
    ui_start_popup(ctx, win);
    allocated = ctx->memory.allocated;
    ui_push_scissor(&popup->buffer, ui_null_rect);

    if (ui_panel_begin(ctx, title, UI_PANEL_POPUP)) {
        /* popup is running therefore invalidate parent panels */
        struct ui_panel *root;
        root = win->layout;
        while (root) {
            root->flags |= UI_WINDOW_ROM;
            root->flags &= ~(ui_flags)UI_WINDOW_REMOVE_ROM;
            root = root->parent;
        }
        win->popup.active = 1;
        popup->layout->offset_x = &popup->scrollbar.x;
        popup->layout->offset_y = &popup->scrollbar.y;
        popup->layout->parent = win->layout;
        return 1;
    } else {
        /* popup was closed/is invalid so cleanup */
        struct ui_panel *root;
        root = win->layout;
        while (root) {
            root->flags |= UI_WINDOW_REMOVE_ROM;
            root = root->parent;
        }
        win->layout->popup_buffer.active = 0;
        win->popup.active = 0;
        ctx->memory.allocated = allocated;
        ctx->current = win;
        ui_free_panel(ctx, popup->layout);
        popup->layout = 0;
        return 0;
    }
}

UI_INTERN int
ui_nonblock_begin(struct ui_context *ctx,
    ui_flags flags, struct ui_rect body, struct ui_rect header,
    enum ui_panel_type panel_type)
{
    struct ui_window *popup;
    struct ui_window *win;
    struct ui_panel *panel;
    int is_active = ui_true;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return 0;

    /* popups cannot have popups */
    win = ctx->current;
    panel = win->layout;
    UI_ASSERT(!(panel->type & UI_PANEL_SET_POPUP));
    (void)panel; 
    popup = win->popup.win;
    if (!popup) {
        /* create window for nonblocking popup */
        popup = (struct ui_window*)ui_create_window(ctx);
        popup->parent = win;
        win->popup.win = popup;
        win->popup.type = panel_type;
        ui_command_buffer_init(&popup->buffer, &ctx->memory, UI_CLIPPING_ON);
    } else {
        /* close the popup if user pressed outside or in the header */
        int pressed, in_body, in_header;
        pressed = ui_input_is_mouse_pressed(&ctx->input, UI_BUTTON_LEFT);
        in_body = ui_input_is_mouse_hovering_rect(&ctx->input, body);
        in_header = ui_input_is_mouse_hovering_rect(&ctx->input, header);
        if (pressed && (!in_body || in_header))
            is_active = ui_false;
    }
    win->popup.header = header;

    if (!is_active) {
        /* remove read only mode from all parent panels */
        struct ui_panel *root;
        root = win->layout;
        while (root) {
            root->flags |= UI_WINDOW_REMOVE_ROM;
            root = root->parent;
        }
        return is_active;
    }

    popup->bounds = body;
    popup->parent = win;
    popup->layout = (struct ui_panel*)ui_create_panel(ctx);
    popup->flags = flags;
    popup->flags |= UI_WINDOW_BORDER;
    popup->flags |= UI_WINDOW_DYNAMIC;
    popup->seq = ctx->seq;
    win->popup.active = 1;
    UI_ASSERT(popup->layout);

    ui_start_popup(ctx, win);
    popup->buffer = win->buffer;
    ui_push_scissor(&popup->buffer, ui_null_rect);
    ctx->current = popup;

    ui_panel_begin(ctx, 0, panel_type);
    win->buffer = popup->buffer;
    popup->layout->parent = win->layout;
    popup->layout->offset_x = &popup->scrollbar.x;
    popup->layout->offset_y = &popup->scrollbar.y;

    /* set read only mode to all parent panels */
    {struct ui_panel *root;
    root = win->layout;
    while (root) {
        root->flags |= UI_WINDOW_ROM;
        root = root->parent;
    }}
    return is_active;
}

UI_API void
ui_popup_close(struct ui_context *ctx)
{
    struct ui_window *popup;
    UI_ASSERT(ctx);
    if (!ctx || !ctx->current) return;

    popup = ctx->current;
    UI_ASSERT(popup->parent);
    UI_ASSERT(popup->layout->type & UI_PANEL_SET_POPUP);
    popup->flags |= UI_WINDOW_HIDDEN;
}

UI_API void
ui_popup_end(struct ui_context *ctx)
{
    struct ui_window *win;
    struct ui_window *popup;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return;

    popup = ctx->current;
    if (!popup->parent) return;
    win = popup->parent;
    if (popup->flags & UI_WINDOW_HIDDEN) {
        struct ui_panel *root;
        root = win->layout;
        while (root) {
            root->flags |= UI_WINDOW_REMOVE_ROM;
            root = root->parent;
        }
        win->popup.active = 0;
    }
    ui_push_scissor(&popup->buffer, ui_null_rect);
    ui_end(ctx);

    win->buffer = popup->buffer;
    ui_finish_popup(ctx, win);
    ctx->current = win;
    ui_push_scissor(&win->buffer, win->layout->clip);
}
/* -------------------------------------------------------------
 *
 *                          TOOLTIP
 *
 * -------------------------------------------------------------- */
UI_API int
ui_tooltip_begin(struct ui_context *ctx, float width)
{
    struct ui_window *win;
    const struct ui_input *in;
    struct ui_rect bounds;
    int ret;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return 0;

    /* make sure that no nonblocking popup is currently active */
    win = ctx->current;
    in = &ctx->input;
    if (win->popup.win && (win->popup.type & UI_PANEL_SET_NONBLOCK))
        return 0;

    bounds.w = width;
    bounds.h = ui_null_rect.h;
    bounds.x = (in->mouse.pos.x + 1) - win->layout->clip.x;
    bounds.y = (in->mouse.pos.y + 1) - win->layout->clip.y;

    ret = ui_popup_begin(ctx, UI_POPUP_DYNAMIC,
        "__##Tooltip##__", UI_WINDOW_NO_SCROLLBAR|UI_WINDOW_BORDER, bounds);
    if (ret) win->layout->flags &= ~(ui_flags)UI_WINDOW_ROM;
    win->popup.type = UI_PANEL_TOOLTIP;
    ctx->current->layout->type = UI_PANEL_TOOLTIP;
    return ret;
}

UI_API void
ui_tooltip_end(struct ui_context *ctx)
{
    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    if (!ctx || !ctx->current) return;
    ctx->current->seq--;
    ui_popup_close(ctx);
    ui_popup_end(ctx);
}

UI_API void
ui_tooltip(struct ui_context *ctx, const char *text)
{
    const struct ui_style *style;
    struct ui_vec2 padding;

    int text_len;
    float text_width;
    float text_height;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    UI_ASSERT(text);
    if (!ctx || !ctx->current || !ctx->current->layout || !text)
        return;

    /* fetch configuration data */
    style = &ctx->style;
    padding = style->window.padding;

    /* calculate size of the text and tooltip */
    text_len = ui_strlen(text);
    text_width = style->font->width(style->font->userdata,
                    style->font->height, text, text_len);
    text_width += (4 * padding.x);
    text_height = (style->font->height + 2 * padding.y);

    /* execute tooltip and fill with text */
    if (ui_tooltip_begin(ctx, (float)text_width)) {
        ui_layout_row_dynamic(ctx, (float)text_height, 1);
        ui_text(ctx, text, text_len, UI_TEXT_LEFT);
        ui_tooltip_end(ctx);
    }
}
/* -------------------------------------------------------------
 *
 *                          CONTEXTUAL
 *
 * -------------------------------------------------------------- */
UI_API int
ui_contextual_begin(struct ui_context *ctx, ui_flags flags, struct ui_vec2 size,
    struct ui_rect trigger_bounds)
{
    struct ui_window *win;
    struct ui_window *popup;
    struct ui_rect body;

    UI_STORAGE const struct ui_rect null_rect = {0,0,0,0};
    int is_clicked = 0;
    int is_active = 0;
    int is_open = 0;
    int ret = 0;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return 0;

    win = ctx->current;
    ++win->popup.con_count;

    /* check if currently active contextual is active */
    popup = win->popup.win;
    is_open = (popup && win->popup.type == UI_PANEL_CONTEXTUAL);
    is_clicked = ui_input_mouse_clicked(&ctx->input, UI_BUTTON_RIGHT, trigger_bounds);
    if (win->popup.active_con && win->popup.con_count != win->popup.active_con)
        return 0;
    if ((is_clicked && is_open && !is_active) || (!is_open && !is_active && !is_clicked))
        return 0;

    /* calculate contextual position on click */
    win->popup.active_con = win->popup.con_count;
    if (is_clicked) {
        body.x = ctx->input.mouse.pos.x;
        body.y = ctx->input.mouse.pos.y;
    } else {
        body.x = popup->bounds.x;
        body.y = popup->bounds.y;
    }
    body.w = size.x;
    body.h = size.y;

    /* start nonblocking contextual popup */
    ret = ui_nonblock_begin(ctx, flags|UI_WINDOW_NO_SCROLLBAR, body,
            null_rect, UI_PANEL_CONTEXTUAL);
    if (ret) win->popup.type = UI_PANEL_CONTEXTUAL;
    else {
        win->popup.active_con = 0;
        if (win->popup.win)
            win->popup.win->flags = 0;
    }
    return ret;
}

UI_API int
ui_contextual_item_text(struct ui_context *ctx, const char *text, int len,
    ui_flags alignment)
{
    struct ui_window *win;
    const struct ui_input *in;
    const struct ui_style *style;

    struct ui_rect bounds;
    enum ui_widget_layout_states state;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return 0;

    win = ctx->current;
    style = &ctx->style;
    state = ui_widget_fitting(&bounds, ctx, style->contextual_button.padding);
    if (!state) return ui_false;

    in = (state == UI_WIDGET_ROM || win->layout->flags & UI_WINDOW_ROM) ? 0 : &ctx->input;
    if (ui_do_button_text(&ctx->last_widget_state, &win->buffer, bounds,
        text, len, alignment, UI_BUTTON_DEFAULT, &style->contextual_button, in, style->font)) {
        ui_contextual_close(ctx);
        return ui_true;
    }
    return ui_false;
}

UI_API int ui_contextual_item_label(struct ui_context *ctx, const char *label, ui_flags align)
{return ui_contextual_item_text(ctx, label, ui_strlen(label), align);}

UI_API int
ui_contextual_item_image_text(struct ui_context *ctx, struct ui_image img,
    const char *text, int len, ui_flags align)
{
    struct ui_window *win;
    const struct ui_input *in;
    const struct ui_style *style;

    struct ui_rect bounds;
    enum ui_widget_layout_states state;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return 0;

    win = ctx->current;
    style = &ctx->style;
    state = ui_widget_fitting(&bounds, ctx, style->contextual_button.padding);
    if (!state) return ui_false;

    in = (state == UI_WIDGET_ROM || win->layout->flags & UI_WINDOW_ROM) ? 0 : &ctx->input;
    if (ui_do_button_text_image(&ctx->last_widget_state, &win->buffer, bounds,
        img, text, len, align, UI_BUTTON_DEFAULT, &style->contextual_button, style->font, in)){
        ui_contextual_close(ctx);
        return ui_true;
    }
    return ui_false;
}

UI_API int ui_contextual_item_image_label(struct ui_context *ctx, struct ui_image img,
    const char *label, ui_flags align)
{return ui_contextual_item_image_text(ctx, img, label, ui_strlen(label), align);}

UI_API int
ui_contextual_item_symbol_text(struct ui_context *ctx, enum ui_symbol_type symbol,
    const char *text, int len, ui_flags align)
{
    struct ui_window *win;
    const struct ui_input *in;
    const struct ui_style *style;

    struct ui_rect bounds;
    enum ui_widget_layout_states state;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return 0;

    win = ctx->current;
    style = &ctx->style;
    state = ui_widget_fitting(&bounds, ctx, style->contextual_button.padding);
    if (!state) return ui_false;

    in = (state == UI_WIDGET_ROM || win->layout->flags & UI_WINDOW_ROM) ? 0 : &ctx->input;
    if (ui_do_button_text_symbol(&ctx->last_widget_state, &win->buffer, bounds,
        symbol, text, len, align, UI_BUTTON_DEFAULT, &style->contextual_button, style->font, in)) {
        ui_contextual_close(ctx);
        return ui_true;
    }
    return ui_false;
}

UI_API int ui_contextual_item_symbol_label(struct ui_context *ctx, enum ui_symbol_type symbol,
    const char *text, ui_flags align)
{return ui_contextual_item_symbol_text(ctx, symbol, text, ui_strlen(text), align);}

UI_API void
ui_contextual_close(struct ui_context *ctx)
{
    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout) return;
    ui_popup_close(ctx);
}

UI_API void
ui_contextual_end(struct ui_context *ctx)
{
    struct ui_window *popup;
    struct ui_panel *panel;
    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    if (!ctx || !ctx->current) return;

    popup = ctx->current;
    panel = popup->layout;
    UI_ASSERT(popup->parent);
    UI_ASSERT(panel->type & UI_PANEL_SET_POPUP);
    if (panel->flags & UI_WINDOW_DYNAMIC) {
        /* Close behavior
        This is a bit hack solution since we do not now before we end our popup
        how big it will be. We therefore do not directly know when a
        click outside the non-blocking popup must close it at that direct frame.
        Instead it will be closed in the next frame.*/
        struct ui_rect body = {0,0,0,0};
        if (panel->at_y < (panel->bounds.y + panel->bounds.h)) {
            struct ui_vec2 padding = ui_panel_get_padding(&ctx->style, panel->type);
            body = panel->bounds;
            body.y = (panel->at_y + panel->footer_height + panel->border + padding.y + panel->row.height);
            body.h = (panel->bounds.y + panel->bounds.h) - body.y;
        }

        {int pressed = ui_input_is_mouse_pressed(&ctx->input, UI_BUTTON_LEFT);
        int in_body = ui_input_is_mouse_hovering_rect(&ctx->input, body);
        if (pressed && in_body)
            popup->flags |= UI_WINDOW_HIDDEN;
        }
    }
    if (popup->flags & UI_WINDOW_HIDDEN)
        popup->seq = 0;
    ui_popup_end(ctx);
    return;
}
/* -------------------------------------------------------------
 *
 *                          COMBO
 *
 * --------------------------------------------------------------*/
UI_INTERN int
ui_combo_begin(struct ui_context *ctx, struct ui_window *win,
    struct ui_vec2 size, int is_clicked, struct ui_rect header)
{
    struct ui_window *popup;
    int is_open = 0;
    int is_active = 0;
    struct ui_rect body;
    ui_hash hash;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return 0;

    popup = win->popup.win;
    body.x = header.x;
    body.w = size.x;
    body.y = header.y + header.h-ctx->style.window.combo_border;
    body.h = size.y;

    hash = win->popup.combo_count++;
    is_open = (popup) ? ui_true:ui_false;
    is_active = (popup && (win->popup.name == hash) && win->popup.type == UI_PANEL_COMBO);
    if ((is_clicked && is_open && !is_active) || (is_open && !is_active) ||
        (!is_open && !is_active && !is_clicked)) return 0;
    if (!ui_nonblock_begin(ctx, 0, body,
        (is_clicked && is_open)?ui_rect(0,0,0,0):header, UI_PANEL_COMBO)) return 0;

    win->popup.type = UI_PANEL_COMBO;
    win->popup.name = hash;
    return 1;
}

UI_API int
ui_combo_begin_text(struct ui_context *ctx, const char *selected, int len,
    struct ui_vec2 size)
{
    const struct ui_input *in;
    struct ui_window *win;
    struct ui_style *style;

    enum ui_widget_layout_states s;
    int is_clicked = ui_false;
    struct ui_rect header;
    const struct ui_style_item *background;
    struct ui_text text;

    UI_ASSERT(ctx);
    UI_ASSERT(selected);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout || !selected)
        return 0;

    win = ctx->current;
    style = &ctx->style;
    s = ui_widget(&header, ctx);
    if (s == UI_WIDGET_INVALID)
        return 0;

    in = (win->layout->flags & UI_WINDOW_ROM || s == UI_WIDGET_ROM)? 0: &ctx->input;
    if (ui_button_behavior(&ctx->last_widget_state, header, in, UI_BUTTON_DEFAULT))
        is_clicked = ui_true;

    /* draw combo box header background and border */
    if (ctx->last_widget_state & UI_WIDGET_STATE_ACTIVED) {
        background = &style->combo.active;
        text.text = style->combo.label_active;
    } else if (ctx->last_widget_state & UI_WIDGET_STATE_HOVER) {
        background = &style->combo.hover;
        text.text = style->combo.label_hover;
    } else {
        background = &style->combo.normal;
        text.text = style->combo.label_normal;
    }
    if (background->type == UI_STYLE_ITEM_IMAGE) {
        text.background = ui_rgba(0,0,0,0);
        ui_draw_image(&win->buffer, header, &background->data.image, ui_white);
    } else {
        text.background = background->data.color;
        ui_fill_rect(&win->buffer, header, style->combo.rounding, background->data.color);
        ui_stroke_rect(&win->buffer, header, style->combo.rounding, style->combo.border, style->combo.border_color);
    }
    {
        /* print currently selected text item */
        struct ui_rect label;
        struct ui_rect button;
        struct ui_rect content;

        enum ui_symbol_type sym;
        if (ctx->last_widget_state & UI_WIDGET_STATE_HOVER)
            sym = style->combo.sym_hover;
        else if (is_clicked)
            sym = style->combo.sym_active;
        else sym = style->combo.sym_normal;

        /* calculate button */
        button.w = header.h - 2 * style->combo.button_padding.y;
        button.x = (header.x + header.w - header.h) - style->combo.button_padding.x;
        button.y = header.y + style->combo.button_padding.y;
        button.h = button.w;

        content.x = button.x + style->combo.button.padding.x;
        content.y = button.y + style->combo.button.padding.y;
        content.w = button.w - 2 * style->combo.button.padding.x;
        content.h = button.h - 2 * style->combo.button.padding.y;

        /* draw selected label */
        text.padding = ui_vec2(0,0);
        label.x = header.x + style->combo.content_padding.x;
        label.y = header.y + style->combo.content_padding.y;
        label.w = button.x - (style->combo.content_padding.x + style->combo.spacing.x) - label.x;;
        label.h = header.h - 2 * style->combo.content_padding.y;
        ui_widget_text(&win->buffer, label, selected, len, &text,
            UI_TEXT_LEFT, ctx->style.font);

        /* draw open/close button */
        ui_draw_button_symbol(&win->buffer, &button, &content, ctx->last_widget_state,
            &ctx->style.combo.button, sym, style->font);
    }
    return ui_combo_begin(ctx, win, size, is_clicked, header);
}

UI_API int ui_combo_begin_label(struct ui_context *ctx, const char *selected, struct ui_vec2 size)
{return ui_combo_begin_text(ctx, selected, ui_strlen(selected), size);}

UI_API int
ui_combo_begin_color(struct ui_context *ctx, struct ui_color color, struct ui_vec2 size)
{
    struct ui_window *win;
    struct ui_style *style;
    const struct ui_input *in;

    struct ui_rect header;
    int is_clicked = ui_false;
    enum ui_widget_layout_states s;
    const struct ui_style_item *background;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return 0;

    win = ctx->current;
    style = &ctx->style;
    s = ui_widget(&header, ctx);
    if (s == UI_WIDGET_INVALID)
        return 0;

    in = (win->layout->flags & UI_WINDOW_ROM || s == UI_WIDGET_ROM)? 0: &ctx->input;
    if (ui_button_behavior(&ctx->last_widget_state, header, in, UI_BUTTON_DEFAULT))
        is_clicked = ui_true;

    /* draw combo box header background and border */
    if (ctx->last_widget_state & UI_WIDGET_STATE_ACTIVED)
        background = &style->combo.active;
    else if (ctx->last_widget_state & UI_WIDGET_STATE_HOVER)
        background = &style->combo.hover;
    else background = &style->combo.normal;

    if (background->type == UI_STYLE_ITEM_IMAGE) {
        ui_draw_image(&win->buffer, header, &background->data.image,ui_white);
    } else {
        ui_fill_rect(&win->buffer, header, style->combo.rounding, background->data.color);
        ui_stroke_rect(&win->buffer, header, style->combo.rounding, style->combo.border, style->combo.border_color);
    }
    {
        struct ui_rect content;
        struct ui_rect button;
        struct ui_rect bounds;

        enum ui_symbol_type sym;
        if (ctx->last_widget_state & UI_WIDGET_STATE_HOVER)
            sym = style->combo.sym_hover;
        else if (is_clicked)
            sym = style->combo.sym_active;
        else sym = style->combo.sym_normal;

        /* calculate button */
        button.w = header.h - 2 * style->combo.button_padding.y;
        button.x = (header.x + header.w - header.h) - style->combo.button_padding.x;
        button.y = header.y + style->combo.button_padding.y;
        button.h = button.w;

        content.x = button.x + style->combo.button.padding.x;
        content.y = button.y + style->combo.button.padding.y;
        content.w = button.w - 2 * style->combo.button.padding.x;
        content.h = button.h - 2 * style->combo.button.padding.y;

        /* draw color */
        bounds.h = header.h - 4 * style->combo.content_padding.y;
        bounds.y = header.y + 2 * style->combo.content_padding.y;
        bounds.x = header.x + 2 * style->combo.content_padding.x;
        bounds.w = (button.x - (style->combo.content_padding.x + style->combo.spacing.x)) - bounds.x;
        ui_fill_rect(&win->buffer, bounds, 0, color);

        /* draw open/close button */
        ui_draw_button_symbol(&win->buffer, &button, &content, ctx->last_widget_state,
            &ctx->style.combo.button, sym, style->font);
    }
    return ui_combo_begin(ctx, win, size, is_clicked, header);
}

UI_API int
ui_combo_begin_symbol(struct ui_context *ctx, enum ui_symbol_type symbol, struct ui_vec2 size)
{
    struct ui_window *win;
    struct ui_style *style;
    const struct ui_input *in;

    struct ui_rect header;
    int is_clicked = ui_false;
    enum ui_widget_layout_states s;
    const struct ui_style_item *background;
    struct ui_color sym_background;
    struct ui_color symbol_color;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return 0;

    win = ctx->current;
    style = &ctx->style;
    s = ui_widget(&header, ctx);
    if (s == UI_WIDGET_INVALID)
        return 0;

    in = (win->layout->flags & UI_WINDOW_ROM || s == UI_WIDGET_ROM)? 0: &ctx->input;
    if (ui_button_behavior(&ctx->last_widget_state, header, in, UI_BUTTON_DEFAULT))
        is_clicked = ui_true;

    /* draw combo box header background and border */
    if (ctx->last_widget_state & UI_WIDGET_STATE_ACTIVED) {
        background = &style->combo.active;
        symbol_color = style->combo.symbol_active;
    } else if (ctx->last_widget_state & UI_WIDGET_STATE_HOVER) {
        background = &style->combo.hover;
        symbol_color = style->combo.symbol_hover;
    } else {
        background = &style->combo.normal;
        symbol_color = style->combo.symbol_hover;
    }

    if (background->type == UI_STYLE_ITEM_IMAGE) {
        sym_background = ui_rgba(0,0,0,0);
        ui_draw_image(&win->buffer, header, &background->data.image, ui_white);
    } else {
        sym_background = background->data.color;
        ui_fill_rect(&win->buffer, header, style->combo.rounding, background->data.color);
        ui_stroke_rect(&win->buffer, header, style->combo.rounding, style->combo.border, style->combo.border_color);
    }
    {
        struct ui_rect bounds = {0,0,0,0};
        struct ui_rect content;
        struct ui_rect button;

        enum ui_symbol_type sym;
        if (ctx->last_widget_state & UI_WIDGET_STATE_HOVER)
            sym = style->combo.sym_hover;
        else if (is_clicked)
            sym = style->combo.sym_active;
        else sym = style->combo.sym_normal;

        /* calculate button */
        button.w = header.h - 2 * style->combo.button_padding.y;
        button.x = (header.x + header.w - header.h) - style->combo.button_padding.y;
        button.y = header.y + style->combo.button_padding.y;
        button.h = button.w;

        content.x = button.x + style->combo.button.padding.x;
        content.y = button.y + style->combo.button.padding.y;
        content.w = button.w - 2 * style->combo.button.padding.x;
        content.h = button.h - 2 * style->combo.button.padding.y;

        /* draw symbol */
        bounds.h = header.h - 2 * style->combo.content_padding.y;
        bounds.y = header.y + style->combo.content_padding.y;
        bounds.x = header.x + style->combo.content_padding.x;
        bounds.w = (button.x - style->combo.content_padding.y) - bounds.x;
        ui_draw_symbol(&win->buffer, symbol, bounds, sym_background, symbol_color,
            1.0f, style->font);

        /* draw open/close button */
        ui_draw_button_symbol(&win->buffer, &bounds, &content, ctx->last_widget_state,
            &ctx->style.combo.button, sym, style->font);
    }
    return ui_combo_begin(ctx, win, size, is_clicked, header);
}

UI_API int
ui_combo_begin_symbol_text(struct ui_context *ctx, const char *selected, int len,
    enum ui_symbol_type symbol, struct ui_vec2 size)
{
    struct ui_window *win;
    struct ui_style *style;
    struct ui_input *in;

    struct ui_rect header;
    int is_clicked = ui_false;
    enum ui_widget_layout_states s;
    const struct ui_style_item *background;
    struct ui_color symbol_color;
    struct ui_text text;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return 0;

    win = ctx->current;
    style = &ctx->style;
    s = ui_widget(&header, ctx);
    if (!s) return 0;

    in = (win->layout->flags & UI_WINDOW_ROM || s == UI_WIDGET_ROM)? 0: &ctx->input;
    if (ui_button_behavior(&ctx->last_widget_state, header, in, UI_BUTTON_DEFAULT))
        is_clicked = ui_true;

    /* draw combo box header background and border */
    if (ctx->last_widget_state & UI_WIDGET_STATE_ACTIVED) {
        background = &style->combo.active;
        symbol_color = style->combo.symbol_active;
        text.text = style->combo.label_active;
    } else if (ctx->last_widget_state & UI_WIDGET_STATE_HOVER) {
        background = &style->combo.hover;
        symbol_color = style->combo.symbol_hover;
        text.text = style->combo.label_hover;
    } else {
        background = &style->combo.normal;
        symbol_color = style->combo.symbol_normal;
        text.text = style->combo.label_normal;
    }
    if (background->type == UI_STYLE_ITEM_IMAGE) {
        text.background = ui_rgba(0,0,0,0);
        ui_draw_image(&win->buffer, header, &background->data.image, ui_white);
    } else {
        text.background = background->data.color;
        ui_fill_rect(&win->buffer, header, style->combo.rounding, background->data.color);
        ui_stroke_rect(&win->buffer, header, style->combo.rounding, style->combo.border, style->combo.border_color);
    }
    {
        struct ui_rect content;
        struct ui_rect button;
        struct ui_rect label;
        struct ui_rect image;

        enum ui_symbol_type sym;
        if (ctx->last_widget_state & UI_WIDGET_STATE_HOVER)
            sym = style->combo.sym_hover;
        else if (is_clicked)
            sym = style->combo.sym_active;
        else sym = style->combo.sym_normal;

        /* calculate button */
        button.w = header.h - 2 * style->combo.button_padding.y;
        button.x = (header.x + header.w - header.h) - style->combo.button_padding.x;
        button.y = header.y + style->combo.button_padding.y;
        button.h = button.w;

        content.x = button.x + style->combo.button.padding.x;
        content.y = button.y + style->combo.button.padding.y;
        content.w = button.w - 2 * style->combo.button.padding.x;
        content.h = button.h - 2 * style->combo.button.padding.y;
        ui_draw_button_symbol(&win->buffer, &button, &content, ctx->last_widget_state,
            &ctx->style.combo.button, sym, style->font);

        /* draw symbol */
        image.x = header.x + style->combo.content_padding.x;
        image.y = header.y + style->combo.content_padding.y;
        image.h = header.h - 2 * style->combo.content_padding.y;
        image.w = image.h;
        ui_draw_symbol(&win->buffer, symbol, image, text.background, symbol_color,
            1.0f, style->font);

        /* draw label */
        text.padding = ui_vec2(0,0);
        label.x = image.x + image.w + style->combo.spacing.x + style->combo.content_padding.x;
        label.y = header.y + style->combo.content_padding.y;
        label.w = (button.x - style->combo.content_padding.x) - label.x;
        label.h = header.h - 2 * style->combo.content_padding.y;
        ui_widget_text(&win->buffer, label, selected, len, &text, UI_TEXT_LEFT, style->font);
    }
    return ui_combo_begin(ctx, win, size, is_clicked, header);
}

UI_API int
ui_combo_begin_image(struct ui_context *ctx, struct ui_image img, struct ui_vec2 size)
{
    struct ui_window *win;
    struct ui_style *style;
    const struct ui_input *in;

    struct ui_rect header;
    int is_clicked = ui_false;
    enum ui_widget_layout_states s;
    const struct ui_style_item *background;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return 0;

    win = ctx->current;
    style = &ctx->style;
    s = ui_widget(&header, ctx);
    if (s == UI_WIDGET_INVALID)
        return 0;

    in = (win->layout->flags & UI_WINDOW_ROM || s == UI_WIDGET_ROM)? 0: &ctx->input;
    if (ui_button_behavior(&ctx->last_widget_state, header, in, UI_BUTTON_DEFAULT))
        is_clicked = ui_true;

    /* draw combo box header background and border */
    if (ctx->last_widget_state & UI_WIDGET_STATE_ACTIVED)
        background = &style->combo.active;
    else if (ctx->last_widget_state & UI_WIDGET_STATE_HOVER)
        background = &style->combo.hover;
    else background = &style->combo.normal;

    if (background->type == UI_STYLE_ITEM_IMAGE) {
        ui_draw_image(&win->buffer, header, &background->data.image, ui_white);
    } else {
        ui_fill_rect(&win->buffer, header, style->combo.rounding, background->data.color);
        ui_stroke_rect(&win->buffer, header, style->combo.rounding, style->combo.border, style->combo.border_color);
    }
    {
        struct ui_rect bounds = {0,0,0,0};
        struct ui_rect content;
        struct ui_rect button;

        enum ui_symbol_type sym;
        if (ctx->last_widget_state & UI_WIDGET_STATE_HOVER)
            sym = style->combo.sym_hover;
        else if (is_clicked)
            sym = style->combo.sym_active;
        else sym = style->combo.sym_normal;

        /* calculate button */
        button.w = header.h - 2 * style->combo.button_padding.y;
        button.x = (header.x + header.w - header.h) - style->combo.button_padding.y;
        button.y = header.y + style->combo.button_padding.y;
        button.h = button.w;

        content.x = button.x + style->combo.button.padding.x;
        content.y = button.y + style->combo.button.padding.y;
        content.w = button.w - 2 * style->combo.button.padding.x;
        content.h = button.h - 2 * style->combo.button.padding.y;

        /* draw image */
        bounds.h = header.h - 2 * style->combo.content_padding.y;
        bounds.y = header.y + style->combo.content_padding.y;
        bounds.x = header.x + style->combo.content_padding.x;
        bounds.w = (button.x - style->combo.content_padding.y) - bounds.x;
        ui_draw_image(&win->buffer, bounds, &img, ui_white);

        /* draw open/close button */
        ui_draw_button_symbol(&win->buffer, &bounds, &content, ctx->last_widget_state,
            &ctx->style.combo.button, sym, style->font);
    }
    return ui_combo_begin(ctx, win, size, is_clicked, header);
}

UI_API int
ui_combo_begin_image_text(struct ui_context *ctx, const char *selected, int len,
    struct ui_image img, struct ui_vec2 size)
{
    struct ui_window *win;
    struct ui_style *style;
    struct ui_input *in;

    struct ui_rect header;
    int is_clicked = ui_false;
    enum ui_widget_layout_states s;
    const struct ui_style_item *background;
    struct ui_text text;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return 0;

    win = ctx->current;
    style = &ctx->style;
    s = ui_widget(&header, ctx);
    if (!s) return 0;

    in = (win->layout->flags & UI_WINDOW_ROM || s == UI_WIDGET_ROM)? 0: &ctx->input;
    if (ui_button_behavior(&ctx->last_widget_state, header, in, UI_BUTTON_DEFAULT))
        is_clicked = ui_true;

    /* draw combo box header background and border */
    if (ctx->last_widget_state & UI_WIDGET_STATE_ACTIVED) {
        background = &style->combo.active;
        text.text = style->combo.label_active;
    } else if (ctx->last_widget_state & UI_WIDGET_STATE_HOVER) {
        background = &style->combo.hover;
        text.text = style->combo.label_hover;
    } else {
        background = &style->combo.normal;
        text.text = style->combo.label_normal;
    }
    if (background->type == UI_STYLE_ITEM_IMAGE) {
        text.background = ui_rgba(0,0,0,0);
        ui_draw_image(&win->buffer, header, &background->data.image, ui_white);
    } else {
        text.background = background->data.color;
        ui_fill_rect(&win->buffer, header, style->combo.rounding, background->data.color);
        ui_stroke_rect(&win->buffer, header, style->combo.rounding, style->combo.border, style->combo.border_color);
    }
    {
        struct ui_rect content;
        struct ui_rect button;
        struct ui_rect label;
        struct ui_rect image;

        enum ui_symbol_type sym;
        if (ctx->last_widget_state & UI_WIDGET_STATE_HOVER)
            sym = style->combo.sym_hover;
        else if (is_clicked)
            sym = style->combo.sym_active;
        else sym = style->combo.sym_normal;

        /* calculate button */
        button.w = header.h - 2 * style->combo.button_padding.y;
        button.x = (header.x + header.w - header.h) - style->combo.button_padding.x;
        button.y = header.y + style->combo.button_padding.y;
        button.h = button.w;

        content.x = button.x + style->combo.button.padding.x;
        content.y = button.y + style->combo.button.padding.y;
        content.w = button.w - 2 * style->combo.button.padding.x;
        content.h = button.h - 2 * style->combo.button.padding.y;
        ui_draw_button_symbol(&win->buffer, &button, &content, ctx->last_widget_state,
            &ctx->style.combo.button, sym, style->font);

        /* draw image */
        image.x = header.x + style->combo.content_padding.x;
        image.y = header.y + style->combo.content_padding.y;
        image.h = header.h - 2 * style->combo.content_padding.y;
        image.w = image.h;
        ui_draw_image(&win->buffer, image, &img, ui_white);

        /* draw label */
        text.padding = ui_vec2(0,0);
        label.x = image.x + image.w + style->combo.spacing.x + style->combo.content_padding.x;
        label.y = header.y + style->combo.content_padding.y;
        label.w = (button.x - style->combo.content_padding.x) - label.x;
        label.h = header.h - 2 * style->combo.content_padding.y;
        ui_widget_text(&win->buffer, label, selected, len, &text, UI_TEXT_LEFT, style->font);
    }
    return ui_combo_begin(ctx, win, size, is_clicked, header);
}

UI_API int ui_combo_begin_symbol_label(struct ui_context *ctx,
    const char *selected, enum ui_symbol_type type, struct ui_vec2 size)
{return ui_combo_begin_symbol_text(ctx, selected, ui_strlen(selected), type, size);}

UI_API int ui_combo_begin_image_label(struct ui_context *ctx,
    const char *selected, struct ui_image img, struct ui_vec2 size)
{return ui_combo_begin_image_text(ctx, selected, ui_strlen(selected), img, size);}

UI_API int ui_combo_item_text(struct ui_context *ctx, const char *text, int len,ui_flags align)
{return ui_contextual_item_text(ctx, text, len, align);}

UI_API int ui_combo_item_label(struct ui_context *ctx, const char *label, ui_flags align)
{return ui_contextual_item_label(ctx, label, align);}

UI_API int ui_combo_item_image_text(struct ui_context *ctx, struct ui_image img, const char *text,
    int len, ui_flags alignment)
{return ui_contextual_item_image_text(ctx, img, text, len, alignment);}

UI_API int ui_combo_item_image_label(struct ui_context *ctx, struct ui_image img,
    const char *text, ui_flags alignment)
{return ui_contextual_item_image_label(ctx, img, text, alignment);}

UI_API int ui_combo_item_symbol_text(struct ui_context *ctx, enum ui_symbol_type sym,
    const char *text, int len, ui_flags alignment)
{return ui_contextual_item_symbol_text(ctx, sym, text, len, alignment);}

UI_API int ui_combo_item_symbol_label(struct ui_context *ctx, enum ui_symbol_type sym,
    const char *label, ui_flags alignment)
{return ui_contextual_item_symbol_label(ctx, sym, label, alignment);}

UI_API void ui_combo_end(struct ui_context *ctx)
{ui_contextual_end(ctx);}

UI_API void ui_combo_close(struct ui_context *ctx)
{ui_contextual_close(ctx);}

UI_API int
ui_combo(struct ui_context *ctx, const char **items, int count,
    int selected, int item_height, struct ui_vec2 size)
{
    int i = 0;
    int max_height;
    struct ui_vec2 item_spacing;
    struct ui_vec2 window_padding;

    UI_ASSERT(ctx);
    UI_ASSERT(items);
    UI_ASSERT(ctx->current);
    if (!ctx || !items ||!count)
        return selected;

    item_spacing = ctx->style.window.spacing;
    window_padding = ui_panel_get_padding(&ctx->style, ctx->current->layout->type);
    max_height = count * item_height + count * (int)item_spacing.y;
    max_height += (int)item_spacing.y * 2 + (int)window_padding.y * 2;
    size.y = UI_MIN(size.y, (float)max_height);
    if (ui_combo_begin_label(ctx, items[selected], size)) {
        ui_layout_row_dynamic(ctx, (float)item_height, 1);
        for (i = 0; i < count; ++i) {
            if (ui_combo_item_label(ctx, items[i], UI_TEXT_LEFT))
                selected = i;
        }
        ui_combo_end(ctx);
    }
    return selected;
}

UI_API int
ui_combo_separator(struct ui_context *ctx, const char *items_separated_by_separator,
    int separator, int selected, int count, int item_height, struct ui_vec2 size)
{
    int i;
    int max_height;
    struct ui_vec2 item_spacing;
    struct ui_vec2 window_padding;
    const char *current_item;
    const char *iter;
    int length = 0;

    UI_ASSERT(ctx);
    UI_ASSERT(items_separated_by_separator);
    if (!ctx || !items_separated_by_separator)
        return selected;

    /* calculate popup window */
    item_spacing = ctx->style.window.spacing;
    window_padding = ui_panel_get_padding(&ctx->style, ctx->current->layout->type);
    max_height = count * item_height + count * (int)item_spacing.y;
    max_height += (int)item_spacing.y * 2 + (int)window_padding.y * 2;
    size.y = UI_MIN(size.y, (float)max_height);

    /* find selected item */
    current_item = items_separated_by_separator;
    for (i = 0; i < count; ++i) {
        iter = current_item;
        while (*iter && *iter != separator) iter++;
        length = (int)(iter - current_item);
        if (i == selected) break;
        current_item = iter + 1;
    }

    if (ui_combo_begin_text(ctx, current_item, length, size)) {
        current_item = items_separated_by_separator;
        ui_layout_row_dynamic(ctx, (float)item_height, 1);
        for (i = 0; i < count; ++i) {
            iter = current_item;
            while (*iter && *iter != separator) iter++;
            length = (int)(iter - current_item);
            if (ui_combo_item_text(ctx, current_item, length, UI_TEXT_LEFT))
                selected = i;
            current_item = current_item + length + 1;
        }
        ui_combo_end(ctx);
    }
    return selected;
}

UI_API int
ui_combo_string(struct ui_context *ctx, const char *items_separated_by_zeros,
    int selected, int count, int item_height, struct ui_vec2 size)
{return ui_combo_separator(ctx, items_separated_by_zeros, '\0', selected, count, item_height, size);}

UI_API int
ui_combo_callback(struct ui_context *ctx, void(*item_getter)(void*, int, const char**),
    void *userdata, int selected, int count, int item_height, struct ui_vec2 size)
{
    int i;
    int max_height;
    struct ui_vec2 item_spacing;
    struct ui_vec2 window_padding;
    const char *item;

    UI_ASSERT(ctx);
    UI_ASSERT(item_getter);
    if (!ctx || !item_getter)
        return selected;

    /* calculate popup window */
    item_spacing = ctx->style.window.spacing;
    window_padding = ui_panel_get_padding(&ctx->style, ctx->current->layout->type);
    max_height = count * item_height + count * (int)item_spacing.y;
    max_height += (int)item_spacing.y * 2 + (int)window_padding.y * 2;
    size.y = UI_MIN(size.y, (float)max_height);

    item_getter(userdata, selected, &item);
    if (ui_combo_begin_label(ctx, item, size)) {
        ui_layout_row_dynamic(ctx, (float)item_height, 1);
        for (i = 0; i < count; ++i) {
            item_getter(userdata, i, &item);
            if (ui_combo_item_label(ctx, item, UI_TEXT_LEFT))
                selected = i;
        }
        ui_combo_end(ctx);
    }
    return selected;
}

UI_API void ui_combobox(struct ui_context *ctx, const char **items, int count,
    int *selected, int item_height, struct ui_vec2 size)
{*selected = ui_combo(ctx, items, count, *selected, item_height, size);}

UI_API void ui_combobox_string(struct ui_context *ctx, const char *items_separated_by_zeros,
    int *selected, int count, int item_height, struct ui_vec2 size)
{*selected = ui_combo_string(ctx, items_separated_by_zeros, *selected, count, item_height, size);}

UI_API void ui_combobox_separator(struct ui_context *ctx, const char *items_separated_by_separator,
    int separator,int *selected, int count, int item_height, struct ui_vec2 size)
{*selected = ui_combo_separator(ctx, items_separated_by_separator, separator,
    *selected, count, item_height, size);}

UI_API void ui_combobox_callback(struct ui_context *ctx,
    void(*item_getter)(void* data, int id, const char **out_text),
    void *userdata, int *selected, int count, int item_height, struct ui_vec2 size)
{*selected = ui_combo_callback(ctx, item_getter, userdata,  *selected, count, item_height, size);}

/*
 * -------------------------------------------------------------
 *
 *                          MENU
 *
 * --------------------------------------------------------------
 */
UI_INTERN int
ui_menu_begin(struct ui_context *ctx, struct ui_window *win,
    const char *id, int is_clicked, struct ui_rect header, struct ui_vec2 size)
{
    int is_open = 0;
    int is_active = 0;
    struct ui_rect body;
    struct ui_window *popup;
    ui_hash hash = ui_murmur_hash(id, (int)ui_strlen(id), UI_PANEL_MENU);

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return 0;

    body.x = header.x;
    body.w = size.x;
    body.y = header.y + header.h;
    body.h = size.y;

    popup = win->popup.win;
    is_open = popup ? ui_true : ui_false;
    is_active = (popup && (win->popup.name == hash) && win->popup.type == UI_PANEL_MENU);
    if ((is_clicked && is_open && !is_active) || (is_open && !is_active) ||
        (!is_open && !is_active && !is_clicked)) return 0;
    if (!ui_nonblock_begin(ctx, UI_WINDOW_NO_SCROLLBAR, body, header, UI_PANEL_MENU))
        return 0;

    win->popup.type = UI_PANEL_MENU;
    win->popup.name = hash;
    return 1;
}

UI_API int
ui_menu_begin_text(struct ui_context *ctx, const char *title, int len,
    ui_flags align, struct ui_vec2 size)
{
    struct ui_window *win;
    const struct ui_input *in;
    struct ui_rect header;
    int is_clicked = ui_false;
    ui_flags state;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return 0;

    win = ctx->current;
    state = ui_widget(&header, ctx);
    if (!state) return 0;
    in = (state == UI_WIDGET_ROM || win->flags & UI_WINDOW_ROM) ? 0 : &ctx->input;
    if (ui_do_button_text(&ctx->last_widget_state, &win->buffer, header,
        title, len, align, UI_BUTTON_DEFAULT, &ctx->style.menu_button, in, ctx->style.font))
        is_clicked = ui_true;
    return ui_menu_begin(ctx, win, title, is_clicked, header, size);
}

UI_API int ui_menu_begin_label(struct ui_context *ctx,
    const char *text, ui_flags align, struct ui_vec2 size)
{return ui_menu_begin_text(ctx, text, ui_strlen(text), align, size);}

UI_API int
ui_menu_begin_image(struct ui_context *ctx, const char *id, struct ui_image img,
    struct ui_vec2 size)
{
    struct ui_window *win;
    struct ui_rect header;
    const struct ui_input *in;
    int is_clicked = ui_false;
    ui_flags state;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return 0;

    win = ctx->current;
    state = ui_widget(&header, ctx);
    if (!state) return 0;
    in = (state == UI_WIDGET_ROM || win->layout->flags & UI_WINDOW_ROM) ? 0 : &ctx->input;
    if (ui_do_button_image(&ctx->last_widget_state, &win->buffer, header,
        img, UI_BUTTON_DEFAULT, &ctx->style.menu_button, in))
        is_clicked = ui_true;
    return ui_menu_begin(ctx, win, id, is_clicked, header, size);
}

UI_API int
ui_menu_begin_symbol(struct ui_context *ctx, const char *id,
    enum ui_symbol_type sym, struct ui_vec2 size)
{
    struct ui_window *win;
    const struct ui_input *in;
    struct ui_rect header;
    int is_clicked = ui_false;
    ui_flags state;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return 0;

    win = ctx->current;
    state = ui_widget(&header, ctx);
    if (!state) return 0;
    in = (state == UI_WIDGET_ROM || win->layout->flags & UI_WINDOW_ROM) ? 0 : &ctx->input;
    if (ui_do_button_symbol(&ctx->last_widget_state,  &win->buffer, header,
        sym, UI_BUTTON_DEFAULT, &ctx->style.menu_button, in, ctx->style.font))
        is_clicked = ui_true;
    return ui_menu_begin(ctx, win, id, is_clicked, header, size);
}

UI_API int
ui_menu_begin_image_text(struct ui_context *ctx, const char *title, int len,
    ui_flags align, struct ui_image img, struct ui_vec2 size)
{
    struct ui_window *win;
    struct ui_rect header;
    const struct ui_input *in;
    int is_clicked = ui_false;
    ui_flags state;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return 0;

    win = ctx->current;
    state = ui_widget(&header, ctx);
    if (!state) return 0;
    in = (state == UI_WIDGET_ROM || win->layout->flags & UI_WINDOW_ROM) ? 0 : &ctx->input;
    if (ui_do_button_text_image(&ctx->last_widget_state, &win->buffer,
        header, img, title, len, align, UI_BUTTON_DEFAULT, &ctx->style.menu_button,
        ctx->style.font, in))
        is_clicked = ui_true;
    return ui_menu_begin(ctx, win, title, is_clicked, header, size);
}

UI_API int ui_menu_begin_image_label(struct ui_context *ctx,
    const char *title, ui_flags align, struct ui_image img, struct ui_vec2 size)
{return ui_menu_begin_image_text(ctx, title, ui_strlen(title), align, img, size);}

UI_API int
ui_menu_begin_symbol_text(struct ui_context *ctx, const char *title, int len,
    ui_flags align, enum ui_symbol_type sym, struct ui_vec2 size)
{
    struct ui_window *win;
    struct ui_rect header;
    const struct ui_input *in;
    int is_clicked = ui_false;
    ui_flags state;

    UI_ASSERT(ctx);
    UI_ASSERT(ctx->current);
    UI_ASSERT(ctx->current->layout);
    if (!ctx || !ctx->current || !ctx->current->layout)
        return 0;

    win = ctx->current;
    state = ui_widget(&header, ctx);
    if (!state) return 0;

    in = (state == UI_WIDGET_ROM || win->layout->flags & UI_WINDOW_ROM) ? 0 : &ctx->input;
    if (ui_do_button_text_symbol(&ctx->last_widget_state, &win->buffer,
        header, sym, title, len, align, UI_BUTTON_DEFAULT, &ctx->style.menu_button,
        ctx->style.font, in)) is_clicked = ui_true;
    return ui_menu_begin(ctx, win, title, is_clicked, header, size);
}

UI_API int ui_menu_begin_symbol_label(struct ui_context *ctx,
    const char *title, ui_flags align, enum ui_symbol_type sym, struct ui_vec2 size )
{return ui_menu_begin_symbol_text(ctx, title, ui_strlen(title), align,sym,size);}

UI_API int ui_menu_item_text(struct ui_context *ctx, const char *title, int len, ui_flags align)
{return ui_contextual_item_text(ctx, title, len, align);}

UI_API int ui_menu_item_label(struct ui_context *ctx, const char *label, ui_flags align)
{return ui_contextual_item_label(ctx, label, align);}

UI_API int ui_menu_item_image_label(struct ui_context *ctx, struct ui_image img,
    const char *label, ui_flags align)
{return ui_contextual_item_image_label(ctx, img, label, align);}

UI_API int ui_menu_item_image_text(struct ui_context *ctx, struct ui_image img,
    const char *text, int len, ui_flags align)
{return ui_contextual_item_image_text(ctx, img, text, len, align);}

UI_API int ui_menu_item_symbol_text(struct ui_context *ctx, enum ui_symbol_type sym,
    const char *text, int len, ui_flags align)
{return ui_contextual_item_symbol_text(ctx, sym, text, len, align);}

UI_API int ui_menu_item_symbol_label(struct ui_context *ctx, enum ui_symbol_type sym,
    const char *label, ui_flags align)
{return ui_contextual_item_symbol_label(ctx, sym, label, align);}

UI_API void ui_menu_close(struct ui_context *ctx)
{ui_contextual_close(ctx);}

UI_API void
ui_menu_end(struct ui_context *ctx)
{ui_contextual_end(ctx);}

#endif
