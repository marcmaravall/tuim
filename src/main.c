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
	
	TuimWindow* w = tuim_window(
		"Tuim Mouse Test", 
		TUIM_RECT (
			.x = tuim_get_context_width(&ctx)/4, .y = tuim_get_context_height(&ctx)/4,
			.width=tuim_get_context_width(&ctx)/2, 
			.height = tuim_get_context_height(&ctx)/2
		)
	);
	tuim_window_set_bounds(w, tuim_size_hint_static(w->rect));

	TuimText* pos = tuim_text("Mouse Position: ");
	TuimText* button = tuim_text("Mouse Button: ");

	tuim_window_add_elements (
		w,
		4,
		TUIM_ELEMENTS (
			tuim_text_to_element (pos),
			tuim_text_to_element (button),
			tuim_checkbox_element  ("Checkbox  "),
			tuim_button_element ("Click Me!"),
		)
	);

	while (1) {
		tuim_begin_frame(&ctx);
		tuim_update_input(&ctx);

		int x, y;
		tuim_get_mouse_position(&ctx, &x, &y);
		tuim_text_format(pos, "Mouse Position: (%d, %d)", x, y);
		
		if (tuim_is_mouse_button(&ctx, TUIM_MOUSE_BUTTON_LEFT))		   tuim_text_format(button, "Mouse Button: (LEFT)");
		else if (tuim_is_mouse_button(&ctx, TUIM_MOUSE_BUTTON_RIGHT))  tuim_text_format(button, "Mouse Button: (RIGHT)");
		else if (tuim_is_mouse_button(&ctx, TUIM_MOUSE_BUTTON_MIDDLE)) tuim_text_format(button, "Mouse Button: (MIDDLE)");
		else tuim_text_format(button, "Mouse Button: (NONE)");

		tuim_window_update(&ctx, w);
		tuim_window_draw(&ctx, w);

		tuim_end_frame(&ctx);
	}
	tuim_destroy_context(&ctx);
	tuim_window_destroy(w);

	return 0;
}
