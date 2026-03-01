#ifndef TUIM_STATE_H
#define TUIM_STATE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "color.h"

#define TUIM_TEXT_ATTR_BOLD				0x01
#define TUIM_TEXT_ATTR_UNDERLINE		0x02
#define TUIM_TEXT_ATTR_REVERSED			0x04
#define TUIM_TEXT_ATTR_BLINK			0x08
#define TUIM_TEXT_ATTR_DIM				0x10
#define TUIM_TEXT_ATTR_HIDDEN			0x20
#define TUIM_TEXT_ATTR_STRIKETHROUGH	0x40
#define TUIM_TEXT_ATTR_ITALIC			0x80

typedef struct {
	TuimColor foreground_color;
	TuimColor background_color;
	uint8_t text_attributes;
} TuimCell;

typedef struct {
	size_t width;
	size_t height;
	TuimCell* cells;
} TuimFrameBuffer;

void tuim_cell_frame_buffer_init(TuimFrameBuffer* state, const size_t width, const size_t height);
void tuim_cell_frame_buffer_clear(TuimFrameBuffer* state);
void tuim_cell_frame_buffer_destroy(TuimFrameBuffer* state);

#endif //TUIM_STATE_H