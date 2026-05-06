#include "viewport.h"

TuimViewport tuim_viewport(const TuimRect rect) {
	TuimViewport vp;

	vp.x = rect.x;
	vp.y = rect.y;
	tuim_frame_buffer_init(&vp.frame_buffer, rect.width, rect.height);

	return vp;
}

void tuim_viewport_draw(TuimContext* ctx, const TuimViewport* vp) {
	MEB_ASSERT(ctx && vp);
	tuim_frame_buffer_draw_frame_buffer(&ctx->frame_buffer, (TuimFrameBuffer*)&vp->frame_buffer, vp->x, vp->y);
}

void tuim_viewport_destroy(TuimViewport* vp) {
	MEB_ASSERT(vp);
	
	tuim_frame_buffer_destroy(&vp->frame_buffer);
}

void tuim_viewport_clear(TuimViewport* vp, TuimColor color) {
	MEB_ASSERT(vp);
	tuim_frame_buffer_clear(&vp->frame_buffer, color);
}

TuimElement tuim_viewport_to_element(const TuimViewport* vp) {
	MEB_ASSERT(vp);

	TuimElement element;
	element.data = (void*)vp;
	element.measure = NULL;
	element.layout = NULL;
	element.update = NULL;
	element.draw = (TuimElementDrawFn)tuim_viewport_draw;
	element.destroy = (TuimElementDestroyFn)tuim_viewport_destroy;
	element.on_focus_gained = NULL;
	element.on_focus_lost = NULL;
	
	return element;
}
