#include "window_manager.h"

// TODO: check for memory leaks and UB

int tuim_window_manager_init(TuimWindowManager* manager, const size_t capacity) {
	assert(manager);
	
	manager->data = (TuimWindow**)calloc(capacity, sizeof(TuimWindow*));
	
	if (!manager->data) {
		return -1;
	}

	manager->size = 0;
	manager->capacity = capacity;
	manager->focused = NULL;

	return 0;
}

int tuim_window_manager_resize(TuimWindowManager* manager, const size_t n) {
	assert(manager);
	
	manager->size = n;
	if (manager->size > manager->capacity) {
		return tuim_window_manager_reserve(manager, manager->size - manager->capacity);
	}
	return 0;
}

int tuim_window_manager_reserve(TuimWindowManager* manager, const size_t n) {
	assert(manager);
	
	manager->capacity += n;
	manager->data = (TuimWindow**)realloc(manager->data, sizeof(TuimWindow*) * manager->capacity);
	
	if (!manager->data) {
		return -1;
	}

	return 0;
}

int tuim_window_manager_add(TuimWindowManager* manager, TuimWindow* window) {
	assert(manager && window);

	if (manager->size >= manager->capacity) {
		size_t newCapacity = manager->capacity ? manager->capacity * 2 : 1;

		if (newCapacity > SIZE_MAX / sizeof(TuimWindow*))
			return -1;

		TuimWindow** newData = realloc(manager->data, sizeof(TuimWindow*) * newCapacity);
		if (!newData)
			return -1;

		manager->data = newData;
		manager->capacity = newCapacity;
	}

	manager->data[manager->size++] = window;
	return 0;
}

int tuim_window_manager_remove(TuimWindowManager* manager, const size_t index) {
	assert(manager);
	if (index >= manager->size) {
		return -1;
	}

	// TODO: implement
}

// UPDATE AND DRAW FUNCTIONS ------------

int tuim_window_manager_draw(const TuimContext* ctx, const TuimWindowManager* manager) {
	assert(ctx && manager);
	
	for (size_t i = 0; i < manager->size; i++) {
		TuimWindow* window = manager->data[i];
		assert(window);

		if (window == manager->focused)
			continue;

		tuim_window_draw(ctx, window);
	}

	if (manager->focused)
		tuim_window_draw(ctx, manager->focused);
	
	return 0;
}

int tuim_window_manager_update(TuimContext* ctx, TuimWindowManager* manager) {
	assert(manager);

	if (manager->focused) {
		if (tuim_window_update(ctx, manager->focused) != TUIM_WINDOW_UPDATE_NONE) {
			return 0;
		}
	}

	for (size_t i = 0; i < manager->size; i++) {
		TuimWindow* window = manager->data[i];
		assert(window);
	
		if (tuim_window_update(ctx, window) != TUIM_WINDOW_UPDATE_NONE) {
			manager->focused = window;
		}
	}
	return 0;
}

TuimWindow* tuim_window_manager_get(const TuimWindowManager* wm, const size_t index) {
	assert(wm);

	if (index >= wm->size) {
		return NULL;
	}

	return wm->data[index];
}
