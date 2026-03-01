#include <backends/windows/windows_backend.h>

// TODO: implement:
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

void tuim_windows_backend_destroy(void* backend_data) {
	
}

// thanks to https://learn.microsoft.com/en-us/windows/console/clearing-the-screen
void tuim_windows_backend_clear(void* backend_data) {
	TuimWindowsBackendData* data = backend_data;
	assert(data);
	
	COORD coord = { 0, 0 };
	DWORD chars_written;
	CONSOLE_SCREEN_BUFFER_INFO buffer_info;

	if (!GetConsoleScreenBufferInfo(data->handle, &buffer_info))
		return;

	DWORD size = buffer_info.dwSize.X * buffer_info.dwSize.Y;

	if (!FillConsoleOutputCharacter(data->handle, ' ', size, coord, &chars_written)) {
		return;
	}

	if (!GetConsoleScreenBufferInfo(data->handle, &buffer_info)) {
		return;
	}

	if (!FillConsoleOutputAttribute(data->handle, buffer_info.wAttributes, size, coord, &chars_written)) {
		return;
	}

	SetConsoleCursorPosition(data->handle, coord);
}

void tuim_windows_backend_render_text(void* backend_data, const char* msg) {
	printf("%s", msg);
}

void tuim_windows_backend_set_cursor_pos(void* backend_data, int x, int y) {
	SetConsoleCursorPosition(((TuimWindowsBackendData*)backend_data)->handle, (COORD){0, 0});
}

void tuim_windows_backend_set_foreground_color(void* backend_data, TuimAnsiColor color) {
	TuimWindowsBackendData* data = backend_data;

	WORD fg = tuim_color_to_win32(color);
	WORD attributes = (data->current_attributes & 0xF0) | fg;

	if (attributes != data->current_attributes) {
		SetConsoleTextAttribute(data->handle, attributes);
		data->current_attributes = attributes;
	}
}

void tuim_windows_backend_set_background_color(void* backend_data, TuimAnsiColor color) {
	TuimWindowsBackendData* data = backend_data;

	WORD bg = tuim_color_to_win32(color) << 4;
	WORD attributes = (data->current_attributes & 0x0F) | bg;

	if (attributes != data->current_attributes) {
		SetConsoleTextAttribute(data->handle, attributes);
		data->current_attributes = attributes;
	}
}

void tuim_windows_backend_set_console_name(void* backend_data, const char* msg) {
	TCHAR old_title[MAX_PATH];
	TCHAR new_title[MAX_PATH];

	assert(GetConsoleTitle(old_title, MAX_PATH));

	StringCchPrintf(new_title, MAX_PATH, TEXT("%s"), msg);

	assert(SetConsoleTitle(new_title));
}

static WORD tuim_color_to_win32(const TuimAnsiColor color) {
	if (color == TUIM_ANSI_COLOR_DEFAULT)
		return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;

	int base;
	int bright = 0;

	if (color >= 90) {
		base = color - 90;
		bright = 1;
	}
	else {
		base = color - 30;
	}

	WORD result = 0;

	if (base & 1) result |= FOREGROUND_RED;
	if (base & 2) result |= FOREGROUND_GREEN;
	if (base & 4) result |= FOREGROUND_BLUE;
	if (bright)   result |= FOREGROUND_INTENSITY;

	return result;
}

TuimBackend tuim_windows_backend() {
	TuimBackend backend;

	backend.data = malloc(sizeof(TuimWindowsBackendData));

	backend.init				 = tuim_windows_backend_init;
	backend.destroy				 = tuim_windows_backend_destroy;
	backend.clear			     = tuim_windows_backend_clear;
	backend.render_text			 = tuim_windows_backend_render_text;
	backend.set_cursor_pos		 = tuim_windows_backend_set_cursor_pos;
	backend.set_foreground_color = tuim_windows_backend_set_foreground_color;
	backend.set_background_color = tuim_windows_backend_set_background_color;
	backend.set_console_name	 = tuim_windows_backend_set_console_name;
	
	return backend;
}
