#include <rect.h>
#include "context.h"

bool tuim_rect_contains(const TuimRect* rect, int px, int py) {
	assert(rect != NULL);
	if (px <= rect->x || px >= rect->x + rect->width)
		return false;
	if (py <= rect->y || py >= rect->y + rect->height)
		return false;
	return true;
}

bool tuim_rect_overlaps(const TuimRect* r1, const TuimRect* r2) {
	assert(r1 != NULL);
	assert(r2 != NULL);

	if (r1->x + r1->width <= r2->x) return false;
	if (r2->x + r2->width <= r1->x) return false;

	if (r1->y + r1->height <= r2->y) return false;
	if (r2->y + r2->height <= r1->y) return false;

	return true;
}

TuimRect tuim_rect_lerp(TuimRect r1, TuimRect r2, float t) {
	TuimRect rect;

	rect.x = r1.x + (r2.x - r1.x) * t;
	rect.y = r1.y + (r2.y - r1.y) * t;

	rect.width = r1.width + (r2.width - r1.width) * t;
	rect.height = r1.height + (r2.height - r1.height) * t;

	return rect;
}

bool tuim_rect_overlaps_p(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {
	if (x1 + w1 <= x2) return false;
	if (x2 + w2 <= x1) return false;
	if (y1 + h1 <= y2) return false;
	if (y2 + h2 <= y1) return false;
	return true;
}

bool tuim_rect_is_valid(TuimRect rect) {
	if (rect.x < 0 || rect.y < 0 || rect.height <= 0 || rect.width <= 0)
		return false;
	return true;
}

void tuim_rect_draw(const TuimContext* ctx, const TuimRect rect) {
	tuim_frame_buffer_draw_rect (
		&ctx->style, &ctx->frame_buffer, 
		rect.x, rect.y, rect.width, rect.height
	);
}
