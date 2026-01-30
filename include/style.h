#ifndef TUIM_STYLE_H
#define TUIM_STYLE_H

typedef enum TuimColor {
    TUIM_COLOR_BLACK = 30, 
    TUIM_COLOR_RED = 31,  
    TUIM_COLOR_GREEN = 32,  
    TUIM_COLOR_YELLOW = 33,
    TUIM_COLOR_BLUE = 34,
    TUIM_COLOR_MAGENTA = 35,
    TUIM_COLOR_CYAN = 36,
    TUIM_COLOR_WHITE = 37,
    TUIM_COLOR_BRIGHT_BLACK = 90, 
    TUIM_COLOR_BRIGHT_RED = 91,  
    TUIM_COLOR_BRIGHT_GREEN = 92,  
    TUIM_COLOR_BRIGHT_YELLOW = 93,
    TUIM_COLOR_BRIGHT_BLUE = 94,
    TUIM_COLOR_BRIGHT_MAGENTA = 95,
    TUIM_COLOR_BRIGHT_CYAN = 96,     
    TUIM_COLOR_BRIGHT_WHITE = 97,
    TUIM_COLOR_DEFAULT = 39             // default foreground
} TuimColor;

inline int tuim_color_to_background(TuimColor color);

typedef struct TuimStyle {
    TuimColor text_foreground_color;
} TuimStyle;

#endif //TUIM_STYLE_H