#ifndef TUIM_ANSI_BACKEND_H
#define TUIM_ANSI_BACKEND_H

#include <stdio.h>
#include <backend.h>

#ifdef _WIN32
#include <windows.h>
#endif

void tuim_ansi_backend_init					();
void tuim_ansi_backend_destroy				();
void tuim_ansi_backend_clear				();
void tuim_ansi_backend_render_text			(void* backend_data, const char* msg);
void tuim_ansi_backend_set_cursor_pos		(void* backend_data, int x, int y);
void tuim_ansi_backend_set_foreground_color	(void* backend_data, TuimColor color);
void tuim_ansi_backend_set_background_color	(void* backend_data, TuimColor color);

TuimBackend tuim_ansi_backend();

#endif //TUIM_DEFAULT_BACKEND_H