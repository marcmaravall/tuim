#ifndef TUIM_CONTEXT_H
#define TUIM_CONTEXT_H

#include <meb.h>

#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "ascii.h"
#include "unicode.h"
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

	// time elapsed since init context
	double time;
} TuimContext;

void tuim_begin_frame	(TuimContext* context);
void tuim_end_frame		(TuimContext* context);

void tuim_init_context	(TuimContext* ctx);
void tuim_destroy_context(TuimContext* ctx);

// backend calls:
void tuim_resize_context(TuimContext* ctx, const size_t width, const size_t height);
void tuim_set_backend_attrib (TuimContext* ctx, const tuim_backend_attrib_t attrib, const char* value);
// this should not be on context.h
char* tuim_get_clipboard(TuimContext* ctx);
double tuim_get_delta_time(TuimContext* ctx);
double tuim_get_fps(TuimContext* ctx);
double tuim_get_time(TuimContext* ctx);

#endif //TUIM_CONTEXT_H