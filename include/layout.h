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
    
    TuimAlign h_align;                    
    TuimAlign v_align;                    
    
    int margin_start;                
    int margin_end;                 
    int padding;                     
    
    bool expand;                     

} TuimLayoutElement;

typedef struct {
    TuimLayoutElement* elements;
    size_t capacity;
    size_t size;

} TuimLayout;

void tuim_layout_draw(const TuimContext* ctx, const TuimLayout* layout);
void tuim_layout_update(const TuimContext* ctx, TuimLayout* layout);

void tuim_layout_init(TuimLayout* layout, size_t capacity);
void tuim_layout_destroy(TuimLayout* layout);
void tuim_layout_add(TuimLayout* layout, const TuimElement* element);
TuimElement* tuim_layout_get(TuimLayout* layout, const size_t index);
void tuim_layout_clear(TuimLayout* layout);

#endif // TUIM_LAYOUT_H