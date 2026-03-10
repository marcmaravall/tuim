#include "input.h"
#include "context.h"

void tuim_update_input(TuimContext* ctx) {
	for (size_t i = 0; i < TUIM_KEY_COUNT; i++) {
		ctx->input_state.keyboard_state.last[i] = ctx->input_state.keyboard_state.current[i];
	}
	tuim_update_mouse_state(&ctx->input_state.mouse_state);
	ctx->backend.update_input(ctx->backend.data, &ctx->input_state);
}
