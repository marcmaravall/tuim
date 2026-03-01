#ifndef TUIM_COLOR_H
#define TUIM_COLOR_H

// TODO: add colors shit
typedef enum {
    TUIM_BLACK,
    TUIM_RED,
    TUIM_GREEN,
    TUIM_YELLOW,
    TUIM_BLUE,
    TUIM_MAGENTA,
    TUIM_CYAN,
    TUIM_WHITE,

    TUIM_BRIGHT_BLACK,
    TUIM_BRIGHT_RED,
    TUIM_BRIGHT_GREEN,
    TUIM_BRIGHT_YELLOW,
    TUIM_BRIGHT_BLUE,
    TUIM_BRIGHT_MAGENTA,
    TUIM_BRIGHT_CYAN,
    TUIM_BRIGHT_WHITE
} TuimColorIndex;

typedef enum {
    TUIM_COLOR_TYPE_NONE,
    TUIM_COLOR_TYPE_INDEXED,
    TUIM_COLOR_TYPE_RGB
} TuimColorType;

typedef struct {
    union color {
        TuimColorIndex indexed_color;
        struct {
            uint8_t red;
            uint8_t green;
            uint8_t blue;
        } rgb_color;
    };
    TuimColorType type;
} TuimColor;

#endif //TUIM_COLOR_H