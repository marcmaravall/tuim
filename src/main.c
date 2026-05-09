// mouse test:
// in reality is not a mouse test...
// implementing...

#include <tuim.h>
#include <backends/abstract/abstract.h>

#define MEB_LOG_TO_FILE
#define MEB_IMPLEMENTATION
#include <meb.h>
#define MDS_IMPLEMENTATION
#include <mds.h>

#include <stdio.h>

int main() {
	TuimContext ctx;
	tuim_init_with_backend(&ctx, tuim_abstract_backend());
	tuim_set_style(&ctx, tuim_style_default_dark());

	TuimWindow window = tuim_window("UNICODE: ñà", (TuimRect){0, 0, 30, 10});
	
	tuim_window_set_bounds(&window, (TuimSizeHint) {
		.preferred_width = 30, 
		.preferred_height = 10, 
		.min_width = 20, 
		.min_height = 5, 
		.max_width = 100, 
		.max_height = 30
	});

	tuim_set_backend_attrib(&ctx, TUIM_BACKEND_ATTRIB_SIZE_FIXED, "false");

	tuim_window_add_elements (
		&window, 
		5,
		
		TUIM_ELEMENTS (
			tuim_text_element("This is a test of the mouse input system. You can drag this window around and resize it."),
			tuim_text_element("Hi"),
			tuim_button_element("Click me!"),
			tuim_checkbox_element("Check me!"),
			tuim_textbox_element ("Type here!")
		)
	);

	while (1) {
		tuim_begin_frame(&ctx);
		tuim_update_input(&ctx);


		tuim_window_update(&ctx, &window);
		tuim_window_draw(&ctx, &window);

		tuim_end_frame(&ctx);
	}

	tuim_window_destroy(&window);

	return 0;
}
