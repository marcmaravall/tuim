#ifndef TUIM_CONTEXT_H
#define TUIM_CONTEXT_H

#include <string.h>
#include <stdio.h>

#include <style.h>

typedef enum TuimAlgin {
	TUIM_LEFT	 = 0,
	TUIM_CENTER   = 1,
	TUIM_RIGHT    = 2,
} TuimAlgin;

#define TUIM_ALGIN_DEFAULT TUIM_LEFT

typedef struct TuimContext {
	int cursor_x;
	int cursor_y;

	int frame_index;
	int available_width;

	TuimAlgin algin;

	TuimStyle style;

} TuimContext;

void tuim_set_alginment(TuimContext* ctx, TuimAlgin al);

void tuim_begin_frame	(TuimContext* context);
void tuim_end_frame		(TuimContext* context);

void tuim_init_context	(TuimContext* ctx);
void tuim_destroy_context(TuimContext* ctx);

#endif //TUIM_CONTEXT_H