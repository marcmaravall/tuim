// THIS IS AN EXAMPLE

#include "tuim.h"
#include "context.h"
#include "rect.h"
#include "input.h"

#include "backend.h"
#include "backends/windows/windows_backend.h"
#include "text.h"
#include "string_format.h"

#include <stdio.h>

int main(void) {
    TuimContext ctx;
    ctx.backend = tuim_windows_backend();
    
    tuim_init_context(&ctx);
    
    int x = 56;
    int y = 12;

	int mouse_x = 0, mouse_y = 0;
    
    while (1) {
        tuim_begin_frame(&ctx);
		tuim_update_input(&ctx);

		tuim_get_mouse_position(&ctx, &mouse_x, &mouse_y);

        if (tuim_is_key_pressed(&ctx, TUIM_KEY_UP)) {
            y--;
        }
        if (tuim_is_key_pressed(&ctx, TUIM_KEY_LEFT)) {
            x--;
        }
        if (tuim_is_key_pressed(&ctx, TUIM_KEY_DOWN)) {
            y++;
        }
        if (tuim_is_key_pressed(&ctx, TUIM_KEY_RIGHT)) {
            x++;
        }

        if (tuim_is_key_down(&ctx, TUIM_KEY_ESCAPE)) {
            break;
        }

        if (tuim_is_mouse_button_pressed(&ctx, TUIM_MOUSE_BUTTON_RIGHT)) {
            x++;
        }

        // print mouse position:
        char mouse_pos_str[64];
        snprintf(mouse_pos_str, 64, "Mouse: %d:%d", mouse_x, mouse_y);
        tuim_frame_buffer_print(&ctx.style, &ctx.frame_buffer, mouse_pos_str, mouse_x, mouse_y);

        Sleep(10);

        tuim_end_frame(&ctx);
    }

    return 0;
}
