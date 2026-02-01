#ifndef TUIM_ANSI_H
#define TUIM_ANSI_H

#include <stdio.h>
#include <stdint.h>

#ifdef _WIN32
#include <windows.h>
#endif

void tuim_ansi_init();
void tuim_ansi_clear();

// helper functions
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

#endif //TUIM_ANSI_H