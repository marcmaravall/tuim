#ifndef TUIM_WINDOW_MANAGER_H
#define TUIM_WINDOW_MANAGER_H

#include "context.h"
#include "window.h"

typedef struct {
	TuimWindow** data;
	TuimWindow* focused;

	size_t size;
	size_t capacity;

} TuimWindowManager;

int tuim_window_manager_resize(TuimWindowManager* manager, const size_t n);
int tuim_window_manager_reserve(TuimWindowManager* manager, const size_t n);

int tuim_window_manager_init(TuimWindowManager* manager, const size_t capacity);
int tuim_window_manager_add	(TuimWindowManager* manager, TuimWindow* window);
int tuim_window_manager_remove(TuimWindowManager* manager, const size_t index);

int tuim_window_manager_on_focus(TuimWindowManager* manager, size_t index);

TuimWindow* tuim_window_manager_get(const TuimWindowManager* wm, const size_t index);

int tuim_window_manager_draw  (TuimContext* ctx, TuimWindowManager* manager);
int tuim_window_manager_update(TuimContext* ctx, TuimWindowManager* manager);

#endif // TUIM_WINDOW_MANAGER_H