#include <tuim.h>
#include <backends/windows/windows_backend.h>

int main(void) {
    TuimContext ctx;
    ctx.backend = tuim_windows_backend();
    tuim_init_context(&ctx);
    ctx.style = tuim_style_default_dark();

	TuimButton button = tuim_default_button();
	button.area.x = 10;
	button.area.y = 10;
    button.label = "Clicked";
	button.area.width = 20;
    button.area.height = 2;
    button.toggled = true;

    while (1) {
        tuim_begin_frame(&ctx);
        tuim_update_input(&ctx);

		tuim_button_update  (&ctx, &button);
		tuim_button_draw    (&ctx, &button);

        tuim_end_frame(&ctx);
    }

    return 0;
}