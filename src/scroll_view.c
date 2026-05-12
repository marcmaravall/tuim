#include "scroll_view.h"

TuimScrollView tuim_scroll_view(const TuimRect rect) {
	TuimScrollView sw;
	
	sw.viewport = tuim_viewport(rect);
	sw.scroll_x = 0;
	sw.scroll_y = 0;

	tuim_layout_init(&sw.layout, 4);

	return sw;
}

void tuim_scroll_view_update(const TuimContext* ctx, TuimScrollView* sw) {
	MEB_ASSERT(ctx && sw);

	// nothing...
}

void tuim_scroll_view_draw(TuimContext* ctx, TuimScrollView* sw) {
	MEB_ASSERT(ctx && sw);

	TuimViewport last = ctx->viewport;
	ctx->viewport = sw->viewport;

	sw->layout.bounds.x -= sw->scroll_x;
	sw->layout.bounds.y -= sw->scroll_y;

	tuim_layout_draw(ctx, &sw->layout);

	sw->layout.bounds.x += sw->scroll_x;
	sw->layout.bounds.y += sw->scroll_y;

	ctx->viewport = last;
	tuim_viewport_draw(ctx, &sw->viewport);
}

void tuim_scroll_view_destroy(TuimScrollView* sw) {
	MEB_ASSERT(sw);

	tuim_viewport_destroy(&sw->viewport);
	tuim_layout_destroy(&sw->layout);
}

TuimElement tuim_scroll_view_to_element(TuimScrollView* sw) {
	MEB_ASSERT(sw);

	TuimElement el;
	el.data = sw;
	el.draw = (TuimElementDrawFn)tuim_scroll_view_draw;
	el.update = (TuimElementUpdateFn)tuim_scroll_view_update;
	el.destroy = (TuimElementDestroyFn)tuim_scroll_view_destroy;
	// TODO: measure and layout functions

	return el;
}
