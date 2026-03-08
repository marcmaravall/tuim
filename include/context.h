#ifndef TUIM_CONTEXT_H
#define TUIM_CONTEXT_H

#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "input.h"
#include "backend.h"

typedef enum {
	TUIM_LEFT,
	TUIM_CENTER,
	TUIM_RIGHT,  
} TuimAlgin;

#define TUIM_ALGIN_DEFAULT TUIM_LEFT

// TODO: implement
typedef struct TuimFrameContext {
	int frame_index;

	int cursor_x;
	int cursor_y;
	
	bool is_mouse_down;

} TuimFrameContext;

typedef struct TuimContext {
	TuimFrameBuffer frame_buffer;
	TuimFrameContext frame_context;

	int available_width;

	TuimAlgin algin;
	TuimStyle style;

	TuimBackend backend;
	TuimInputState input_state;

} TuimContext;

void tuim_set_alginment(TuimContext* ctx, TuimAlgin al);

void tuim_begin_frame	(TuimContext* context);
void tuim_end_frame		(TuimContext* context);

void tuim_init_context	(TuimContext* ctx);
void tuim_destroy_context(TuimContext* ctx);

#endif //TUIM_CONTEXT_H