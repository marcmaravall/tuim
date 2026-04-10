#include "textbox.h"

// TODO: implement textbox

TuimTextbox tuim_default_textbox() {
	TuimTextbox textbox;

	textbox.text = "[TUIM_DEFAULT_TEXTBOX]";

	textbox.style.fg = TUIM_WHITE_STRUCT_INDEXED;
	textbox.style.bg = TUIM_BLACK_STRUCT_INDEXED;
	textbox.style.selected_fg = TUIM_BLACK_STRUCT_INDEXED;
	textbox.style.selected_bg = TUIM_WHITE_STRUCT_INDEXED;
	textbox.style.cursor_color = TUIM_YELLOW_STRUCT_INDEXED;

	textbox.area.x = 0;
	textbox.area.y = 0;
	textbox.area.width = strlen(textbox.h);
	textbox.area.height = 1;

	return textbox;
}

void tuim_update_textbox(TuimContext* ctx, TuimTextbox* textbox) {

}

void tuim_draw_textbox(TuimContext* ctx, const TuimTextbox* textbox) {

}

void tuim_destroy_textbox(TuimTextbox* textbox) {

}

TuimElement tuim_textbox_to_element(TuimTextbox* textbox) {
	TuimElement el;

	el.data = textbox;
	el.destroy = tuim_destroy_textbox;
	el.draw = tuim_draw_textbox;
	el.layout = tuim_textbox_layout;
	el.update = tuim_update_textbox;
	el.measure = tuim_textbox_measure;

	return el;
}

void tuim_textbox_layout(TuimTextbox* textbox, TuimRect area) {

}

TuimSizeHint tuim_textbox_measure(TuimTextbox* textbox) {
	TuimSizeHint hint;

	return hint;
}

