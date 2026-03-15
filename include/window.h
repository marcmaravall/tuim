#ifndef TUIM_WIDGET_H
#define TUIM_WIDGET_H

#include "rect.h"
#include "frame_buffer.h"
#include "id.h"
#include "input.h"
#include "context.h"

#include <assert.h>
#include <stdbool.h>

#define TUIM_WINDOW_DRAW_BORDER_HORIZONTAL 0x01
#define TUIM_WINDOW_DRAW_BORDER_VERTICAL   0x02
#define TUIM_WINDOW_DRAW_BORDER \
	TUIM_WINDOW_DRAW_BORDER_HORIZONTAL | TUIM_WINDOW_DRAW_BORDER_VERTICAL

// TODO: continue

#define TUIM_WINDOW_DEFAULT_WIDTH  20
#define TUIM_WINDOW_DEFAULT_HEIGHT 5 

typedef struct {
	TuimRect rect;

	struct TuimWindowStyle {
		TuimColor title_bar_color;
		TuimColor background;
		TuimColor border_color;
		TuimColor title_color;

		uint8_t flags;
	};

	char* title;

	int start_mouse_resize_x;
	int start_mouse_resize_y;
	
	int drag_offset_x;
	int drag_offset_y;
	
	bool is_resizing;
	bool is_dragging;
} TuimWindow;

TuimWindow tuim_default_window();

void tuim_window_draw  (TuimContext* ctx, TuimWindow* widget);
void tuim_window_update(TuimContext* ctx, TuimWindow* widget);
bool tuim_window_is_hovered(const TuimContext* ctx, const TuimWindow* widget);

#endif // TUIM_WIDGET_H