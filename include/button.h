#ifndef TUIM_BUTTON_H
#define TUIM_BUTTON_H

#include "context.h"
#include "rect.h"
#include "frame_buffer.h"
#include "input.h"
#include "color.h"
#include "element.h"

#include <meb.h>
#include <stdint.h>

typedef void (*TuimOnClickFn)(void* user_data);
typedef void (*TuimOnHoverFn)(void* user_data);
typedef void (*TuimOnReleaseFn)(void* user_data);

typedef struct {
    TuimRect area;
    const char* label;

    struct TuimButtonStyle {
        TuimColor background;
        TuimColor foreground;

        TuimColor hover_background;
        TuimColor hover_foreground;
    } style;

    bool hovered;
    bool pressing;
    bool clicked;

    bool was_down;
    bool pressed_inside;

    TuimOnClickFn on_click;
    TuimOnHoverFn on_hover;
    TuimOnReleaseFn on_release;
    void* user_data;
} TuimButton;

TuimButton tuim_default_button();
TuimButton tuim_button(const char* label);

TuimButton tuim_button_callbacks (
    const char*     label,
	TuimOnClickFn   on_click,
	TuimOnHoverFn   on_hover,
	TuimOnReleaseFn on_release,
    void*           user_data
);

void tuim_button_draw	(TuimContext* ctx, const TuimButton* button);
void tuim_button_update (const TuimContext* ctx, TuimButton* button);
void tuim_button_destroy(TuimButton* button);

TuimElement tuim_button_to_element(TuimButton* button);
TuimSizeHint tuim_button_measure(const TuimButton* button);
void tuim_button_layout(TuimButton* button, const TuimRect rect);

TuimRect tuim_button_calculate_area(const TuimButton* button);

#endif // TUIM_BUTTON_H