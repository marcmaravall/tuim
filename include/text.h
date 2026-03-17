#ifndef TUIM_TEXT_H
#define TUIM_TEXT_H

#include "context.h"
#include "backend.h"
#include "frame_buffer.h"

#include "color.h"

typedef struct {
	TuimColor foreground;
	TuimColor background;

	// TuimRect area;	// TODO: do width rect
	// if you dont undestand a shit fuch you from the past

	int x;
	int y;

	char* text;
} TuimText;

TuimText tuim_default_text();
void tuim_draw_text(TuimContext* ctx, const TuimText* text);

#endif //TUIM_TEXT_H