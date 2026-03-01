#ifndef TUIM_BACKEND_H
#define TUIM_BACKEND_H

#include <style.h>

typedef struct TuimBackend {
	void (*init)(void* backend_data);
	void (*destroy)(void* backend_data);
	void (*clear)(void* backend_data);
	void (*render_text)(void* backend_data, const char* msg);
	void (*set_cursor_pos)(void* backend_data, int x, int y);
	void (*set_foreground_color)(void* backend_data, TuimColor color);
	void (*set_background_color)(void* backend_data, TuimColor color);
	void (*set_console_name)(void* backend_data, const char* msg);
	void* data;
} TuimBackend;

#endif //TUIM_BACKEND_H