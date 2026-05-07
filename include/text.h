#ifndef TUIM_TEXT_H
#define TUIM_TEXT_H

#include "context.h"
#include "backend.h"
#include "frame_buffer.h"
#include "size_hint.h"
#include "color.h"
#include "element.h"

#include <meb.h>
#include <mds.h>

#include <string.h>
#include <stdarg.h>

typedef struct {
	TuimColor foreground;
	TuimColor background;

	TuimRect area;
	mdString text;
} TuimText;

TuimText* tuim_default_text();
TuimText* tuim_text(const char* str);

void tuim_text_update(TuimContext* ctx, TuimText* data);
void tuim_draw_text(TuimContext* ctx, const TuimText* text);
void tuim_text_destroy(TuimText* text);

void tuim_text_format(TuimText* text, const char* format, ...);

void tuim_text_on_focus_gained	(TuimText* data);
void tuim_text_on_focus_lost	(TuimText* data);

// allocates a new TuimText and returns its element
TuimElement tuim_text_element (const char* str);

TuimElement tuim_text_to_element(const TuimText* text);

#endif //TUIM_TEXT_H