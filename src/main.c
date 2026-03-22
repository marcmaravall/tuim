#include <tuim.h>
#include <backends/windows/windows_backend.h>

#define MEB_LOG_TO_FILE
#define MEB_IMPLEMENTATION
#include <meb.h>

int main(void) {
	MebContext log_ctx;
	meb_init(&log_ctx, "debug.txt");
	meb_log_level(&log_ctx, MEB_INFO);
	meb_prof_mode(&log_ctx, MEB_MILLISECONDS);

    TuimContext ctx;
    ctx.backend = tuim_windows_backend();
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

    TuimText text = tuim_default_text();
    text.area.x = 53;
    text.area.y = 0;

    while (1) {
		meb_log(&log_ctx, "Starting frame");
		meb_prof_start(&log_ctx);

        tuim_begin_frame(&ctx);
        tuim_update_input(&ctx);

        tuim_window_manager_update(&ctx, &manager);
        tuim_window_manager_draw  (&ctx, &manager);

        tuim_end_frame(&ctx);

		meb_log(&log_ctx, "Ending frame");
		meb_prof_end(&log_ctx);
    }

	tuim_destroy_context(&ctx);
	meb_close(&log_ctx);

    return 0;
}