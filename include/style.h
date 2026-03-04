#ifndef TUIM_STYLE_H
#define TUIM_STYLE_H

#include <stdint.h>
#include <assert.h>

#include "color.h"

typedef struct TuimStyle {
    TuimColor background_color;
    TuimColor text_foreground_color;
    TuimColor rect_background_color;
} TuimStyle;

TuimStyle tuim_style_set_default();

#endif //TUIM_STYLE_H