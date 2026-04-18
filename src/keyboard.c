#include "keyboard.h"
#include "context.h"

bool tuim_is_key_down(const TuimContext* ctx, const tuim_key_code_t key_code) {
	MEB_ASSERT(key_code < TUIM_KEY_COUNT);
	return ctx->input_state.keyboard_state.current[key_code] && !ctx->input_state.keyboard_state.last[key_code];
}

bool tuim_is_key(const TuimContext* ctx, const tuim_key_code_t key_code) {
	MEB_ASSERT(key_code < TUIM_KEY_COUNT);
	return ctx->input_state.keyboard_state.current[key_code];
}

bool tuim_is_key_up(const TuimContext* ctx, const tuim_key_code_t key_code) {
	MEB_ASSERT(key_code < TUIM_KEY_COUNT);
	return !ctx->input_state.keyboard_state.current[key_code] && ctx->input_state.keyboard_state.last[key_code];
}

bool tuim_is_ctrl(const TuimContext* ctx, const tuim_key_code_t key_code) {
	MEB_ASSERT(ctx);
	return tuim_is_key(ctx, TUIM_KEY_CTRL) && tuim_is_key_down(ctx, key_code);
}

bool tuim_is_alt(const TuimContext* ctx, const tuim_key_code_t key_code) {
	MEB_ASSERT(ctx);
	return tuim_is_key(ctx, TUIM_KEY_ALT) && tuim_is_key_down(ctx, key_code);
}

bool tuim_is_shift(const TuimContext* ctx, const tuim_key_code_t key_code) {
	MEB_ASSERT(ctx);
	return tuim_is_key(ctx, TUIM_KEY_SHIFT) && tuim_is_key_down(ctx, key_code);
}
