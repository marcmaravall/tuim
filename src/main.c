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
	tuim_set_target_fps(&ctx, 30.0);

	TuimText* debug = tuim_text("Scroll view");

	TuimScrollView sw = tuim_scroll_view((TuimRect) { 40, 0, 30, 20 });

	tuim_layout_add_elements(
		&sw.layout,
		4,
		TUIM_ELEMENTS(
			tuim_text_to_element(debug),
			tuim_text_element("Example test in the viewport"),
			tuim_checkbox_element("Example checkbox"),

			tuim_text_element(""),
		)
	);

	for (size_t i = 0; i < 20; ++i) {
		char buffer[32];
		snprintf(buffer, 32, "Element %zu", i);
		tuim_layout_add(&sw.layout, tuim_checkbox_element(buffer));
	}

	TuimWindow* win = tuim_window("Test Window", (TuimRect) { 0, 0, 80, 25 });
	tuim_window_add_elements(win, 1, TUIM_ELEMENTS(tuim_scroll_view_to_element(&sw)));

	while (1) {
		tuim_begin_frame(&ctx);
		tuim_update_input(&ctx);
		
		int scroll = tuim_get_mouse_scroll(&ctx);
		sw.scroll_y += scroll/60;

		tuim_text_format(debug, "FPS: %f   DELTA TIME: %f", tuim_get_fps(&ctx), tuim_get_delta_time(&ctx));

		tuim_window_update(&ctx, win);
		tuim_window_draw(&ctx, win);

		tuim_end_frame(&ctx);
	}

	tuim_destroy_context(&ctx);

	return 0;
}
