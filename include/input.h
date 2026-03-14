#ifndef TUIM_INPUT_H
#define TUIM_INPUT_H

#include "keyboard.h"
#include "mouse.h"

typedef struct TuimContext TuimContext;

typedef struct {
	TuimKeyboardState keyboard_state;
	TuimMouseState mouse_state;
} TuimInputState;

void tuim_clear_input(TuimInputState* state);
void tuim_update_input(TuimContext* ctx);

#endif // TUIM_INPUT_H