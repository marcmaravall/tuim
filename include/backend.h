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

typedef void (*TuimBackendInitFn)(void* data);
typedef void (*TuimBackendDestroyFn)(void* data);
typedef void (*TuimBackendRenderFn)(void* data);

typedef void (*TuimBackendGetSizeFn)(void* data, size_t* width, size_t* height);
typedef void (*TuimBackendSetSizeFn)(void* data, size_t width, size_t height);

typedef void (*TuimBackendPassFrameBufferFn)(void* data, const TuimFrameBuffer* frame_buffer);

typedef void (*TuimBackendUpdateInputFn)(void* data, TuimInputState* input_state);

typedef void (*TuimBackendSetAttribFn)(void* data, tuim_backend_attrib_t attrib, const char* value);
typedef bool (*TuimBackendAttribSupportedFn)(void* data, tuim_backend_attrib_t attrib);

typedef char (*TuimBackendGetCharFn)(void* data);
typedef bool (*TuimBackendInputRepeatFn)(void* data, tuim_key_code_t key_code);
typedef char* (*TuimBackendGetClipboardFn)(void* data);
typedef double (*TuimBackendGetDeltaTimeFn)(void* data);

typedef struct {
    TuimBackendInitFn init;
    TuimBackendDestroyFn destroy;
    TuimBackendRenderFn render;

    TuimBackendGetSizeFn get_size;
    TuimBackendSetSizeFn set_size;

    TuimBackendPassFrameBufferFn pass_frame_buffer;

    TuimBackendUpdateInputFn update_input;

    TuimBackendSetAttribFn set_attrib;
    TuimBackendAttribSupportedFn attrib_supported;

    TuimBackendGetCharFn get_char;
    TuimBackendInputRepeatFn inp_rep;
    TuimBackendGetClipboardFn get_clipboard;
	TuimBackendGetDeltaTimeFn get_delta_time;

    void* data;
} TuimBackend;

TuimBackend tuim_null_backend();

#endif //TUIM_BACKEND_H
