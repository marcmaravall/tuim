#include "text.h"

TuimText* tuim_default_text() {
	TuimText* text = malloc(sizeof(TuimText));

	// text->text = // mds_new("[TUIM_DEFAULT_TEXT");

	text->background = TUIM_BLACK_STRUCT_INDEXED;
	text->foreground = TUIM_WHITE_STRUCT_INDEXED;
	text->area.x = 0;
	text->area.y = 0;
	text->area.width = text->text.size;
	text->area.height = 1;

	return text;
}

TuimText* tuim_text(const char* str) {
	TuimText* text = malloc(sizeof(TuimText));

	text->text = mds_new(str);

	text->background = TUIM_BLACK_STRUCT_INDEXED;
	text->foreground = TUIM_WHITE_STRUCT_INDEXED;
	text->area.x = 0;
	text->area.y = 0;
	text->area.width = text->text.size;
	text->area.height = 1;

	return text;
}

TuimSizeHint tuim_text_measure(void* text) {
	MEB_ASSERT(text);

	TuimText* t = text;
	size_t width = 0;
	size_t height = 1;
	while (mds_get(t->text)[width]) {
		if (mds_get(t->text)[width] == '\n')
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
	MEB_ASSERT(text);

	TuimText* t = text;
	t->area = area;
}

TuimElement tuim_text_to_element(const TuimText* text) {
	MEB_ASSERT(text);
	
	TuimElement element;
	
	element.data			= (void*)text;
	element.measure			= (TuimElementMeasureFn)tuim_text_measure;
	element.layout			= (TuimElementLayoutFn)	tuim_text_layout;
	element.draw			= (TuimElementDrawFn)	tuim_draw_text;
	element.update			= (TuimElementUpdateFn)	tuim_text_update;
	element.destroy			= (TuimElementDestroyFn)	tuim_text_destroy;
	element.on_focus_gained = (TuimElementOnFocusGainedFn)	tuim_text_on_focus_gained;
	element.on_focus_lost	= (TuimElementOnFocusLostFn)	tuim_text_on_focus_lost;
	
	return element;
}

TuimElement tuim_text_element(const char* str) {
	MEB_ASSERT(str);

	TuimElement* el = tuim_text(str);
	return tuim_text_to_element(el);
}

void tuim_text_update(TuimContext* ctx, TuimText* text) {
	MEB_ASSERT(ctx && text);
}

void tuim_draw_text(TuimContext* ctx, const TuimText* text) {
	MEB_ASSERT(ctx);
	MEB_ASSERT(text);

	// TODO: draw in area:

	if (text->area.width <= 0 || text->area.height <= 0)
		return;

	int len = text->area.width;
	if (len > text->text.size) {
		len = text->text.size;
	}

	if (text->area.x < 0)
		len += text->area.x;

	tuim_frame_buffer_print_with_size (
		&ctx->viewport.frame_buffer, text->foreground,
		text->background, mds_get(text->text), text->area.x, text->area.y, (size_t)len
	);
}

void tuim_text_format(TuimText* text, const char* format, ...) {
	MEB_ASSERT(text && format);

	va_list args;
	va_start(args, format);
	int needed = vsnprintf(NULL, 0, format, args);
	va_end(args);

	if (needed < 0) return;

	size_t required = (size_t)needed;
	mds_resize(&text->text, required + 1);

	va_start(args, format);
	vsnprintf(mds_get(text->text), required + 1, format, args);
	va_end(args);

	text->text.size = required;
}

// TODO: add some animation to check if it works
void tuim_text_on_focus_gained(TuimText* data) {
	MEB_ASSERT(data);
}

void tuim_text_on_focus_lost(TuimText* data) {
	MEB_ASSERT(data);
}

void tuim_text_destroy(TuimText* text) {
	MEB_ASSERT(text);

	mds_free(&text->text);
}
