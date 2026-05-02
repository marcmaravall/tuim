#include "element.h"

TuimElement tuim_element(
	void* data,
	TuimElementMeasureFn measure,
	TuimElementLayoutFn layout,
	TuimElementUpdateFn update,
	TuimElementDrawFn draw,
	TuimElementDestroyFn destroy,
	TuimElementOnFocusGainedFn on_focus_gained,
	TuimElementOnFocusLostFn on_focus_lost
) {
	TuimElement el;
	
	el.data = data;
	el.measure = measure;
	el.layout = layout;
	el.update = update;
	el.draw = draw;
	el.destroy = destroy;

	el.on_focus_gained	= on_focus_gained;
	el.on_focus_lost	= on_focus_lost;
	
	return el;
}

TuimElement tuim_element_null() {
	TuimElement el;
	
	el.data = NULL;
	el.measure = NULL;
	el.layout = NULL;
	el.update = NULL;
	el.draw = NULL;
	el.destroy = NULL;
	el.on_focus_gained	= NULL;
	el.on_focus_lost	= NULL;
	
	return el;
}

bool tuim_element_is_valid(const TuimElement* el) {
	MEB_ASSERT(el);

	if (el->destroy && el->draw && el->layout && el->measure && el->update)
		return true;
	return false;
}

bool tuim_element_is_null(const TuimElement* el) {
	MEB_ASSERT(el);
	
	return	el->data == NULL && 
			el->destroy == NULL && 
			el->draw == NULL && 
			el->layout == NULL && 
			el->measure == NULL && 
			el->update == NULL &&
			el->on_focus_gained == NULL &&
			el->on_focus_lost	== NULL;
}
