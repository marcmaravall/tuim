#ifndef TUIM_WINDOW_H
#define TUIM_WINDOW_H

#include "rect.h"
#include "frame_buffer.h"
#include "id.h"
#include "input.h"
#include "context.h"
#include "element.h"
#include "layout.h"

#include "text.h"
#include "button.h"
#include "checkbox.h"
#include "text_list.h"

#include <meb.h>

#include <stdlib.h>
#include <stdbool.h>

#define TUIM_WINDOW_LAYOUT_DEFAULT_CAPACITY 4

#define TUIM_WINDOW_DRAW_BORDER_HORIZONTAL 0x01
#define TUIM_WINDOW_DRAW_BORDER_VERTICAL   0x02
#define TUIM_WINDOW_DRAW_BORDER \
	TUIM_WINDOW_DRAW_BORDER_HORIZONTAL | TUIM_WINDOW_DRAW_BORDER_VERTICAL

// TODO: continue

#define TUIM_WINDOW_DEFAULT_WIDTH  20
#define TUIM_WINDOW_DEFAULT_HEIGHT 5 

#ifndef TUIM_WINDOW_DEFAULT_MIN_WIDTH
#	define TUIM_WINDOW_DEFAULT_MIN_WIDTH 1
#endif
#ifndef TUIM_WINDOW_DEFAULT_MIN_HEIGHT
#	define TUIM_WINDOW_DEFAULT_MIN_HEIGHT 1
#endif
#ifndef TUIM_WINDOW_DEFAULT_MAX_WIDTH
#	define TUIM_WINDOW_DEFAULT_MAX_WIDTH INT_MAX
#endif
#ifndef TUIM_WINDOW_DEFAULT_MAX_HEIGHT
#	define TUIM_WINDOW_DEFAULT_MAX_HEIGHT INT_MIN
#endif

#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif
#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif

typedef struct {
	// layout
	TuimLayout layout;

	TuimRect rect;

	struct TuimWindowStyle {
		TuimColor title_bar_color;
		TuimColor background;
		TuimColor border_color;
		TuimColor title_color;

		uint8_t flags;
	} style;

	// ---------
	size_t min_width;
	size_t min_height;
	size_t max_width;
	size_t max_height;
	// ---------

	char* title;

	// ---------
	int start_mouse_resize_x;
	int start_mouse_resize_y;
	
	int drag_offset_x;
	int drag_offset_y;

	bool is_resizing;
	bool is_dragging;
	// ---------
} TuimWindow;

TuimWindow* tuim_default_window();
TuimWindow* tuim_window(const char* title, const TuimRect rect);

TuimSizeHint tuim_window_measure(const TuimWindow* data);
void tuim_window_draw  (TuimContext* ctx, TuimWindow* window);
void tuim_window_destroy (TuimWindow* window);

void tuim_window_layout(TuimWindow* window, const TuimRect rect);

void tuim_window_resize(TuimWindow* window, const TuimRect rect);

// values for tuim_window_update
#define TUIM_WINDOW_UPDATE_NONE 0
#define TUIM_WINDOW_UPDATE_RESIZED 1
#define TUIM_WINDOW_UPDATE_DRAGGED 2
#define TUIM_WINDOW_UPDATE_RELEASED 3
int tuim_window_update(TuimContext* ctx, TuimWindow* window);

TuimElement tuim_window_element(const char* title, const TuimRect);
TuimElement tuim_window_to_element(TuimWindow* window);

bool tuim_window_is_hovered(const TuimContext* ctx, const TuimWindow* window);

void tuim_window_add_element  (TuimWindow* window, const TuimElement el);
void tuim_window_add_elements (TuimWindow* window, const size_t count, TuimElement els[]);

TuimElement tuim_window_add_text(TuimWindow* window, const char* str, TuimText* text);
TuimElement tuim_window_add_button(TuimWindow* window, const char* str, TuimButton* button);
TuimElement tuim_window_add_checkbox(TuimWindow* window, const char* str, TuimCheckbox* checkbox);
TuimElement tuim_window_add_text_list(TuimWindow* window, const size_t capacity, TuimTextList* list);

void tuim_window_set_bounds(TuimWindow* window, const TuimSizeHint sh);

#endif // TUIM_WINDOW_H