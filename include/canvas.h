#ifndef TUIM_CANVAS_H
#define TUIM_CANVAS_H

#include "color.h"
#include "context.h";
#include "frame_buffer.h"
#include "rect.h"
#include "element.h"

#include <stdio.h>
#include <assert.h>

typedef struct {
	TuimRect rect;
	TuimColor* pixels;
} TuimCanvas;

void tuim_canvas_init  (TuimCanvas* canvas, const size_t width, const size_t height);

void tuim_canvas_draw  (TuimContext* ctx, const TuimCanvas* canvas);
void tuim_canvas_update(TuimContext* ctx, TuimCanvas* canvas);

void tuim_canvas_fill(TuimCanvas* canvas, const TuimColor color);
void tuim_canvas_rect(TuimCanvas* canvas, const TuimRect rect, const TuimColor color);
void tuim_canvas_point(TuimCanvas* canvas, const int x, const int y, const TuimColor color);
// void tuim_canvas_texture(TuimCanvas* canvas);

void tuim_canvas_destroy(TuimCanvas* canvas);

void tuim_canvas_layout(TuimCanvas* canvas, const TuimRect rect);
TuimSizeHint tuim_canvas_measure(const TuimCanvas* canvas);

TuimElement tuim_canvas_to_element(TuimCanvas* canvas);

#endif // TUIM_CANVAS_H