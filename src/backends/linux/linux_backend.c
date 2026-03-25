#include "backends/linux/linux_backend.h"

// code copied from 
// https://stackoverflow.com/questions/50884685/how-to-get-cursor-position-in-c-using-ansi-code
void tuim_linux_get_mouse_position(size_t* x, size_t* y) {
	write(1, "\033[6n", 4);
	char buf[30]={0};
 	int ret, i, pow;
 	char ch;

 	struct termios term, restore;

 	tcgetattr(0, &term);
 	tcgetattr(0, &restore);
 	term.c_lflag &= ~(ICANON|ECHO);
 	tcsetattr(0, TCSANOW, &term);

 	write(1, "\033[6n", 4);

 	for( i = 0, ch = 0; ch != 'R'; i++ )
 	{
 	   ret = read(0, &ch, 1);
 	   if (!ret) {
 	      tcsetattr(0, TCSANOW, &restore);
 	      fprintf(stderr, "getpos: error reading response!\n");
 	   }

 	   buf[i] = ch;
 	   printf("buf[%d]: \t%c \t%d\n", i, ch, ch);
 	}

 	if (i < 2) {
 	   tcsetattr(0, TCSANOW, &restore);
 	   printf("i < 2\n");
 	}

 	for( i -= 2, pow = 1; buf[i] != ';'; i--, pow *= 10)
 	    *x = *x + ( buf[i] - '0' ) * pow;

 	for( i-- , pow = 1; buf[i] != '['; i--, pow *= 10)
 	    *y = *y + ( buf[i] - '0' ) * pow;

 	tcsetattr(0, TCSANOW, &restore);
}

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

// this shit runs at only 5fps
void tuim_linux_backend_render(void* data) {
    assert(data);
    const TuimLinuxBackendData* bdata = data;
    assert(bdata->fb);

    const TuimFrameBuffer* fb = bdata->fb;

    static char* out = NULL;
    static size_t capacity = 0;

    size_t needed = ((fb->width + 1) * fb->height) * 16 + 16;

    if (needed > capacity) {
        capacity = needed;
        out = realloc(out, capacity);
        assert(out);
    }

    char* ptr = out;

    *ptr++ = '\x1b';
    *ptr++ = '[';
    *ptr++ = 'H';

    int last_fg = -1;
    int last_bg = -1;

    for (size_t i = 0; i < fb->height; i++) {
        const TuimFrameBufferCell* row = &fb->cells[i * fb->width];

        for (size_t j = 0; j < fb->width; j++) {
            const TuimFrameBufferCell cell = row[j];

            TuimColorIndex fg_i = cell.foreground_color.color.indexed_color;
            TuimColorIndex bg_i = cell.background_color.color.indexed_color;

            if (fg_i != last_fg) {
                const char* fg = tuim_color_to_ansi_foreground[fg_i];
                size_t len = strlen(fg);
                memcpy(ptr, fg, len);
                ptr += len;
                last_fg = fg_i;
            }

            if (bg_i != last_bg) {
                const char* bg = tuim_color_to_ansi_background[bg_i];
                size_t len = strlen(bg);
                memcpy(ptr, bg, len);
                ptr += len;
                last_bg = bg_i;
            }

            *ptr++ = cell.ascii_char;
        }

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

// USED CHATGPT:
void tuim_linux_update_input(void* data, TuimInputState* input_state) {
    assert(data && input_state);

    TuimMouseState* mouse = &input_state->mouse_state;

    mouse->previous = mouse->current;

    char c;
    ssize_t n;

    while ((n = read(STDIN_FILENO, &c, 1)) > 0) {
        unsigned char uc = (unsigned char)c;

        if (uc == 27) { // ESC
            char seq[64];
            int i = 0;

            while (i < (int)sizeof(seq) - 1) {
                if (read(STDIN_FILENO, &seq[i], 1) != 1)
                    break;

                if (seq[i] == 'M' || seq[i] == 'm') {
                    i++;
                    break;
                }
                i++;
            }
            seq[i] = '\0';

            if (seq[0] == '[' && seq[1] == '<') {
                int b, x, y;

                if (sscanf(seq, "[<%d;%d;%d", &b, &x, &y) == 3) {
                    mouse->mouse_x = x;
                    mouse->mouse_y = y;

                    int btn = b & 3;
                    int is_release = (i > 0 && seq[i-1] == 'm');

                    uint8_t mask = 0;

                    if (btn == 0) mask = TUIM_MOUSE_BUTTON_LEFT;
                    if (btn == 1) mask = TUIM_MOUSE_BUTTON_MIDDLE;
                    if (btn == 2) mask = TUIM_MOUSE_BUTTON_RIGHT;

                    if (mask) {
                        if (is_release)
                            mouse->current &= ~mask;
                        else
                            mouse->current |= mask;
                    }
                }
            }

        } else {
            input_state->keyboard_state.current[uc] = 1;
        }
    }
}
// ---

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
