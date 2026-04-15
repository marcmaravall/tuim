#include "list.h"

// TODO: implement

TuimTextList tuim_text_list(const size_t capacity) {
	TuimTextList list;

	list.size = 1;
	list.capacity = capacity;
	list.data = malloc(capacity);

	return list;
}

TuimTextListElement tuim_text_list_get(TuimTextList* list, const size_t index) {
	assert(el);
	if (index > list->size) {
		assert(0 && "ERROR: invalid index on text list!");
	}
	return list->data[index];
}

void tuim_text_list_update(TuimTextList* ctx, TuimTextList* list) {
	assert(list);
}

void tuim_text_list_draw(TuimTextList* ctx, const TuimTextList* list) {
	assert(ctx && list);

}

void tuim_text_list_destroy(TuimTextList* list) {
	assert(list);

	for (size_t i = 0; i < list->size; i++) {
		if (list->data[i].on_heap)
			free(list->data[i].label);
	}

	free(list->data);
}

TuimSizeHint tuim_text_list_measure(TuimTextList* list) {
	assert(list);
}

TuimElement tuim_text_list_to_element(const TuimTextList* list) {
	assert(list);

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
	
}
