#include "backends/linux/linux_backend.h"

void tuim_linux_enable_raw_mode(struct termios* old, struct termios* new) {

    tcgetattr(STDIN_FILENO, old);
    new = old;

	new->c_lflag &= ~(ECHO | ICANON | ISIG);
	new->c_iflag &= ~(IXON | ICRNL);

    tcsetattr(STDIN_FILENO, TCSANOW, new);
}

void tuim_linux_disable_raw_mode(const struct termios* old) {
    tcsetattr(STDIN_FILENO, TCSANOW, old);
}

void tuim_linux_backend_init(void* data) {
	assert(data);
	TuimLinuxBackendData* bdata = data;
	tuim_linux_enable_raw_mode(&bdata->old, &bdata->new);

	// fullscreen
	printf("\x1b[?1049h");
}

void tuim_linux_backend_destroy(void* data) {
	assert(data);
}

void tuim_linux_backend_render(void* data) {
	assert(data);
	TuimLinuxBackendData* bdata = data;

	printf("\x1b[2J");
    printf("\x1b[H");
}

void tuim_linux_backend_get_size(void* data, size_t* width, size_t* height) {
	assert(data && width && height);
	struct winsize w;
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
		*width = w.ws_row;
		*height = w.ws_col;

		TuimLinuxBackendData* bdata = data;
		bdata->width = *width;
		bdata->height = *height;
	} else {
		assert(0);
	}
}

void tuim_linux_backend_pass_frame_buffer(void* data, const TuimFrameBuffer* frame_buffer) {
	assert(data && frame_buffer);
	TuimLinuxBackendData* bdata = data;
	bdata->fb = frame_buffer;
}

void tuim_linux_update_input(void* data, TuimInputState* input_state) {
	assert(data && input_state);
	TuimLinuxBackendData* bdata = data;

	
}

TuimBackend tuim_linux_backend() {
    TuimBackend backend;

	TuimLinuxBackendData* data = malloc(sizeof(TuimLinuxBackendData));

	backend.data = data;
    backend.init = tuim_linux_backend_init;
	backend.destroy = tuim_linux_backend_destroy;
	backend.render = tuim_linux_backend_render;
	backend.get_size = tuim_linux_backend_get_size;
	backend.pass_frame_buffer = tuim_linux_backend_pass_frame_buffer;
	backend.update_input = tuim_linux_update_input;

    return backend;
}
