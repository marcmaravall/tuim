#include "button.h"

TuimButton tuim_default_button() {
	TuimButton button;

	button.label = "Button";
	button.area.x = 0;
	button.area.y = 0;
	button.area.width = 10;
	button.area.height = 10;
	button.toggled = false;
	button.flags = 0;

	button.style.background = TUIM_BLACK_STRUCT_INDEXED;
	button.style.foreground = TUIM_WHITE_STRUCT_INDEXED;
	button.style.hover_background = TUIM_GREEN_STRUCT_INDEXED;
	button.style.hover_foreground = TUIM_WHITE_STRUCT_INDEXED;  
	button.style.active_background = TUIM_BLUE_STRUCT_INDEXED;
	button.style.active_foreground = TUIM_WHITE_STRUCT_INDEXED;

	return button;
}

void tuim_button_draw(const TuimContext* ctx, const TuimButton* button) {
	assert(ctx && button);

	const char* status = button->toggled ? "[x] " : "[ ] ";
	size_t len = strlen(status);

	const char* label = button->label ? button->label : "";

	TuimColor bg;
	TuimColor fg;

	if (button->hovered) {
		bg = button->style.hover_background;
		fg = button->style.hover_foreground;
	}
	else if (button->toggled) {
		bg = button->style.active_background;
		fg = button->style.active_foreground;
	}
	else {
		bg = button->style.background;
		fg = button->style.foreground;
	}

	tuim_frame_buffer_print(
		ctx, fg, bg,
		status,
		button->area.x,
		button->area.y
	);

	tuim_frame_buffer_print(
		ctx, fg, bg,
		label,
		button->area.x + len,
		button->area.y
	);
}

void tuim_button_update(const TuimContext* ctx, TuimButton* button) {
	assert(ctx && button);

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
			button->toggled = !button->toggled;
		}
	}

	button->was_down = is_down;
}

// TODO: add cases depending on flags
TuimRect tuim_button_calculate_area(const TuimButton* button) {
	assert(button);

	TuimRect area = button->area;
	size_t label_len = strlen(button->label);
	area.width = 4 + label_len; 
	
	return area;
}
