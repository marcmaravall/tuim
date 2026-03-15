#ifndef TUIM_STATE_H
#define TUIM_STATE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
#include <math.h>

#include "color.h"
#include "style.h"

typedef struct {
	TuimColor foreground_color;
	TuimColor background_color;
	char ascii_char;
} TuimFrameBufferCell;

typedef struct {
	size_t width;
	size_t height;
	TuimFrameBufferCell* cells;
} TuimFrameBuffer;

#define TUIM_FRAME_BUFFER_AT(fb, x, y) ((fb)->cells[(y) * (fb)->width + (x)])
#define TUIM_FRAME_BUFFER_SET_AT(fb, x, y, cell) (fb)->cells[(y) * (fb)->width + (x)] = (cell);

// TODO: add color functions options
// TODO: remove style dependencies from frame buffer
// TODO: add custom ansi chars for all shapes

void tuim_frame_buffer_init		(TuimFrameBuffer* ascii_char, const size_t width, const size_t height);
void tuim_frame_buffer_clear	(const TuimStyle* const style, TuimFrameBuffer* ascii_char);

void tuim_frame_buffer_print	   (const TuimStyle* style, TuimFrameBuffer* fb, const char* msg, const int x, const int y);
void tuim_frame_buffer_print_color (TuimFrameBuffer* fb, const TuimColor fg, const TuimColor bg, const char* msg, const int x, const int y);

void tuim_frame_buffer_draw_line (const TuimStyle* style, TuimFrameBuffer* fb, int x0, int y0, const int x1, const int y1);
void tuim_frame_buffer_draw_line_color (TuimFrameBuffer* fb, const TuimColor color, int x0, int y0, const int x1, const int y1);

void tuim_frame_buffer_draw_rect (const TuimStyle* style, TuimFrameBuffer* fb, const int x, const int y, const int width, const int height);
void tuim_frame_buffer_draw_rect_color (TuimFrameBuffer* fb, const TuimColor color, const int x, const int y, const int width, const int height);

void tuim_frame_buffer_draw_equation_line (const TuimStyle* style, TuimFrameBuffer* fb, const double m, const double n);

void tuim_frame_buffer_destroy(TuimFrameBuffer* ascii_char);

#endif //TUIM_STATE_H