#include "mouse.h"
#include "context.h"

bool tuim_is_mouse_inside(const TuimContext* ctx, const TuimRect area) {
	MEB_ASSERT(ctx);
	
	// local mouse position relative to viewport
    int local_x = ctx->input_state.mouse_state.mouse_x - ctx->viewport.x;
    int local_y = ctx->input_state.mouse_state.mouse_y - ctx->viewport.y;
   
    return 
        local_x >= area.x &&
        local_x <  area.x + area.width &&
        local_y >= area.y &&
        local_y <  area.y + area.height;
}

void tuim_get_mouse_position(const TuimContext* state, int* x, int* y) {
    MEB_ASSERT(state);
    *x = state->input_state.mouse_state.mouse_x;
    *y = state->input_state.mouse_state.mouse_y;

	*x -= state->viewport.x;
	*y -= state->viewport.y;
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
