#ifndef TUIM_STYLE_H
#define TUIM_STYLE_H

#include <stdint.h>
#include <assert.h>

#include "color.h"

typedef struct TuimStyle {
    TuimColor rect_background_color;
    TuimColor rect_foreground_color;

	TuimColor text_background_color;
    TuimColor text_foreground_color;

    TuimColor line_color;

    TuimColor clear_color;
} TuimStyle;

TuimStyle tuim_style_default();

typedef struct TuimContext TuimContext;

void tuim_set_rect_background_color(TuimContext* ctx, const TuimColor color);
void tuim_set_rect_foreground_color(TuimContext* ctx, const TuimColor color);
void tuim_set_text_foreground_color(TuimContext* ctx, const TuimColor color);
void tuim_set_text_background_color(TuimContext* ctx, const TuimColor color);
void tuim_set_line_color(TuimContext* ctx,  const TuimColor color);
void tuim_set_clear_color(TuimContext* ctx, const TuimColor color);

#endif //TUIM_STYLE_H