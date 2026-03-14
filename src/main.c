// basic window demo, click and drag the window around

#include <tuim.h>
#include <backends/windows/windows_backend.h>

int main(void) {
    TuimContext ctx;
    ctx.backend = tuim_windows_backend();
    tuim_init_context(&ctx);
    
    tuim_init_context(&ctx);
	ctx.style = tuim_style_default_dark();
    
    TuimWidget example = tuim_default_widget();
    example.title = "This is an example window!";
    example.rect.x = 0;
    example.rect.y = 0;

    TuimWidget example1 = tuim_default_widget();
    example1.title = "This is another example";
    example1.rect.x = 60;
    example1.rect.y = 12;

    while (1) {
		tuim_begin_frame(&ctx);
		tuim_update_input(&ctx);

        tuim_widget_draw(&ctx, &example);
        tuim_widget_update(&ctx, &example);

        tuim_widget_draw(&ctx, &example1);
        tuim_widget_update(&ctx, &example1);

		tuim_end_frame(&ctx);
    }

    return 0;
}
