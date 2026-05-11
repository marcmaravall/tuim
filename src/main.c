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
	tuim_set_style(&ctx, tuim_style_default_dark());

	TuimWindow* w = tuim_window("Tuim Basic Test:", (TuimRect) { 0, 0, 30, 10 });

	while (1) {
		tuim_begin_frame(&ctx);
		tuim_update_input(&ctx);

		tuim_window_update(&ctx, w);
		tuim_window_draw(&ctx, w);

		tuim_end_frame(&ctx);
	}

	tuim_window_destroy(w);
	tuim_destroy_context(&ctx);

	return 0;
}
