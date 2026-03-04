#include <rect.h>

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

// TODO: implement with framebuffer backend
void tuim_draw_rect(TuimContext* ctx, TuimRect rect) {
	assert(ctx != NULL);
	assert(tuim_rect_is_valid(rect));

	const int start_x = rect.x;
	const int start_y = rect.y;
	const int end_x = rect.x + rect.width;
	const int end_y = rect.y + rect.height;

	// ctx->backend.set_cursor_pos(ctx->backend.data, start_x, start_y);

	char* mem_buff = malloc(sizeof(char)*(rect.width+1));
	assert(mem_buff != NULL);
	if (!memset(mem_buff, ' ', rect.width)) {
		free(mem_buff);
		assert(0);
		return;
	}
	mem_buff[rect.width] = 0x00;

	// ctx->backend.set_background_color(ctx->backend.data, ctx->style.rect_background_color);

	for (int i = start_y; i < end_y; i++) {
		// ctx->backend.set_cursor_pos(ctx->backend.data, start_x, i);
		// ctx->backend.render_text(ctx->backend.data, mem_buff);
		// ctx->backend.render_text(ctx->backend.data, "\n");
	}

	// ctx->backend.set_background_color(ctx->backend.data, ctx->style.background_color);
}

bool tuim_rect_is_valid(TuimRect rect) {
	if (rect.x < 0 || rect.y < 0 || rect.height <= 0 || rect.width <= 0)
		return false;
	return true;
}
