#include <tuim.h>
#include <backends/windows/windows_backend.h>

int main(void) {
    TuimContext ctx;
    ctx.backend = tuim_windows_backend();
    tuim_init_context(&ctx);
    ctx.style = tuim_style_default_dark();

    TuimWindow example = tuim_default_window();
    example.title = "This is an example window!";

    while (1) {
        tuim_begin_frame(&ctx);
        tuim_update_input(&ctx);

		tuim_window_update(&ctx, &example);
		tuim_window_draw(&ctx, &example);

        tuim_end_frame(&ctx);
    }

    return 0;
}