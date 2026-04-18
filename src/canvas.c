#include "canvas.h"

void tuim_canvas_init(TuimCanvas* canvas, const size_t width, const size_t height) {
	MEB_ASSERT(canvas);

	canvas->rect.x = 0;
	canvas->rect.y = 0;
	canvas->rect.width = width;
	canvas->rect.height = height;

	canvas->pixels = malloc(sizeof(TuimColor) * width*height);
}

void tuim_canvas_draw(TuimContext* ctx, const TuimCanvas* canvas) {
	MEB_ASSERT(ctx && canvas);

	const int y_from = canvas->rect.y;
	const int y_to = y_from + canvas->rect.height;
	const int x_from = canvas->rect.x;
	const int x_to = x_from + canvas->rect.width;

	for (int y = y_from; y < y_to; ++y) {
		for (int x = x_from; x < x_to; ++x) {

			if (x < 0 || y < 0 ||
				x >= ctx->frame_buffer.width ||
				y >= ctx->frame_buffer.height) {
				continue;
			}

			int fb_index = y * ctx->frame_buffer.width + x;
			int canvas_index =
				(y - y_from) * canvas->rect.width + (x - x_from);

			ctx->frame_buffer.cells[fb_index].ascii_char = ' ';
			ctx->frame_buffer.cells[fb_index].background_color =
				canvas->pixels[canvas_index];
		}
	}
}

void tuim_canvas_destroy(TuimCanvas* canvas) {
	MEB_ASSERT(canvas);

	free(canvas->pixels);
	canvas->rect.width = 0;
	canvas->rect.height = 0;
}

void tuim_canvas_update(TuimContext* ctx, TuimCanvas* canvas) {
	MEB_ASSERT(ctx && canvas);

	// NOTHING
}

TuimSizeHint tuim_canvas_measure(const TuimCanvas* canvas) {
	MEB_ASSERT(canvas);

	TuimSizeHint size;

	size.preferred_height = canvas->rect.height;
	size.preferred_width = canvas->rect.width;
	size.max_height = INT_MAX;
	size.max_width = INT_MAX;
	size.min_height = 1;
	size.min_width = 1;

	return size;
}

// TODO: solve problems
void tuim_canvas_layout(TuimCanvas* canvas, const TuimRect rect) {
	MEB_ASSERT(canvas);

	if (tuim_rect_equals(canvas->rect, rect))
		return;

	canvas->rect = rect;

	if (rect.width * rect.height == 0)
		return;

	TuimColor* new_pixels = realloc (
		canvas->pixels,
		sizeof(TuimColor) * rect.width * rect.height
	);

	MEB_ASSERT(new_pixels);
	canvas->pixels = new_pixels;
}

TuimElement tuim_canvas_to_element(TuimCanvas* canvas) {
	MEB_ASSERT(canvas);

	TuimElement el;
	el.data = canvas;
	el.draw = tuim_canvas_draw;
	el.layout = tuim_canvas_layout;
	el.measure = tuim_canvas_measure;
	el.destroy = tuim_canvas_destroy;
	el.update = tuim_canvas_update;

	return el;
}

void tuim_canvas_fill(TuimCanvas* canvas, const TuimColor color) {
	MEB_ASSERT(canvas);

	const size_t height = canvas->rect.height;
	const size_t width = canvas->rect.width;

	for (size_t y = 0; y < height; y++) {
		for (size_t x = 0; x < width; x++) {
			canvas->pixels[y * width + x] = color;
		}
	}
}


void tuim_canvas_rect(TuimCanvas* canvas, const TuimRect rect, const TuimColor color) {
	MEB_ASSERT(canvas);
	
	const int y0 = (int)rect.y;
	const int x0 = (int)rect.x;
	const int y1 = (int)rect.y + (int)rect.height;
	const int x1 = (int)rect.x + (int)rect.width;
	
	const size_t width = canvas->rect.width;
	const size_t height = canvas->rect.height;

	for (int y = y0; y < y1; y++) {
		for (int x = x0; x < x1; x++) {
			if (x < 0 || y < 0 ||
				x >= width || y >= height) {
				continue;
			}

			canvas->pixels[y * width + x] = color;
		}
	}
}

void tuim_canvas_point(TuimCanvas* canvas, const int x, const int y, const TuimColor color) {
	MEB_ASSERT(canvas);

	const size_t width = canvas->rect.width;
	const size_t height = canvas->rect.height;

	if (x < 0 || y < 0 || x >= width || y >= height) {
		return;
	}

	canvas->pixels[y * canvas->rect.width + x] = color;
}
