#include "frame_buffer.h"

void tuim_frame_buffer_init(TuimFrameBuffer* fb, const size_t width, const size_t height) {
	fb->width  = width;
	fb->height = height;
	fb->cells = (TuimFrameBufferCell*)malloc(sizeof(TuimFrameBufferCell) * width * height);
}

void tuim_frame_buffer_clear(const TuimStyle* const style, TuimFrameBuffer* fb) {
	assert(fb );

	for (int i = 0; i < fb->height; i++) {
		for (int j = 0; j < fb->width; j++) {
			TUIM_FRAME_BUFFER_AT(fb, j, i).ascii_char = ' ';
			TUIM_FRAME_BUFFER_AT(fb, j, i).foreground_color = style->clear_color;
			TUIM_FRAME_BUFFER_AT(fb, j, i).background_color = style->clear_color;
		}
	}
}

void tuim_frame_buffer_print(const TuimStyle* style, TuimFrameBuffer* fb,const char* msg, const int x, const int y) {
	
	assert(style != NULL);
	assert(fb != NULL);
	assert(msg != NULL);

	if (y < 0 || y >= (int)fb->height) {
		return;
	}

	size_t len = strlen(msg);

	int start_x = x;
	size_t msg_index = 0;

	if (start_x < 0) {
		msg_index = (size_t)(-start_x);
		start_x = 0;

		if (msg_index >= len) {
			return;
		}
	}

	for (; msg_index < len; msg_index++, start_x++) {
		if (start_x >= (int)fb->width) {
			break;
		}

		TuimFrameBufferCell* cell = &TUIM_FRAME_BUFFER_AT(fb, start_x, y);

		cell->ascii_char = msg[msg_index];
		cell->foreground_color = style->text_foreground_color;
		cell->background_color = style->text_background_color;
	}
}

void tuim_frame_buffer_print_color(TuimFrameBuffer* fb, const TuimColor fg, const TuimColor bg, const char* msg, const int x, const int y) {
	assert(fb != NULL);
	assert(msg != NULL);

	if (y < 0 || y >= (int)fb->height) {
		return;
	}

	size_t len = strlen(msg);

	int start_x = x;
	size_t msg_index = 0;

	if (start_x < 0) {
		msg_index = (size_t)(-start_x);
		start_x = 0;

		if (msg_index >= len) {
			return;
		}
	}

	for (; msg_index < len; msg_index++, start_x++) {
		if (start_x >= (int)fb->width) {
			break;
		}

		TuimFrameBufferCell* cell = &TUIM_FRAME_BUFFER_AT(fb, start_x, y);

		cell->ascii_char = msg[msg_index];
		cell->foreground_color = fg;
		cell->background_color = bg;
	}
}

void tuim_frame_buffer_draw_line(const TuimStyle* style, TuimFrameBuffer* fb, int x0, int y0, const int x1, const int y1) {
	assert(fb && style);
	
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

void tuim_frame_buffer_draw_line_color(TuimFrameBuffer* fb, const TuimColor color, int x0, int y0, const int x1, const int y1) {
	assert(fb);

	int dx = abs(x1 - x0);
	int sx = x0 < x1 ? 1 : -1;
	int dy = -abs(y1 - y0);
	int sy = y0 < y1 ? 1 : -1;
	int err = dx + dy;

	while (true) {
		if (x0 >= 0 && x0 < fb->width && y0 >= 0 && y0 < fb->height) {
			TuimFrameBufferCell* cell = &TUIM_FRAME_BUFFER_AT(fb, x0, y0);
			cell->ascii_char = ' ';
			cell->background_color = color;
			cell->foreground_color = color;
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
	assert(fb);

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

void tuim_frame_buffer_draw_rect(const TuimStyle* style, TuimFrameBuffer* fb, const int x, const int y, const int width, const int height) {
	assert(fb);

	for (int j = y; j < y + height; j++) {
		for (int i = x; i < x + width; i++) {
			if (i >= fb->width || j >= fb->height) {
				continue;
			}

			TUIM_FRAME_BUFFER_AT(fb, i, j).background_color = style->rect_background_color;
			TUIM_FRAME_BUFFER_AT(fb, i, j).foreground_color = style->rect_foreground_color;
			TUIM_FRAME_BUFFER_AT(fb, i, j).ascii_char = ' ';
		}
	}
}

void tuim_frame_buffer_draw_rect_color(
	TuimFrameBuffer* fb, const TuimColor color, 
	const int x, const int y, const int width, const int height) {
	
	assert(fb);

	for (int j = y; j < y + height; j++) {
		for (int i = x; i < x + width; i++) {
			if (i >= fb->width || j >= fb->height) {
				continue;
			}

			TUIM_FRAME_BUFFER_AT(fb, i, j).background_color = color;
			TUIM_FRAME_BUFFER_AT(fb, i, j).foreground_color = color;
			TUIM_FRAME_BUFFER_AT(fb, i, j).ascii_char = ' ';
		}
	}
}

void tuim_frame_buffer_destroy(TuimFrameBuffer* ascii_char) {
	free(ascii_char);
}
