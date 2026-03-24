#ifndef TUIM_KEYBOARD_H
#define TUIM_KEYBOARD_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum {
    TUIM_KEY_UNKNOWN = 0,

    // arrows
    TUIM_KEY_UP,
    TUIM_KEY_DOWN,
    TUIM_KEY_LEFT,
    TUIM_KEY_RIGHT,

    TUIM_KEY_ESCAPE,
    TUIM_KEY_SHIFT,
    TUIM_KEY_CTRL,
    TUIM_KEY_ALT,
    TUIM_KEY_SUPER,

    TUIM_KEY_ENTER,
    TUIM_KEY_BACKSPACE,
    TUIM_KEY_TAB,
    TUIM_KEY_DELETE,
    TUIM_KEY_INSERT,

    // navigation
    TUIM_KEY_HOME,
    TUIM_KEY_END,
    TUIM_KEY_PAGEUP,
    TUIM_KEY_PAGEDOWN,

    // functions
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

    // lock
    TUIM_KEY_CAPSLOCK,
    TUIM_KEY_NUMLOCK,
    TUIM_KEY_SCROLLLOCK,

    // print
    TUIM_KEY_PRINTSCREEN,
    TUIM_KEY_PAUSE,

    // keypad
    TUIM_KEY_KP_0,
    TUIM_KEY_KP_1,
    TUIM_KEY_KP_2,
    TUIM_KEY_KP_3,
    TUIM_KEY_KP_4,
    TUIM_KEY_KP_5,
    TUIM_KEY_KP_6,
    TUIM_KEY_KP_7,
    TUIM_KEY_KP_8,
    TUIM_KEY_KP_9,
    TUIM_KEY_KP_DECIMAL,
    TUIM_KEY_KP_DIVIDE,
    TUIM_KEY_KP_MULTIPLY,
    TUIM_KEY_KP_SUBTRACT,
    TUIM_KEY_KP_ADD,
    TUIM_KEY_KP_ENTER,
} TuimVirtualKey;

#define TUIM_KEY_COUNT 256

typedef struct {
	bool last    [TUIM_KEY_COUNT];
	bool current [TUIM_KEY_COUNT];
} TuimKeyboardState;

typedef struct TuimContext TuimContext;

bool tuim_is_key_down	  (const TuimContext* ctx, const size_t key_code);
bool tuim_is_key          (const TuimContext* ctx, const size_t key_code);
bool tuim_is_key_up       (const TuimContext* ctx, const size_t key_code);

#endif // TUIM_KEYBOARD_H