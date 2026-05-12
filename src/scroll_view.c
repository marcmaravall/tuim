#include "scroll_view.h"

TuimScrollView tuim_scroll_view(const TuimRect rect) {
	TuimScrollView sw;
	sw.viewport = tuim_viewport(rect);
	tuim_layout_init(&sw.layout, 4);
}


void tuim_scroll_view_update(const TuimContext* ctx, TuimScrollView* sw) {
	MEB_ASSERT(ctx && sw);

	// nothing...
}

void tuim_scroll_view_draw(TuimContext* ctx, TuimScrollView* sw) {
	MEB_ASSERT(ctx && sw);
	tuim_viewport_draw(ctx, &sw->viewport);
}

void tuim_scroll_view_destroy(TuimScrollView* sw) {
	MEB_ASSERT(sw);

	tuim_viewport_destroy(&sw->viewport);
	tuim_layout_destroy(&sw->layout);
}

// TODO: (TODO: add mmessage for todo)
TuimElement tuim_scroll_view_to_element(TuimScrollView* sw) {
	MEB_ASSERT(sw);

	TuimElement el;
	el.data = sw;

	return el;
}
