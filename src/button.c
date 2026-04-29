#include "button.h"

TuimButton tuim_default_button() {
	TuimButton button;

	button.label = "Button";
	button.area.x = 0;
	button.area.y = 0;
	button.area.width = 10;
	button.area.height = 1;
	button.hovered = false;
	button.clicked = false;
	button.pressed_inside = false;
	button.pressing = false;
	button.was_down = false;

	button.style.background = TUIM_BLACK_STRUCT_INDEXED;
	button.style.foreground = TUIM_WHITE_STRUCT_INDEXED;
	button.style.hover_background = TUIM_GREEN_STRUCT_INDEXED;
	button.style.hover_foreground = TUIM_WHITE_STRUCT_INDEXED;

	button.user_data = NULL;
	button.on_click = NULL;
	button.on_hover = NULL;
	button.on_release = NULL;

	return button;
}

void tuim_button_draw(TuimContext* ctx, const TuimButton* button) {
	MEB_ASSERT(ctx && button);

	const char* label = button->label ? button->label : "";

	TuimColor bg;
	TuimColor fg;

	if (button->hovered) {
		bg = button->style.hover_background;
		fg = button->style.hover_foreground;
	}
	else {
		bg = button->style.background;
		fg = button->style.foreground;
	}

	tuim_frame_buffer_print(
		&ctx->frame_buffer, fg, bg,
		label,
		button->area.x,
		button->area.y
	);
}

void tuim_button_update(const TuimContext* ctx, TuimButton* button) {
	MEB_ASSERT(ctx && button);

	button->area = tuim_button_calculate_area(button);

	int mouse_x, mouse_y;
	tuim_get_mouse_position(ctx, &mouse_x, &mouse_y);

	bool is_down = tuim_is_mouse_button(ctx, TUIM_MOUSE_BUTTON_LEFT);
	bool is_inside = tuim_is_mouse_inside(ctx, button->area);

	button->pressing = is_inside && is_down;
	button->hovered = is_inside;
	button->clicked = false;

	if (is_down && !button->was_down) {
		button->pressed_inside = is_inside;
	}

	if (!is_down && button->was_down) {
		if (button->pressed_inside && is_inside) {
			button->clicked = true;

			if (button->on_release)
				button->on_release(button->user_data);
		}
	}

	// ---------
	if (is_down && is_inside) {
		if (button->on_click)
			button->on_click(button->user_data);
	}

	if (button->hovered) {
		if (button->on_hover)
			button->on_hover(button->user_data);
	}

	// ---------

	button->was_down = is_down;
}

void tuim_button_destroy(TuimButton* button) {
	MEB_ASSERT(button);
	// idk
}

TuimElement tuim_button_to_element(TuimButton* button) {
	MEB_ASSERT(button);

	TuimElement el;
	el.data = button;
	el.draw = tuim_button_draw;
	el.update = tuim_button_update;
	el.destroy = tuim_button_destroy;
	el.layout = tuim_button_layout;
	el.measure = tuim_button_measure;

	return el;
}

TuimSizeHint tuim_button_measure(const TuimButton* button) {
	MEB_ASSERT(button);
	TuimSizeHint size;

	size.min_width = 1;
	size.min_height = 1;
	size.preferred_width = strlen(button->label);
	size.preferred_height = 1;
	size.max_width = size.preferred_width;
	size.max_height = size.preferred_height;

	return size;
}

void tuim_button_layout(TuimButton* button, const TuimRect rect) {
	MEB_ASSERT(button);
	button->area.x = rect.x;
	button->area.y = rect.y;
}

TuimRect tuim_button_calculate_area(const TuimButton* button) {
	MEB_ASSERT(button && button->label);

	TuimRect area = button->area;
	size_t label_len = strlen(button->label);
	area.width = (int)label_len; 
	
	return area;
}

TuimButton tuim_button(const char* label) {
	MEB_ASSERT(label);

	TuimButton button = tuim_default_button();
	button.label = label;
	return button;
}

TuimButton tuim_button_callbacks(
	const char* label,
	TuimOnClickFn   on_click,
	TuimOnHoverFn   on_hover,
	TuimOnReleaseFn on_release,
	void* user_data
) {
	MEB_ASSERT(label);
	TuimButton button = tuim_default_button();
	button.label = label;
	button.on_click = on_click;
	button.on_hover = on_hover;
	button.on_release = on_release;
	button.user_data = user_data;
	return button;
}
