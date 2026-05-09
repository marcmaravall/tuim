// Backend optimized for windows api

#ifndef _WIN32
#error "ERROR: this backend is not supported in your current OS!"
#else
#define TUIM_WINDOWS_BACKEND_H

#include <meb.h>

#include <windows.h>
#include <conio.h>
#include <tchar.h>
#include <strsafe.h>
#include <stdlib.h>
#include <string.h>

#include "color.h"
#include "backend.h"
#include "input.h"
#include "unicode.h"

typedef struct {
	HWND   window;
	HANDLE handle;
	WORD   current_attributes;

    TuimFrameBufferCell* shadow_buffer;
	TuimFrameBuffer* buffer;
	COORD buffer_size;

    char char_pressed;
    WCHAR unicode_pressed;

    WCHAR vk_pressed;

    bool resized;
	bool window_active;
} TuimWindowsBackendData;

void tuim_windows_backend_init(void* data);
void tuim_windows_backend_destroy(void* data);
void tuim_windows_backend_pass_frame_buffer(void* data, TuimFrameBuffer* frame_buffer);
void tuim_windows_backend_render(void* data);
void tuim_windows_backend_get_size(void* backend_data, size_t* x, size_t* y);

void tuim_windows_backend_update_input(void* data, TuimInputState* input_state);
void tuim_windows_backend_input_record_to_input_state(const INPUT_RECORD* record, TuimKeyboardState* input_state);

static WORD tuim_color_to_win32(const TuimColor color);

static const WORD tuim_color_to_win32_table[256] = {
    [TUIM_BLACK] = 0,
    [TUIM_RED] = 4,
    [TUIM_GREEN] = 2,
    [TUIM_YELLOW] = 6,
    [TUIM_BLUE] = 1,
    [TUIM_MAGENTA] = 5,
    [TUIM_CYAN] = 3,
    [TUIM_WHITE] = 7,
    [TUIM_BRIGHT_BLACK] = 8,
    [TUIM_BRIGHT_RED] = 12,
    [TUIM_BRIGHT_GREEN] = 10,
    [TUIM_BRIGHT_YELLOW] = 14,
    [TUIM_BRIGHT_BLUE] = 9,
    [TUIM_BRIGHT_MAGENTA] = 13,
    [TUIM_BRIGHT_CYAN] = 11,
    [TUIM_BRIGHT_WHITE] = 15,
};

void tuim_windows_backend_resize_console(TuimWindowsBackendData* data, const size_t width, const size_t height);

void tuim_windows_backend_resize_buffer(TuimWindowsBackendData* data, const SHORT width, const SHORT height);

static const TuimVirtualKey tuim_win32_vk_to_tuim[256] = {
    [VK_UP] = TUIM_KEY_UP,
    [VK_DOWN] = TUIM_KEY_DOWN,
    [VK_LEFT] = TUIM_KEY_LEFT,
    [VK_RIGHT] = TUIM_KEY_RIGHT,

    [VK_ESCAPE] = TUIM_KEY_ESCAPE,

    [VK_SHIFT] = TUIM_KEY_SHIFT,
    [VK_CONTROL] = TUIM_KEY_CTRL,
    [VK_MENU] = TUIM_KEY_ALT,

    [VK_RETURN] = TUIM_KEY_ENTER,
    [VK_BACK] = TUIM_KEY_BACKSPACE,
    [VK_TAB] = TUIM_KEY_TAB,
    [VK_DELETE] = TUIM_KEY_DELETE,
    [VK_INSERT] = TUIM_KEY_INSERT,

    [VK_HOME] = TUIM_KEY_HOME,
    [VK_END] = TUIM_KEY_END,
    [VK_PRIOR] = TUIM_KEY_PAGEUP,
    [VK_NEXT] = TUIM_KEY_PAGEDOWN,

    [VK_F1] = TUIM_KEY_F1,
    [VK_F2] = TUIM_KEY_F2,
    [VK_F3] = TUIM_KEY_F3,
    [VK_F4] = TUIM_KEY_F4,
    [VK_F5] = TUIM_KEY_F5,
    [VK_F6] = TUIM_KEY_F6,
    [VK_F7] = TUIM_KEY_F7,
    [VK_F8] = TUIM_KEY_F8,
    [VK_F9] = TUIM_KEY_F9,
    [VK_F10] = TUIM_KEY_F10,
    [VK_F11] = TUIM_KEY_F11,
    [VK_F12] = TUIM_KEY_F12,
};

static const TuimVirtualKey tuim_to_win32_vk[256] = {
    [TUIM_KEY_UP] = VK_UP,
    [TUIM_KEY_DOWN] = VK_DOWN,
    [TUIM_KEY_LEFT] = VK_LEFT,
    [TUIM_KEY_RIGHT] = VK_RIGHT,

    [TUIM_KEY_ESCAPE] = VK_ESCAPE,

    [TUIM_KEY_SHIFT] = VK_SHIFT,
    [TUIM_KEY_CTRL] = VK_CONTROL,
    [TUIM_KEY_ALT] = VK_MENU,

    [TUIM_KEY_ENTER] = VK_RETURN,
    [TUIM_KEY_BACKSPACE] = VK_BACK,
    [TUIM_KEY_TAB] = VK_TAB,
    [TUIM_KEY_DELETE] = VK_DELETE,
    [TUIM_KEY_INSERT] = VK_INSERT,

    [TUIM_KEY_HOME] = VK_HOME,
    [TUIM_KEY_END] = VK_END,
    [TUIM_KEY_PAGEUP] = VK_PRIOR,
    [TUIM_KEY_PAGEDOWN] = VK_NEXT,

    [TUIM_KEY_F1] = VK_F1,
    [TUIM_KEY_F2] = VK_F2,
    [TUIM_KEY_F3] = VK_F3,
    [TUIM_KEY_F4] = VK_F4,
    [TUIM_KEY_F5] = VK_F5,
    [TUIM_KEY_F6] = VK_F6,
    [TUIM_KEY_F7] = VK_F7,
    [TUIM_KEY_F8] = VK_F8,
    [TUIM_KEY_F9] = VK_F9,
    [TUIM_KEY_F10] = VK_F10,
    [TUIM_KEY_F11] = VK_F11,
    [TUIM_KEY_F12] = VK_F12,
};

TuimVirtualKey tuim_windows_backend_win32_to_virtual_key(const uint8_t key);

TuimBackend tuim_windows_backend();

// ATTRIBUTES:

// static const char* const tuim_win32_attributes_supported[12] {
// 
// }

void tuim_windows_backend_set_attrib(TuimWindowsBackendData* data, const tuim_backend_attrib_t attrib, const char* val);
bool tuim_windows_backend_attrib_supported(TuimWindowsBackendData* data, const tuim_backend_attrib_t attrib);

char tuim_windows_backend_get_char(TuimWindowsBackendData* data);

// input processed by win32, for example, for typing applications
bool tuim_windows_backend_inp_rep(TuimWindowsBackendData* data, const tuim_key_code_t key);

char* tuim_windows_backend_get_clipboard (TuimWindowsBackendData* data);

double tuim_windows_backend_get_delta_time(TuimWindowsBackendData* data);

#endif //_WIN32