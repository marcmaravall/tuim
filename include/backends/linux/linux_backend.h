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

typedef struct {
    struct termios old;
    struct termios new;

    const TuimFrameBuffer* fb;
    size_t width;
    size_t height;
} TuimLinuxBackendData;

void tuim_linux_enable_raw_mode(struct termios* old, struct termios* new); 
void tuim_linux_disable_raw_mode(const struct termios* old);

void tuim_linux_backend_init(void* data);
void tuim_linux_backend_destroy(void* data);
void tuim_linux_backend_render(void* data);
void tuim_linux_backend_get_size(void* data, size_t* width, size_t* height);
void tuim_linux_backend_pass_frame_buffer(void* data, const TuimFrameBuffer* frame_buffer);
void tuim_linux_update_input(void* data, TuimInputState* input_state);

TuimBackend tuim_linux_backend();

#endif // TUIM_LINUX_BACKEND_H