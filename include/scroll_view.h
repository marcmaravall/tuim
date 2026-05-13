#ifndef TUIM_SCROLL_VIEW_H
#define TUIM_SCROLL_VIEW_H

// TODO: implement

#include "element.h"
#include "viewport.h"
#include "size_hint.h"
#include "layout.h"

#include <meb.h>

typedef struct {
	TuimViewport viewport;
	TuimLayout	  layout;

	int scroll_x;
	int scroll_y;

	size_t content_width;
	size_t content_height;
} TuimScrollView;

TuimScrollView tuim_scroll_view(const TuimRect);

void tuim_scroll_view_update  (const TuimContext* ctx, TuimScrollView* sw);
void tuim_scroll_view_draw	  (TuimContext* ctx, TuimScrollView* sw);
void tuim_scroll_view_destroy (TuimScrollView* sw);

TuimElement tuim_scroll_view_to_element(TuimScrollView* sw);

#endif // TUIM_SCROLL_VIEW_H