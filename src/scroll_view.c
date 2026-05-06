#include "scroll_view.h"

// TODO: (TODO: add mmessage for todo)
TuimElement tuim_scroll_view_to_element(TuimScrollView* sw) {
	MEB_ASSERT(sw);

	TuimElement el;
	el.data = sw;

	return el;
}
