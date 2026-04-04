#ifndef TUIM_TEXT_H
#define TUIM_TEXT_H

#include "context.h"
#include "backend.h"
#include "frame_buffer.h"
#include "size_hint.h"
#include "color.h"
#include "element.h"

typedef struct {
	TuimColor foreground;
	TuimColor background;

	TuimRect area;

	char* text;
} TuimText;

TuimText tuim_default_text();
TuimText tuim_text(char* str);

void tuim_text_update(TuimContext* ctx, TuimText* data);
void tuim_draw_text(TuimContext* ctx, const TuimText* text);

TuimElement tuim_text_to_element(const TuimText* text);

#endif //TUIM_TEXT_H