// THIS IS AN EXAMPLE

#include "tuim.h"
#include "context.h"
#include "rect.h"

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
    
    while (1) {
        tuim_begin_frame(&ctx);
		tuim_update_input(&ctx);

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

        tuim_frame_buffer_draw_rect(&ctx.style, &ctx.frame_buffer, x, y, 5, 5);

        Sleep(10);

        tuim_end_frame(&ctx);
    }

    return 0;
}
