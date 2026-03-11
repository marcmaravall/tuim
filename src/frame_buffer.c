#include "frame_buffer.h"

void tuim_frame_buffer_init(TuimFrameBuffer* fb, const size_t width, const size_t height) {
	fb->width  = width;
	fb->height = height;
	fb->cells = (TuimFrameBufferCell*)malloc(sizeof(TuimFrameBufferCell) * width * height);
}

void tuim_frame_buffer_clear(const TuimStyle* const style, TuimFrameBuffer* ascii_char) {
	assert(ascii_char != NULL);

	for (size_t i = 0; i < ascii_char->height; i++) {
		for (size_t j = 0; j < ascii_char->width; j++) {
			TUIM_FRAME_BUFFER_AT(ascii_char, j, i).ascii_char = ' ';
			TUIM_FRAME_BUFFER_AT(ascii_char, j, i).foreground_color = style->clear_color;
			TUIM_FRAME_BUFFER_AT(ascii_char, j, i).background_color = style->clear_color;
		}
	}
}

void tuim_frame_buffer_print(const TuimStyle* style, TuimFrameBuffer* fb, const char* msg, const size_t x, const size_t y) {
	assert(fb != NULL);
	assert(msg != NULL);
	// assert(x < fb->width);
	// assert(y < fb->height);
	
	if (x >= fb->width || y >= fb->height) {
		return;
	}
	
	size_t len = strlen(msg);
	for (size_t i = 0; i < len; i++) {
		if (x + i >= fb->width) {
			break;
		}
		TUIM_FRAME_BUFFER_AT(fb, x+i, y).ascii_char = msg[i];
		TUIM_FRAME_BUFFER_AT(fb, x + i, y).foreground_color = style->text_foreground_color;
		TUIM_FRAME_BUFFER_AT(fb, x + i, y).background_color = style->text_background_color;
	}
}

void tuim_frame_buffer_draw_line(const TuimStyle* style, TuimFrameBuffer* fb, int x0, int y0, const int x1, const int y1) {
	assert(fb != NULL);
	
	int dx = abs(x1 - x0);
	int sx = x0 < x1 ? 1 : -1;
	int dy = -abs(y1 - y0);
	int sy = y0 < y1 ? 1 : -1;
	int err = dx + dy;

	while (true) {
		if (x0 >= 0 && x0 < fb->width && y0 >= 0 && y0 < fb->height) {
			TuimFrameBufferCell* cell = &TUIM_FRAME_BUFFER_AT(fb, x0, y0);
			cell->ascii_char = ' ';
			cell->background_color = style->line_color;
			cell->foreground_color = style->line_color;
		}

		if (x0 == x1 && y0 == y1)
			break;

		int e2 = 2 * err;

		if (e2 >= dy) {
			err += dy;
			x0 += sx;
		}

		if (e2 <= dx) {
			err += dx;
			y0 += sy;
		}
	}
}

void tuim_frame_buffer_draw_equation_line(const TuimStyle* style, TuimFrameBuffer* fb, const double m, const double n) {
	assert(fb != NULL);

	int x0 = 0;
	int y0 = 0;

	for (size_t x = 0; x < fb->width; x++) {
		double y = (m * x) + n;

		int yi = (int)y;

		tuim_frame_buffer_draw_line(style, fb, x0, y0, x, yi);
		x0 = x;
		y0 = yi;
	}
}

void tuim_frame_buffer_draw_rect(const TuimStyle* style, TuimFrameBuffer* fb, const size_t x, const size_t y, const size_t width, const size_t height) {
	assert(fb != NULL);

	for (size_t j = y; j < y + height; j++) {
		for (size_t i = x; i < x + width; i++) {
			if (i >= fb->width || j >= fb->height) {
				continue;
			}
			TUIM_FRAME_BUFFER_AT(fb, i, j).background_color = style->rect_background_color;
			TUIM_FRAME_BUFFER_AT(fb, i, j).foreground_color = style->rect_foreground_color;
			TUIM_FRAME_BUFFER_AT(fb, i, j).ascii_char = ' ';
		}
	}
}

void tuim_frame_buffer_destroy(TuimFrameBuffer* ascii_char) {
	free(ascii_char);
}
