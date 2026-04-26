#include "textbox.h"

// TODO: implement textbox

TuimTextbox tuim_textbox(char* str) {
	TuimTextbox textbox = tuim_default_textbox();

	// for testing
#define TEXT_DEBUG_SIZE 512
	size_t count = TEXT_DEBUG_SIZE; // strlen(str) * 4;
	textbox.text = calloc(count + 1, sizeof(char));

	if (!textbox.text) {
		return textbox; 
	}

	memcpy(textbox.text, str, count + 1); 

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
	textbox.area.height = 2;
	textbox.cursor_pos = 0;
	textbox.is_selected = false;

	return textbox;
}

void tuim_update_textbox(TuimContext* ctx, TuimTextbox* textbox) {
	MEB_ASSERT(ctx && textbox);

	char c = tuim_get_char(ctx);

	if (tuim_is_mouse_button_down(ctx, TUIM_MOUSE_BUTTON_LEFT)) {
		textbox->is_selected = tuim_is_mouse_inside(ctx, textbox->area);
		if (textbox->is_selected) { 
			textbox->cursor_pos = ctx->input_state.mouse_state.mouse_x - textbox->area.x;
		}
	}

	if (!textbox->is_selected)
		return;

	if (tuim_is_key_rep(ctx, TUIM_KEY_LEFT)) {
		if (textbox->cursor_pos != 0)
			textbox->cursor_pos--;
	}
	else if (tuim_is_key_rep(ctx, TUIM_KEY_RIGHT)) {
		textbox->cursor_pos++;
	}

	if (tuim_is_ctrl(ctx, TUIM_KEY_V)) {
		MEB_LOGF("%s", tuim_get_clipboard(ctx));
	}

	// backspace
	else if (c == 0x08) {
		textbox->text[textbox->cursor_pos] = '\0';
		textbox->cursor_pos = max(textbox->cursor_pos-1, 0);
	}
	else if (!tuim_is_printable_utf8(c)) {
		
	}
	else if (c != 0) {
		textbox->text[textbox->cursor_pos] = c;
		textbox->cursor_pos++;
		textbox->text[textbox->cursor_pos] = '\0';
	}
}

void tuim_draw_textbox(TuimContext* ctx, const TuimTextbox* textbox) {
	MEB_ASSERT(ctx && textbox);

	TuimColor bg = textbox->is_selected ? textbox->style.bg : textbox->style.selected_bg;
	TuimColor fg = textbox->is_selected ? textbox->style.fg : textbox->style.selected_fg;

	tuim_frame_buffer_print (&ctx->frame_buffer, 
		fg, bg, textbox->text,
		textbox->area.x, textbox->area.y
	);

	if (textbox->is_selected)
		tuim_frame_buffer_set_background (
			&ctx->frame_buffer, textbox->style.cursor_color, 
			textbox->area.x + textbox->cursor_pos, textbox->area.y
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
