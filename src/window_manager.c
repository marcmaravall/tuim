#include "window_manager.h"

int tuim_window_manager_init(TuimWindowManager* manager, const size_t capacity) {
	assert(manager);
	
	manager->data = (TuimWindow**)malloc(sizeof(TuimWindow*) * capacity);
	
	if (!manager->data) {
		return -1;
	}

	manager->size = 0;
	manager->capacity = capacity;

	return 0;
}

int tuim_window_manager_add(TuimWindowManager* manager, const TuimWindow* window) {
	assert(manager && window);

	if (manager->size >= manager->capacity) {
		assert(0 && "Window manager capacity exceeded!");
		return -1;
	}

	return 0;
}

int tuim_window_manager_draw(const TuimContext* ctx, const TuimWindowManager* manager) {
	assert(ctx && manager);
	
	

	return 0;
}

TuimWindow* tuim_window_manager_get(const TuimWindowManager* wm, const size_t index) {
	assert(wm);

	if (index >= wm->size) {
		return NULL;
	}

	return wm->data[index];
}

int tuim_window_manager_update(TuimWindowManager* manager) {
	assert(manager);
	
	return 0;
}
