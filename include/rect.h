#ifndef TUIM_RECT_H
#define TUIM_RECT_H

#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

#include "context.h"

typedef struct {
	int x;
	int y;
	int width;
	int height;
} TuimRect;

#define TUIM_RECT_AREA(x) (x.width*x.height)

// TODO: test behaviour

bool tuim_rect_is_valid(TuimRect rect);

bool tuim_rect_contains(const TuimRect* rect, int px, int py);
bool tuim_rect_overlaps(const TuimRect* r1, const TuimRect* r2);
bool tuim_rect_overlaps_p(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);

TuimRect tuim_rect_lerp(TuimRect r1, TuimRect r2, float t);

#endif //TUIM_RECT_H
