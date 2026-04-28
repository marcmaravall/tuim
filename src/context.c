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
	ctx->time = 0.0;
}

void tuim_end_frame(TuimContext* ctx) {
	ctx->backend.pass_frame_buffer(ctx->backend.data, &ctx->frame_buffer);
	ctx->backend.render(ctx->backend.data);
	ctx->time += tuim_get_delta_time(ctx);
}

void tuim_destroy_context(TuimContext* ctx) {
	MEB_ASSERT(ctx);
	ctx->backend.destroy(ctx->backend.data);
	free(ctx->frame_buffer.cells);
	free(ctx->backend.data);
}

// backend calls:

void tuim_resize_context(TuimContext* ctx, const size_t width, const size_t height) {
	MEB_ASSERT(ctx);

	if (!ctx->backend.set_size) {
		// warning or something	
		return;
	}

	ctx->backend.set_size(ctx->backend.data, width, height);
}

void tuim_set_backend_attrib(TuimContext* ctx, const tuim_backend_attrib_t attrib, const char* value) {
	MEB_ASSERT(ctx);

	if (!ctx->backend.set_attrib) {
		return;
	}

	if (ctx->backend.set_attrib)
		ctx->backend.set_attrib(ctx->backend.data, attrib, value);
}

char* tuim_get_clipboard(TuimContext* ctx) {
	MEB_ASSERT(ctx);

	if (!ctx->backend.get_clipboard)
		return NULL;
	return ctx->backend.get_clipboard(ctx->backend.data);
}

double tuim_get_delta_time(TuimContext* ctx) {
	MEB_ASSERT(ctx);
	if (ctx->backend.get_delta_time)
		return ctx->backend.get_delta_time(ctx->backend.data);
	return 0.0;
}

double tuim_get_fps(TuimContext* ctx) {
	double delta_time = tuim_get_delta_time(ctx);
	if (delta_time > 0.0)
		return 1.0 / delta_time;
	return 0.0;
}

double tuim_get_time(TuimContext* ctx) {
	MEB_ASSERT(ctx);
	return ctx->time;
}
