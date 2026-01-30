#include <ansi.h>

void tuim_ansi_init() {
#ifdef _WIN32
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode = 0;
	if (hOut == INVALID_HANDLE_VALUE) 
		return;
	if (!GetConsoleMode(hOut, &dwMode)) 
		return;
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hOut, dwMode);
#endif
}

inline void tuim_ansi_clear() {
	printf("\033[2J");
}

inline void tuim_ansi_set_cursor_pos_home() {
	printf("\033[H");
}

inline void tuim_ansi_set_cursor_pos(int x, int y) {
	printf("\033[%d;%dH", y, x);
}

inline void tuim_ansi_cursor_up(int n) {
	printf("\033[%dA", n);
}

inline void tuim_ansi_cursor_down(int n) {
	printf("\033[%dB", n);
}

inline void tuim_ansi_cursor_right(int n) {
	printf("\033[%dC", n);
}

inline void tuim_ansi_cursor_left(int n) {
	printf("\033[%dD", n);
}

inline void tuim_ansi_cursor_save_pos() {
	printf("\033[s");
}

inline void tuim_ansi_cursor_restore_pos() {
	printf("\033[u"); 
}
