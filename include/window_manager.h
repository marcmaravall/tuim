#ifndef TUIM_WINDOW_MANAGER_H
#define TUIM_WINDOW_MANAGER_H

#include "context.h"
#include "window.h"

typedef struct {
	TuimWindow** data;
	size_t size;
	size_t capacity;
} TuimWindowManager;

int tuim_window_manager_init(TuimWindowManager* manager, const size_t capacity);
int tuim_window_manager_add	(TuimWindowManager* manager, const TuimWindow* window);

TuimWindow* tuim_window_manager_get(const TuimWindowManager* wm, const size_t index);

int tuim_window_manager_draw  (const TuimContext* ctx, const TuimWindowManager* manager);
int tuim_window_manager_update(TuimWindowManager* manager);

#endif // TUIM_WINDOW_MANAGER_H