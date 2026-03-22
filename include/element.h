#ifndef TUIM_ELEMENT_H
#define TUIM_ELEMENT_H

#include "rect.h"
#include "size_hint.h"

typedef struct {
	void* data;
	TuimSizeHint (*measure)(void* data);
	void (*layout)(void* data, const TuimRect rect);
} TuimElement;

#endif // TUIM_ELEMENT_H