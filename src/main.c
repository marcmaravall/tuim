// this is only a test, not a part of the library 
// it's only used to test the library internally and its not supposed to work very well
#include <tuim.h>
#ifdef __linux__
#include <backends/linux/linux_backend.h>
#elif defined _WIN32
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <backends/windows/windows_backend.h>
#endif

#define MEB_LOG_TO_FILE
#define MEB_IMPLEMENTATION
#include <meb.h>
#include <time.h>
#include <stdio.h>

MebContext log_ctx;

void on_button_click(void* user_data) {
	meb_log(&log_ctx, "Button clicked!");
}

int main(void) {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    meb_init(&log_ctx, "debug.txt");
    meb_log_level(&log_ctx, MEB_INFO);
    meb_prof_mode(&log_ctx, MEB_MILLISECONDS);

    TuimContext ctx;
#ifdef __linux__
    ctx.backend = tuim_linux_backend();
#elif defined _WIN32
    ctx.backend = tuim_windows_backend();
#endif
    tuim_init_context(&ctx);
    ctx.style = tuim_style_default_dark();

    int frames = 0;

    TuimWindow w = tuim_default_window();

    // --- TEXT 1 (static)
    /*TuimText t1 = tuim_default_text();
    t1.text = "hello, world!";
    TuimElement el1 = tuim_text_to_element(&t1);
    tuim_layout_add(&w.layout, &el1);

    // --- TEXT 2 (dynamic mouse state)
    TuimText t2 = tuim_default_text();
    char buffer_mouse[64];
    t2.text = buffer_mouse;
    TuimElement el2 = tuim_text_to_element(&t2);
    tuim_layout_add(&w.layout, &el2);

    // --- TEXT 3 (frame counter)
    TuimText t3 = tuim_default_text();
    char buffer_frames[64];
    t3.text = buffer_frames;
    TuimElement el3 = tuim_text_to_element(&t3);
    tuim_layout_add(&w.layout, &el3);

    // --- TEXT 4 (interactive)
    TuimText t4 = tuim_default_text();
    char buffer_input[64];
    t4.text = buffer_input;
    TuimElement el4 = tuim_text_to_element(&t4);
    tuim_layout_add(&w.layout, &el4);*/

    TuimText text;
	TuimElement e = tuim_window_add_text(&w, "this is a text", &text);
    TuimCheckbox button2;
    TuimElement e2 = tuim_window_add_checkbox(&w, "this is a checkbox", &button2);

    TuimButton button3;
    TuimElement e3 = tuim_window_add_button(&w, "button but better", &button3);
    button3.on_click = on_button_click;
    button3.user_data = &button3;

    TuimButton button = tuim_button("hla");

    while (1) {
        // meb_log(&log_ctx, "Starting frame");
        // meb_prof_start(&log_ctx);

        tuim_begin_frame(&ctx);
        tuim_update_input(&ctx);

        bool pressed = tuim_is_mouse_button(&ctx, TUIM_MOUSE_BUTTON_LEFT);

        // --- update dynamic text
        /*snprintf(buffer_mouse, sizeof(buffer_mouse),
            "mouse pressed: %s", pressed ? "yes" : "no");

        snprintf(buffer_frames, sizeof(buffer_frames),
            "frames: %d", frames);

        snprintf(buffer_input, sizeof(buffer_input),
            "press SPACE to change text");

        if (tuim_is_key_down(&ctx, ' ')) {
            t1.text = "you pressed SPACE!";
        }

        if (tuim_is_key_down(&ctx, 'R')) {
            t1.text = "hello, world!";
        }*/

        if (tuim_is_key_down(&ctx, 'A')) {
            break;
        }

        tuim_window_update(&ctx, &w);
        tuim_window_draw(&ctx, &w);

		// tuim_button_draw(&ctx, &button);
        // tuim_button_update(&ctx, &button);

        tuim_end_frame(&ctx);

        // meb_log(&log_ctx, "Ending frame");
        // meb_prof_end(&log_ctx);

        frames++;
    }

    tuim_destroy_context(&ctx);
    meb_close(&log_ctx);

    _CrtDumpMemoryLeaks();

    return 0;
}