// Backend optimized for windows api

#ifndef _WIN32
#error "ERROR: this backend is not supported in your current OS!"
#else
#define TUIM_WINDOWS_BACKEND_H

#include <windows.h>
#include <conio.h>
#include <tchar.h>
#include <strsafe.h>
#include <assert.h>
#include <stdlib.h>

#include "backend.h"

typedef struct {
	HWND   window;
	HANDLE handle;
	WORD   current_attributes;

	CHAR_INFO* buffer;
	COORD buffer_size;

	bool window_active;
} TuimWindowsBackendData;

void tuim_windows_backend_init(void* data);
void tuim_windows_backend_destroy(void* data);
void tuim_windows_backend_pass_frame_buffer(void* data, const TuimFrameBuffer* frame_buffer);
void tuim_windows_backend_render(void* data);
void tuim_windows_backend_get_size(void* backend_data, size_t* x, size_t* y);

void tuim_windows_backend_update_input(void* data, TuimInputState* input_state);
void tuim_windows_backend_input_record_to_input_state(const INPUT_RECORD* record, TuimInputState* input_state);

static WORD tuim_color_to_win32(const TuimColor color);

TuimBackend tuim_windows_backend();

#endif //_WIN32