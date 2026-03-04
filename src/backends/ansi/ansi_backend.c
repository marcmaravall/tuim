#include "backends/ansi/ansi_backend.h"

TuimAnsiColor tuim_color_to_ansi(TuimColor color) {
	if (color.type == TUIM_COLOR_TYPE_INDEXED) {
		switch (color.indexed_color) {
			case TUIM_BLACK: return TUIM_ANSI_COLOR_BLACK;
			case TUIM_RED: return TUIM_ANSI_COLOR_RED;
			case TUIM_GREEN: return TUIM_ANSI_COLOR_GREEN;
			case TUIM_YELLOW: return TUIM_ANSI_COLOR_YELLOW;
			case TUIM_BLUE: return TUIM_ANSI_COLOR_BLUE;
			case TUIM_MAGENTA: return TUIM_ANSI_COLOR_MAGENTA;
			case TUIM_CYAN: return TUIM_ANSI_COLOR_CYAN;
			case TUIM_WHITE: return TUIM_ANSI_COLOR_WHITE;
			case TUIM_BRIGHT_BLACK: return TUIM_ANSI_COLOR_BRIGHT_BLACK;
			case TUIM_BRIGHT_RED: return TUIM_ANSI_COLOR_BRIGHT_RED;
			case TUIM_BRIGHT_GREEN: return TUIM_ANSI_COLOR_BRIGHT_GREEN;
			case TUIM_BRIGHT_YELLOW: return TUIM_ANSI_COLOR_BRIGHT_YELLOW;
			case TUIM_BRIGHT_BLUE: return TUIM_ANSI_COLOR_BRIGHT_BLUE;
			case TUIM_BRIGHT_MAGENTA: return TUIM_ANSI_COLOR_BRIGHT_MAGENTA;
			case TUIM_BRIGHT_CYAN: return TUIM_ANSI_COLOR_BRIGHT_CYAN;
			case TUIM_BRIGHT_WHITE: return TUIM_ANSI_COLOR_BRIGHT_WHITE;
			default: break;
		}
	}
	assert(0);	// INVALID CASEs
}

void tuim_ansi_backend_init() {
#ifdef _WIN32
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode = 0;
	if (hOut == INVALID_HANDLE_VALUE)
		return;
	if (!GetConsoleMode(hOut, &dwMode))
		return;
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hOut, dwMode);

	// remove resize:
	HWND window = GetConsoleWindow();
	LONG style = GetWindowLong(window, GWL_STYLE);

	style &= ~WS_SIZEBOX;
	style &= ~WS_MAXIMIZEBOX;

	SetWindowLong(window, GWL_STYLE, style);

	// remove scrollbar
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hOut, &csbi);

	COORD newSize;
	newSize.X = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	newSize.Y = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

	SetConsoleScreenBufferSize(hOut, newSize);
#else

#endif
}

void tuim_ansi_backend_destroy() {
	// NOTHING
}

void tuim_ansi_backend_clear() {
#ifdef _WIN32
	system("cls");
#endif
	fflush(stdout);
}

void tuim_ansi_backend_render_text(void* backend_data, const char* msg) {
	printf("%s", msg);
}

void tuim_ansi_backend_set_cursor_pos(void* backend_data, int x, int y) {
	printf("\033[%d;%dH", y, x);
}

// TODO: do with TuimColor
void tuim_ansi_backend_set_foreground_color(void* backend_data, TuimColor color) {
	(void)backend_data;

	printf("\x1b[%dm", tuim_color_to_ansi(color));
}

void tuim_ansi_backend_set_background_color(void* backend_data, TuimColor color) {
	(void)backend_data;

	printf("\x1b[%dm", TO_BACKGROUND(tuim_color_to_ansi(color)));
}

TuimBackend tuim_ansi_backend() {
	TuimBackend backend;
	backend.init = tuim_ansi_backend_init;
	backend.destroy = tuim_ansi_backend_destroy;
	backend.clear = tuim_ansi_backend_clear;
	backend.render_text = tuim_ansi_backend_render_text;
	backend.set_cursor_pos = tuim_ansi_backend_set_cursor_pos;
	backend.set_foreground_color = tuim_ansi_backend_set_foreground_color;
	backend.set_background_color = tuim_ansi_backend_set_background_color;
	return backend;
}
