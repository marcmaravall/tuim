#include <tuim.h>
#ifdef _WIN32
#include <backends/windows/windows_backend.h>
#endif

#define MEB_LOG_TO_FILE
#define MEB_IMPLEMENTATION
#include <meb.h>
#define MDS_IMPLEMENTATION
#include <mds.h>

#include <stdio.h>

int main() {
	TuimContext ctx;
	tuim_init_with_backend(&ctx, tuim_windows_backend());
	ctx.style = tuim_style_default_dark();

	TuimViewport vp = tuim_viewport((TuimRect){50, 7, 30, 15});
	tuim_frame_buffer_clear(&vp.frame_buffer, TUIM_BLACK_STRUCT_INDEXED);

	TuimWindow w = tuim_default_window();

    while (1) {
	    TuimViewport global_vp = ctx.viewport;
        tuim_begin_frame(&ctx);
        tuim_update_input(&ctx);

        tuim_viewport_clear(&ctx.viewport, TUIM_BRIGHT_BLACK_STRUCT_INDEXED);

        int abs_mouse_x = ctx.input_state.mouse_state.mouse_x;
        int abs_mouse_y = ctx.input_state.mouse_state.mouse_y;

        ctx.input_state.mouse_state.mouse_x -= vp.x;
        ctx.input_state.mouse_state.mouse_y -= vp.y;

        ctx.viewport = vp;
        tuim_window_update(&ctx, &w);
        tuim_viewport_clear(&vp, TUIM_BLACK_STRUCT_INDEXED);
        tuim_window_draw(&ctx, &w);

        ctx.input_state.mouse_state.mouse_x = abs_mouse_x;
        ctx.input_state.mouse_state.mouse_y = abs_mouse_y;

        ctx.viewport = global_vp;

        tuim_end_frame(&ctx);

        Sleep(16);
    }

	return 0;
}
