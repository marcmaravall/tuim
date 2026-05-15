#include "context.h"

void tuim_begin_frame(TuimContext* ctx) {
	MEB_ASSERT(ctx);

	tuim_frame_buffer_clear(&ctx->viewport.frame_buffer, ctx->style.clear_color);
}

void tuim_init_context(TuimContext* ctx) {
	ctx->backend.init(ctx->backend.data);

	size_t width, height;
	ctx->backend.get_size(ctx->backend.data, &width, &height);

	tuim_clear_input(&ctx->input_state);
	ctx->viewport = tuim_viewport((TuimRect){0, 0, width, height});

	ctx->style = tuim_style_default();
	ctx->time = 0.0;
	ctx->target_fps = 144.0;

	ctx->delta_time = 0.0;
	ctx->fps = 0.0;
}

TuimContext tuim_context(const TuimBackend backend, const TuimStyle style) {
	TuimContext ctx;
	ctx.backend = backend;
	tuim_init_context(&ctx);
	ctx.style = style;
	return ctx;
}

void tuim_init_with_backend(TuimContext* ctx, TuimBackend backend) {
	ctx->backend = backend;
	tuim_init_context(ctx);
}

void tuim_end_frame(TuimContext* ctx) {
	tuim_viewport_draw(ctx, &ctx->viewport);
	ctx->backend.pass_frame_buffer(ctx->backend.data, &ctx->viewport.frame_buffer);
	ctx->backend.render(ctx->backend.data);

	double render_time = ctx->backend.get_delta_time(ctx->backend.data);

	if (ctx->backend.sleep_ms && ctx->target_fps > 0.0) {
		double target_frame_time = 1.0 / ctx->target_fps;

		if (render_time < target_frame_time) {
			int sleep_ms = (int)((target_frame_time - render_time) * 1000.0);
			ctx->backend.sleep_ms(ctx->backend.data, sleep_ms);
		}
	}

	ctx->delta_time = ctx->backend.get_delta_time(ctx->backend.data) + render_time;

	ctx->fps = ctx->delta_time > 0.0 ? 1.0 / ctx->delta_time : 0.0;
	ctx->time += ctx->delta_time;
}

void tuim_destroy_context(TuimContext* ctx) {
	MEB_ASSERT(ctx);
	ctx->backend.destroy(ctx->backend.data);
	free(ctx->viewport.frame_buffer.cells);
	free(ctx->backend.data);
}

void tuim_set_style(TuimContext* ctx, TuimStyle style) {
	MEB_ASSERT(ctx);
	ctx->style = style;
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
	return ctx->delta_time;
}

double tuim_get_fps(TuimContext* ctx) {
	MEB_ASSERT(ctx);
	return ctx->fps;
}

double tuim_get_time(TuimContext* ctx) {
	MEB_ASSERT(ctx);
	return ctx->time;
}

int tuim_get_context_width(const TuimContext* ctx) {
	MEB_ASSERT(ctx);
	return ctx->viewport.frame_buffer.width;
}

int tuim_get_context_height(const TuimContext* ctx) {
	MEB_ASSERT(ctx);
	return ctx->viewport.frame_buffer.height;
}

void tuim_set_target_fps(TuimContext* ctx, const double fps) {
	MEB_ASSERT(ctx);
	ctx->target_fps = fps;
}
