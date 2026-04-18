#include "text_list.h"

TuimTextListStyle tuim_text_list_default_style() {
	TuimTextListStyle style;

	style.foreground = TUIM_LIST_STYLE_DEFAULT_FG;
	style.background = TUIM_LIST_STYLE_DEFAULT_BG;

	return style;
}

// TODO: implement
// TODO: test behaviour

TuimTextList tuim_text_list(const size_t capacity) {
	TuimTextList list;

	list.size = 0;
	list.capacity = capacity;
	list.data = malloc(sizeof(TuimTextListElement)*capacity);
	
	list.area.x = 0;
	list.area.y = 0;
	list.max_size = 0;

	list.style = tuim_text_list_default_style();

	return list;
}

char* tuim_text_list_get(TuimTextList* list, const size_t index) {
	MEB_ASSERT(list);
	if (index >= list->size)
		MEB_ASSERT(0 && "ERROR: invalid index on text list");
	
	char* res = list->data[index].label;
	MEB_ASSERT(res);
	return res;
}

TuimTextListElement tuim_text_list_get_el (TuimTextList* list, const size_t index) {
	MEB_ASSERT(list);
	if (index >= list->size) {
		MEB_ASSERT(0 && "ERROR: invalid index on text list!");
	}
	return list->data[index];
}

void tuim_text_list_update(TuimContext* ctx, TuimTextList* list) {
	MEB_ASSERT(list);

	list->area.height = list->size;
	list->area.width = list->max_size;
}

void tuim_text_list_draw(TuimContext* ctx, const TuimTextList* list) {
	MEB_ASSERT(ctx && list);

	// renders vertical strings for simplicity on early development:
	// TODO: continue implementing this

	const size_t x = list->area.x;
	const size_t start_y = list->area.y;
	const size_t end_y = start_y + list->area.height;

	for (size_t i = start_y; i < end_y; i++) {
		TuimTextListElement el = list->data[i-start_y];
		tuim_frame_buffer_print (
			&ctx->frame_buffer, list->style.foreground, 
			list->style.background, el.label, x, i
		);
	}
}

void tuim_text_list_destroy(TuimTextList* list) {
	MEB_ASSERT(list);

	for (size_t i = 0; i < list->size; i++) {
		if (list->data[i].free_on_delete)
			free(list->data[i].label);
	}

	free(list->data);
}

TuimSizeHint tuim_text_list_measure(TuimTextList* list) {
	MEB_ASSERT(list);

	TuimSizeHint sh;

	sh = tuim_size_hint_dynamic(list->area);

	return sh;
}

TuimElement tuim_text_list_to_element(const TuimTextList* list) {
	MEB_ASSERT(list);

	TuimElement el;
	el.data = list;
	el.update = tuim_text_list_update;
	el.draw = tuim_text_list_draw;
	el.destroy = tuim_text_list_destroy;
	el.layout = tuim_text_list_layout;
	el.measure = tuim_text_list_measure;

	return el;
}

void tuim_text_list_layout(TuimTextList* list, const TuimRect rect) {
	MEB_ASSERT(list);

	list->area = rect;
}

void tuim_text_list_add(TuimTextList* list, char* text) {
	MEB_ASSERT(list && text);
	
	list->size++;
	if (list->size >= list->capacity) {
		list->capacity *= 2;
		list->data = realloc(list->data, sizeof(TuimTextListElement) * list->capacity);
		
		MEB_ASSERT(list->data);
	}

	size_t len = strlen(text);
	if (list->max_size < len)
		list->max_size = len;

	list->data[list->size - 1].label = text;
	list->data[list->size - 1].free_on_delete = false;
}

void tuim_text_list_clear(TuimTextList* list) {
	MEB_ASSERT(list);

	list->size = 0;
}

void tuim_text_list_pop_back(TuimTextList* list) {
	MEB_ASSERT(list);

	if (list->size > 0)
		list->size--;
}
