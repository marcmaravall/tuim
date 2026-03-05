#include "backends/windows/windows_backend.h"

void tuim_windows_backend_init(void* backend_data) {
	assert(backend_data);
	
	TuimWindowsBackendData* data = backend_data;
	data->window = GetConsoleWindow();
	data->handle = GetStdHandle(STD_OUTPUT_HANDLE);
	data->current_attributes = 0x07;

	DWORD dwMode = 0;
	if (data->handle == INVALID_HANDLE_VALUE)
		return;
	if (!GetConsoleMode(data->handle, &dwMode))
		return;

	// remove resize:
	LONG style = GetWindowLong(data->window, GWL_STYLE);

	style &= ~WS_SIZEBOX;
	style &= ~WS_MAXIMIZEBOX;

	SetWindowLong(data->window, GWL_STYLE, style);

	// remove scrollbar
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(data->handle, &csbi);

	data->current_attributes = csbi.wAttributes;

	COORD newSize;
	newSize.X = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	newSize.Y = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

	SetConsoleScreenBufferSize(data->handle, newSize);

	// TODO: remove cursor
}

void tuim_windows_backend_get_size(void* backend_data, size_t* x, size_t* y) {
	assert(backend_data != NULL);
	
	TuimWindowsBackendData* data = backend_data;
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	if (!GetConsoleScreenBufferInfo(data->handle, &csbi)) {
		return;
	}

	*x = csbi.dwSize.X;
	*y = csbi.dwSize.Y;
}

void tuim_windows_backend_destroy(void* data) {

}

void tuim_windows_backend_pass_frame_buffer(void* backend_data, const TuimFrameBuffer* frame_buffer) {
	assert(backend_data != NULL);
	assert(frame_buffer != NULL);

	TuimWindowsBackendData* data = backend_data;

	size_t width = frame_buffer->width;
	size_t height = frame_buffer->height;

	if (height != data->buffer_size.Y || width != data->buffer_size.X) {

		CHAR_INFO* new_buffer = realloc(
			data->buffer,
			width * height * sizeof(CHAR_INFO)
		);

		assert(new_buffer != NULL);

		data->buffer = new_buffer;

		data->buffer_size.X = (SHORT)width;
		data->buffer_size.Y = (SHORT)height;
	}

	size_t total = width * height;

	for (size_t i = 0; i < total; i++) {
		data->buffer[i].Char.AsciiChar = frame_buffer->cells[i].state;

		data->buffer[i].Attributes = FOREGROUND_GREEN;	// TODO: do with attributes conversion
	}
}

void tuim_windows_backend_render(void* backend_data) {
	TuimWindowsBackendData* data = backend_data;

	COORD bufferCoord = { 0,0 };
	SMALL_RECT region = { 0,0, data->buffer_size.X - 1, data->buffer_size.Y- 1 };

	WriteConsoleOutput(
		data->handle,
		data->buffer,
		data->buffer_size,
		bufferCoord,
		&region
	);
}

static WORD tuim_color_to_win32(const TuimColor color) {
	WORD win_color = 0;
	if (color.type == TUIM_COLOR_TYPE_INDEXED) {
		switch (color.indexed_color) {
			case TUIM_BLACK: win_color = 0; break;
			case TUIM_RED: win_color = 4; break;
			case TUIM_GREEN: win_color = 2; break;
			case TUIM_YELLOW: win_color = 6; break;
			case TUIM_BLUE: win_color = 1; break;
			case TUIM_MAGENTA: win_color = 5; break;
			case TUIM_CYAN: win_color = 3; break;
			case TUIM_WHITE: win_color = 7; break;
			case TUIM_BRIGHT_BLACK: win_color = 8; break;
			case TUIM_BRIGHT_RED: win_color = 12; break;
			case TUIM_BRIGHT_GREEN: win_color = 10; break;
			case TUIM_BRIGHT_YELLOW: win_color = 14; break;
			case TUIM_BRIGHT_BLUE: win_color = 9; break;
			case TUIM_BRIGHT_MAGENTA: win_color = 13; break;
			case TUIM_BRIGHT_CYAN: win_color = 11; break;
			case TUIM_BRIGHT_WHITE: win_color = 15; break;
			default:
				win_color = color.indexed_color % 16;
				break;
		}
	} else if (color.type == TUIM_COLOR_TYPE_RGB) {
		win_color |= (color.rgb_color.red > 128) ? FOREGROUND_RED : 0;
		win_color |= (color.rgb_color.green > 128) ? FOREGROUND_GREEN : 0;
		win_color |= (color.rgb_color.blue > 128) ? FOREGROUND_BLUE : 0;
		if (color.rgb_color.red > 128 || color.rgb_color.green > 128 || color.rgb_color.blue > 128)
			win_color |= FOREGROUND_INTENSITY;
	}
	return win_color;
}

TuimBackend tuim_windows_backend() {
	TuimBackend backend;

	TuimWindowsBackendData* data = malloc(sizeof(TuimWindowsBackendData));
	if (!data) return backend;

	data->buffer = NULL;
	data->buffer_size.X = 0;
	data->buffer_size.Y = 0;
	data->window = NULL;
	data->handle = NULL;
	data->current_attributes = 0;

	backend.data = data;

	backend.init = tuim_windows_backend_init;
	backend.destroy = tuim_windows_backend_destroy;
	backend.render = tuim_windows_backend_render;
	backend.get_size = tuim_windows_backend_get_size;
	backend.pass_frame_buffer = tuim_windows_backend_pass_frame_buffer;

	return backend;
}
