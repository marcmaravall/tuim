#include "backends/linux/linux_backend.h"

void tuim_linux_backend_init(void* data) {

}

void tuim_linux_backend_destroy(void* data) {

}

void tuim_linux_backend_render(void* data) {

}

void tuim_linux_backend_get_size(void* data, size_t* x, size_t* y) {

}

void tuim_linux_backend_pass_frame_buffer(void* data, const TuimFrameBuffer* frame_buffer) {

}

void tuim_linux_update_input(void* data, TuimInputState* input_state) {

}

TuimBackend tuim_linux_backend() {
    TuimBackend backend;

    backend.init = tuim_linux_backend_init;
	backend.destroy = tuim_linux_backend_destroy;
	backend.render = tuim_linux_backend_render;
	backend.get_size = tuim_linux_backend_get_size;
	backend.pass_frame_buffer = tuim_linux_backend_pass_frame_buffer;
	backend.update_input = tuim_linux_update_input;

    return backend;
}
