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

    TuimWindow* example = malloc(sizeof(TuimWindow)); 
    *example = tuim_default_window();

    TuimWindow* example1 = malloc(sizeof(TuimWindow));
    *example1 = tuim_default_window();
    example1->rect.x = 10;

    TuimWindow* example2 = malloc(sizeof(TuimWindow));
    *example2 = tuim_default_window();
    example2->rect.y = 10;


    TuimWindowManager manager;
	tuim_window_manager_init(&manager, 10);
	tuim_window_manager_add(&manager, example);
    tuim_window_manager_add(&manager, example1);
    tuim_window_manager_add(&manager, example2);

    int frames = 0;

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
        tuim_frame_buffer_print(&ctx.frame_buffer, TUIM_WHITE_STRUCT_INDEXED, TUIM_BLUE_STRUCT_INDEXED, buffer, 45, 14);
        tuim_window_manager_update(&ctx, &manager);
        tuim_window_manager_draw  (&ctx, &manager);

        if (tuim_is_key_down(&ctx, 'a')) {
            break;
        }

        tuim_end_frame(&ctx);

		meb_log(&log_ctx, "Ending frame");
		meb_prof_end(&log_ctx);
        frames++;
    }

    free(example);
    free(example1);
    free(example2);

    tuim_window_manager_free(&manager);
	tuim_destroy_context(&ctx);
	meb_close(&log_ctx);

    return 0;
}