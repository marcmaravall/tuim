#ifndef TUIM_STATE_H
#define TUIM_STATE_H

#include <meb.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
#include <math.h>

#include "color.h"

typedef struct {
	TuimColor foreground_color;
	TuimColor background_color;
	char ascii_char;
} TuimFrameBufferCell;

bool tuim_frame_buffer_cell_equal(const TuimFrameBufferCell a, const TuimFrameBufferCell b);

typedef struct {
	size_t width;
	size_t height;
	TuimFrameBufferCell* cells;
} TuimFrameBuffer;

#define TUIM_FRAME_BUFFER_AT(fb, x, y) ((fb)->cells[(y) * (fb)->width + (x)])
#define TUIM_FRAME_BUFFER_SET_AT(fb, x, y, cell) (fb)->cells[(y) * (fb)->width + (x)] = (cell);

// TODO: add custom ascii chars for all shapes

void tuim_frame_buffer_init		(TuimFrameBuffer* fb, const size_t width, const size_t height);
void tuim_frame_buffer_clear	(TuimFrameBuffer* fb, const TuimColor color);

void tuim_frame_buffer_print	 (TuimFrameBuffer* fb, const TuimColor fg, const TuimColor bg, const char* msg, const int x, const int y);

void tuim_frame_buffer_draw_line 
	(TuimFrameBuffer* fb, const TuimColor color, 
	int x0, int y0, const int x1, const int y1);

void tuim_frame_buffer_draw_line_char
(TuimFrameBuffer* fb, const TuimColor color,
	const char c, int x0, int y0, const int x1, const int y1);

void tuim_frame_buffer_draw_rect 
	(TuimFrameBuffer* fb, const TuimColor color, 
	const int x, const int y, const int width, const int height);

void tuim_frame_buffer_draw_rect_char 
	(TuimFrameBuffer* fb, const TuimColor color, 
	const char c, const int x, const int y, const int width, const int height);

void tuim_frame_buffer_draw_equation_line (const TuimColor color, TuimFrameBuffer* fb, const double m, const double n);

void tuim_frame_buffer_set_background (TuimFrameBuffer* fb, const TuimColor color, const int x, const int y);

void tuim_frame_buffer_destroy(TuimFrameBuffer* fb);

void tuim_frame_buffer_resize(TuimFrameBuffer* fb, const size_t width, const size_t height);

#endif //TUIM_STATE_H