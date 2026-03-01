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
#include <backend.h>

typedef struct {
	HWND   window;
	HANDLE handle;
	WORD   current_attributes;
} TuimWindowsBackendData;

void tuim_windows_backend_init(void* backend_data);
void tuim_windows_backend_destroy(void* backend_data);
void tuim_windows_backend_clear(void* backend_data);
void tuim_windows_backend_render_text(void* backend_data, const char* msg);
void tuim_windows_backend_set_cursor_pos(void* backend_data, int x, int y);
void tuim_windows_backend_set_foreground_color(void* backend_data, TuimAnsiColor color);
void tuim_windows_backend_set_background_color(void* backend_data, TuimAnsiColor color);
void tuim_windows_backend_set_console_name(void* backend_data, const char* msg);

static WORD tuim_color_to_win32(const TuimAnsiColor color);

TuimBackend tuim_windows_backend();

#endif //_WIN32