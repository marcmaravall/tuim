#ifndef TUIM_CHECKBOX_H
#define TUIM_CHECKBOX_H

#include "element.h"
#include "rect.h"
#include "color.h"
#include "frame_buffer.h"
#include "context.h"
#include "mouse.h"

#include <mds.h>
#include <meb.h>

#include <stdint.h>

typedef struct {
    TuimRect area;
	mdString label;

    struct TuimCheckboxStyle {
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
} TuimCheckbox;

TuimCheckbox* tuim_default_checkbox();
TuimCheckbox* tuim_checkbox(const char* label);

void tuim_checkbox_draw(TuimContext* ctx, const TuimCheckbox* checkbox);
void tuim_checkbox_update(const TuimContext* ctx, TuimCheckbox* checkbox);
void tuim_checkbox_destroy(TuimCheckbox* checkbox);

TuimElement tuim_checkbox_element   (const char* label);
TuimElement tuim_checkbox_to_element(TuimCheckbox* checkbox);

TuimSizeHint tuim_checkbox_measure(const TuimCheckbox* checkbox);
void tuim_checkbox_layout(TuimCheckbox* checkbox, const TuimRect rect);

TuimRect tuim_checkbox_calculate_area(const TuimCheckbox* checkbox);

#endif // TUIM_CHECKBOX_H