#include "style.h"

TuimStyle tuim_style_set_default() {
	TuimStyle style;

	style.text_foreground_color = TUIM_WHITE_STRUCT_INDEXED;
	style.background_color      = TUIM_BLACK_STRUCT_INDEXED;
	style.rect_background_color = TUIM_BLUE_STRUCT_INDEXED;

	return style;
}