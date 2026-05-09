#include "frame_buffer.h"

// helpers:
static int cell_set_grapheme(TuimFrameBufferCell* cell, const uint8_t* src) {
	int byte_len = tuim_utf8_codepoint_len(src);
	int width = tuim_utf8_display_width(src);

	if (byte_len == 0 || byte_len > TUIM_MAX_GRAPHEME_BYTES - 1) {
		cell->grapheme[0] = ' ';
		cell->grapheme[1] = '\0';
		cell->grapheme_length = 1;
		cell->display_width = 1;
		return byte_len ? byte_len : 1;
	}

	memcpy(cell->grapheme, src, byte_len);
	cell->grapheme[byte_len] = '\0';
	cell->grapheme_length = (uint8_t)byte_len;
	cell->display_width = (uint8_t)(width > 0 ? width : 1);
	return byte_len;
}

static void cell_set_space(TuimFrameBufferCell* cell) {
	cell->grapheme[0] = ' ';
	cell->grapheme[1] = '\0';
	cell->grapheme_length = 1;
	cell->display_width = 1;
}

static void cell_set_continuation(TuimFrameBufferCell* cell) {
	cell->grapheme[0] = '\0';
	cell->grapheme_length = 0;
	cell->display_width = 0;
}
// ---

// this doesnt use rgb compare, only indexed color
bool tuim_frame_buffer_cell_equal(const TuimFrameBufferCell a, const TuimFrameBufferCell b) {
	return (a.grapheme_length == b.grapheme_length) &&
		(memcmp(a.grapheme, b.grapheme, a.grapheme_length) == 0) &&
		(a.background_color.color.indexed_color == b.background_color.color.indexed_color);
}

void tuim_frame_buffer_init(TuimFrameBuffer* fb, const size_t width, const size_t height) {
	fb->width  = width;
	fb->height = height;
	fb->cells = (TuimFrameBufferCell*)malloc(sizeof(TuimFrameBufferCell) * width * height);
}

void tuim_frame_buffer_clear(TuimFrameBuffer* fb, const TuimColor color) {
	MEB_ASSERT(fb);
	for (int i = 0; i < (int)fb->height; i++) {
		for (int j = 0; j < (int)fb->width; j++) {
			TuimFrameBufferCell* cell = &TUIM_FRAME_BUFFER_AT(fb, j, i);
			cell_set_space(cell);
			cell->foreground_color = color;
			cell->background_color = color;
		}
	}
}

void tuim_frame_buffer_print(
	TuimFrameBuffer* fb,
	const TuimColor fg, const TuimColor bg,
	const char* msg, const int x, const int y)
{
	MEB_ASSERT(fb != NULL);
	MEB_ASSERT(msg != NULL);

	if (y < 0 || y >= (int)fb->height) return;

	const uint8_t* src = (const uint8_t*)msg;
	int            start_x = x;

	// Si x es negativo, saltamos graphemes hasta llegar a columna 0
	while (start_x < 0 && *src) {
		int byte_len = tuim_utf8_codepoint_len(src);
		int width = tuim_utf8_display_width(src);
		start_x += (width > 0 ? width : 1);
		src += byte_len;
	}

	while (*src) {
		if (start_x >= (int)fb->width) break;

		TuimFrameBufferCell* cell = &TUIM_FRAME_BUFFER_AT(fb, start_x, y);
		int bytes_consumed = cell_set_grapheme(cell, src);
		cell->foreground_color = fg;
		cell->background_color = bg;

		// Si es wide y cabe la celda de continuación, marcarla
		if (cell->display_width == 2 && start_x + 1 < (int)fb->width) {
			TuimFrameBufferCell* cont = &TUIM_FRAME_BUFFER_AT(fb, start_x + 1, y);
			cell_set_continuation(cont);
			cont->foreground_color = fg;
			cont->background_color = bg;
		}

		start_x += cell->display_width;
		src += bytes_consumed;
	}
}

// `len` es ahora en COLUMNAS DE DISPLAY, no en bytes.
void tuim_frame_buffer_print_with_size (
	TuimFrameBuffer* fb,
	const TuimColor fg, const TuimColor bg,
	const char* msg, const int x, const int y,
	const size_t col_len)
{
	MEB_ASSERT(fb != NULL);
	MEB_ASSERT(msg != NULL);

	if (y < 0 || y >= (int)fb->height) return;

	const uint8_t* src = (const uint8_t*)msg;
	int            start_x = x;
	size_t         cols_written = 0;

	while (start_x < 0 && *src) {
		int byte_len = tuim_utf8_codepoint_len(src);
		int width = tuim_utf8_display_width(src);
		start_x += (width > 0 ? width : 1);
		src += byte_len;
	}

	while (*src && cols_written < col_len) {
		if (start_x >= (int)fb->width) break;

		TuimFrameBufferCell* cell = &TUIM_FRAME_BUFFER_AT(fb, start_x, y);
		int bytes_consumed = cell_set_grapheme(cell, src);
		cell->foreground_color = fg;
		cell->background_color = bg;

		if (cell->display_width == 2 && start_x + 1 < (int)fb->width) {
			TuimFrameBufferCell* cont = &TUIM_FRAME_BUFFER_AT(fb, start_x + 1, y);
			cell_set_continuation(cont);
			cont->foreground_color = fg;
			cont->background_color = bg;
		}

		cols_written += cell->display_width;
		start_x += cell->display_width;
		src += bytes_consumed;
	}
}

