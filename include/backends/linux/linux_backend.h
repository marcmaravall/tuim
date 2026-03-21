#ifndef TUIM_LINUX_BACKEND_H
#define TUIM_LINUX_BACKEND_H

#include "backend.h"
#include "frame_buffer.h"
#include "input.h"

void tuim_linux_backend_init(void* data);
void tuim_linux_backend_destroy(void* data);
void tuim_linux_backend_render(void* data);
void tuim_linux_backend_get_size(void* data, size_t* x, size_t* y);
void tuim_linux_backend_pass_frame_buffer(void* data, const TuimFrameBuffer* frame_buffer);
void tuim_linux_update_input(void* data, TuimInputState* input_state);

TuimBackend tuim_linux_backend();

#endif // TUIM_LINUX_BACKEND_H