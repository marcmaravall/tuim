#ifndef TUIM_BACKEND_H
#define TUIM_BACKEND_H

#include "frame_buffer.h"
#include "style.h"
#include "input.h"
#include "unicode.h"

#include <stdbool.h>

typedef uint64_t tuim_backend_attrib_t;

#define STR_TO_BOOL(s) (((s) == "true") ? true : false)

// true, false
#define TUIM_BACKEND_ATTRIB_SIZE_FIXED 0x01

// this can be extended
// ------

typedef struct {
	void (*init)	(void* data);
	void (*destroy)	(void* data);
	void (*render)	(void* data);

	void (*get_size)(void* data, size_t* width, size_t* height);
	void (*set_size)(void* data, const size_t width, const size_t height);

	void (*pass_frame_buffer)(void* data, const TuimFrameBuffer* frame_buffer);

	void (*update_input)(void* data, TuimInputState* input_state);

	void (*set_attrib)(void* data, tuim_backend_attrib_t attrib, const char* value);
	bool (*attrib_supported) (void* data, const tuim_backend_attrib_t attrib);

	char (*get_char)(void* data);
	bool (*inp_rep)(void* data, const tuim_key_code_t key_code);



	void* data;
} TuimBackend;

#endif //TUIM_BACKEND_H