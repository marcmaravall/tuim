#include "window.h"

TuimWindow tuim_default_window() {
	TuimWindow w;
	tuim_layout_init(&w.layout, TUIM_WINDOW_LAYOUT_DEFAULT_CAPACITY);
	
	w.title = "hello, world!";
	
	w.min_height = TUIM_WINDOW_DEFAULT_MIN_HEIGHT;
	w.min_width  = TUIM_WINDOW_DEFAULT_MIN_WIDTH;
	w.max_height = TUIM_WINDOW_DEFAULT_MAX_HEIGHT;
	w.max_width = TUIM_WINDOW_DEFAULT_MAX_WIDTH;

	w.drag_offset_x = -1;
	w.drag_offset_y = -1;
	w.is_dragging = false;
	w.is_resizing = false;

	w.rect.x = 0;
	w.rect.y = 0;
	w.rect.width  = TUIM_WINDOW_DEFAULT_WIDTH;
	w.rect.height = TUIM_WINDOW_DEFAULT_HEIGHT;

	tuim_window_resize(&w, w.rect);

	w.start_mouse_resize_x = -1;
	w.start_mouse_resize_y = -1;

	w.style.border_color = TUIM_BRIGHT_BLUE_STRUCT_INDEXED;
	w.style.title_bar_color = TUIM_BLUE_STRUCT_INDEXED;
	w.style.title_color = TUIM_WHITE_STRUCT_INDEXED;
	w.style.background = TUIM_BLACK_STRUCT_INDEXED;

	return w;
}

// TODO: use less malloc 
void tuim_window_draw(TuimContext* ctx, TuimWindow* window) {
	MEB_ASSERT(ctx && window);
	MEB_ASSERT(window->title);

	const size_t title_size = strlen(window->title);
	char* title_to_render;

	if (title_size > window->rect.width) {
		title_to_render = malloc(window->rect.width + 1);
		MEB_ASSERT(title_to_render);

		strncpy(title_to_render, window->title, window->rect.width);

		size_t from = window->rect.width - 3;
		for (size_t i = from; i < window->rect.width; ++i) {
			title_to_render[i] = '.';
		}

		title_to_render[window->rect.width] = '\0';

		MEB_ASSERT(title_to_render);
	}
	else {
		title_to_render = window->title;
	}

	const int x0 = window->rect.x;
	const int y0 = window->rect.y;
	const int x1 = window->rect.x + window->rect.width - 1;
	const int y1 = window->rect.y + window->rect.height-1;

	tuim_frame_buffer_draw_rect (
		&ctx->frame_buffer, window->style.background, x0, y0, 
		window->rect.width, window->rect.height
	);

	tuim_layout_draw(ctx, &window->layout);
	
	tuim_frame_buffer_draw_line (
		&ctx->frame_buffer, window->style.border_color,
		x1, y0, x1, y1
	);

	tuim_frame_buffer_draw_line (
		&ctx->frame_buffer, window->style.border_color,
		x0, y1, x1, y1
	);

	tuim_frame_buffer_draw_line (
		&ctx->frame_buffer, window->style.title_bar_color,
		x0, y0, x1, y0
	);

	tuim_frame_buffer_print (
		&ctx->frame_buffer, window->style.title_color, window->style.title_bar_color,
		title_to_render, window->rect.x, window->rect.y
	);
}

void tuim_window_destroy(TuimWindow* window) {
	MEB_ASSERT(window);
	// window->title = NULL;
}

