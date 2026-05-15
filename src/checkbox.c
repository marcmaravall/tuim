#include "checkbox.h"

TuimCheckbox* tuim_default_checkbox() {
	TuimCheckbox* checkbox = malloc(sizeof(TuimCheckbox));

	checkbox->label = mds_new("checkbox");
	checkbox->area.x = 0;
	checkbox->area.y = 0;
	checkbox->area.width = 10;
	checkbox->area.height = 1;
	checkbox->toggled = false;
	checkbox->hovered = false;
	checkbox->clicked = false;
	checkbox->pressed_inside = false;
	checkbox->pressing = false;
	checkbox->was_down = false;
	checkbox->flags = 0;

	checkbox->style.background = TUIM_BLACK_STRUCT_INDEXED;
	checkbox->style.foreground = TUIM_WHITE_STRUCT_INDEXED;
	checkbox->style.hover_background = TUIM_GREEN_STRUCT_INDEXED;
	checkbox->style.hover_foreground = TUIM_WHITE_STRUCT_INDEXED;
	checkbox->style.active_background = TUIM_BLUE_STRUCT_INDEXED;
	checkbox->style.active_foreground = TUIM_WHITE_STRUCT_INDEXED;

	return checkbox;
}

void tuim_checkbox_draw(TuimContext* ctx, const TuimCheckbox* checkbox) {
	MEB_ASSERT(ctx && checkbox);

	const char* status = checkbox->toggled ? "[x] " : "[ ] ";
	size_t len = strlen(status);

	char* label = (mds_get(checkbox->label) == NULL) ? "" : mds_get(checkbox->label);

	TuimColor bg;
	TuimColor fg;

	if (checkbox->hovered) {
		bg = checkbox->style.hover_background;
		fg = checkbox->style.hover_foreground;
	}
	else if (checkbox->toggled) {
		bg = checkbox->style.active_background;
		fg = checkbox->style.active_foreground;
	}
	else {
		bg = checkbox->style.background;
		fg = checkbox->style.foreground;
	}

	tuim_frame_buffer_print (
		&ctx->viewport.frame_buffer, fg, bg,
		status,
		checkbox->area.x,
		checkbox->area.y
	);

	tuim_frame_buffer_print(
		&ctx->viewport.frame_buffer, fg, bg,
		label,
		checkbox->area.x + (int)len,
		checkbox->area.y
	);
}

void tuim_checkbox_update(const TuimContext* ctx, TuimCheckbox* checkbox) {
	MEB_ASSERT(ctx && checkbox);

	checkbox->area = tuim_checkbox_calculate_area(checkbox);

	int mouse_x, mouse_y;
	tuim_get_mouse_position(ctx, &mouse_x, &mouse_y);

	bool is_down = tuim_is_mouse_button_down(ctx, TUIM_MOUSE_BUTTON_LEFT);
	bool is_inside = tuim_is_mouse_inside(ctx, checkbox->area);

	checkbox->pressing = is_inside && is_down;
	checkbox->hovered = is_inside;
	checkbox->clicked = false;

	if (is_down && !checkbox->was_down) {
		checkbox->pressed_inside = is_inside;
	}

	if (!is_down && checkbox->was_down) {
		if (checkbox->pressed_inside && is_inside) {
			checkbox->clicked = true;
			checkbox->toggled = !checkbox->toggled;
		}
	}

	checkbox->was_down = is_down;
}

void tuim_checkbox_destroy(TuimCheckbox* checkbox) {
	MEB_ASSERT(checkbox);
	// idk
}

TuimElement tuim_checkbox_element(const char* label) {
	MEB_ASSERT(label);
	TuimCheckbox* checkbox = tuim_checkbox(label);
	return tuim_checkbox_to_element(checkbox);
}

TuimElement tuim_checkbox_to_element(TuimCheckbox* checkbox) {
	MEB_ASSERT(checkbox);

	TuimElement el;
	el.data = checkbox;
	el.draw = tuim_checkbox_draw;
	el.update = tuim_checkbox_update;
	el.destroy = tuim_checkbox_destroy;
	el.layout = tuim_checkbox_layout;
	el.measure = tuim_checkbox_measure;

	return el;
}

TuimSizeHint tuim_checkbox_measure(const TuimCheckbox* checkbox) {
	MEB_ASSERT(checkbox);
	TuimSizeHint size;

	size.min_width = 4; // for "[ ] "
	size.min_height = 1;
	size.preferred_width = 4 + mds_size(checkbox->label);
	size.preferred_height = 1;
	size.max_width = size.preferred_width;
	size.max_height = size.preferred_height;

	return size;
}

void tuim_checkbox_layout(TuimCheckbox* Checkbox, const TuimRect rect) {
	MEB_ASSERT(Checkbox);
	Checkbox->area.x = rect.x;
	Checkbox->area.y = rect.y;
}

inline bool tuim_checkbox_get(const TuimCheckbox* checkbox) {
	return checkbox->toggled;
}

// TODO: add cases depending on flags
TuimRect tuim_checkbox_calculate_area(const TuimCheckbox* checkbox) {
	MEB_ASSERT(checkbox);

	TuimRect area = checkbox->area;
	size_t label_len = mds_size(checkbox->label) + 4;
	area.width = (int)label_len;

	return area;
}

TuimCheckbox* tuim_checkbox(const char* label) {
	TuimCheckbox* checkbox = tuim_default_checkbox();
	checkbox->label = mds_new(label);
	return checkbox;
}
