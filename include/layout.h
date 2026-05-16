#ifndef TUIM_LAYOUT_H
#define TUIM_LAYOUT_H

#include <meb.h>

#include "element.h"
#include "rect.h"

#include "text.h"
#include "button.h"
#include "checkbox.h"

typedef enum {
    TUIM_LEFT_ALIGN,
    TUIM_CENTER_ALIGN,
    TUIM_RIGHT_ALIGN,
} TuimAlign;

#define TUIM_ALGIN_DEFAULT TUIM_LEFT_ALIGN

typedef struct {
    TuimElement data;

    float flex;

    int base_size;

    TuimAlign h_align;
    TuimAlign v_align;

    int margin_start;
    int margin_end;
    int padding;

    bool expand;

} TuimLayoutElement;

typedef enum {
    TUIM_ROW,
    TUIM_COLUMN,
} TuimDirection;

typedef enum {
    TUIM_START_JUSTIFY,
    TUIM_CENTER_JUSTIFY,
    TUIM_END_JUSTIFY,
    TUIM_SPACE_BETWEEN,
    TUIM_SPACE_AROUND,
} TuimJustify;

typedef struct {
    TuimLayoutElement* elements;
    size_t capacity;
    size_t size;

    TuimDirection direction;
    TuimJustify justify;
    TuimAlign align_items;

    int spacing;
    TuimRect bounds;
} TuimLayout;

void tuim_layout_draw(TuimContext* ctx, const TuimLayout* layout);

void tuim_layout_update(TuimContext* ctx, TuimLayout* layout);

#define TUIM_LAYOUT_DEFAULT_BOUNDS_X 0
#define TUIM_LAYOUT_DEFAULT_BOUNDS_Y 0
#define TUIM_LAYOUT_DEFAULT_BOUNDS_WIDTH  INT_MAX
#define TUIM_LAYOUT_DEFAULT_BOUNDS_HEIGHT INT_MAX
#define TUIM_LAYOUT_DEFAULT_DIRECTION TUIM_COLUMN
#define TUIM_LAYOUT_DEFAULT_SPACING 0

void tuim_layout_init(TuimLayout* layout, size_t capacity);

void tuim_layout_destroy(TuimLayout* layout);

TuimElement tuim_layout_get(TuimLayout* layout, const size_t index);
void tuim_layout_clear(TuimLayout* layout);

TuimElement tuim_layout_to_element(TuimLayout* layout);
TuimSizeHint tuim_layout_measure(const TuimLayout* layout);
void tuim_layout_layout(TuimLayout* layout, const TuimRect area);

void tuim_layout_add(TuimLayout* layout, TuimElement element);
void tuim_layout_add_elements(TuimLayout* layout, size_t count, TuimElement elements[]);

// ---------
TuimElement tuim_layout_add_text(TuimLayout* layout, char* str, TuimText* text);
TuimElement tuim_layout_add_button(TuimLayout* layout, const char* str, TuimButton* button);
TuimElement tuim_layout_add_checkbox(TuimLayout* layout, const char* str, TuimCheckbox* checkbox);

#endif // TUIM_LAYOUT_H