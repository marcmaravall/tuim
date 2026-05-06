#include "scroll_view.h"

TuimElement tuim_scroll_view_to_element(TuimScrollView* sw) {
	MEB_ASSERT(sw);

	TuimElement el;
	el.data = sw;
	el.destroy = tuim_scroll_view_destroy;
	el.draw = tuim_layout_draw;
	el.layout = tuim_scroll_view_draw;

	return el;
}
