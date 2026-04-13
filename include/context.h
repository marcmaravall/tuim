#ifndef TUIM_CONTEXT_H
#define TUIM_CONTEXT_H

#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "backend.h"
#include "frame_buffer.h"

// TODO: implement
typedef struct {
	int frame_index;

	int cursor_x;
	int cursor_y;
	
	bool is_mouse_down;

} TuimFrameContext;

typedef struct TuimContext {
	TuimFrameBuffer frame_buffer;
	TuimFrameContext frame_context;

	int available_width;

	TuimStyle style;

	TuimBackend backend;

	TuimInputState input_state;
} TuimContext;

void tuim_begin_frame	(TuimContext* context);
void tuim_end_frame		(TuimContext* context);

void tuim_init_context	(TuimContext* ctx);
void tuim_destroy_context(TuimContext* ctx);

// backend calls:
void tuim_resize_context(TuimContext* ctx, const size_t width, const size_t height);
void tuim_set_backend_attrib (TuimContext* ctx, const char* attrib, const char* value);

#endif //TUIM_CONTEXT_H