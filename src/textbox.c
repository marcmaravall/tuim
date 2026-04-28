#include "textbox.h"

// TODO: improve this implementation

static bool tuim_textbox_grow(TuimTextbox* tb) {
    if (tb->length < tb->capacity)
        return true;

    size_t new_cap = tb->capacity * 2 + 64;
    char* new_buf = realloc(tb->text, new_cap + 1);
    if (!new_buf)
        return false;

    tb->text = new_buf;
    tb->capacity = new_cap;
    return true;
}

static bool tuim_textbox_insert_char(TuimTextbox* tb, char c) {
    if (!tuim_textbox_grow(tb))
        return false;

    memmove (
        &tb->text[tb->cursor_pos + 1],
        &tb->text[tb->cursor_pos],
        tb->length - (size_t)tb->cursor_pos + 1
    );

    tb->text[tb->cursor_pos] = c;
    tb->length++;
    tb->cursor_pos++;
    return true;
}

static void tuim_textbox_backspace(TuimTextbox* tb) {
    if (tb->cursor_pos == 0 || tb->length == 0)
        return;

    tb->cursor_pos--;
    memmove(
        &tb->text[tb->cursor_pos],
        &tb->text[tb->cursor_pos + 1],
        tb->length - (size_t)tb->cursor_pos
    );
    tb->length--;
}

static int tuim_textbox_line_start(const TuimTextbox* tb, int row) {
    if (row == 0)
        return 0;

    int current_row = 0;
    for (int i = 0; tb->text[i] != '\0'; i++) {
        if (tb->text[i] == '\n') {
            current_row++;
            if (current_row == row)
                return i + 1;
        }
    }
    return -1;
}

static int tuim_textbox_line_length(const TuimTextbox* tb, int line_start) {
    int len = 0;
    for (int i = line_start; tb->text[i] != '\0' && tb->text[i] != '\n'; i++)
        len++;
    return len;
}

static int tuim_textbox_line_count(const TuimTextbox* tb) {
    if (!tb->text)
        return 1;

    int count = 1;
    for (int i = 0; tb->text[i] != '\0'; i++) {
        if (tb->text[i] == '\n')
            count++;
    }
    return count;
}

static void tuim_textbox_get_row_col(const TuimTextbox* tb, int pos,
    int* out_row, int* out_col) {
    *out_row = 0;
    *out_col = 0;
    for (int i = 0; i < pos && tb->text[i] != '\0'; i++) {
        if (tb->text[i] == '\n') {
            (*out_row)++;
            *out_col = 0;
        }
        else {
            (*out_col)++;
        }
    }
}

TuimTextbox tuim_default_textbox(void) {
    TuimTextbox tb;

    tb.style.fg = TUIM_WHITE_STRUCT_INDEXED;
    tb.style.bg = TUIM_BLACK_STRUCT_INDEXED;
    tb.style.selected_fg = TUIM_BLACK_STRUCT_INDEXED;
    tb.style.selected_bg = TUIM_WHITE_STRUCT_INDEXED;
    tb.style.cursor_color = TUIM_YELLOW_STRUCT_INDEXED;

    tb.area.x = 0;
    tb.area.y = 0;
    tb.area.width = 0;
    tb.area.height = 1;

    tb.capacity = TUIM_TEXTBOX_INITIAL_CAPACITY;
    tb.length = 0;
    tb.text = calloc(tb.capacity + 1, sizeof(char));

    tb.cursor_pos = 0;
    tb.is_selected = false;

    return tb;
}

TuimTextbox tuim_textbox(const char* str) {
    MEB_ASSERT(str);

    TuimTextbox tb = tuim_default_textbox();
    if (!tb.text)
        return tb;

    size_t len = strlen(str);

    if (len > tb.capacity) {
        free(tb.text);
        tb.capacity = len + TUIM_TEXTBOX_INITIAL_CAPACITY;
        tb.text = calloc(tb.capacity + 1, sizeof(char));
        if (!tb.text) {
            tb.capacity = 0;
            tb.length = 0;
            return tb;
        }
    }

    memcpy(tb.text, str, len + 1);
    tb.length = len;

    return tb;
}

void tuim_destroy_textbox(TuimTextbox* textbox) {
    MEB_ASSERT(textbox);

    free(textbox->text);
    textbox->text = NULL;
    textbox->length = 0;
    textbox->capacity = 0;
}

void tuim_update_textbox(TuimContext* ctx, TuimTextbox* textbox) {
    MEB_ASSERT(ctx && textbox);

    if (!textbox->text)
        return;

    if (tuim_is_mouse_button_down(ctx, TUIM_MOUSE_BUTTON_LEFT)) {
        textbox->is_selected = tuim_is_mouse_inside(ctx, textbox->area);

        if (textbox->is_selected) {
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

    if (tuim_is_key_down(ctx, TUIM_KEY_LEFT)) {
        if (textbox->cursor_pos > 0)
            textbox->cursor_pos--;
    }
    else if (tuim_is_key_down(ctx, TUIM_KEY_RIGHT)) {
        if (textbox->cursor_pos < (int)textbox->length)
            textbox->cursor_pos++;
    }
    else if (tuim_is_key_down(ctx, TUIM_KEY_UP)) {
        int row, col;
        tuim_textbox_get_row_col(textbox, textbox->cursor_pos, &row, &col);

        if (row > 0) {
            int prev_start = tuim_textbox_line_start(textbox, row - 1);
            int prev_len = tuim_textbox_line_length(textbox, prev_start);
            int target_col = (col < prev_len) ? col : prev_len;
            textbox->cursor_pos = prev_start + target_col;
        }
    }
    else if (tuim_is_key_down(ctx, TUIM_KEY_DOWN)) {
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

    if (!textbox->text)
        return;

    TuimColor bg = textbox->is_selected ? textbox->style.selected_bg : textbox->style.bg;
    TuimColor fg = textbox->is_selected ? textbox->style.selected_fg : textbox->style.fg;

    int cursor_row = 0, cursor_col = 0;
    if (textbox->is_selected)
        tuim_textbox_get_row_col(textbox, textbox->cursor_pos, &cursor_row, &cursor_col);

    char* line_buf = malloc(textbox->capacity + 1);
    if (!line_buf)
        return;

    int line_row = 0;
    int line_start = 0;

    while (true) {
        int line_len = 0;
        for (int i = line_start;
            textbox->text[i] != '\0' && textbox->text[i] != '\n';
            i++)
        {
            line_buf[line_len++] = textbox->text[i];
        }
        line_buf[line_len] = '\0';

        tuim_frame_buffer_print(
            &ctx->frame_buffer,
            fg, bg, line_buf,
            textbox->area.x,
            textbox->area.y + line_row
        );

        if (textbox->is_selected && line_row == cursor_row) {
            // TODO: improve blinking

            if ((int)(tuim_get_time(ctx)*4) % 2 == 0) {
                tuim_frame_buffer_set_background(
                    &ctx->frame_buffer,
                    textbox->style.cursor_color,
                    textbox->area.x + cursor_col,
                    textbox->area.y + line_row
                );
            }
        }

        int next_newline = line_start + line_len;
        if (textbox->text[next_newline] == '\0')
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

    if (textbox->text) {
        for (int i = 0; textbox->text[i] != '\0'; i++) {
            if (textbox->text[i] == '\n') {
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