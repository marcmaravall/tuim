#ifndef TUIM_CONTEXT_H
#define TUIM_CONTEXT_H
#include <stdio.h>

typedef struct TuimContext {
	int		frame_index;
	float	delta_time;

	int cursor_x;
	int cursor_y;

	int available_width;

} TuimContext;

void tuim_init_context(TuimContext* context);
void tuim_destroy_context(TuimContext* context);

#endif //TUIM_CONTEXT_H