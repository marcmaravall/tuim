#ifndef TUIM_KEYBOARD_H
#define TUIM_KEYBOARD_H

#include <meb.h>

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// TODO: solve problems with keypad and windows backend

typedef enum {
    TUIM_KEY_UNKNOWN = 0,

    // arrows
    TUIM_KEY_UP = 1,
    TUIM_KEY_DOWN = 2,
    TUIM_KEY_LEFT = 3,
    TUIM_KEY_RIGHT = 4,

    TUIM_KEY_ESCAPE = 5,
    TUIM_KEY_SHIFT = 6,
    TUIM_KEY_CTRL = 7,
    TUIM_KEY_ALT = 8,
    TUIM_KEY_SUPER = 9,

    TUIM_KEY_ENTER = 10,
    TUIM_KEY_BACKSPACE = 11,
    TUIM_KEY_TAB = 12,
    TUIM_KEY_DELETE = 13,
    TUIM_KEY_INSERT = 14,

    // navigation
    TUIM_KEY_HOME = 15,
    TUIM_KEY_END = 16,
    TUIM_KEY_PAGEUP = 17,
    TUIM_KEY_PAGEDOWN = 18,

    // functions
    TUIM_KEY_F1 = 19,
    TUIM_KEY_F2 = 20,
    TUIM_KEY_F3 = 21,
    TUIM_KEY_F4 = 22,
    TUIM_KEY_F5 = 23,
    TUIM_KEY_F6 = 24,
    TUIM_KEY_F7 = 25,
    TUIM_KEY_F8 = 26,
    TUIM_KEY_F9 = 27,
    TUIM_KEY_F10 = 28,
    TUIM_KEY_F11 = 29,
    TUIM_KEY_F12 = 30,

    // lock
    TUIM_KEY_CAPSLOCK = 31,
    TUIM_KEY_NUMLOCK  = 32,
    TUIM_KEY_SCROLLLOCK = 33,

    // print
    TUIM_KEY_PRINTSCREEN = 34,
    TUIM_KEY_PAUSE = 35,

    // keypad
    TUIM_KEY_KP_0 = 40,
    TUIM_KEY_KP_1 = 41,
    TUIM_KEY_KP_2 = 42,
    TUIM_KEY_KP_3 = 43,
    TUIM_KEY_KP_4 = 44,
    TUIM_KEY_KP_5 = 45,
    TUIM_KEY_KP_6 = 46,
    TUIM_KEY_KP_7 = 47,
    TUIM_KEY_KP_8 = 48,
    TUIM_KEY_KP_9 = 49,
    TUIM_KEY_KP_DECIMAL = 50,
    TUIM_KEY_KP_DIVIDE = 51,
    TUIM_KEY_KP_MULTIPLY = 52,
    TUIM_KEY_KP_SUBTRACT = 53,
    TUIM_KEY_KP_ADD = 54,
    TUIM_KEY_KP_ENTER = 55,
} TuimVirtualKey;

#define TUIM_KEY_A 'A'
#define TUIM_KEY_B 'B'
#define TUIM_KEY_C 'C'
#define TUIM_KEY_D 'D'
#define TUIM_KEY_E 'E'
#define TUIM_KEY_F 'F'
#define TUIM_KEY_G 'G'
#define TUIM_KEY_H 'H'
#define TUIM_KEY_I 'I'
#define TUIM_KEY_J 'J'
#define TUIM_KEY_K 'K'
#define TUIM_KEY_L 'L'
#define TUIM_KEY_M 'M'
#define TUIM_KEY_N 'N'
#define TUIM_KEY_O 'O'
#define TUIM_KEY_P 'P'
#define TUIM_KEY_Q 'Q'
#define TUIM_KEY_R 'R'
#define TUIM_KEY_S 'S'
#define TUIM_KEY_T 'T'
#define TUIM_KEY_U 'U'
#define TUIM_KEY_V 'V'
#define TUIM_KEY_W 'W'
#define TUIM_KEY_X 'X'
#define TUIM_KEY_Y 'Y'
#define TUIM_KEY_Z 'Z'

#define TUIM_KEY_SPACE ' '

#define TUIM_KEY_COUNT 256

typedef struct {
	bool last    [TUIM_KEY_COUNT];
	bool current [TUIM_KEY_COUNT];
} TuimKeyboardState;

typedef struct TuimContext TuimContext;
typedef size_t tuim_key_code_t;

bool tuim_is_key_down	(const TuimContext* ctx, const tuim_key_code_t key_code);
bool tuim_is_key        (const TuimContext* ctx, const tuim_key_code_t key_code);
bool tuim_is_key_up     (const TuimContext* ctx, const tuim_key_code_t key_code);

bool tuim_is_ctrl       (const TuimContext* ctx, const tuim_key_code_t key_code);
bool tuim_is_alt        (const TuimContext* ctx, const tuim_key_code_t key_code);
bool tuim_is_shift      (const TuimContext* ctx, const tuim_key_code_t key_code);

#endif // TUIM_KEYBOARD_H