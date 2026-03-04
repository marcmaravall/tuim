#ifndef TUIM_BACKEND_H
#define TUIM_BACKEND_H

#include "frame_buffer.h"
#include "style.h"

// TODO: change backend to frame buffer for efficiency

/*typedef struct TuimBackend {
	void (*init)(void* backend_data);
	void (*destroy)(void* backend_data);
	void (*clear)(void* backend_data);
	void (*render_text)(void* backend_data, const char* msg);
	void (*set_cursor_pos)(void* backend_data, int x, int y);
	void (*set_foreground_color)(void* backend_data, TuimColor color);
	void (*set_background_color)(void* backend_data, TuimColor color);
	void (*set_console_name)(void* backend_data, const char* msg);
	void* data;
} TuimBackend;
*/

// frame buffer backend
typedef struct {
	void (*init)	(void* data);
	void (*destroy)	(void* data);
	void (*render)	(void* data);

	void (*pass_frame_buffer)(void* data, const TuimFrameBuffer* frame_buffer);

	void* data;
} TuimBackend;

#endif //TUIM_BACKEND_H