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

void tuim_frame_buffer_destroy(TuimFrameBuffer* state) {
	free(state);
}
