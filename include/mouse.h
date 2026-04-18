#ifndef TUIM_MOUSE_INPUT_H
#define TUIM_MOUSE_INPUT_H

#include <meb.h>

#include <stdint.h>
#include <stdbool.h> 

#include "rect.h"

#define TUIM_MOUSE_BUTTON_LEFT	 0x01
#define TUIM_MOUSE_BUTTON_RIGHT  0x02
#define TUIM_MOUSE_BUTTON_MIDDLE 0x04

typedef struct {
	int mouse_x;
	int mouse_y;

	uint8_t previous;
	uint8_t current;
} TuimMouseState;

typedef struct TuimContext TuimContext;

void tuim_get_mouse_position(const TuimContext* state, int* x, int* y);

void tuim_clear_mouse_state(TuimMouseState* state);
void tuim_update_mouse_state (TuimMouseState* state);

bool tuim_is_mouse_inside(const TuimContext* state, const TuimRect area);

bool tuim_is_mouse_button_down		(const TuimContext* state, const size_t button);
bool tuim_is_mouse_button_up		(const TuimContext* state, const size_t button);
bool tuim_is_mouse_button			(const TuimContext* state, const size_t button);

#endif // TUIM_MOUSE_INPUT_H