#include <text.h>

void tuim_draw_text(TuimContext* context, char* msg) {
	context->backend.set_foreground_color(
		context->backend.data, 
		context->style.text_foreground_color
	);

	context->backend.set_background_color(
		context->backend.data,
		context->style.background_color
	);

	context->backend.render_text(context->backend.data, msg);
}
