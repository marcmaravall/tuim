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

	TuimText text = tuim_default_text();
    text.text = "hello, world!";
	text.area.x = 10;
	text.area.y = 10;

	TuimElement example = tuim_text_to_element(&text);

    TuimLayout layout;
	tuim_layout_init(&layout, 4);
	tuim_layout_add(&layout, &example);

    /*size_t width, height;
    ctx.backend.get_size(ctx.backend.data, &width, &height);
    printf("width: %ld. height: %ld\n", width, height)
    */;
    while (1) {
		meb_log(&log_ctx, "Starting frame");
		meb_prof_start(&log_ctx);

        tuim_begin_frame(&ctx);
        tuim_update_input(&ctx);

        bool pressed = tuim_is_mouse_button(&ctx, TUIM_MOUSE_BUTTON_LEFT);

        char buffer[256];
        snprintf(buffer, 256, "pressed: %d", pressed);

		tuim_layout_update(&ctx, &layout);
        
		// tuim_draw_text(&ctx, &text);

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