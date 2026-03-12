#ifndef TUIM_WIDGET_H
#define TUIM_WIDGET_H

#include "rect.h"
#include "frame_buffer.h"
#include "id.h"

#include <assert.h>
#include <stdbool.h>

// representation of a draggable widget
typedef struct {
	TuimRect rect;
	char* title;
	TuimID id;
	uint32_t flags;
} TuimWidget;

void tuim_widget_begin (TuimWidget* widget, const char* title);
void tuim_widget_draw  (TuimWidget* widget, const TuimFrameBuffer* fb);
void tuim_widget_update(TuimWidget* widget, const TuimInputState* state);
void tuim_widget_end   (TuimWidget* widget);

#endif // TUIM_WIDGET_H