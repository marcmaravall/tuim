#include "window.h"

TuimWindow tuim_default_window() {
	TuimWindow w;
	w.drag_offset_x = -1;
	w.drag_offset_y = -1;
	w.is_dragging = false;
	w.is_resizing = false;

	w.rect.x = 0;
	w.rect.y = 0;
	w.rect.width  = TUIM_WINDOW_DEFAULT_WIDTH;
	w.rect.height = TUIM_WINDOW_DEFAULT_HEIGHT;

	w.start_mouse_resize_x = -1;
	w.start_mouse_resize_y = -1;

	w.border_color = TUIM_BRIGHT_BLUE_STRUCT_INDEXED;
	w.title_bar_color = TUIM_BLUE_STRUCT_INDEXED;
	w.title_color = TUIM_WHITE_STRUCT_INDEXED;
	w.background = TUIM_BLACK_STRUCT_INDEXED;

	return w;
}

// TODO: use less malloc and shit 
void tuim_window_draw(TuimContext* ctx, TuimWindow* widget) {
	assert(ctx && widget);
	assert(widget->title);

	const size_t title_size = strlen(widget->title);
	char* title_to_render;

	if (title_size > widget->rect.width) {
		title_to_render = malloc(widget->rect.width + 1);
		assert(title_to_render);

		strncpy(title_to_render, widget->title, widget->rect.width);

		size_t from = widget->rect.width - 3;
		for (size_t i = from; i < widget->rect.width; ++i) {
			title_to_render[i] = '.';
		}

		title_to_render[widget->rect.width] = '\0';

		assert(title_to_render);
	}
	else {
		title_to_render = widget->title;
	}

	const int x0 = widget->rect.x;
	const int y0 = widget->rect.y;
	const int x1 = widget->rect.x + widget->rect.width - 1;
	const int y1 = widget->rect.y + widget->rect.height-1;

	tuim_frame_buffer_draw_rect_color(
		&ctx->frame_buffer, widget->background, x0, y0, 
		widget->rect.width, widget->rect.height
	);
	
	tuim_frame_buffer_draw_line_color (
		&ctx->frame_buffer, widget->border_color,
		x1, y0, x1, y1
	);

	tuim_frame_buffer_draw_line_color(
		&ctx->frame_buffer, widget->border_color,
		x0, y1, x1, y1
	);

	tuim_frame_buffer_draw_line_color (
		&ctx->frame_buffer, widget->title_bar_color,
		x0, y0, x1, y0
	);

	tuim_frame_buffer_print_color (
		&ctx->frame_buffer, widget->title_color, widget->title_bar_color,
		title_to_render, widget->rect.x, widget->rect.y
	);
}

void tuim_window_update(TuimContext* ctx, TuimWindow* widget) {
	assert(ctx && widget);

	int mouse_x, mouse_y;
	tuim_get_mouse_position(ctx, &mouse_x, &mouse_y);

	bool mouse_inside = tuim_is_mouse_inside(&ctx->input_state.mouse_state, widget->rect);

	bool left_down = tuim_is_mouse_button_down(ctx, TUIM_MOUSE_BUTTON_LEFT);
	bool left_pressed = tuim_is_mouse_button(ctx, TUIM_MOUSE_BUTTON_LEFT);
	bool left_released = tuim_is_mouse_button_up(ctx, TUIM_MOUSE_BUTTON_LEFT);

	int right = widget->rect.x + widget->rect.width - 1;
	int bottom = widget->rect.y + widget->rect.height - 1;

	bool on_resize_corner = (mouse_x == right && mouse_y == bottom);

	// start action
	if (left_down && mouse_inside) {

		if (on_resize_corner) {
			widget->is_resizing = true;
			widget->is_dragging = false;

			widget->start_mouse_resize_x = mouse_x;
			widget->start_mouse_resize_y = mouse_y;
		}
		else {
			widget->is_dragging = true;
			widget->is_resizing = false;

			widget->drag_offset_x = mouse_x - widget->rect.x;
			widget->drag_offset_y = mouse_y - widget->rect.y;
		}
	}

	if (left_released) {
		widget->is_dragging = false;
		widget->is_resizing = false;
	}

	// dragging 
	if (widget->is_dragging && left_pressed) {
		widget->rect.x = mouse_x - widget->drag_offset_x;
		widget->rect.y = mouse_y - widget->drag_offset_y;
	}

	// resizing
	else if (widget->is_resizing && left_pressed) {
		int new_w = mouse_x - widget->rect.x + 1;
		int new_h = mouse_y - widget->rect.y + 1;

		widget->rect.width = max(1, new_w);
		widget->rect.height = max(1, new_h);
	}
}

bool tuim_window_is_hovered(const TuimContext* ctx, const TuimWindow* widget) {
	assert(ctx && widget);

	if (widget->is_dragging || widget->is_resizing)
		return true;
	return false;
}
