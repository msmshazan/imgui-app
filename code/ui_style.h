#if !defined(UI_STYLE_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Mohamed Shazan $
   $Notice: All Rights Reserved. $
   ======================================================================== */
enum theme {THEME_BLACK, THEME_WHITE, THEME_RED, THEME_BLUE, THEME_DARK};

void
set_style(struct ui_context *ctx, enum theme theme)
{
    struct ui_color table[UI_COLOR_COUNT];
    if (theme == THEME_WHITE) {
        table[UI_COLOR_TEXT] = ui_rgba(70, 70, 70, 255);
        table[UI_COLOR_WINDOW] = ui_rgba(175, 175, 175, 255);
        table[UI_COLOR_HEADER] = ui_rgba(175, 175, 175, 255);
        table[UI_COLOR_BORDER] = ui_rgba(0, 0, 0, 255);
        table[UI_COLOR_BUTTON] = ui_rgba(185, 185, 185, 255);
        table[UI_COLOR_BUTTON_HOVER] = ui_rgba(170, 170, 170, 255);
        table[UI_COLOR_BUTTON_ACTIVE] = ui_rgba(160, 160, 160, 255);
        table[UI_COLOR_TOGGLE] = ui_rgba(150, 150, 150, 255);
        table[UI_COLOR_TOGGLE_HOVER] = ui_rgba(120, 120, 120, 255);
        table[UI_COLOR_TOGGLE_CURSOR] = ui_rgba(175, 175, 175, 255);
        table[UI_COLOR_SELECT] = ui_rgba(190, 190, 190, 255);
        table[UI_COLOR_SELECT_ACTIVE] = ui_rgba(175, 175, 175, 255);
        table[UI_COLOR_SLIDER] = ui_rgba(190, 190, 190, 255);
        table[UI_COLOR_SLIDER_CURSOR] = ui_rgba(80, 80, 80, 255);
        table[UI_COLOR_SLIDER_CURSOR_HOVER] = ui_rgba(70, 70, 70, 255);
        table[UI_COLOR_SLIDER_CURSOR_ACTIVE] = ui_rgba(60, 60, 60, 255);
        table[UI_COLOR_PROPERTY] = ui_rgba(175, 175, 175, 255);
        table[UI_COLOR_EDIT] = ui_rgba(150, 150, 150, 255);
        table[UI_COLOR_EDIT_CURSOR] = ui_rgba(0, 0, 0, 255);
        table[UI_COLOR_COMBO] = ui_rgba(175, 175, 175, 255);
        table[UI_COLOR_CHART] = ui_rgba(160, 160, 160, 255);
        table[UI_COLOR_CHART_COLOR] = ui_rgba(45, 45, 45, 255);
        table[UI_COLOR_CHART_COLOR_HIGHLIGHT] = ui_rgba( 255, 0, 0, 255);
        table[UI_COLOR_SCROLLBAR] = ui_rgba(180, 180, 180, 255);
        table[UI_COLOR_SCROLLBAR_CURSOR] = ui_rgba(140, 140, 140, 255);
        table[UI_COLOR_SCROLLBAR_CURSOR_HOVER] = ui_rgba(150, 150, 150, 255);
        table[UI_COLOR_SCROLLBAR_CURSOR_ACTIVE] = ui_rgba(160, 160, 160, 255);
        table[UI_COLOR_TAB_HEADER] = ui_rgba(180, 180, 180, 255);
        ui_style_from_table(ctx, table);
    } else if (theme == THEME_RED) {
        table[UI_COLOR_TEXT] = ui_rgba(190, 190, 190, 255);
        table[UI_COLOR_WINDOW] = ui_rgba(30, 33, 40, 215);
        table[UI_COLOR_HEADER] = ui_rgba(181, 45, 69, 220);
        table[UI_COLOR_BORDER] = ui_rgba(51, 55, 67, 255);
        table[UI_COLOR_BUTTON] = ui_rgba(181, 45, 69, 255);
        table[UI_COLOR_BUTTON_HOVER] = ui_rgba(190, 50, 70, 255);
        table[UI_COLOR_BUTTON_ACTIVE] = ui_rgba(195, 55, 75, 255);
        table[UI_COLOR_TOGGLE] = ui_rgba(51, 55, 67, 255);
        table[UI_COLOR_TOGGLE_HOVER] = ui_rgba(45, 60, 60, 255);
        table[UI_COLOR_TOGGLE_CURSOR] = ui_rgba(181, 45, 69, 255);
        table[UI_COLOR_SELECT] = ui_rgba(51, 55, 67, 255);
        table[UI_COLOR_SELECT_ACTIVE] = ui_rgba(181, 45, 69, 255);
        table[UI_COLOR_SLIDER] = ui_rgba(51, 55, 67, 255);
        table[UI_COLOR_SLIDER_CURSOR] = ui_rgba(181, 45, 69, 255);
        table[UI_COLOR_SLIDER_CURSOR_HOVER] = ui_rgba(186, 50, 74, 255);
        table[UI_COLOR_SLIDER_CURSOR_ACTIVE] = ui_rgba(191, 55, 79, 255);
        table[UI_COLOR_PROPERTY] = ui_rgba(51, 55, 67, 255);
        table[UI_COLOR_EDIT] = ui_rgba(51, 55, 67, 225);
        table[UI_COLOR_EDIT_CURSOR] = ui_rgba(190, 190, 190, 255);
        table[UI_COLOR_COMBO] = ui_rgba(51, 55, 67, 255);
        table[UI_COLOR_CHART] = ui_rgba(51, 55, 67, 255);
        table[UI_COLOR_CHART_COLOR] = ui_rgba(170, 40, 60, 255);
        table[UI_COLOR_CHART_COLOR_HIGHLIGHT] = ui_rgba( 255, 0, 0, 255);
        table[UI_COLOR_SCROLLBAR] = ui_rgba(30, 33, 40, 255);
        table[UI_COLOR_SCROLLBAR_CURSOR] = ui_rgba(64, 84, 95, 255);
        table[UI_COLOR_SCROLLBAR_CURSOR_HOVER] = ui_rgba(70, 90, 100, 255);
        table[UI_COLOR_SCROLLBAR_CURSOR_ACTIVE] = ui_rgba(75, 95, 105, 255);
        table[UI_COLOR_TAB_HEADER] = ui_rgba(181, 45, 69, 220);
        ui_style_from_table(ctx, table);
    } else if (theme == THEME_BLUE) {
        table[UI_COLOR_TEXT] = ui_rgba(20, 20, 20, 255);
        table[UI_COLOR_WINDOW] = ui_rgba(202, 212, 214, 215);
        table[UI_COLOR_HEADER] = ui_rgba(137, 182, 224, 220);
        table[UI_COLOR_BORDER] = ui_rgba(140, 159, 173, 255);
        table[UI_COLOR_BUTTON] = ui_rgba(137, 182, 224, 255);
        table[UI_COLOR_BUTTON_HOVER] = ui_rgba(142, 187, 229, 255);
        table[UI_COLOR_BUTTON_ACTIVE] = ui_rgba(147, 192, 234, 255);
        table[UI_COLOR_TOGGLE] = ui_rgba(177, 210, 210, 255);
        table[UI_COLOR_TOGGLE_HOVER] = ui_rgba(182, 215, 215, 255);
        table[UI_COLOR_TOGGLE_CURSOR] = ui_rgba(137, 182, 224, 255);
        table[UI_COLOR_SELECT] = ui_rgba(177, 210, 210, 255);
        table[UI_COLOR_SELECT_ACTIVE] = ui_rgba(137, 182, 224, 255);
        table[UI_COLOR_SLIDER] = ui_rgba(177, 210, 210, 255);
        table[UI_COLOR_SLIDER_CURSOR] = ui_rgba(137, 182, 224, 245);
        table[UI_COLOR_SLIDER_CURSOR_HOVER] = ui_rgba(142, 188, 229, 255);
        table[UI_COLOR_SLIDER_CURSOR_ACTIVE] = ui_rgba(147, 193, 234, 255);
        table[UI_COLOR_PROPERTY] = ui_rgba(210, 210, 210, 255);
        table[UI_COLOR_EDIT] = ui_rgba(210, 210, 210, 225);
        table[UI_COLOR_EDIT_CURSOR] = ui_rgba(20, 20, 20, 255);
        table[UI_COLOR_COMBO] = ui_rgba(210, 210, 210, 255);
        table[UI_COLOR_CHART] = ui_rgba(210, 210, 210, 255);
        table[UI_COLOR_CHART_COLOR] = ui_rgba(137, 182, 224, 255);
        table[UI_COLOR_CHART_COLOR_HIGHLIGHT] = ui_rgba( 255, 0, 0, 255);
        table[UI_COLOR_SCROLLBAR] = ui_rgba(190, 200, 200, 255);
        table[UI_COLOR_SCROLLBAR_CURSOR] = ui_rgba(64, 84, 95, 255);
        table[UI_COLOR_SCROLLBAR_CURSOR_HOVER] = ui_rgba(70, 90, 100, 255);
        table[UI_COLOR_SCROLLBAR_CURSOR_ACTIVE] = ui_rgba(75, 95, 105, 255);
        table[UI_COLOR_TAB_HEADER] = ui_rgba(156, 193, 220, 255);
        ui_style_from_table(ctx, table);
    } else if (theme == THEME_DARK) {
        table[UI_COLOR_TEXT] = ui_rgba(210, 210, 210, 255);
        table[UI_COLOR_WINDOW] = ui_rgba(57, 67, 71, 215);
        table[UI_COLOR_HEADER] = ui_rgba(51, 51, 56, 220);
        table[UI_COLOR_BORDER] = ui_rgba(46, 46, 46, 255);
        table[UI_COLOR_BUTTON] = ui_rgba(48, 83, 111, 255);
        table[UI_COLOR_BUTTON_HOVER] = ui_rgba(58, 93, 121, 255);
        table[UI_COLOR_BUTTON_ACTIVE] = ui_rgba(63, 98, 126, 255);
        table[UI_COLOR_TOGGLE] = ui_rgba(50, 58, 61, 255);
        table[UI_COLOR_TOGGLE_HOVER] = ui_rgba(45, 53, 56, 255);
        table[UI_COLOR_TOGGLE_CURSOR] = ui_rgba(48, 83, 111, 255);
        table[UI_COLOR_SELECT] = ui_rgba(57, 67, 61, 255);
        table[UI_COLOR_SELECT_ACTIVE] = ui_rgba(48, 83, 111, 255);
        table[UI_COLOR_SLIDER] = ui_rgba(50, 58, 61, 255);
        table[UI_COLOR_SLIDER_CURSOR] = ui_rgba(48, 83, 111, 245);
        table[UI_COLOR_SLIDER_CURSOR_HOVER] = ui_rgba(53, 88, 116, 255);
        table[UI_COLOR_SLIDER_CURSOR_ACTIVE] = ui_rgba(58, 93, 121, 255);
        table[UI_COLOR_PROPERTY] = ui_rgba(50, 58, 61, 255);
        table[UI_COLOR_EDIT] = ui_rgba(50, 58, 61, 225);
        table[UI_COLOR_EDIT_CURSOR] = ui_rgba(210, 210, 210, 255);
        table[UI_COLOR_COMBO] = ui_rgba(50, 58, 61, 255);
        table[UI_COLOR_CHART] = ui_rgba(50, 58, 61, 255);
        table[UI_COLOR_CHART_COLOR] = ui_rgba(48, 83, 111, 255);
        table[UI_COLOR_CHART_COLOR_HIGHLIGHT] = ui_rgba(255, 0, 0, 255);
        table[UI_COLOR_SCROLLBAR] = ui_rgba(50, 58, 61, 255);
        table[UI_COLOR_SCROLLBAR_CURSOR] = ui_rgba(48, 83, 111, 255);
        table[UI_COLOR_SCROLLBAR_CURSOR_HOVER] = ui_rgba(53, 88, 116, 255);
        table[UI_COLOR_SCROLLBAR_CURSOR_ACTIVE] = ui_rgba(58, 93, 121, 255);
        table[UI_COLOR_TAB_HEADER] = ui_rgba(48, 83, 111, 255);
        ui_style_from_table(ctx, table);
    } else {
        ui_style_default(ctx);
    }
}

#define UI_STYLE_H
#endif
