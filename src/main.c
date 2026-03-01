#include <tuim.h>
#include <context.h>
#include <os.h>
#include "rect.h"

#include <backend.h>
#include <backends/windows/windows_backend.h>

#include <text.h>
#include <stdio.h>

int main(void) {
    TuimContext ctx;
    tuim_init_context(&ctx);
    TuimBackend b = tuim_windows_backend();
    b.init(b.data);
    b.set_console_name(b.data, "hola");
    ctx.backend = b;

    TuimRect rect;
    rect.x = 10;
    rect.y = 2;

    rect.width = 10;
    rect.height = 10;

    TuimRect rect1;
    rect1.x = 20;
    rect1.y = 6;

    rect1.width = 13;
    rect1.height = 5;

    while (1) {
        tuim_begin_frame(&ctx);
        tuim_set_rect_background_color(&ctx, TUIM_COLOR_RED);
        tuim_draw_rect(&ctx, rect);
        tuim_set_rect_background_color(&ctx, TUIM_COLOR_GREEN);
        tuim_draw_rect(&ctx, rect1);

        Sleep(10);

        tuim_end_frame(&ctx);
    }

    return 0;
}
