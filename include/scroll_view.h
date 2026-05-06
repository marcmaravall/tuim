#ifndef TUIM_SCROLL_VIEW_H
#define TUIM_SCROLL_VIEW_H

// TODO: implement

#include "element.h"
#include "size_hint.h"
#include "layout.h"

#include <meb.h>

typedef struct {
	TuimLayout layout;
} TuimScrollView;

void tuim_scroll_view_destroy(TuimScrollView* sw);

TuimElement tuim_scroll_view_to_element(TuimScrollView* sw);

#endif // TUIM_SCROLL_VIEW_H