// returns 1 if the window was updated, 0 otherwise
int tuim_window_update(TuimContext* ctx, TuimWindow* window) {
	MEB_ASSERT(ctx && window);

	tuim_layout_update(ctx, &window->layout);

	int mouse_x, mouse_y;
	tuim_get_mouse_position(ctx, &mouse_x, &mouse_y);

	bool mouse_inside = tuim_is_mouse_inside(ctx, window->rect);

	bool left_down = tuim_is_mouse_button_down(ctx, TUIM_MOUSE_BUTTON_LEFT);
	bool left_pressed = tuim_is_mouse_button(ctx, TUIM_MOUSE_BUTTON_LEFT);
	bool left_released = tuim_is_mouse_button_up(ctx, TUIM_MOUSE_BUTTON_LEFT);

	int right = window->rect.x + window->rect.width - 1;
	int bottom = window->rect.y + window->rect.height - 1;

	bool on_resize_corner = (mouse_x == right && mouse_y == bottom);

	int res = TUIM_WINDOW_UPDATE_NONE;

	if (left_down && on_resize_corner) {
		window->is_resizing = true;
		window->is_dragging = false;

		window->start_mouse_resize_x = mouse_x;
		window->start_mouse_resize_y = mouse_y;
	}

	// start action
	if (left_down && mouse_inside) {
		window->is_dragging = true;
		window->is_resizing = false;
	
		window->drag_offset_x = mouse_x - window->rect.x;
		window->drag_offset_y = mouse_y - window->rect.y;
	}

	if (left_released) {
		window->is_dragging = false;
		window->is_resizing = false;

		res = TUIM_WINDOW_UPDATE_RELEASED;
	}

	// dragging 
	if (window->is_dragging && left_pressed) {
		window->rect.x = mouse_x - window->drag_offset_x;
		window->rect.y = mouse_y - window->drag_offset_y;
		res = TUIM_WINDOW_UPDATE_DRAGGED;
	}

	// resizing
	else if (window->is_resizing && left_pressed) {
		int new_w = mouse_x - window->rect.x + 1;
		int new_h = mouse_y - window->rect.y + 1;

		int min_w = max((int)window->min_width, 1);
		int min_h = max((int)window->min_height, 1);

		window->rect.width = max(min_w, new_w);
		window->rect.height = max(min_h, new_h);

		window->rect.width = (int)min(window->rect.width, (int)window->max_width);
		window->rect.height = (int)min(window->rect.height, (int)window->max_height);

		res = TUIM_WINDOW_UPDATE_RESIZED;
	}

	if (res != TUIM_WINDOW_UPDATE_NONE)
		// change layout settings
		tuim_window_resize(window, window->rect);

	return res;
}

void tuim_window_resize(TuimWindow* window, const TuimRect rect) {
	MEB_ASSERT(window);

	window->rect = rect;
	window->layout.bounds.x = window->rect.x;
	window->layout.bounds.y = window->rect.y + 1;
	window->layout.bounds.width = window->rect.width;
	window->layout.bounds.height = window->rect.height - 1;
}

void tuim_window_layout(TuimWindow* window, const TuimRect rect) {
	MEB_ASSERT(window);
	// for now, just set window rect 
	window->rect = rect;
	window->rect.width  = max((int)window->min_width,  window->rect.width);
	window->rect.height = max((int)window->min_height, window->rect.height);
	window->rect.width  = min((int)window->max_width,  window->rect.width);
	window->rect.height = min((int)window->max_height, window->rect.height);
}

TuimSizeHint tuim_window_measure(const TuimWindow* data) {
	MEB_ASSERT(data);

	TuimSizeHint size;
	size.max_width = data->max_width;
	size.max_height = data->max_height;
	size.min_width = data->min_width;
	size.min_height = data->min_height;
	size.preferred_width = data->rect.width;
	size.preferred_height = data->rect.height;

	return size;
}

TuimElement tuim_window_to_element(TuimWindow* window) {
	MEB_ASSERT(window);
	TuimElement el;
	el.data = window;
	el.update = tuim_window_update;
	el.draw   = tuim_window_draw;
	el.layout = tuim_window_layout;
	el.measure = tuim_window_measure;
	el.destroy = tuim_window_destroy;
	return el;
}

bool tuim_window_is_hovered(const TuimContext* ctx, const TuimWindow* window) {
	MEB_ASSERT(ctx && window);

	if (window->is_dragging || window->is_resizing)
		return true;
	return false;
}

// helper functions

void tuim_window_add_element(TuimWindow* window, const TuimElement el) {
	MEB_ASSERT(window);

	tuim_layout_add(&window->layout, el);
}

TuimElement tuim_window_add_text(TuimWindow* window, const char* str, TuimText* text) {
	MEB_ASSERT(window && str && text);
	
	*text = tuim_text(str);
	TuimElement el = tuim_text_to_element(text);
	tuim_layout_add(&window->layout, el);
	return el;
}

TuimElement tuim_window_add_button(TuimWindow* window, const char* str, TuimButton* button) {
	MEB_ASSERT(window && str && button);
	
	*button = tuim_button(str);
	TuimElement el = tuim_button_to_element(button);
	tuim_layout_add(&window->layout, el);
	return el;
}

TuimElement tuim_window_add_checkbox(TuimWindow* window, const char* str, TuimCheckbox* check) {
	MEB_ASSERT(window && str && check);
	
	*check = tuim_checkbox(str);
	TuimElement el = tuim_checkbox_to_element(check);
	tuim_layout_add(&window->layout, el);
	return el;
}

TuimElement tuim_window_add_text_list(TuimWindow* window, const size_t capacity, TuimTextList* list) {
	MEB_ASSERT(window && list);
	
	*list = tuim_text_list(capacity);
	TuimElement el = tuim_text_list_to_element(list);
	tuim_layout_add(&window->layout, el);
	
	return el;
}
