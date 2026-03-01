#include <style.h>

TuimStyle tuim_style_set_default() {
	TuimStyle style;

	style.text_foreground_color = TUIM_ANSI_COLOR_DEFAULT;
	style.background_color      = TUIM_ANSI_COLOR_BLACK;
	style.rect_background_color = TUIM_ANSI_COLOR_BLUE;

	return style;
}