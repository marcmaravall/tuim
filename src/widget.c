#include "widget.h"

void tuim_widget_begin(TuimWidget* widget, const char* title) {
	widget->name = title;
}

void tuim_widget_draw(TuimWidget* widget, const TuimFrameBuffer* fb) {
	assert(widget);
	assert(fb);
}

void tuim_widget_update(TuimWidget* widget, const TuimInputState* state) {
	assert(state);
	assert(widget);

	if (tuim_is_mouse_inside(widget, state->mouse_state) && state->mouse_state.current) {
		widget->rect.x = state->mouse_state.mouse_x;
		widget->rect.y = state->mouse_state.mouse_y;
	}
}

void tuim_widget_end(TuimWidget* widget) {

}
