#ifndef TUIM_SIZE_HINT_H
#define TUIM_SIZE_HINT_H

#include <stdio.h>

typedef struct {
	size_t min_width;
	size_t min_height;

	size_t preferred_width;
	size_t preferred_height;

	size_t max_width;
	size_t max_height;

} TuimSizeHint;

#endif // TUIM_SIZE_HINT_H