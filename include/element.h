#ifndef TUIM_ELEMENT_H
#define TUIM_ELEMENT_H

#include <stdbool.h>

#include "rect.h"
#include "size_hint.h"

typedef TuimSizeHint(*TuimElementMeasureFn)(void* data);
typedef void (*TuimElementLayoutFn) (void* data, const TuimRect area);
typedef void (*TuimElementUpdateFn) (TuimContext* ctx, void* data);
typedef void (*TuimElementDrawFn)   (TuimContext* ctx, void* data);
typedef void (*TuimElementDestroyFn)(void* data);

typedef struct {
	void* data;
	TuimElementMeasureFn measure;
	TuimElementLayoutFn layout;

	TuimElementUpdateFn update;
	TuimElementDrawFn draw;
	TuimElementDestroyFn destroy;

} TuimElement;

// helpers:

TuimElement tuim_element (
	void* data, 
	TuimElementMeasureFn measure, 
	TuimElementLayoutFn layout, 
	TuimElementUpdateFn update, 
	TuimElementDrawFn draw, 
	TuimElementDestroyFn destroy
);

TuimElement tuim_element_null();

// only if all callbacks != NULL
bool tuim_element_is_valid (const TuimElement* el);

// only if everything is NULL
bool tuim_element_is_null  (const TuimElement* el);

#endif // TUIM_ELEMENT_H