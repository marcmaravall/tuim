#include "keyboard.h"
#include "context.h"

bool tuim_is_key_down(const TuimContext* ctx, const size_t key_code) {
	MEB_ASSERT(key_code < TUIM_KEY_COUNT);
	return ctx->input_state.keyboard_state.current[key_code] && !ctx->input_state.keyboard_state.last[key_code];
}

bool tuim_is_key(const TuimContext* ctx, const size_t key_code) {
	MEB_ASSERT(key_code < TUIM_KEY_COUNT);
	return ctx->input_state.keyboard_state.current[key_code];
}

bool tuim_is_key_up(const TuimContext* ctx, const size_t key_code) {
	MEB_ASSERT(key_code < TUIM_KEY_COUNT);
	return !ctx->input_state.keyboard_state.current[key_code] && ctx->input_state.keyboard_state.last[key_code];
}
