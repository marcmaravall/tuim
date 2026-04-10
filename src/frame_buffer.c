#include "frame_buffer.h"

void tuim_frame_buffer_init(TuimFrameBuffer* fb, const size_t width, const size_t height) {
	fb->width  = width;
	fb->height = height;
	fb->cells = (TuimFrameBufferCell*)malloc(sizeof(TuimFrameBufferCell) * width * height);
}

void tuim_frame_buffer_clear(TuimFrameBuffer* fb, const TuimColor color) {
	assert(fb );

	for (int i = 0; i < fb->height; i++) {
		for (int j = 0; j < fb->width; j++) {
			TUIM_FRAME_BUFFER_AT(fb, j, i).ascii_char = ' ';
			TUIM_FRAME_BUFFER_AT(fb, j, i).foreground_color = color;
			TUIM_FRAME_BUFFER_AT(fb, j, i).background_color = color;
		}
	}
}

void tuim_frame_buffer_print(TuimFrameBuffer* fb, const TuimColor fg, const TuimColor bg, const char* msg, const int x, const int y) {
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

void tuim_frame_buffer_draw_line
	(TuimFrameBuffer* fb, const TuimColor color, int x0, int y0, const int x1, const int y1) {

	tuim_frame_buffer_draw_line_char(fb, color, ' ', x0, y0, x1, y1);
}

void tuim_frame_buffer_draw_line_char
	(TuimFrameBuffer* fb, const TuimColor color,
	const char c, int x0, int y0, const int x1, const int y1) {
	assert(fb);

	int dx = abs(x1 - x0);
	int sx = x0 < x1 ? 1 : -1;
	int dy = -abs(y1 - y0);
	int sy = y0 < y1 ? 1 : -1;
	int err = dx + dy;

	while (true) {
		if (x0 >= 0 && x0 < fb->width && y0 >= 0 && y0 < fb->height) {
			TuimFrameBufferCell* cell = &TUIM_FRAME_BUFFER_AT(fb, x0, y0);
			cell->ascii_char = c;
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

void tuim_frame_buffer_draw_equation_line(const TuimColor color, TuimFrameBuffer* fb, const double m, const double n) {
	assert(fb);

	int x0 = 0;
	int y0 = 0;

	for (size_t x = 0; x < fb->width; x++) {
		double y = (m * x) + n;

		int yi = (int)y;

		tuim_frame_buffer_draw_line(fb, color, x0, y0, x, yi);
		x0 = x;
		y0 = yi;
	}
}

void tuim_frame_buffer_draw_rect(
	TuimFrameBuffer* fb, const TuimColor color, 
	const int x, const int y, const int width, const int height) {
	
	tuim_frame_buffer_draw_rect_char(fb, color, ' ', x, y, width, height);
}

void tuim_frame_buffer_draw_rect_char(TuimFrameBuffer* fb, const TuimColor color, 
	const char c, const int x, const int y, const int width, const int height) {
	assert(fb);

	for (int j = y; j < y + height; j++) {
		for (int i = x; i < x + width; i++) {
			if (i >= fb->width || j >= fb->height) {
				continue;
			}

			TUIM_FRAME_BUFFER_AT(fb, i, j).background_color = color;
			TUIM_FRAME_BUFFER_AT(fb, i, j).foreground_color = color;
			TUIM_FRAME_BUFFER_AT(fb, i, j).ascii_char = c;
		}
	}
}

void tuim_frame_buffer_destroy(TuimFrameBuffer* fb) {
	free(fb);
}

void tuim_frame_buffer_resize(TuimFrameBuffer* fb, const size_t width, const size_t height) {
	assert(fb);
	if (width == fb->width && height == fb->height) {
		return;
	}

	if (width == 0 || height == 0) {
		assert(0 && "ERROR: frame_buffer size is 0");
	}

	TuimFrameBufferCell* new_ = realloc(fb->cells, sizeof(TuimFrameBufferCell) * width * height);
	assert(new_);

	fb->cells = new_;
	fb->width = width;
	fb->height = height;

	tuim_frame_buffer_clear(fb, TUIM_WHITE_STRUCT_INDEXED);
}
