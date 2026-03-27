#include "layout.h"

void tuim_layout_draw(const TuimContext* ctx, const TuimLayout* layout) {
	for (size_t i = 0; i < layout->size; ++i) {
		TuimLayoutElement* element = &layout->elements[i];
		if (!element->data) {
			continue;
		}
		// TODO: implement
	}
}

void tuim_layout_update(const TuimContext* ctx, TuimLayout* layout) {
	int total_fixed = 0;
	float total_flex = 0.0f;

	for (size_t i = 0; i < layout->size; i++) {
		TuimLayoutElement current = layout->elements[i];

		if (current.flex == 0.0f) {
			total_fixed += current.base_size + current.margin_start + current.margin_end;
		}
		else {
			total_flex += current.flex;
		}
	}

	int container_size = (layout->direction == TUIM_ROW) ? layout->bounds.width : layout->bounds.height;
	int total_spacing = (layout->size - 1) * layout->spacing;
	int remaining = container_size - total_fixed - total_spacing;

	if (remaining < 0) 
		remaining = 0;

	int* computed_sizes = malloc(sizeof(int) * layout->size);
	assert(computed_sizes);

	for (size_t i = 0; i < layout->size; i++) {
		TuimLayoutElement current = layout->elements[i];

		if (current.flex > 0.0f && total_flex > 0.0f) {
			computed_sizes[i] = (int)((current.flex / total_flex) * remaining);
		}
		else {
			computed_sizes[i] = current.base_size;
		}
	}

	int cursor = (layout->direction == TUIM_ROW)
		? layout->bounds.x
		: layout->bounds.y;

	for (size_t i = 0; i < layout->size; ++i) {
		TuimLayoutElement* current = &layout->elements[i];
		TuimElement* el = current->data;

		cursor += current->margin_start;

		int main_size = computed_sizes[i];

		TuimRect rect;

		if (layout->direction == TUIM_ROW) {
			rect.x = cursor;
			rect.y = layout->bounds.y;
			rect.width = main_size;
			rect.height = layout->bounds.height;
		}
		else {
			rect.x = layout->bounds.x;
			rect.y = cursor;
			rect.width = layout->bounds.width;
			rect.height = main_size;
		}

		el->layout(el->data, rect);

		cursor += main_size;
		cursor += current->margin_end;

		if (i < layout->size - 1) {
			cursor += layout->spacing;
		}
	}
}

TuimElement* tuim_layout_get(TuimLayout* layout, const size_t index) {
	assert(layout);
	assert(index < layout->size);

	return layout->elements[index].data;
}

void tuim_layout_init(TuimLayout* layout, size_t capacity) {
	assert(layout);
	layout->elements = malloc(sizeof(TuimLayoutElement) * capacity);
	layout->capacity = capacity;
	layout->size = 0;
}

void tuim_layout_destroy(TuimLayout* layout) {
	assert(layout);
	free(layout->elements);
}

void tuim_layout_add(TuimLayout* layout, TuimElement* element) {
	assert(layout && element);

	if (layout->size >= layout->capacity) {
		size_t new_capacity = layout->capacity * 2;
		TuimLayoutElement* new_elements = 
			realloc(layout->elements, sizeof(TuimLayoutElement) * new_capacity);
		
		assert(new_elements);
		
		layout->elements = new_elements;
		layout->capacity = new_capacity;
	}

	layout->elements[layout->size].data = element;
}

void tuim_layout_clear(TuimLayout* layout) {
	assert(layout);
	
	layout->size = 0;
}
