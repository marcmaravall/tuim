#include "viewport.h"
#include "context.h"

TuimViewport tuim_viewport(const TuimRect rect) {
	TuimViewport vp;

	vp.x = rect.x;
	vp.y = rect.y;

	tuim_frame_buffer_init(&vp.frame_buffer, rect.width, rect.height);
	
	return vp;
}

void tuim_viewport_update(TuimContext* ctx, TuimViewport* vp) {
	MEB_ASSERT(ctx && vp);
}

void tuim_viewport_draw(TuimContext* ctx, const TuimViewport* vp) {
	MEB_ASSERT(ctx && vp);
	tuim_frame_buffer_draw_frame_buffer(&ctx->viewport.frame_buffer, (TuimFrameBuffer*)&vp->frame_buffer, vp->x, vp->y);
}

void tuim_viewport_destroy(TuimViewport* vp) {
	MEB_ASSERT(vp);
	
	tuim_frame_buffer_destroy(&vp->frame_buffer);
}

void tuim_viewport_clear(TuimViewport* vp, TuimColor color) {
	MEB_ASSERT(vp);
	tuim_frame_buffer_clear(&vp->frame_buffer, color);
}


TuimSizeHint tuim_viewport_measure(const TuimViewport* vp) {
	MEB_ASSERT(vp);

	TuimSizeHint sh;
	sh.min_width = vp->frame_buffer.width;
	sh.min_height = vp->frame_buffer.height;
	sh.preferred_width = vp->frame_buffer.width;
	sh.preferred_height = vp->frame_buffer.height;
	sh.max_width = vp->frame_buffer.width;
	sh.max_height = vp->frame_buffer.height;
	
	return sh;
}

void tuim_viewport_layout(TuimViewport* vp, const TuimRect rect) {
	MEB_ASSERT(vp);

	vp->x = rect.x;
	vp->y = rect.y;

	tuim_frame_buffer_resize(&vp->frame_buffer, rect.width, rect.height);
}

TuimElement tuim_viewport_to_element(const TuimViewport* vp) {
	MEB_ASSERT(vp);

	TuimElement element;
	element.data = (void*)vp;
	element.measure = tuim_viewport_measure;
	element.layout = tuim_viewport_layout;
	element.update = tuim_viewport_update;
	element.draw = (TuimElementDrawFn)tuim_viewport_draw;
	element.destroy = (TuimElementDestroyFn)tuim_viewport_destroy;
	element.on_focus_gained = NULL;
	element.on_focus_lost = NULL;
	
	return element;
}
