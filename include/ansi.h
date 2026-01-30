#ifndef TUIM_ANSI_H
#define TUIM_ANSI_H

#include <stdio.h>
#include <stdint.h>

#ifdef _WIN32
#include <windows.h>
#endif

void tuim_ansi_init();
void tuim_ansi_clear();

void tuim_ansi_set_cursor_pos_home();
void tuim_ansi_set_cursor_pos(int x, int y);
void tuim_ansi_cursor_up  (int n);
void tuim_ansi_cursor_down(int n);
void tuim_ansi_cursor_left(int n);
void tuim_ansi_cursor_down(int n);
void tuim_ansi_cursor_save_pos();
void tuim_ansi_cursor_restore_pos();

#endif //TUIM_ANSI_H