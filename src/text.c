#include "text.h"

TuimText tuim_default_text() {
	TuimText text;

	text.background = TUIM_BLACK_STRUCT_INDEXED;
	text.foreground = TUIM_WHITE_STRUCT_INDEXED;

	return text;
}

TuimSizeHint tuim_text_measure(void* text) {
	assert(text);

	TuimText* t = text;
	size_t width = 0;
	size_t height = 1;
	while (t->text[width]) {
		if (t->text[width] == '\n')
			height++;
		width++;
	}
	return (TuimSizeHint) {
		.min_width = width,
		.min_height = height,
		.preferred_width = width,
		.preferred_height = height,
		.max_width = width,
		.max_height = height,
	};
}

void tuim_text_layout(void* text, TuimRect area) {
	assert(text);

	TuimText* t = text;
	t->area = area;
}

TuimElement tuim_text_to_element(const TuimText* text) {
	assert(text);
	
	TuimElement element;
	
	element.data = (void*)text;
	element.measure = tuim_text_measure;
	element.layout = tuim_text_layout;
	
	return element;
}

void tuim_draw_text(TuimContext* ctx, const TuimText* text) {
	assert(ctx);

	tuim_frame_buffer_print (
		&ctx->frame_buffer, text->foreground, text->background, text->text, text->area.x, text->area.y
	);
}
