#ifndef TUIM_RECT_H
#define TUIM_RECT_H

#include <assert.h>
#include <context.h>
#include <stdbool.h>

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

TuimRect tuim_rect_lerp(TuimRect r1, TuimRect r2, float t);

void tuim_draw_rect(TuimContext* ctx, TuimRect rect);

#endif //TUIM_RECT_H