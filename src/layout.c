#include "layout.h"

void tuim_layout_draw(const TuimContext* ctx, const TuimLayout* layout) {

}

void tuim_layout_update(const TuimContext* ctx, TuimLayout* layout) {

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
