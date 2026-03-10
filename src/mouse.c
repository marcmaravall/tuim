#include "mouse.h"
#include "context.h"

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
