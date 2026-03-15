#include <tuim.h>
#include <backends/windows/windows_backend.h>

int main(void) {
    TuimContext ctx;
    ctx.backend = tuim_windows_backend();
    tuim_init_context(&ctx);
    ctx.style = tuim_style_default_dark();

    TuimWindow example = tuim_default_window();
    example.title = "This is an example window!";
    example.rect.x = 0;
    example.rect.y = 0;
	example.border_color = TUIM_BRIGHT_BLUE_STRUCT_INDEXED;
	example.title_bar_color = TUIM_BLUE_STRUCT_INDEXED;

    TuimWindow example1 = tuim_default_window();
    example1.title = "This is another example";
    example1.rect.x = 60;
    example1.rect.y = 12;

    // TODO: abstract this shit and solve errors
    
    TuimWindow* windows[] = { &example, &example1 };
    TuimWindow* active_window = NULL;

    TuimText text;

    while (1) {
        tuim_begin_frame(&ctx);
        tuim_update_input(&ctx);

        for (int i = 0; i < 2; i++) {
            if (tuim_window_is_hovered(&ctx, windows[i]) &&
                tuim_is_mouse_button(&ctx, TUIM_MOUSE_BUTTON_LEFT)) {
                active_window = windows[i];
            }
        }

        for (int i = 0; i < 2; i++) {
            if (windows[i] != active_window) {
                if (tuim_window_is_hovered(&ctx, windows[i]) &&
                    tuim_is_mouse_button_down(&ctx, TUIM_MOUSE_BUTTON_LEFT)) {
                    break;
				}
                tuim_window_draw(&ctx, windows[i]);
                tuim_window_update(&ctx, windows[i]);
            }
        }

        if (active_window) {
            tuim_window_draw(&ctx, active_window);
            tuim_window_update(&ctx, active_window);
        }

        tuim_end_frame(&ctx);
    }

    return 0;
}