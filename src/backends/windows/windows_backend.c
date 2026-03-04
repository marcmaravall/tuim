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

void tuim_windows_backend_set_foreground_color(void* backend_data, TuimColor color) {
	TuimWindowsBackendData* data = backend_data;

	WORD fg = tuim_color_to_win32(color);
	WORD attributes = (data->current_attributes & 0xF0) | fg;

	if (attributes != data->current_attributes) {
		SetConsoleTextAttribute(data->handle, attributes);
		data->current_attributes = attributes;
	}
}

void tuim_windows_backend_set_background_color(void* backend_data, TuimColor color) {
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

// TODO: do this in a cleaner way
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
