// TODO: remove os data and move it to backend

#ifndef TUIM_OS_H
#define TUIM_OS_H

#ifdef _WIN32
#include <windows.h>
#endif

typedef struct TuimContext TuimContext;

// TODO: implement:

typedef struct TuimOsData {
#ifdef _WIN32
    HWND   win32_window;
    HANDLE win32_handle;
#else
    char c;
#endif

} TuimOsData;

typedef struct TuimOsConsoleBufferInfo {
#ifdef _WIN32
    HANDLE win32_handle;
#endif

    int width;
    int height;

    int cursor_x;
    int cursor_y;

} TuimOsConsoleBufferInfo;

#ifdef _WIN32

void tuim_win32_init(TuimOsData* data);
int  tuim_win32_get_console_buffer_info(TuimOsConsoleBufferInfo* info);

#else
// TODO: implement
#pragma warning("WARNING: the current os is not supported!")
#endif

void tuim_os_init(TuimContext* ctx);
int  tuim_os_get_console_buffer_info(TuimOsConsoleBufferInfo* info);

#endif // TUIM_OS_H
