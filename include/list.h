// at this moment this is only a wrapper...

#ifndef TUIM_TEXT_LIST_H
#define TUIM_TEXT_LIST_H

#include "layout.h"
#include "color.h"
#include "element.h"
#include "context.h"
#include "size_hint.h"
#include "rect.h"

typedef struct {
	char* label;
	bool free_on_delete;
} TuimTextListElement;

typedef struct {
	TuimRect area;
	TuimColor fg;
	TuimColor bg;

	TuimTextListElement* data;
	size_t size;
	size_t capacity;
} TuimTextList;

TuimTextList tuim_text_list(const size_t capacity);

char* tuim_text_list_get (TuimTextList* list, const size_t index);
TuimTextListElement tuim_text_list_get_el (TuimTextList* list, const size_t index);

void tuim_text_list_update(TuimTextList* ctx, TuimTextList* list);
void tuim_text_list_draw(TuimTextList* ctx, const TuimTextList* list);
void tuim_text_list_destroy(TuimTextList* list);

TuimSizeHint tuim_text_list_measure(TuimTextList* list);
TuimElement tuim_text_list_to_element(const TuimTextList* list);
void tuim_text_list_layout(TuimTextList* list, const TuimRect rect);

void tuim_text_list_add(TuimTextList* list, char* text);
void tuim_text_list_clear(TuimTextList* list);
void tuim_text_list_pop_back(TuimTextList* list);

#endif // TUIM_TEXT_LIST_H