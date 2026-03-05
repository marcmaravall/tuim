#include "context.h"

void tuim_set_alginment(TuimContext* ctx, TuimAlgin al) {
	ctx->algin = al;
}

void tuim_begin_frame(TuimContext* ctx) {
	tuim_frame_buffer_clear(&ctx->frame_buffer);
}

void tuim_init_context(TuimContext* ctx) {
	ctx->backend.init(ctx->backend.data);

	size_t width, height;
	ctx->backend.get_size(ctx->backend.data, &width, &height);

	tuim_frame_buffer_init(&ctx->frame_buffer, width, height);

	ctx->algin = TUIM_ALGIN_DEFAULT;
	ctx->style = tuim_style_set_default();
}

void tuim_end_frame(TuimContext* ctx) {
	ctx->backend.pass_frame_buffer(ctx->backend.data, &ctx->frame_buffer);
	ctx->backend.render(ctx->backend.data);
}

void tuim_destroy_context(TuimContext* ctx) {

}

void tuim_set_rect_background_color(TuimContext* ctx, TuimColor color) {
	assert(ctx != NULL);
	ctx->style.rect_background_color = color;
}

void tuim_set_background_color(TuimContext* ctx, TuimColor color) {
	assert(ctx != NULL);
	ctx->style.background_color = color;
}

void tuim_set_text_foreground_color(TuimContext* ctx, TuimColor color) {
	assert(ctx != NULL);
	ctx->style.text_foreground_color = color;
}
