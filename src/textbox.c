#include "textbox.h"

// TODO: improve this implementation

static bool tuim_textbox_grow(TuimTextbox* tb) {
    if (tb->text.size < tb->text.capacity)
        return true;

    size_t new_cap = tb->text.capacity * 2 + 64;
    char* new_buf = realloc(tb->text.data, new_cap + 1);
    if (!new_buf)
        return false;

    tb->text.data = new_buf;
    tb->text.capacity = new_cap;
    return true;
}

static bool tuim_textbox_insert_char(TuimTextbox* tb, char c) {
    if (!tuim_textbox_grow(tb))
        return false;

    char* text = mds_get(tb->text);
    memmove (
        &text[tb->cursor_pos + 1],
        &text[tb->cursor_pos],
        mds_size(tb->text) - (size_t)tb->cursor_pos + 1
    );

    text[tb->cursor_pos] = c;
	tb->text.size++;
    tb->cursor_pos++;
    return true;
}

static void tuim_textbox_backspace(TuimTextbox* tb) {
    if (tb->cursor_pos == 0 || mds_size(tb->text) == 0)
        return;

    tb->cursor_pos--;
    memmove(
        &mds_get(tb->text)[tb->cursor_pos],
        &mds_get(tb->text)[tb->cursor_pos + 1],
        mds_size(tb->text) - (size_t)tb->cursor_pos
    );
    tb->text.size--;
}

static int tuim_textbox_line_start(const TuimTextbox* tb, int row) {
    if (row == 0)
        return 0;

    int current_row = 0;
	char* text = mds_get(tb->text);
    for (int i = 0; text[i] != '\0'; i++) {
        if (text[i] == '\n') {
            current_row++;
            if (current_row == row)
                return i + 1;
        }
    }
    return -1;
}

static int tuim_textbox_line_length(const TuimTextbox* tb, int line_start) {
    int len = 0;
	char* text = mds_get(tb->text);
    for (int i = line_start; text[i] != '\0' && text[i] != '\n'; i++)
        len++;
    return len;
}

static int tuim_textbox_line_count(const TuimTextbox* tb) {
    if (!mds_get(tb->text))
        return 1;

    int count = 1;
	char* text = mds_get(tb->text);
    for (int i = 0; text[i] != '\0'; i++) {
        if (text[i] == '\n')
            count++;
    }
    return count;
}

static void tuim_textbox_get_row_col(const TuimTextbox* tb, int pos,
    int* out_row, int* out_col) {
    *out_row = 0;
    *out_col = 0;
	char* text = mds_get(tb->text);
    for (int i = 0; i < pos && text[i] != '\0'; i++) {
        if (text[i] == '\n') {
            (*out_row)++;
            *out_col = 0;
        }
        else {
            (*out_col)++;
        }
    }
}

TuimTextbox* tuim_default_textbox(void) {
    TuimTextbox* tb = malloc(sizeof(TuimTextbox));

    tb->style.fg = TUIM_WHITE_STRUCT_INDEXED;
    tb->style.bg = TUIM_BLACK_STRUCT_INDEXED;
    tb->style.selected_fg = TUIM_BLACK_STRUCT_INDEXED;
    tb->style.selected_bg = TUIM_WHITE_STRUCT_INDEXED;
    tb->style.cursor_color = TUIM_YELLOW_STRUCT_INDEXED;

    tb->area.x = 0;
    tb->area.y = 0;
    tb->area.width = 0;
    tb->area.height = 1;

	tb->text = mds_new("");

    tb->blink_time = 0.0;
    tb->blink_delay = 2;

    tb->cursor_pos = 0;
    tb->is_selected = false;

    return tb;
}

TuimTextbox* tuim_textbox(const char* str) {
    MEB_ASSERT(str);

    TuimTextbox* tb = tuim_default_textbox();
	tb->text = mds_new(str);

    return tb;
}

void tuim_destroy_textbox(TuimTextbox* textbox) {
    MEB_ASSERT(textbox);

	mds_free(&textbox->text);
}

