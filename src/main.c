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

	TuimScrollView sw = tuim_scroll_view((TuimRect) { 40, 10, 30, 20 });
	tuim_layout_add_elements(
		&sw.layout,
		2,
		TUIM_ELEMENTS (
			tuim_text_element("Example test in the viewport"),
			tuim_checkbox_element("Example checkbox")
		)
	);

	while (1) {
		tuim_begin_frame(&ctx);
		tuim_update_input(&ctx);
		
		if (tuim_is_key_rep(&ctx, TUIM_KEY_LEFT)) {
			sw.scroll_x -= 1;
		}
		if (tuim_is_key_rep(&ctx, TUIM_KEY_RIGHT)) {
			sw.scroll_x += 1;
		}
		if (tuim_is_key_rep(&ctx, TUIM_KEY_UP)) {
			sw.scroll_y -= 1;
		}
		if (tuim_is_key_rep(&ctx, TUIM_KEY_DOWN)) {
			sw.scroll_y += 1;
		}

		tuim_scroll_view_update(&ctx, &sw);
		tuim_scroll_view_draw(&ctx, &sw);

		tuim_end_frame(&ctx);
	}

	tuim_destroy_context(&ctx);

	return 0;
}
