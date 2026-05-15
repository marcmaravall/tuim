#ifndef TUIM_VIEWPORT_H
#define TUIM_VIEWPORT_H

#include <meb.h>

#include "element.h"
#include "frame_buffer.h"
#include "rect.h"

typedef struct TuimContext TuimContext;

typedef struct TuimViewport {
	TuimFrameBuffer frame_buffer;
	int x;
	int y;
} TuimViewport;

TuimViewport tuim_viewport(const TuimRect);

void tuim_viewport_draw(TuimContext* ctx, const TuimViewport* vp);
void tuim_viewport_update(TuimContext* ctx, TuimViewport* vp);
void tuim_viewport_destroy(TuimViewport* vp);

TuimSizeHint tuim_viewport_measure (const TuimViewport* vp);
void tuim_viewport_layout		   (TuimViewport* vp, const TuimRect rect);

TuimElement tuim_viewport_to_element(const TuimViewport* vp);

void tuim_viewport_clear(TuimViewport* vp, TuimColor color);

#endif // TUIM_VIEWPORT_H