void tuim_update_textbox(TuimContext* ctx, TuimTextbox* textbox) {
    MEB_ASSERT(ctx && textbox);

    if (!mds_get(textbox->text))
        return;

    if (tuim_is_mouse_button_down (ctx, TUIM_MOUSE_BUTTON_LEFT)) {
        textbox->is_selected = tuim_is_mouse_inside(ctx, textbox->area);

        if (textbox->is_selected) {
            textbox->blink_time = 0.0;
            
            int click_col = ctx->input_state.mouse_state.mouse_x - textbox->area.x;
            int click_row = ctx->input_state.mouse_state.mouse_y - textbox->area.y;

            int total_lines = tuim_textbox_line_count(textbox);
            if (click_row < 0)               click_row = 0;
            if (click_row >= total_lines)    click_row = total_lines - 1;

            int ls = tuim_textbox_line_start(textbox, click_row);
            int ll = tuim_textbox_line_length(textbox, ls);

            if (click_col < 0)  click_col = 0;
            if (click_col > ll) click_col = ll;

            textbox->cursor_pos = ls + click_col;
        }
    }

    if (!textbox->is_selected)
        return;
    
    textbox->blink_time += tuim_get_delta_time(ctx);

    if (tuim_is_key_rep(ctx, TUIM_KEY_LEFT)) {
        if (textbox->cursor_pos > 0)
            textbox->cursor_pos--;
    }
    else if (tuim_is_key_rep(ctx, TUIM_KEY_RIGHT)) {
        if (textbox->cursor_pos < (int)mds_size(textbox->text))
            textbox->cursor_pos++;
    }
    else if (tuim_is_key_rep(ctx, TUIM_KEY_UP)) {
        int row, col;
        tuim_textbox_get_row_col(textbox, textbox->cursor_pos, &row, &col);

        if (row > 0) {
            int prev_start = tuim_textbox_line_start(textbox, row - 1);
            int prev_len = tuim_textbox_line_length(textbox, prev_start);
            int target_col = (col < prev_len) ? col : prev_len;
            textbox->cursor_pos = prev_start + target_col;
        }
    }
    else if (tuim_is_key_rep(ctx, TUIM_KEY_DOWN)) {
        int row, col;
        tuim_textbox_get_row_col(textbox, textbox->cursor_pos, &row, &col);

        int next_start = tuim_textbox_line_start(textbox, row + 1);
        if (next_start >= 0) {
            int next_len = tuim_textbox_line_length(textbox, next_start);
            int target_col = (col < next_len) ? col : next_len;
            textbox->cursor_pos = next_start + target_col;
        }
    }

    char c = tuim_get_char(ctx);

    if (c == 0x08) {
        tuim_textbox_backspace(textbox);
    }
    else if (c == '\r' || c == '\n') {
        tuim_textbox_insert_char(textbox, '\n');
    }
    else if (c != 0) {
        tuim_textbox_insert_char(textbox, c);
    }
}

void tuim_draw_textbox(TuimContext* ctx, const TuimTextbox* textbox) {
    MEB_ASSERT(ctx && textbox);

    if (!mds_get(textbox->text))
        return;

    TuimColor bg = textbox->is_selected ? textbox->style.selected_bg : textbox->style.bg;
    TuimColor fg = textbox->is_selected ? textbox->style.selected_fg : textbox->style.fg;

    int cursor_row = 0, cursor_col = 0;
    if (textbox->is_selected)
        tuim_textbox_get_row_col(textbox, textbox->cursor_pos, &cursor_row, &cursor_col);

    char* line_buf = malloc(mds_size(textbox->text) + 1);

    if (!line_buf)
        return;

    int line_row = 0;
    int line_start = 0;

    while (true) {
        int line_len = 0;
        for (int i = line_start;
            mds_get(textbox->text)[i] != '\0' && mds_get(textbox->text)[i] != '\n';
            i++)
        {
            line_buf[line_len++] = mds_get(textbox->text)[i];
        }
        line_buf[line_len] = '\0';

        tuim_frame_buffer_print(
            &ctx->viewport.frame_buffer,
            fg, bg, line_buf,
            textbox->area.x,
            textbox->area.y + line_row
        );

        if (textbox->is_selected && line_row == cursor_row) {
            // TODO: continue improving blinking system
            if (((int)textbox->blink_time) % textbox->blink_delay == 0) {
                tuim_frame_buffer_set_background(
                    &ctx->viewport.frame_buffer,
                    textbox->style.cursor_color,
                    textbox->area.x + cursor_col,
                    textbox->area.y + line_row
                );
            }
        }

        int next_newline = line_start + line_len;
        if (mds_get(textbox->text)[next_newline] == '\0')
            break;

        line_start = next_newline + 1;
        line_row++;
    }

    free(line_buf);
}

void tuim_textbox_layout(TuimTextbox* textbox, const TuimRect area) {
    MEB_ASSERT(textbox);
    textbox->area = area;
}

TuimSizeHint tuim_textbox_measure(TuimTextbox* textbox) {
    MEB_ASSERT(textbox);

    int max_width = 0;
    int line_width = 0;
    int line_height = 1;

    mdString str = textbox->text;
    if (mds_get(str)) {
        for (int i = 0; mds_get(str)[i] != '\0'; i++) {
            if (mds_get(str)[i] == '\n') {
                if (line_width > max_width)
                    max_width = line_width;
                line_width = 0;
                line_height++;
            }
            else {
                line_width++;
            }
        }
    }
    if (line_width > max_width)
        max_width = line_width;

    TuimRect natural = {
        .x = 0,
        .y = 0,
        .width = max_width,
        .height = line_height
    };

    return tuim_size_hint_static(natural);
}

TuimElement tuim_textbox_element(const char* str) {
	MEB_ASSERT(str);
    TuimTextbox* textbox = tuim_textbox(str);
    return tuim_textbox_to_element(textbox);
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
