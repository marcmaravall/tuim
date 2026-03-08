#include "style.h"
#include "context.h"

TuimStyle tuim_style_default() {
	TuimStyle style;

	style.rect_background_color = TUIM_BLUE_STRUCT_INDEXED;
	style.line_color = TUIM_WHITE_STRUCT_INDEXED;
	style.rect_background_color = TUIM_BLUE_STRUCT_INDEXED;
	style.rect_foreground_color = TUIM_WHITE_STRUCT_INDEXED;
	style.text_background_color = TUIM_BLUE_STRUCT_INDEXED;
	style.text_foreground_color = TUIM_WHITE_STRUCT_INDEXED;
	style.clear_color = TUIM_GREEN_STRUCT_INDEXED;

	return style;
}

void tuim_set_rect_background_color(TuimContext* ctx, const TuimColor color) {
	assert(ctx != NULL);
	ctx->style.rect_background_color = color;
}
void tuim_set_rect_foreground_color(TuimContext* ctx, const TuimColor color) {
	assert(ctx != NULL);
	ctx->style.rect_foreground_color = color;
}

void tuim_set_text_foreground_color(TuimContext* ctx, const TuimColor color) {
	assert(ctx != NULL);
	ctx->style.text_foreground_color = color;
}
void tuim_set_text_background_color(TuimContext* ctx, const TuimColor color) {
	assert(ctx != NULL);
	ctx->style.text_background_color = color;
}

void tuim_set_line_color(TuimContext* ctx, const TuimColor color) {
	assert(ctx != NULL);
	ctx->style.line_color = color;
}

void tuim_set_clear_color(TuimContext* ctx, const TuimColor color) {
	assert(ctx != NULL);
	ctx->style.clear_color = color;
}
