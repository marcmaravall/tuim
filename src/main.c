#include <tuim.h>
#include <backends/windows/windows_backend.h>

int main(void) {
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

    while (1) {
        tuim_begin_frame(&ctx);
        tuim_update_input(&ctx);

        tuim_window_manager_update(&ctx, &manager);
        tuim_window_manager_draw  (&ctx, &manager);

        tuim_end_frame(&ctx);
    }

    return 0;
}