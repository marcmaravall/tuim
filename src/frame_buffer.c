#include "frame_buffer.h"

void tuim_cell_frame_buffer_init(TuimFrameBuffer* state, const size_t width, const size_t height) {
	state->width = width;
	state->height = height;
	state->cells = (TuimCell*)malloc(sizeof(TuimCell) * width * height);
}

void tuim_cell_frame_buffer_clear(TuimFrameBuffer* state) {
	assert(state != NULL);

	for (size_t i = 0; i < state->width * state->height; i++) {
		state->cells[i].foreground_color.indexed_color = TUIM_WHITE;
		state->cells[i].background_color.indexed_color = TUIM_WHITE;
		state->cells[i].text_attributes = NULL;
	}
}

void tuim_cell_frame_buffer_destroy(TuimFrameBuffer* state) {
	free(state);
}
