#ifndef TUIM_STYLE_H
#define TUIM_STYLE_H

#include <stdint.h>
#include <assert.h>

#include "color.h"

// ansi colors:
typedef enum TuimAnsiColor {
    TUIM_ANSI_COLOR_BLACK = 30,
    TUIM_ANSI_COLOR_RED = 31,  
    TUIM_ANSI_COLOR_GREEN = 32,  
    TUIM_ANSI_COLOR_YELLOW = 33,
    TUIM_ANSI_COLOR_BLUE = 34,
    TUIM_ANSI_COLOR_MAGENTA = 35,
    TUIM_ANSI_COLOR_CYAN = 36,
    TUIM_ANSI_COLOR_WHITE = 37,
    TUIM_ANSI_COLOR_BRIGHT_BLACK = 90, 
    TUIM_ANSI_COLOR_BRIGHT_RED = 91,  
    TUIM_ANSI_COLOR_BRIGHT_GREEN = 92,  
    TUIM_ANSI_COLOR_BRIGHT_YELLOW = 93,
    TUIM_ANSI_COLOR_BRIGHT_BLUE = 94,
    TUIM_ANSI_COLOR_BRIGHT_MAGENTA = 95,
    TUIM_ANSI_COLOR_BRIGHT_CYAN = 96,     
    TUIM_ANSI_COLOR_BRIGHT_WHITE = 97,
    TUIM_ANSI_COLOR_DEFAULT = 39             // default foreground
} TuimAnsiColor;

typedef uint32_t TuimColorRGB;

#define TO_BACKGROUND(x) ((x) + 10)

typedef struct TuimStyle {
    uint8_t background_color;
    uint8_t text_foreground_color;
    uint8_t rect_background_color;
} TuimStyle;

TuimStyle tuim_style_set_default();

#endif //TUIM_STYLE_H