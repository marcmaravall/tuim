#ifndef TUIM_LINUX_BACKEND_H
#define TUIM_LINUX_BACKEND_H

#include <stdio.h>
#include <assert.h>

#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "backend.h"
#include "frame_buffer.h"
#include "input.h"
#include "color.h"

typedef struct {
    struct termios old;
    struct termios new;

    const TuimFrameBuffer* fb;
    size_t width;
    size_t height;
} TuimLinuxBackendData;

static const char* tuim_color_to_ansi_foreground[16] = {
    [TUIM_BLACK]   = "\x1b[30m", 
    [TUIM_RED]     = "\x1b[31m", 
    [TUIM_GREEN]   = "\x1b[32m", 
    [TUIM_YELLOW]  = "\x1b[33m", 
    [TUIM_BLUE]    = "\x1b[34m", 
    [TUIM_MAGENTA] = "\x1b[35m", 
    [TUIM_CYAN]    = "\x1b[36m", 
    [TUIM_WHITE]   = "\x1b[37m", 
    [TUIM_BRIGHT_BLACK]   = "\x1b[90m",
    [TUIM_BRIGHT_RED]     = "\x1b[91m",
    [TUIM_BRIGHT_GREEN]   = "\x1b[92m",
    [TUIM_BRIGHT_YELLOW]  = "\x1b[93m",
    [TUIM_BRIGHT_BLUE]    = "\x1b[94m",
    [TUIM_BRIGHT_MAGENTA] = "\x1b[95m",
    [TUIM_BRIGHT_CYAN]    = "\x1b[96m",
    [TUIM_BRIGHT_WHITE]   = "\x1b[97m"
};

static const char* tuim_color_to_ansi_background[16] = {
    [TUIM_BLACK]   = "\x1b[40m", 
    [TUIM_RED]     = "\x1b[41m", 
    [TUIM_GREEN]   = "\x1b[42m", 
    [TUIM_YELLOW]  = "\x1b[43m", 
    [TUIM_BLUE]    = "\x1b[44m", 
    [TUIM_MAGENTA] = "\x1b[45m", 
    [TUIM_CYAN]    = "\x1b[46m", 
    [TUIM_WHITE]   = "\x1b[47m", 
    [TUIM_BRIGHT_BLACK]   = "\x1b[100m",
    [TUIM_BRIGHT_RED]     = "\x1b[101m",
    [TUIM_BRIGHT_GREEN]   = "\x1b[102m",
    [TUIM_BRIGHT_YELLOW]  = "\x1b[103m",
    [TUIM_BRIGHT_BLUE]    = "\x1b[104m",
    [TUIM_BRIGHT_MAGENTA] = "\x1b[105m",
    [TUIM_BRIGHT_CYAN]    = "\x1b[106m",
    [TUIM_BRIGHT_WHITE]   = "\x1b[107m"
};

void tuim_linux_enable_raw_mode(struct termios* old, struct termios* new); 
void tuim_linux_disable_raw_mode(const struct termios* old);

void tuim_linux_get_mouse_position(void* data, size_t* x, size_t* y);

void tuim_linux_backend_init(void* data);
void tuim_linux_backend_destroy(void* data);
void tuim_linux_backend_render(void* data);
void tuim_linux_backend_get_size(void* data, size_t* width, size_t* height);
void tuim_linux_backend_pass_frame_buffer(void* data, const TuimFrameBuffer* frame_buffer);
void tuim_linux_update_input(void* data, TuimInputState* input_state);

TuimBackend tuim_linux_backend();

#endif // TUIM_LINUX_BACKEND_H