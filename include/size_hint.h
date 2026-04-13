#ifndef TUIM_SIZE_HINT_H
#define TUIM_SIZE_HINT_H

#include "rect.h"

#include <limits.h>
#include <stdio.h>

#define TUIM_SIZE_HINT_MAX INT_MAX

// values are on terminal cells
typedef struct {
	size_t min_width;
	size_t min_height;

	size_t preferred_width;
	size_t preferred_height;

	size_t max_width;
	size_t max_height;

} TuimSizeHint;

TuimSizeHint tuim_size_hint_dynamic (const TuimRect rect);
TuimSizeHint tuim_size_hint_static  (const TuimRect rect);

#endif // TUIM_SIZE_HINT_H