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

    TuimWindow w = tuim_default_window();
    w.max_height = INT_MAX;
    w.max_width = INT_MAX;
    w.rect.width = 100;
    w.rect.height = 10;

    TuimCanvas canvas;
    tuim_canvas_init(&canvas, 10, 5);
    tuim_canvas_fill(&canvas, TUIM_RED_STRUCT_INDEXED);
    TuimElement cEl = tuim_canvas_to_element(&canvas);
    tuim_layout_add(&w.layout, cEl);

    tuim_resize_context(&ctx, 120, 30);
    tuim_set_backend_attrib(&ctx, TUIM_BACKEND_ATTRIB_SIZE_FIXED, "true");

    TuimTextList list = tuim_text_list(10);
    tuim_text_list_add(&list, "- hola ");
    tuim_text_list_add(&list, "- another list ");
    tuim_text_list_add(&list, "- another list elemet by the way ");

    tuim_window_add_element(&w, tuim_text_list_to_element(&list));

    while (1) {

        tuim_begin_frame(&ctx);
        tuim_update_input(&ctx);
        
        bool pressed = tuim_is_mouse_button(&ctx, TUIM_MOUSE_BUTTON_LEFT);

        if (tuim_is_key_down(&ctx, 'A')) {
            break;
        }

        tuim_window_update(&ctx, &w);
        tuim_canvas_fill(&canvas, TUIM_WHITE_STRUCT_INDEXED);

        tuim_window_draw(&ctx, &w);

        tuim_end_frame(&ctx);
    }

    tuim_destroy_context(&ctx);
    meb_close(&log_ctx);

    _CrtDumpMemoryLeaks();

    return 0;
}