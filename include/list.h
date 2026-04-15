// at this moment this is only a wrapper...

#ifndef TUIM_LIST_H
#define TUIM_LIST_H

#include "layout.h"
#include "element.h"
#include "context.h"
#include "size_hint.h"
#include "rect.h"

typedef struct {
	char* label;
	bool on_heap;
} TuimTextListElement;

typedef struct {
	TuimTextListElement* data;
	size_t size;
	size_t capacity;
} TuimTextList;

TuimTextList tuim_text_list(const size_t capacity);

TuimTextListElement tuim_text_list_get(TuimTextList* list, const size_t index);

void tuim_text_list_update(TuimTextList* ctx, TuimTextList* list);
void tuim_text_list_draw(TuimTextList* ctx, const TuimTextList* list);
void tuim_text_list_destroy(TuimTextList* list);

TuimSizeHint tuim_text_list_measure(TuimTextList* list);
TuimElement tuim_text_list_to_element(const TuimTextList* list);
void tuim_text_list_layout(TuimTextList* list, const TuimRect rect);

#endif // TUIM_LIST_H