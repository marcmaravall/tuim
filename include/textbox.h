#ifndef TUIM_TEXTBOX_H
#define TUIM_TEXTBOX_H

#include "rect.h"
#include "element.h"
#include "context.h"
#include "frame_buffer.h"
#include "color.h"
#include "ascii.h"

#include <meb.h>
#include <string.h>
#include <stdio.h>

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

	bool is_selected;

	// index position in text
	int cursor_pos;
} TuimTextbox;

TuimTextbox tuim_textbox(char* str);
TuimTextbox tuim_default_textbox();

void tuim_update_textbox (TuimContext* ctx, TuimTextbox* textbox);
void tuim_draw_textbox   (TuimContext* ctx, const TuimTextbox* textbox);
void tuim_destroy_textbox(TuimTextbox* textbox);

TuimElement tuim_textbox_to_element(TuimTextbox* textbox);
void tuim_textbox_layout(TuimTextbox* textbox, const TuimRect area);
TuimSizeHint tuim_textbox_measure(TuimTextbox* textbox);

#endif // TUIM_TEXTBOX_H