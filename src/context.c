#include "context.h"

void tuim_set_alginment(TuimContext* ctx, TuimAlgin al) {
	ctx->algin = al;
}

void tuim_begin_frame(TuimContext* ctx) {
	// ctx->backend.clear(ctx->backend.data);
}

void tuim_end_frame(TuimContext* ctx) {

}

void tuim_init_context(TuimContext* ctx) {

	ctx->algin = TUIM_ALGIN_DEFAULT;
	ctx->style = tuim_style_set_default();
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
