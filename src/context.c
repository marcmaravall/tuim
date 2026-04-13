#include "context.h"

void tuim_begin_frame(TuimContext* ctx) {
	tuim_frame_buffer_clear(&ctx->frame_buffer, ctx->style.clear_color);
}

void tuim_init_context(TuimContext* ctx) {
	ctx->backend.init(ctx->backend.data);

	size_t width, height;
	ctx->backend.get_size(ctx->backend.data, &width, &height);

	tuim_clear_input(&ctx->input_state);
	tuim_frame_buffer_init(&ctx->frame_buffer, width, height);

	ctx->style = tuim_style_default();
}

void tuim_end_frame(TuimContext* ctx) {
	ctx->backend.pass_frame_buffer(ctx->backend.data, &ctx->frame_buffer);
	ctx->backend.render(ctx->backend.data);
}

void tuim_destroy_context(TuimContext* ctx) {
	assert(ctx);
	ctx->backend.destroy(ctx->backend.data);
	free(ctx->frame_buffer.cells);
	free(ctx->backend.data);
}

// backend calls:

void tuim_resize_context(TuimContext* ctx, const size_t width, const size_t height) {
	assert(ctx);

	if (!ctx->backend.set_size) {
		// warning or something	
	}

	ctx->backend.set_size(ctx->backend.data, width, height);
}
