#include "mouse.h"
#include "context.h"

bool tuim_is_mouse_inside(const TuimContext* state, const TuimRect area) {
	assert(state);
    int x;
    int y;
	tuim_get_mouse_position(state, &x, &y);
    return x >= area.x &&
        x < area.x + area.width-1 &&
        y >= area.y &&
        y < area.y + area.height-1;
}

void tuim_get_mouse_position(const TuimContext* state, int* x, int* y) {
    assert(state);
    *x = state->input_state.mouse_state.mouse_x;
    *y = state->input_state.mouse_state.mouse_y;
}

void tuim_update_mouse_state(TuimMouseState* state) {
	assert(state);
	state->previous = state->current;
}

void tuim_clear_mouse_state(TuimMouseState* state) {
	assert(state);
	state->current = 0;
	state->previous = 0;
}

bool tuim_is_mouse_button_down(const TuimContext* state, const size_t button) {
    assert(state);
    return (state->input_state.mouse_state.current & button) &&
        !(state->input_state.mouse_state.previous & button);
}

bool tuim_is_mouse_button(const TuimContext* state, const size_t button) {
    assert(state);
    return (state->input_state.mouse_state.current & button);
}

bool tuim_is_mouse_button_up(const TuimContext* state, const size_t button) {
    assert(state);
    return !(state->input_state.mouse_state.current & button) &&
        (state->input_state.mouse_state.previous & button);
}
