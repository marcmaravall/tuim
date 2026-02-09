#include <context.h>

void tuim_set_alginment(TuimContext* ctx, TuimAlgin al) {
	ctx->algin = al;
}

void tuim_begin_frame(TuimContext* ctx) {
	// TODO: implement
}

void tuim_end_frame(TuimContext* ctx) {
	// ctx->frame_index++;
}

void tuim_init_context(TuimContext* ctx) {
	// ctx->cursor_x = 1;
	// ctx->cursor_y = 1;
	// ctx->frame_index = 1;
	ctx->algin = TUIM_ALGIN_DEFAULT;
	
	tuim_style_set_default(&ctx->style);
}

void tuim_destroy_context(TuimContext* ctx) {

}
