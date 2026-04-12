#include "text.h"

TuimText tuim_default_text() {
	TuimText text;

	const char* default_str = "[TUIM_DEFAULT_TEXT]";
	size_t len = strlen(default_str);

	text.length = len;
	text.capacity = len + 1;
	text.text = malloc(text.capacity);
	assert(text.text);
	memcpy(text.text, default_str, text.capacity);

	text.background = TUIM_BLACK_STRUCT_INDEXED;
	text.foreground = TUIM_WHITE_STRUCT_INDEXED;
	text.area.x = 0;
	text.area.y = 0;
	text.area.height = 1;

	return text;
}

TuimText tuim_text(const char* str) {
	TuimText text = tuim_default_text();

	size_t len = strlen(str);
	text.length = len;
	text.capacity = len + 1;

	free(text.text);
	text.text = malloc(text.capacity);
	assert(text.text);
	memcpy(text.text, str, text.capacity);

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
	element.draw = tuim_draw_text;
	element.update = tuim_text_update;
	element.destroy = tuim_text_destroy;
	
	return element;
}

void tuim_text_update(TuimContext* ctx, TuimText* data) {
	assert(ctx && data);
}

void tuim_draw_text(TuimContext* ctx, const TuimText* text) {
	assert(ctx);
	assert(text);
	assert(text->text);

	// TODO: draw in area:


	tuim_frame_buffer_print(
		&ctx->frame_buffer, text->foreground, text->background, text->text, text->area.x, text->area.y
	);
}

void tuim_text_format(TuimText* text, const char* format, ...) {
	assert(text && format);

	va_list args;
	va_start(args, format);
	int needed = vsnprintf(NULL, 0, format, args);
	va_end(args);

	assert(needed >= 0);
	size_t required = (size_t)needed + 1;

	if (required > text->capacity) {
		char* buf = realloc(text->text, required);
		assert(buf);
		text->text = buf;
		text->capacity = required;
	}

	va_start(args, format);
	vsnprintf(text->text, text->capacity, format, args);
	va_end(args);

	text->length = (size_t)needed;
}

void tuim_text_destroy(TuimText* text) {
	assert(text);

	free(text->text);
	text->text = NULL;
	text->length = 0;
	text->capacity = 0;
}
