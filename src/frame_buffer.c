#include "frame_buffer.h"

void tuim_frame_buffer_init(TuimFrameBuffer* state, const size_t width, const size_t height) {
	state->width  = width;
	state->height = height;
	state->cells = (TuimFrameBufferCell*)malloc(sizeof(TuimFrameBufferCell) * width * height);
}

void tuim_frame_buffer_clear(TuimFrameBuffer* state) {
	assert(state != NULL);

	for (size_t i = 0; i < state->width * state->height; i++) {
		// TODO: set attributes
		state->cells[i].foreground_color.indexed_color = TUIM_WHITE;
		state->cells[i].background_color.indexed_color = TUIM_WHITE;
		state->cells[i].state = ' ';
	}
}

void tuim_frame_buffer_print(TuimFrameBuffer* state, const char* msg, const size_t x, const size_t y) {
	assert(state != NULL);
	assert(msg != NULL);
	assert(x < state->width);
	assert(y < state->height);
	size_t len = strlen(msg);
	for (size_t i = 0; i < len; i++) {
		if (x + i >= state->width) {
			break;
		}
		TUIM_FRAME_BUFFER_AT(state, x+i, y).state = msg[i];
	}
}

void tuim_frame_buffer_draw_line(TuimFrameBuffer* state, int x0, int y0, const int x1, const int y1) {
	assert(state != NULL);
	
	int dx = abs(x1 - x0);
	int sx = x0 < x1 ? 1 : -1;
	int dy = -abs(y1 - y0);
	int sy = y0 < y1 ? 1 : -1;
	int err = dx + dy;

	while (true) {
		if (x0 >= 0 && x0 < state->width && y0 >= 0 && y0 < state->height)
			TUIM_FRAME_BUFFER_AT(state, x0, y0).state = 'x';

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

void tuim_frame_buffer_draw_equation_line(TuimFrameBuffer* state, const double m, const double n) {
	assert(state != NULL);

	int x0 = 0;
	int y0 = 0;

	for (size_t x = 0; x < state->width; x++) {
		double y = (m * x) + n;

		int yi = (int)y;

		tuim_frame_buffer_draw_line(state, x0, y0, x, yi);
		x0 = x;
		y0 = yi;
	}
}

void tuim_frame_buffer_draw_rect(TuimFrameBuffer* state, const size_t x, const size_t y, const size_t width, const size_t height) {
	assert(state != NULL);

	for (size_t j = y; j < y + height; j++) {
		for (size_t i = x; i < x + width; i++) {
			if (i >= state->width || j >= state->height) {
				continue;
			}
			TUIM_FRAME_BUFFER_AT(state, i, j).state = 'x';	// for debug
		}
	}
}

void tuim_frame_buffer_destroy(TuimFrameBuffer* state) {
	free(state);
}
