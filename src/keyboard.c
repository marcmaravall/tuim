#include "keyboard.h"
#include "context.h"

void tuim_clear_input(TuimKeyboardState* state) {
	for (size_t i = 0; i < TUIM_KEY_COUNT; i++) {
		state->current[i] = false;
		state->last[i]    = false;
	}
}

bool tuim_is_key_down(const TuimContext* ctx, const size_t key_code) {
	assert(key_code < TUIM_KEY_COUNT);
	return ctx->input_state.keyboard_state.current[key_code] && !ctx->input_state.keyboard_state.last[key_code];
}

bool tuim_is_key_pressed(const TuimContext* ctx, const size_t key_code) {
	assert(key_code < TUIM_KEY_COUNT);
	return ctx->input_state.keyboard_state.current[key_code];
}

bool tuim_is_key_up(const TuimContext* ctx, const size_t key_code) {
	assert(key_code < TUIM_KEY_COUNT);
	return !ctx->input_state.keyboard_state.current[key_code] && ctx->input_state.keyboard_state.last[key_code];
}
