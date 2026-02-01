#include <os.h>

#ifdef _WIN32

// TODO: add settings
void tuim_win32_init(TuimOsData* data) {
	data->win32_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode = 0;
	if (data->win32_handle == INVALID_HANDLE_VALUE)
		return;
	if (!GetConsoleMode(data->win32_handle, &dwMode))
		return;
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(data->win32_handle, dwMode);

	// remove resize:
	HWND window = GetConsoleWindow();
	LONG style = GetWindowLong(window, GWL_STYLE);

	style &= ~WS_SIZEBOX;
	style &= ~WS_MAXIMIZEBOX;

	SetWindowLong(window, GWL_STYLE, style);

	// remove scrollbar
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(data->win32_handle, &csbi);

	COORD newSize;
	newSize.X = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	newSize.Y = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

	SetConsoleScreenBufferSize(data->win32_handle, newSize);
}

int tuim_win32_get_console_buffer_info(TuimOsConsoleBufferInfo* info) {
	CONSOLE_SCREEN_BUFFER_INFO win_buffer;
	
	assert(info != NULL);

	info->win32_handle = GetStdHandle(STD_OUTPUT_HANDLE);

	if (!GetConsoleScreenBufferInfo(info->win32_handle, &win_buffer)) {
		return -1;
	}

	info->width  = win_buffer.srWindow.Right - win_buffer.srWindow.Left + 1;
	info->height = win_buffer.srWindow.Bottom - win_buffer.srWindow.Top + 1;

	info->cursor_x = win_buffer.dwCursorPosition.X;
	info->cursor_y = win_buffer.dwCursorPosition.Y;

	return 0;
}

void tuim_os_init() {
	tuim_win32_init();
}

int tuim_os_get_console_buffer_info(TuimOsConsoleBufferInfo* info) {
	return tuim_win32_get_console_buffer_info(info);
}

#else

void tuim_os_init() {
	assert(0);	// os not supported
}

#endif