void tuim_frame_buffer_draw_line
	(TuimFrameBuffer* fb, const TuimColor color, int x0, int y0, const int x1, const int y1) {

	tuim_frame_buffer_draw_line_char(fb, color, ' ', x0, y0, x1, y1);
}

void tuim_frame_buffer_draw_line_char
	(TuimFrameBuffer* fb, const TuimColor color,
	const char c, int x0, int y0, const int x1, const int y1) {
	MEB_ASSERT(fb);

	int dx = abs(x1 - x0);
	int sx = x0 < x1 ? 1 : -1;
	int dy = -abs(y1 - y0);
	int sy = y0 < y1 ? 1 : -1;
	int err = dx + dy;

	while (true) {
		if (x0 >= 0 && x0 < fb->width && y0 >= 0 && y0 < fb->height) {
			TuimFrameBufferCell* cell = &TUIM_FRAME_BUFFER_AT(fb, x0, y0);
			cell->ascii_char = c;
			cell->background_color = color;
			cell->foreground_color = color;
		}

		if (x0 == x1 && y0 == y1)
			break;

		int e2 = 2 * err;

		if (e2 >= dy) {
			err += dy;
			x0 += sx;
		}

		if (e2 <= dx) {
			err += dx;
			y0 += sy;
		}
	}
}

void tuim_frame_buffer_draw_equation_line(const TuimColor color, TuimFrameBuffer* fb, const double m, const double n) {
	MEB_ASSERT(fb);

	int x0 = 0;
	int y0 = 0;

	for (int x = 0; x < fb->width; x++) {
		double y = (m * x) + n;

		int yi = (int)y;

		tuim_frame_buffer_draw_line(fb, color, x0, y0, x, (int)yi);
		x0 = x;
		y0 = yi;
	}
}

void tuim_frame_buffer_set_background (
	TuimFrameBuffer* fb, const TuimColor color, 
	const int x, const int y) {

	MEB_ASSERT(fb);

	if (x < 0 || y < 0 || x >= (int)fb->width || y >= (int)fb->height) {
		return;
	}

	TUIM_FRAME_BUFFER_AT(fb, x, y).background_color = color;
}

void tuim_frame_buffer_draw_rect(
	TuimFrameBuffer* fb, const TuimColor color, 
	const int x, const int y, const int width, const int height) {
	
	tuim_frame_buffer_draw_rect_char(fb, color, ' ', x, y, width, height);
}

void tuim_frame_buffer_draw_rect_char(TuimFrameBuffer* fb, const TuimColor color, 
	const char c, const int x, const int y, const int width, const int height) {
	MEB_ASSERT(fb);

	for (int j = y; j < y + height; j++) {
		for (int i = x; i < x + width; i++) {
			if (i >= fb->width || j >= fb->height) {
				continue;
			}

			TUIM_FRAME_BUFFER_AT(fb, i, j).background_color = color;
			TUIM_FRAME_BUFFER_AT(fb, i, j).foreground_color = color;
			TUIM_FRAME_BUFFER_AT(fb, i, j).ascii_char = c;
		}
	}
}

void tuim_frame_buffer_draw_frame_buffer(
	TuimFrameBuffer* dest, TuimFrameBuffer* src, const int x, const int y
) {
	MEB_ASSERT(dest && src);

	for (int j = 0; j < src->width; j++) {
		for (int i = 0; i < src->height; i++) {
			if (x + j >= dest->width || y + i >= dest->height || x + j < 0 || y+i < 0) {
				continue;
			}
			TUIM_FRAME_BUFFER_AT(dest, x + j, y + i) = TUIM_FRAME_BUFFER_AT(src, j, i);
		}
	}

}

void tuim_frame_buffer_destroy(TuimFrameBuffer* fb) {
	MEB_ASSERT(fb);
	free(fb->cells);
	fb->cells = NULL;
}

void tuim_frame_buffer_resize(TuimFrameBuffer* fb, const size_t width, const size_t height) {
	MEB_ASSERT(fb);
	if (width == fb->width && height == fb->height) {
		return;
	}

	if (width == 0 || height == 0) {
		MEB_ASSERT(0 && "ERROR: frame_buffer size is 0");
	}

	size_t size = sizeof(TuimFrameBufferCell) * width * height;
	TuimFrameBufferCell* new_ = realloc(fb->cells, size);
	MEB_ASSERT(new_);

	fb->cells = new_;
	fb->width = width;
	fb->height = height;

	tuim_frame_buffer_clear(fb, TUIM_WHITE_STRUCT_INDEXED);
}
