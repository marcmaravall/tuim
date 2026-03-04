#ifndef TUIM_ANSI_BACKEND_H
#define TUIM_ANSI_BACKEND_H

#include <stdio.h>
#include <assert.h>

#include "color.h"
#include "backend.h"

#ifdef _WIN32
#include <windows.h>
#endif

typedef enum TuimAnsiColor {
    TUIM_ANSI_COLOR_BLACK = 30,
    TUIM_ANSI_COLOR_RED = 31,
    TUIM_ANSI_COLOR_GREEN = 32,
    TUIM_ANSI_COLOR_YELLOW = 33,
    TUIM_ANSI_COLOR_BLUE = 34,
    TUIM_ANSI_COLOR_MAGENTA = 35,
    TUIM_ANSI_COLOR_CYAN = 36,
    TUIM_ANSI_COLOR_WHITE = 37,
    TUIM_ANSI_COLOR_BRIGHT_BLACK = 90,
    TUIM_ANSI_COLOR_BRIGHT_RED = 91,
    TUIM_ANSI_COLOR_BRIGHT_GREEN = 92,
    TUIM_ANSI_COLOR_BRIGHT_YELLOW = 93,
    TUIM_ANSI_COLOR_BRIGHT_BLUE = 94,
    TUIM_ANSI_COLOR_BRIGHT_MAGENTA = 95,
    TUIM_ANSI_COLOR_BRIGHT_CYAN = 96,
    TUIM_ANSI_COLOR_BRIGHT_WHITE = 97,
    TUIM_ANSI_COLOR_DEFAULT = 39             // default foreground
} TuimAnsiColor;

#define TO_BACKGROUND(x) ((x) + 10)

TuimAnsiColor tuim_color_to_ansi(TuimColor color);

void tuim_ansi_backend_init					();
void tuim_ansi_backend_destroy				();
void tuim_ansi_backend_clear				();
void tuim_ansi_backend_render_text			(void* backend_data, const char* msg);
void tuim_ansi_backend_set_cursor_pos		(void* backend_data, int x, int y);
void tuim_ansi_backend_set_foreground_color	(void* backend_data, TuimColor color);
void tuim_ansi_backend_set_background_color	(void* backend_data, TuimColor color);

TuimBackend tuim_ansi_backend();

#endif //TUIM_DEFAULT_BACKEND_H