// THIS IS AN EXAMPLE

#include "tuim.h"
#include "context.h"
#include "rect.h"

#include "backend.h"
#include "backends/windows/windows_backend.h"
#include "text.h"

#include <stdio.h>

int main(void) {
    TuimContext ctx;
    TuimBackend b = tuim_windows_backend();
    ctx.backend = b;
    
    tuim_init_context(&ctx);

    while (1) {
        tuim_begin_frame(&ctx);

		tuim_frame_buffer_draw_equation_line(&ctx.frame_buffer, 0.2, 0);
		tuim_frame_buffer_draw_rect(&ctx.frame_buffer, 52, 12, 20, 5);
        tuim_frame_buffer_print(&ctx.frame_buffer, " Hello, world! ", 53, 15);

        Sleep(10);

        tuim_end_frame(&ctx);
    }

    return 0;
}
