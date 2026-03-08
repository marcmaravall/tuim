#ifndef TUIM_INPUT_H
#define TUIM_INPUT_H

#include <stdbool.h>
#include <stdint.h>

// TODO: implement virtual keys for windows backend
typedef enum {
    TUIM_KEY_UNKNOWN,
    TUIM_KEY_UP,
    TUIM_KEY_DOWN,
    TUIM_KEY_LEFT,
    TUIM_KEY_RIGHT,
    TUIM_KEY_ESCAPE,
    TUIM_KEY_ENTER,
    TUIM_KEY_BACKSPACE,
    TUIM_KEY_TAB,
    TUIM_KEY_DELETE,
    TUIM_KEY_HOME,
    TUIM_KEY_END,
    TUIM_KEY_PAGEUP,
    TUIM_KEY_PAGEDOWN,
    TUIM_KEY_F1,
    TUIM_KEY_F2,
    TUIM_KEY_F3,
    TUIM_KEY_F4,
    TUIM_KEY_F5,
    TUIM_KEY_F6,
    TUIM_KEY_F7,
    TUIM_KEY_F8,
    TUIM_KEY_F9,
    TUIM_KEY_F10,
    TUIM_KEY_F11,
    TUIM_KEY_F12,
} TuimVirtualKey;

#define TUIM_KEY_COUNT 256

typedef struct {
	bool last    [TUIM_KEY_COUNT];
	bool current [TUIM_KEY_COUNT];
} TuimInputState;

void tuim_clear_input(TuimInputState* state);

typedef struct TuimContext TuimContext;

void tuim_update_input(TuimContext* ctx);

bool tuim_is_key_down	  (const TuimContext* ctx, const size_t key_code);
bool tuim_is_key_pressed  (const TuimContext* ctx, const size_t key_code);
bool tuim_is_key_up (const TuimContext* ctx, const size_t key_code);

#endif // TUIM_INPUT_H