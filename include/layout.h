#ifndef TUIM_LAYOUT_H
#define TUIM_LAYOUT_H

#include "element.h"
#include "rect.h"

typedef enum {
    TUIM_LEFT,
    TUIM_CENTER,
    TUIM_RIGHT,
} TuimAlign;

#define TUIM_ALGIN_DEFAULT TUIM_LEFT

typedef struct {
    TuimElement* data;

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

void tuim_layout_init(TuimLayout* layout, size_t capacity);
void tuim_layout_destroy(TuimLayout* layout);
void tuim_layout_add(TuimLayout* layout, TuimElement* element);
TuimElement* tuim_layout_get(TuimLayout* layout, const size_t index);
void tuim_layout_clear(TuimLayout* layout);

#endif // TUIM_LAYOUT_H