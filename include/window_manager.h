#ifndef TUIM_WINDOW_MANAGER_H
#define TUIM_WINDOW_MANAGER_H

#include "context.h"
#include "window.h"

typedef struct {
	TuimWindow** data;
	size_t size;
} TuimWindowManager;

int tuim_window_manager_draw  (const TuimContext* ctx, const TuimWindowManager* manager);
int tuim_window_manager_update(TuimWindowManager* manager);

#endif // TUIM_WINDOW_MANAGER_H