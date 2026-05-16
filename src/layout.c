#include "layout.h"

void tuim_layout_draw(TuimContext* ctx, const TuimLayout* layout) {
	MEB_ASSERT(ctx && layout && "tuim_layout_draw: invalid arguments!");
	for (size_t i = 0; i < layout->size; ++i) {
		TuimLayoutElement* element = &layout->elements[i];
		if (element->data.draw)
			element->data.draw(ctx, element->data.data);
	}
}

// TODO: refactor this monstruosity
void tuim_layout_update(TuimContext* ctx, TuimLayout* layout) {
    MEB_ASSERT(ctx && layout);

    // calculate total flex and fixed
    int   total_fixed = 0;
    float total_flex = 0.0f;

    for (size_t i = 0; i < layout->size; i++) {
        TuimLayoutElement current = layout->elements[i];

        if (current.flex == 0.0f) {
            int effective_size = current.base_size;

            if (effective_size == 0) {
                if (current.data.measure == NULL)
                    continue;

                TuimSizeHint hint = current.data.measure(current.data.data);
                effective_size = (layout->direction == TUIM_ROW)
                    ? (int)hint.preferred_width
                    : (int)hint.preferred_height;
            }

            total_fixed += effective_size + current.margin_start + current.margin_end;
        }
        else {
            total_flex += current.flex;
        }
    }

    // space not used

    int container_size = (layout->direction == TUIM_ROW)
        ? layout->bounds.width : layout->bounds.height;
    int total_spacing = ((int)layout->size - 1) * layout->spacing;
    int remaining = container_size - total_fixed - total_spacing;

    if (remaining < 0)
        remaining = 0;

    // justify: 

    int cursor_offset = 0;
    int dynamic_spacing = layout->spacing;

    if (total_flex == 0.0f) {
        switch (layout->justify) {
        case TUIM_CENTER_JUSTIFY:
            cursor_offset = remaining / 2;
            break;

        case TUIM_END_JUSTIFY:
            cursor_offset = remaining;
            break;

        case TUIM_SPACE_BETWEEN:
            if (layout->size > 1)
                dynamic_spacing = layout->spacing
                + remaining / (int)(layout->size - 1);
            break;

        case TUIM_SPACE_AROUND:
            if (layout->size > 0) {
                int per = remaining / (int)layout->size;
                dynamic_spacing = layout->spacing + per;
                cursor_offset = per / 2;
            }
            break;

        default:
            break;
        }
    }

    // calculate sizes

    int* computed_sizes = malloc(sizeof(int) * layout->size);
    MEB_ASSERT(computed_sizes);

    for (size_t i = 0; i < layout->size; i++) {
        TuimLayoutElement current = layout->elements[i];

        if (current.flex > 0.0f && total_flex > 0.0f) {
            computed_sizes[i] = (int)((current.flex / total_flex) * remaining);
        }
        else if (current.base_size == 0) {
            if (!current.data.measure) {
                computed_sizes[i] = 0;
            }
            else {
                TuimSizeHint hint = current.data.measure(current.data.data);
                computed_sizes[i] = (layout->direction == TUIM_ROW)
                    ? (int)hint.preferred_width
                    : (int)hint.preferred_height;
            }
        }
        else {
            computed_sizes[i] = current.base_size;
        }
    }

    // size and position
    int cursor = (layout->direction == TUIM_ROW)
        ? layout->bounds.x
        : layout->bounds.y;

    cursor += cursor_offset;

    int cross_origin = (layout->direction == TUIM_ROW)
        ? layout->bounds.y : layout->bounds.x;
    int cross_total = (layout->direction == TUIM_ROW)
        ? layout->bounds.height : layout->bounds.width;

    for (size_t i = 0; i < layout->size; ++i) {
        TuimLayoutElement* current = &layout->elements[i];
        TuimElement el = current->data;

        cursor += current->margin_start;

        int main_size = computed_sizes[i];

        int cross_size = cross_total;

        if (!current->expand && el.measure) {
            TuimSizeHint hint = el.measure(el.data);
            int natural = (layout->direction == TUIM_ROW)
                ? (int)hint.preferred_height
                : (int)hint.preferred_width;
            cross_size = (natural < cross_total) ? natural : cross_total;
        }

        TuimAlign cross_align = (layout->direction == TUIM_ROW)
            ? current->v_align
            : current->h_align;

        int cross_pos;
        switch (cross_align) {
        case TUIM_CENTER_ALIGN:
            cross_pos = cross_origin + (cross_total - cross_size) / 2;
            break;
        case TUIM_RIGHT_ALIGN:
            cross_pos = cross_origin + cross_total - cross_size;
            break;
        default:
            cross_pos = cross_origin;
            break;
        }

        // build rect

        TuimRect rect;

        if (layout->direction == TUIM_ROW) {
            rect.x = cursor;
            rect.y = cross_pos;
            rect.width = main_size;
            rect.height = cross_size;
        }
        else {
            rect.x = cross_pos;
            rect.y = cursor;
            rect.width = cross_size;
            rect.height = main_size;
        }

        if (el.layout)
            el.layout(el.data, rect);

        cursor += main_size;
        cursor += current->margin_end;

        if (i < layout->size - 1)
            cursor += dynamic_spacing;

        if (el.update)
            el.update(ctx, el.data);
    }

    free(computed_sizes);
}

