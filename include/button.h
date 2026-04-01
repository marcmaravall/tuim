#ifndef TUIM_BUTTON_H
#define TUIM_BUTTON_H

#include "context.h"
#include "rect.h"
#include "frame_buffer.h"
#include "input.h"
#include "color.h"
#include "element.h"

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

void tuim_button_draw	(TuimContext* ctx, const TuimButton* button);
void tuim_button_update (const TuimContext* ctx, TuimButton* button);
void tuim_button_destroy(TuimButton* button);

TuimElement tuim_button_to_element(TuimButton* button);
TuimSizeHint tuim_button_measure(const TuimButton* button);
void tuim_button_layout(TuimButton* button, const TuimRect rect);

TuimRect tuim_button_calculate_area(const TuimButton* button);

#endif // TUIM_BUTTON_H