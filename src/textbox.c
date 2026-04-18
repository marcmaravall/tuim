#include "textbox.h"

// TODO: implement textbox

TuimTextbox tuim_textbox(char* str) {
	TuimTextbox textbox;

	textbox = tuim_default_textbox();
	textbox.text = str;

	return textbox;
}

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
	textbox.area.width = (int)strlen(textbox.text);
	textbox.area.height = 1;

	return textbox;
}

void tuim_update_textbox(TuimContext* ctx, TuimTextbox* textbox) {
	MEB_ASSERT(ctx && textbox);

	// TODO: implement: 
	bool is_mayus = false;
}

void tuim_draw_textbox(TuimContext* ctx, const TuimTextbox* textbox) {
	MEB_ASSERT(ctx && textbox);
	tuim_frame_buffer_print (&ctx->frame_buffer, 
		textbox->style.fg, textbox->style.bg, textbox->text,
		textbox->area.x, textbox->area.y
	);
}

void tuim_destroy_textbox(TuimTextbox* textbox) {
	MEB_ASSERT(textbox);
}

TuimElement tuim_textbox_to_element(TuimTextbox* textbox) {
	MEB_ASSERT(textbox);
	
	TuimElement el;

	el.data = textbox;
	el.destroy = tuim_destroy_textbox;
	el.draw = tuim_draw_textbox;
	el.layout = tuim_textbox_layout;
	el.update = tuim_update_textbox;
	el.measure = tuim_textbox_measure;

	return el;
}

void tuim_textbox_layout(TuimTextbox* textbox, const TuimRect area) {
	MEB_ASSERT(textbox);

	textbox->area = area;
}

TuimSizeHint tuim_textbox_measure(TuimTextbox* textbox) {
	MEB_ASSERT(textbox);

	TuimSizeHint sh;
	sh = tuim_size_hint_static(textbox->area);

	return sh;
}
