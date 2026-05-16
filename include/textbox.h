#ifndef TUIM_TEXTBOX_H
#define TUIM_TEXTBOX_H

#include "rect.h"
#include "element.h"
#include "context.h"
#include "frame_buffer.h"
#include "color.h"

#include <meb.h>
#include <mds.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct {
    TuimColor fg;
    TuimColor bg;

    TuimColor selected_fg;
    TuimColor selected_bg;

    TuimColor cursor_color;
} TuimTextboxStyle;

typedef struct {
    TuimTextboxStyle style;
    TuimRect         area;

    // it combines mdString with old implementation (it works so im not going to modify it)
    mdString text;

    bool is_selected;
    int  cursor_pos;

    double blink_time;
    int    blink_delay;

} TuimTextbox;

#define TUIM_TEXTBOX_INITIAL_CAPACITY 63

TuimTextbox* tuim_textbox(const char* str);
TuimTextbox* tuim_default_textbox(void);

void tuim_update_textbox(TuimContext* ctx, TuimTextbox* textbox);
void tuim_draw_textbox(TuimContext* ctx, const TuimTextbox* textbox);
void tuim_destroy_textbox(TuimTextbox* textbox);

TuimElement  tuim_textbox_element   (const char* str);
TuimElement  tuim_textbox_to_element(TuimTextbox* textbox);
void         tuim_textbox_layout(TuimTextbox* textbox, const TuimRect area);
TuimSizeHint tuim_textbox_measure(TuimTextbox* textbox);

#endif // TUIM_TEXTBOX_H