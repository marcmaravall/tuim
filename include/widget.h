#ifndef TUIM_WIDGET_H
#define TUIM_WIDGET_H

#include "rect.h"
#include "frame_buffer.h"
#include "id.h"
#include "input.h"
#include "context.h"

#include <assert.h>
#include <stdbool.h>

#define TUIM_WIDGET_DEFAULT_WIDTH  20
#define TUIM_WIDGET_DEFAULT_HEIGHT 5 

typedef struct {
	TuimRect rect;

	struct TuimWidgetStyle {
		TuimColor title_bar_color;
		TuimColor title_bar;
	};

	char* title;

	int start_mouse_resize_x;
	int start_mouse_resize_y;
	
	int drag_offset_x;
	int drag_offset_y;
	
	bool is_resizing;
	bool is_dragging;
} TuimWidget;

TuimWidget tuim_default_widget();

void tuim_widget_draw  (TuimContext* ctx, TuimWidget* widget);
void tuim_widget_update(TuimContext* ctx, TuimWidget* widget);

#endif // TUIM_WIDGET_H