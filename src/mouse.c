#include "mouse.h"

bool tuim_is_mouse_inside(const TuimMouseState* state, const TuimRect area) {
    if (state->mouse_x < area.x || state->mouse_y < state->mouse_y ||
        state->mouse_x > area.x + area.width || state->mouse_y > area.y + area.height)
        return false;

    return true;
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

bool tuim_is_mouse_button_pressed(const TuimContext* state, const size_t button) {
    assert(state);
    return (state->input_state.mouse_state.current & button);
}

bool tuim_is_mouse_button_up(const TuimContext* state, const size_t button) {
    assert(state);
    return !(state->input_state.mouse_state.current & button) &&
        (state->input_state.mouse_state.previous & button);
}
