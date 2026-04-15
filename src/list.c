#include "list.h"

// TODO: implement

TuimTextList tuim_text_list(const size_t capacity) {
	TuimTextList list;

	list.size = 1;
	list.capacity = capacity;
	list.data = malloc(capacity);

	return list;
}

char* tuim_text_list_get(TuimTextList* list, const size_t index) {
	assert(list);
	if (index >= list->size)
		assert(0 && "ERROR: invalid index on text list");
	
	char* res = list->data[index].label;
	assert(res);
	return res;
}

TuimTextListElement tuim_text_list_get_el (TuimTextList* list, const size_t index) {
	assert(list);
	if (index >= list->size) {
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
		if (list->data[i].free_on_delete)
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

void tuim_text_list_add(TuimTextList* list, char* text) {
	assert(list && text);
	
	list->size++;
	if (list->size > list->capacity) {
		list->capacity *= 2;
		list->data = realloc(list->data, sizeof(TuimTextListElement) * list->capacity);
		
		assert(list->data);
	}

	list->data[list->size - 1].label = text;
	list->data[list->size - 1].free_on_delete = false;
}

void tuim_text_list_clear(TuimTextList* list) {
	assert(list);

	list->size = 0;
}

void tuim_text_list_pop_back(TuimTextList* list) {
	assert(list);

	if (list->size > 0)
		list->size--;
}
