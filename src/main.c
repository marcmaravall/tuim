// this is only a test, not a part of the library
// it's only used to test the library internally and its not supposed to work very well

#include <tuim.h>
#ifdef __linux__
#include <backends/linux/linux_backend.h>
#elif defined _WIN32
#include <backends/windows/windows_backend.h>
#endif

#define MEB_LOG_TO_FILE
#define MEB_IMPLEMENTATION
#include <meb.h>
#include <time.h>

int main(void) {
	MebContext log_ctx;
	meb_init(&log_ctx, "debug.txt");
	meb_log_level(&log_ctx, MEB_INFO);
	meb_prof_mode(&log_ctx, MEB_MILLISECONDS);

    TuimContext ctx;
#ifdef __linux__
    ctx.backend = tuim_linux_backend();
#elif defined _WIN32
    ctx.backend = tuim_windows_backend();
#endif
    tuim_init_context(&ctx);
    ctx.style = tuim_style_default_dark();

    int frames = 0;

    TuimWindow w = tuim_default_window();

    TuimElement e1 = tuim_window_to_element(&w);
        
    TuimLayout layout;
    tuim_layout_init(&layout, 4);

    layout.bounds.x = 0;
    layout.bounds.y = 0;
    layout.bounds.width = 80;
    layout.bounds.height = 10;

    layout.direction = TUIM_COLUMN;
    layout.spacing = 1;

    tuim_layout_add(&layout, &e1);

    while (1) {
		meb_log(&log_ctx, "Starting frame");
		meb_prof_start(&log_ctx);

        tuim_begin_frame(&ctx);
        tuim_update_input(&ctx);

        bool pressed = tuim_is_mouse_button(&ctx, TUIM_MOUSE_BUTTON_LEFT);

        char buffer[256];
        snprintf(buffer, 256, "pressed: %d", pressed);

		tuim_layout_update(&ctx, &layout);

		tuim_layout_draw(&ctx, &layout);

        if (tuim_is_key_down(&ctx, 'a')) {
            break;
        }

        tuim_end_frame(&ctx);

		meb_log(&log_ctx, "Ending frame");
		meb_prof_end(&log_ctx);
        frames++;
    }

	free(layout.elements);

	tuim_destroy_context(&ctx);
	meb_close(&log_ctx);

    return 0;
}