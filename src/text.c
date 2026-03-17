#include "text.h"

TuimText tuim_default_text() {
	TuimText text;

	text.background = TUIM_BLACK_STRUCT_INDEXED;
	text.foreground = TUIM_WHITE_STRUCT_INDEXED;

	return text;
}

void tuim_draw_text(TuimContext* ctx, const TuimText* text) {
	assert(ctx);

	tuim_frame_buffer_print (
		&ctx->frame_buffer, text->foreground, text->background, text->text, text->x, text->y
	);
}
