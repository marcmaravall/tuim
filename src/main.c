// basic window demo, click and drag the window around

#include <tuim.h>
#include <backends/windows/windows_backend.h>

#define WINDOW_WIDTH 30
#define WINDOW_HEIGHT 5

TuimContext ctx;

int mouse_x = 0, mouse_y = 0;

int windows_pos_x = 0, windows_pos_y = 0;

bool is_dragging = false;

void update_window() {
    if (tuim_is_mouse_button_pressed(&ctx, TUIM_MOUSE_BUTTON_LEFT) &&
        (tuim_rect_overlaps_p(mouse_x, mouse_y, 1, 1, windows_pos_x, windows_pos_y, WINDOW_WIDTH, WINDOW_HEIGHT) || is_dragging)) {
        windows_pos_x = mouse_x;
        windows_pos_y = mouse_y;
        is_dragging = true;
    }
    else {
        is_dragging = false;
    }

    tuim_frame_buffer_draw_rect(
        &ctx.style, &ctx.frame_buffer,
        windows_pos_x, windows_pos_y, WINDOW_WIDTH, WINDOW_HEIGHT
    );

    tuim_set_text_foreground_color(&ctx, TUIM_BLUE_STRUCT_INDEXED);

    tuim_frame_buffer_print(
        &ctx.style, &ctx.frame_buffer,
        " - Window                    x", windows_pos_x, windows_pos_y
	);

	tuim_set_text_foreground_color(&ctx, TUIM_RED_STRUCT_INDEXED);

    tuim_frame_buffer_print(
        &ctx.style, &ctx.frame_buffer,
        " - option 1 ... ", windows_pos_x, windows_pos_y+2
    );
}

int main(void) {
    ctx.backend = tuim_windows_backend();
    
    tuim_init_context(&ctx);
	ctx.style = tuim_style_default_dark();
    
    while (1) {
		tuim_begin_frame(&ctx);
		tuim_update_input(&ctx);
		tuim_get_mouse_position(&ctx, &mouse_x, &mouse_y);

        update_window();

		tuim_end_frame(&ctx);
    }

    return 0;
}
