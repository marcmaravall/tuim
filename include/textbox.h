#ifndef TUIM_TEXTBOX_H
#define TUIM_TEXTBOX_H

#include "rect.h"
#include "element.h"
#include "color.h"

#include <meb.h>
#include <string.h>

typedef struct {
	TuimColor fg;
	TuimColor bg;

	TuimColor selected_fg;
	TuimColor selected_bg;

	TuimColor cursor_color;
} TuimTextboxStyle;

typedef struct {
	TuimTextboxStyle style;
	TuimRect area;
	char* text;
} TuimTextbox;

TuimTextbox tuim_default_textbox();

void tuim_update_textbox (TuimContext* ctx, TuimTextbox* textbox);
void tuim_draw_textbox   (TuimContext* ctx, const TuimTextbox* textbox);
void tuim_destroy_textbox(TuimTextbox* textbox);

TuimElement tuim_textbox_to_element(const TuimTextbox* textbox);
void tuim_textbox_layout(TuimTextbox* textbox, TuimRect area);
TuimSizeHint tuim_textbox_measure(TuimTextbox* textbox);

#endif // TUIM_TEXTBOX_H