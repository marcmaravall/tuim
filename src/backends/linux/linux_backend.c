#include "backends/linux/linux_backend.h"

void tuim_linux_enable_raw_mode(struct termios* old, struct termios* new) {

    tcgetattr(STDIN_FILENO, old);
    *new = *old;

	new->c_lflag &= ~(ECHO | ICANON | ISIG);
	new->c_iflag &= ~(IXON | ICRNL);

	new->c_cc[VMIN] = 0;
	new->c_cc[VTIME] = 1;

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

	// quit cursor
	printf("\033[?25l");

	// mouse
	printf("\033[?1003h"); 
	printf("\033[?1006h");

	fflush(stdout);
}

// TODO: solve attribute error
void tuim_linux_backend_destroy(void* data) {
	assert(data);
	TuimLinuxBackendData* bdata = data;
	tuim_linux_disable_raw_mode(&bdata->old);

	// remove
    printf("\033[?1003l");
    printf("\033[?1006l");
	// show cursor
	printf("\033[?25h"); 

    fflush(stdout);
}

// TODO: solve errors:
void tuim_linux_backend_render(void* data) {
    assert(data);
    const TuimLinuxBackendData* bdata = data;

	assert(bdata->fb);

    const TuimFrameBuffer* fb = bdata->fb;

    static char* out = NULL;
    static size_t capacity = 0;

    size_t needed = (fb->width + 1) * fb->height + 16;

    if (needed > capacity) {
        capacity = needed;
        out = realloc(out, capacity);
    }

    char* ptr = out;

    ptr += sprintf(ptr, "\x1b[H");
	//printf("\x1b[H");

    for (size_t i = 0; i < fb->height; i++) {
        for (size_t j = 0; j < fb->width; j++) {
            TuimFrameBufferCell cell = TUIM_FRAME_BUFFER_AT(fb, j, i);
            //printf("%c", cell.ascii_char);
			*ptr++ = cell.ascii_char;
        }
		//printf("\n");
        *ptr++ = '\n';
    }

    write(STDOUT_FILENO, out, ptr - out);
}

void tuim_linux_backend_get_size(void* data, size_t* width, size_t* height) {
	assert(data && width && height);
	struct winsize w;
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
		*width = w.ws_col;
		*height = w.ws_row;

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

    for (int i = 0; i < 256; i++) {
        input_state->keyboard_state.current[i] = 0;
    }

    char c;
    ssize_t n;

    while ((n = read(STDIN_FILENO, &c, 1)) > 0) {
        unsigned char uc = (unsigned char)c;

        if (uc == 27) { // ESC
            char seq[2];
            if (read(STDIN_FILENO, &seq[0], 1) == 0) continue;
            if (read(STDIN_FILENO, &seq[1], 1) == 0) continue;

            if (seq[0] == '[') {
				// TODO: implement special keys
            }
        } else {
            input_state->keyboard_state.current[uc] = 1;
        }
    }
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
