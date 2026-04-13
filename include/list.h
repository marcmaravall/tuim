// at this moment this is only a wrapper...

#ifndef TUIM_LIST_H
#define TUIM_LIST_H

#include "layout.h"
#include "element.h"
#include "context.h"
#include "size_hint.h"
#include "rect.h"

typedef struct {
	TuimLayout layout;
} TuimList;

TuimList tuim_list(const size_t capacity);

void tuim_list_update(TuimList* ctx, TuimList* list);
void tuim_list_draw(TuimList* ctx, const TuimList* list);
void tuim_list_destroy(TuimList* list);

TuimSizeHint tuim_list_measure(TuimList* list);
TuimElement tuim_list_to_element(const TuimList* list);
void tuim_list_layout(TuimList* list, const TuimRect rect);

#endif // TUIM_LIST_H