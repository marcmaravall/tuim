#ifndef TUIM_MOUSE_INPUT_H
#define TUIM_MOUSE_INPUT_H

#include <assert.h>s
#include <stdint.h>
#include <stdbool.h>

#define TUIM_MOUSE_BUTTON_LEFT 0x01
#define TUIM_MOUSE_BUTTON_RIGHT 0x02
#define TUIM_MOUSE_BUTTON_MIDDLE 0x04

typedef struct {
	uint8_t previous;
	uint8_t current;
} TuimMouseState;

typedef struct TuimContext TuimContext;

void tuim_clear_mouse_state(TuimMouseState* state);
void tuim_update_mouse_state (TuimMouseState* state);

bool tuim_is_mouse_button_down		(const TuimContext* state, const size_t button);
bool tuim_is_mouse_button_pressed	(const TuimContext* state, const size_t button);
bool tuim_is_mouse_button_up		(const TuimContext* state, const size_t button);

#endif // TUIM_MOUSE_INPUT_H