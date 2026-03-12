// basic window demo, click and drag the window around

#include <tuim.h>
#include <backends/windows/windows_backend.h>

int main(void) {
    TuimContext ctx;
    tuim_init_context(&ctx);
    ctx.backend = tuim_windows_backend();
    
    tuim_init_context(&ctx);
	ctx.style = tuim_style_default_dark();
    
    TuimWidget example;
    example.rect.x = 0;
    example.rect.y = 0;
    example.rect.width  = 20;
    example.rect.height = 5;
    example.flags = 0;
    example.id = 69;

    while (1) {
		tuim_begin_frame(&ctx);
		tuim_update_input(&ctx);

        tuim_widget_draw(&example, &ctx.frame_buffer);
        tuim_widget_update(&example, &ctx.input_state.mouse_state);

		tuim_end_frame(&ctx);
    }

    return 0;
}
