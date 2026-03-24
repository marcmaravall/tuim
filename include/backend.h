#ifndef TUIM_BACKEND_H
#define TUIM_BACKEND_H

#include "frame_buffer.h"
#include "style.h"
#include "input.h"

typedef struct {
	void (*init)	(void* data);
	void (*destroy)	(void* data);
	void (*render)	(void* data);

	void (*get_size)(void* data, size_t* width, size_t* height);
	void (*pass_frame_buffer)(void* data, const TuimFrameBuffer* frame_buffer);

	void (*update_input)(void* data, TuimInputState* input_state);

	void* data;
} TuimBackend;

#endif //TUIM_BACKEND_H