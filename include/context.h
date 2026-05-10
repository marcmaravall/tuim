#ifndef TUIM_CONTEXT_H
#define TUIM_CONTEXT_H

#include <meb.h>

#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "viewport.h"
#include "ascii.h"
#include "unicode.h"
#include "backend.h"

typedef struct TuimContext {
	// viewport of all the screen, this can be changed for custom viewports, for example, 
	// getting mouse input only in a specific area of the screen, or rendering only in a specific area of the screen.
	TuimViewport viewport;

	TuimStyle style;
	TuimBackend backend;
	TuimInputState input_state;

	double time;
} TuimContext;

void tuim_begin_frame	(TuimContext* context);
void tuim_end_frame		(TuimContext* context);

void tuim_init_context	(TuimContext* ctx);
void tuim_init_with_backend(TuimContext* ctx, TuimBackend backend);
void tuim_destroy_context(TuimContext* ctx);

void tuim_set_style(TuimContext* ctx, TuimStyle style);

// backend calls:
void tuim_resize_context(TuimContext* ctx, const size_t width, const size_t height);
void tuim_set_backend_attrib (TuimContext* ctx, const tuim_backend_attrib_t attrib, const char* value);

// this should not be on context.h (or yes)
char* tuim_get_clipboard(TuimContext* ctx);
double tuim_get_delta_time(TuimContext* ctx);
double tuim_get_fps(TuimContext* ctx);
double tuim_get_time(TuimContext* ctx);

int tuim_get_context_width (const TuimContext* ctx);
int tuim_get_context_height(const TuimContext* ctx);

#endif //TUIM_CONTEXT_H