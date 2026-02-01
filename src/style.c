#include <style.h>

void tuim_style_set_default(TuimStyle* style) {
	style->text_foreground_color = TUIM_COLOR_WHITE;
	style->background_color		 = TO_BACKGROUND(TUIM_COLOR_BLACK);
}