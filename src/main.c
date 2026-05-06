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

	TuimFrameBuffer fb = ctx.frame_buffer;

	while (1) {
		tuim_begin_frame(&ctx);
		tuim_update_input(&ctx);

		tuim_window_update(&ctx, &w);

		ctx.frame_buffer = fb;
		tuim_viewport_clear(&vp, TUIM_BLACK_STRUCT_INDEXED);

		ctx.frame_buffer = vp.frame_buffer;
		tuim_window_draw(&ctx, &w);
		ctx.frame_buffer = fb;
		tuim_viewport_draw(&ctx, &vp);

		tuim_end_frame(&ctx);
	}

	return 0;
}