TuimElement tuim_layout_get(TuimLayout* layout, const size_t index) {
	MEB_ASSERT(layout);
	MEB_ASSERT(index < layout->size);

	return layout->elements[index].data;
}

void tuim_layout_init(TuimLayout* layout, size_t capacity) {
	MEB_ASSERT(layout);

	layout->elements = malloc(sizeof(TuimLayoutElement) * capacity);
	MEB_ASSERT(layout->elements && "tuim_layout_init: Failed to allocate memory for layout elements!");

	if (capacity <= 0) {
		MEB_LOG_WARNING("tuim_layout_init: tried to initialize capacity with 0.");
		layout->capacity = 1;
	}
	else 
		layout->capacity = capacity;

	layout->size = 0;
	
	layout->bounds.x = TUIM_LAYOUT_DEFAULT_BOUNDS_X;
	layout->bounds.y = TUIM_LAYOUT_DEFAULT_BOUNDS_Y;
	layout->bounds.width = TUIM_LAYOUT_DEFAULT_BOUNDS_WIDTH;
	layout->bounds.height = TUIM_LAYOUT_DEFAULT_BOUNDS_HEIGHT;
	
	layout->direction = TUIM_LAYOUT_DEFAULT_DIRECTION;
	layout->spacing = TUIM_LAYOUT_DEFAULT_SPACING;
    layout->align_items = TUIM_CENTER_ALIGN;
}

// i dont know if works, i havent tested it
void tuim_layout_destroy(TuimLayout* layout) {
	MEB_ASSERT(layout);

	for (size_t i = 0; i < layout->size; i++) {
		TuimElement el = layout->elements[i].data;
		el.destroy(el.data);
	}
	free(layout->elements);
}

void tuim_layout_add (TuimLayout* layout, TuimElement element) {
	MEB_ASSERT(layout && layout->elements);

	if (layout->size >= layout->capacity) {
		size_t new_capacity = layout->capacity ? layout->capacity * 2 : 1;
		
		TuimLayoutElement* new_elements =
			realloc(layout->elements, sizeof(TuimLayoutElement) * new_capacity);

		MEB_ASSERT(new_elements);
		layout->elements = new_elements;
		layout->capacity = new_capacity;
	}

	layout->elements[layout->size].data = element;
	layout->elements[layout->size].flex = 0.0f;
	layout->elements[layout->size].base_size = 0;
	layout->elements[layout->size].margin_start = 0;
	layout->elements[layout->size].margin_end = 0;

	layout->size++;
}

void tuim_layout_add_elements(TuimLayout* layout, size_t count, TuimElement elements[]) {
	MEB_ASSERT(layout && elements && "tuim_layout_add_elements: invalid arguments!");
	for (size_t i = 0; i < count; ++i) {
		tuim_layout_add(layout, elements[i]);
	}
}

void tuim_layout_clear(TuimLayout* layout) {
	MEB_ASSERT(layout);

	for (size_t i = 0; i < layout->size; i++) {
		TuimElement el = layout->elements[i].data;
		el.destroy(el.data);
	}

	layout->size = 0;
}

// ---------------
// TODO: test behaviour

TuimElement tuim_layout_to_element(TuimLayout* layout) {
	MEB_ASSERT(layout);

	TuimElement el;
	el.data = layout;
	el.destroy	= (TuimElementDestroyFn)tuim_layout_destroy;
	el.draw		= (TuimElementDrawFn)tuim_layout_draw;
	el.update	= (TuimElementUpdateFn)tuim_layout_update;
	el.measure	= (TuimElementMeasureFn)tuim_layout_measure;
	el.layout	= (TuimElementLayoutFn)tuim_layout_layout;

	el.on_focus_gained	= NULL;
	el.on_focus_lost	= NULL;

	return el;
}

TuimSizeHint tuim_layout_measure(const TuimLayout* layout) {
	MEB_ASSERT(layout);
	
	TuimSizeHint sh;

	sh.max_width = INT_MAX;
	sh.max_height = INT_MAX;
	sh.preferred_width = layout->bounds.width;
	sh.preferred_height = layout->bounds.height;
	sh.min_width = 0;
	sh.min_height = 0;

	return sh;
}

void tuim_layout_layout(TuimLayout* layout, const TuimRect area) {
	MEB_ASSERT(layout);

	layout->bounds = area;
}

// ---------------

TuimElement tuim_layout_add_text(TuimLayout* layout, char* str, TuimText* text) {
	MEB_ASSERT(layout && str && text && "tuim_layout_add_text: invalid arguments!");
	
	text = tuim_text(str);
	TuimElement el = tuim_text_to_element(text);
	tuim_layout_add(layout, el);

	return el;
}

TuimElement tuim_layout_add_button(TuimLayout* layout, const char* str, TuimButton* button) {
	MEB_ASSERT(layout && str && button && "tuim_layout_add_button: invalid arguments!");

	button = tuim_button(str);
	TuimElement el = tuim_button_to_element(button);
	tuim_layout_add(layout, el);
	return el;
}

TuimElement tuim_layout_add_checkbox(TuimLayout* layout, const char* str, TuimCheckbox* checkbox) {
	MEB_ASSERT(layout && str && checkbox && "tuim_layout_add_checkbox: invalid arguments!");
	
	checkbox = tuim_checkbox(str);
	TuimElement el = tuim_checkbox_to_element(checkbox);
	tuim_layout_add(layout, el);
	return el;
}
