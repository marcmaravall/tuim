#ifndef TUIM_BUTTON_H
#define TUIM_BUTTON_H

#include "rect.h"
#include "frame_buffer.h"
#include "input.h"
#include "color.h"

#include <stdint.h>

// TODO: create button flags

typedef struct {
    TuimRect area;
    const char* label;

    struct TuimButtonStyle {
        TuimColor background;
        TuimColor foreground;

        TuimColor hover_background;
        TuimColor hover_foreground;
        
        TuimColor active_background;
        TuimColor active_foreground;
    } style;

    bool toggled;

    bool hovered;
    bool pressing;
    bool clicked;

    bool was_down;
    bool pressed_inside;

    uint8_t flags;
} TuimButton;

TuimButton tuim_default_button();

void tuim_button_draw	(const TuimContext* ctx, const TuimButton* button);
void tuim_button_update (const TuimContext* ctx, TuimButton* button);

TuimRect tuim_button_calculate_area(const TuimButton* button);

#endif // TUIM_BUTTON_H