#include "scroll_view.h"

TuimScrollView tuim_scroll_view(const TuimRect rect) {
	TuimScrollView sw;
	
	sw.viewport = tuim_viewport(rect);
	sw.scroll_x = 0;
	sw.scroll_y = 0;
	sw.content_height = 100;
	sw.content_width = 100;

	tuim_layout_init(&sw.layout, 4);

	return sw;
}

void tuim_scroll_view_update(TuimContext* ctx, TuimScrollView* sw) {
	MEB_ASSERT(ctx && sw);

	TuimViewport last = ctx->viewport;
	ctx->viewport = sw->viewport;

	sw->layout.bounds.x = sw->scroll_x;
	sw->layout.bounds.y = sw->scroll_y;
	sw->layout.bounds.width  = sw->content_width;
	sw->layout.bounds.height = sw->content_height;

	tuim_layout_update(ctx, &sw->layout);
	
	ctx->viewport = last;
}

void tuim_scroll_view_draw(TuimContext* ctx, TuimScrollView* sw) {
	MEB_ASSERT(ctx && sw);

	tuim_frame_buffer_clear(&sw->viewport.frame_buffer, TUIM_BLACK_STRUCT_INDEXED);

	TuimViewport last = ctx->viewport;
	ctx->viewport = sw->viewport;

	tuim_layout_draw(ctx, &sw->layout);

	ctx->viewport = last;
	tuim_viewport_draw(ctx, &sw->viewport);
}

void tuim_scroll_view_destroy(TuimScrollView* sw) {
	MEB_ASSERT(sw);

	tuim_viewport_destroy(&sw->viewport);
	tuim_layout_destroy(&sw->layout);
}


TuimSizeHint tuim_scroll_view_measure(const TuimScrollView* sw) {
	MEB_ASSERT(sw);
	
	return tuim_size_hint_dynamic(
		TUIM_RECT(
			sw->viewport.x, sw->viewport.y, 
			sw->viewport.frame_buffer.width, sw->viewport.frame_buffer.height
		)
	);
}

void tuim_scroll_view_layout(TuimScrollView* sw, const TuimRect rect) {
	MEB_ASSERT(sw);

	sw->viewport.x = rect.x;
	sw->viewport.y = rect.y;

	tuim_frame_buffer_resize(&sw->viewport.frame_buffer, (size_t)rect.width, (size_t)rect.height);
}

TuimElement tuim_scroll_view_to_element(TuimScrollView* sw) {
	MEB_ASSERT(sw);

	TuimElement el;
	el.data = sw;
	el.draw = (TuimElementDrawFn)tuim_scroll_view_draw;
	el.update = (TuimElementUpdateFn)tuim_scroll_view_update;
	el.destroy = (TuimElementDestroyFn)tuim_scroll_view_destroy;
	el.measure = (TuimElementMeasureFn)tuim_scroll_view_measure;
	el.layout = (TuimElementLayoutFn)tuim_scroll_view_layout;

	return el;
}
