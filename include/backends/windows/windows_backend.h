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
} TuimWindowsBackendData;

void tuim_windows_backend_init(void* data);
void tuim_windows_backend_destroy(void* data);
void tuim_windows_backend_pass_frame_buffer(void* data, const TuimFrameBuffer* frame_buffer);

static WORD tuim_color_to_win32(const TuimColor color);

TuimBackend tuim_windows_backend();

#endif //_WIN32