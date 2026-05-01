#include "mouse.h"
#include "context.h"

bool tuim_is_mouse_inside(const TuimContext* ctx, const TuimRect area) {
	MEB_ASSERT(ctx);
    return 
        ctx->input_state.mouse_state.mouse_x >= area.x &&
        ctx->input_state.mouse_state.mouse_x < area.x + area.width &&
        ctx->input_state.mouse_state.mouse_y >= area.y &&
        ctx->input_state.mouse_state.mouse_y < area.y + area.height;
}

void tuim_get_mouse_position(const TuimContext* state, int* x, int* y) {
    MEB_ASSERT(state);
    *x = state->input_state.mouse_state.mouse_x;
    *y = state->input_state.mouse_state.mouse_y;
}

void tuim_update_mouse_state(TuimMouseState* state) {
	MEB_ASSERT(state);
	state->previous = state->current;
}

void tuim_clear_mouse_state(TuimMouseState* state) {
	MEB_ASSERT(state);
	state->current = 0;
	state->previous = 0;
}

bool tuim_is_mouse_button_down(const TuimContext* state, const size_t button) {
    MEB_ASSERT(state);
    return (state->input_state.mouse_state.current & button) &&
        !(state->input_state.mouse_state.previous & button);
}

bool tuim_is_mouse_button (const TuimContext* state, const size_t button) {
    MEB_ASSERT(state);
    return (state->input_state.mouse_state.current & button);
}

bool tuim_is_mouse_button_up(const TuimContext* state, const size_t button) {
    MEB_ASSERT(state);
    return !(state->input_state.mouse_state.current & button) &&
        (state->input_state.mouse_state.previous & button);
}
