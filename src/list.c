#include "list.h"

// TODO: implement

TuimList tuim_list(const size_t capacity) {
	TuimList list;

	tuim_layout_init(&list.layout, capacity);

	return list;
}

void tuim_list_update(TuimList* ctx, TuimList* list) {
	assert(list);

	tuim_list_update(ctx, &list->layout);
}

void tuim_list_draw(TuimList* ctx, const TuimList* list) {
	assert(ctx && list);

	tuim_layout_draw(ctx, &list->layout);
}

void tuim_list_destroy(TuimList* list) {
	assert(list);
}

TuimSizeHint tuim_list_measure(TuimList* list) {
	assert(list);
}

TuimElement tuim_list_to_element(const TuimList* list) {
	assert(list);

	TuimElement el;
	el.data = list;
	el.update = tuim_list_update;
	el.draw = tuim_list_draw;
	el.destroy = tuim_list_destroy;
	el.layout = tuim_list_layout;
	el.measure = tuim_list_measure;

	return el;
}

void tuim_list_layout(TuimList* list, const TuimRect rect) {
	
}
