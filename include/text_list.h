// at this moment this is only a wrapper...

#ifndef TUIM_TEXT_LIST_H
#define TUIM_TEXT_LIST_H

#include "layout.h"
#include "color.h"
#include "element.h"
#include "color.h"
#include "context.h"
#include "size_hint.h"
#include "rect.h"

#include <stdint.h>

typedef struct {
	char* label;
	bool free_on_delete;
} TuimTextListElement;


// style flags:

#ifndef TUIM_LIST_STYLE_DEFAULT_FG
#define TUIM_LIST_STYLE_DEFAULT_FG TUIM_WHITE_STRUCT_INDEXED
#endif
#ifndef TUIM_LIST_STYLE_DEFAULT_BG
#define TUIM_LIST_STYLE_DEFAULT_BG TUIM_BLACK_STRUCT_INDEXED
#endif

typedef struct {
	TuimColor foreground;
	TuimColor background;

	uint8_t flags;
} TuimTextListStyle;

TuimTextListStyle tuim_text_list_default_style();

typedef struct {
	TuimTextListStyle style;
	TuimRect area;

	TuimTextListElement* data;
	// max strlen(label) on data
	size_t max_size;

	size_t size;
	size_t capacity;
} TuimTextList;

TuimTextList tuim_text_list(const size_t capacity);

char* tuim_text_list_get (TuimTextList* list, const size_t index);
TuimTextListElement tuim_text_list_get_el (TuimTextList* list, const size_t index);

void tuim_text_list_update(TuimContext* ctx, TuimTextList* list);
void tuim_text_list_draw(TuimContext* ctx, const TuimTextList* list);
void tuim_text_list_destroy(TuimTextList* list);

TuimSizeHint tuim_text_list_measure(TuimTextList* list);
TuimElement tuim_text_list_to_element(const TuimTextList* list);
void tuim_text_list_layout(TuimTextList* list, const TuimRect rect);

void tuim_text_list_add(TuimTextList* list, char* text);
void tuim_text_list_clear(TuimTextList* list);
void tuim_text_list_pop_back(TuimTextList* list);

#endif // TUIM_TEXT_LIST_H