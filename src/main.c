// this is only a test, not a part of the library
// it's only used to test the library internally and its not supposed to work very well
// made by claude.ai (only this test)

// ============================================================
//  TUIM LIBRARY - COMPREHENSIVE TEST
//  Tests: context, styles, windows, widgets, input, canvas,
//         callbacks, delta time, resize, multiple frames.
// ============================================================

// Source - https://stackoverflow.com/q/16883037
// Posted by Juan Martinez, modified by community. See post 'Timeline' for change history
// Retrieved 2026-04-29, License - CC BY-SA 3.0

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#define MEB_LOG_TO_FILE
#define MEB_IMPLEMENTATION
#include <meb.h>
#undef MEB_IMPLEMENTATION

#include <tuim.h>
#ifdef __linux__
#include <backends/linux/linux_backend.h>
#elif defined _WIN32
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <backends/windows/windows_backend.h>
#endif

#include <time.h>
#include <stdio.h>
#include <string.h>

// ============================================================
// SECTION 1 - Callbacks
// ============================================================

static int s_button_click_count = 0;

void on_button_click(void* user_data) {
    s_button_click_count++;
    MEB_LOGF("button clicked! total clicks: %d", s_button_click_count);
    (void)user_data;
}

void on_quit_button_click(void* user_data) {
    MEB_LOG("quit button clicked - requesting exit...");
    bool* should_quit = (bool*)user_data;
    *should_quit = true;
}

// ============================================================
// SECTION 2 - Helper: build the main test window
// ============================================================

static void build_main_window(
    TuimWindow* w,
    TuimCanvas* canvas,
    TuimTextList* log_list,
    TuimTextbox* textbox,
    TuimText* dt_text,
    TuimText* mouse_text,
    TuimText* key_text,
    TuimText* char_text,
    TuimText* click_text,
    TuimCheckbox* checkbox,
    TuimCheckbox* checkbox2,
    TuimButton* btn,
    TuimButton* quit_btn,
    bool* should_quit
) {
    // -- Window geometry --
    w->max_height = INT_MAX;
    w->max_width = INT_MAX;
    w->rect.width = 80;
    w->rect.height = 30;

    // -- Canvas (color fill test) --
    tuim_canvas_init(canvas, 20, 3);
    tuim_canvas_fill(canvas, TUIM_RED_STRUCT_INDEXED);
    tuim_window_add_element(w, tuim_canvas_to_element(canvas));

    // -- Text list (static labels) --
    *log_list = tuim_text_list(8);
    tuim_text_list_add(log_list, "  [TUIM TEST SUITE]");
    tuim_text_list_add(log_list, "  Press ESC to quit");
    tuim_text_list_add(log_list, "  Press A/B/C to test keys");
    tuim_text_list_add(log_list, "  Ctrl+Z / Alt+Z / Shift+Z for modifiers");
    tuim_text_list_add(log_list, "  Mouse LMB/RMB/MMB tracked below");
    tuim_text_list_add(log_list, "  Type any char to see it echoed");
    tuim_text_list_add(log_list, "  ----------------------------------------");
    tuim_window_add_element(w, tuim_text_list_to_element(log_list));

    // -- Dynamic text widgets --
    *dt_text = tuim_text("delta time : ---");
    *mouse_text = tuim_text("mouse      : ---");
    *key_text = tuim_text("key        : ---");
    *char_text = tuim_text("char input : ---");
    *click_text = tuim_text("btn clicks : 0");

    tuim_window_add_element(w, tuim_text_to_element(dt_text));
    tuim_window_add_element(w, tuim_text_to_element(mouse_text));
    tuim_window_add_element(w, tuim_text_to_element(key_text));
    tuim_window_add_element(w, tuim_text_to_element(char_text));
    tuim_window_add_element(w, tuim_text_to_element(click_text));

    // -- Textbox --
    *textbox = tuim_textbox("Type here...");
    tuim_window_add_element(w, tuim_textbox_to_element(textbox));

    // -- Checkboxes --
    *checkbox = tuim_checkbox("Enable feature A");
    *checkbox2 = tuim_checkbox("Enable feature B");
    tuim_window_add_element(w, tuim_checkbox_to_element(checkbox));
    tuim_window_add_element(w, tuim_checkbox_to_element(checkbox2));

    // -- Buttons --
    *btn = tuim_button_callbacks("Click me!", on_button_click, NULL, NULL, NULL);
    *quit_btn = tuim_button_callbacks ("[ QUIT ]", on_quit_button_click, NULL, NULL, should_quit);
    tuim_window_add_element(w, tuim_button_to_element(btn));
    tuim_window_add_element(w, tuim_button_to_element(quit_btn));
}

// ============================================================
// SECTION 4 - main
// ============================================================

int main(void) {
    MEB_INIT("debug.txt");
    MEB_LOG("=== TUIM test suite started ===");

    // --------------------------------------------------------
    // 4.1  Context init
    // --------------------------------------------------------
    TuimContext ctx;
#ifdef __linux__
    ctx.backend = tuim_linux_backend();
#elif defined _WIN32
    ctx.backend = tuim_windows_backend();
#endif
    tuim_init_context(&ctx);

    // --------------------------------------------------------
    // 4.2  Style test — start with default dark
    // --------------------------------------------------------
    ctx.style = tuim_style_default_dark();
    MEB_LOG("style: default dark");

    // --------------------------------------------------------
    // 4.3  Resize context
    // --------------------------------------------------------
    tuim_resize_context(&ctx, 120, 35);
    MEB_LOG("context resized to 120x35");

    // --------------------------------------------------------
    // 4.4  Build widgets & window
    // --------------------------------------------------------
    bool should_quit = false;

    TuimWindow   w;
    TuimCanvas   canvas;
    TuimTextList log_list;
    TuimTextbox  textbox;
    TuimText     dt_text, mouse_text, key_text, char_text, click_text;
    TuimCheckbox checkbox, checkbox2;
    TuimButton   btn, quit_btn;

    w = tuim_default_window();
    build_main_window(
        &w, &canvas, &log_list,
        &textbox,
        &dt_text, &mouse_text, &key_text, &char_text, &click_text,
        &checkbox, &checkbox2,
        &btn, &quit_btn,
        &should_quit
    );

    // --------------------------------------------------------
    // 4.5  State tracking
    // --------------------------------------------------------
    int   color_idx = 0;  // current canvas color index
    int   style_idx = 0;  // 0=dark, 1=light, 2=default
    char  last_char[2] = { 0 };
    char  last_key[32] = "none";
    char  last_mouse[32] = "none";

    MEB_LOG("entering main loop...");

    // --------------------------------------------------------
    // 4.6  Main loop
    // --------------------------------------------------------
    while (!should_quit) {
        tuim_begin_frame(&ctx);
        tuim_update_input(&ctx);

        // ---- Delta time ----
        double dt = tuim_get_delta_time(&ctx);
        tuim_text_format(&dt_text, "delta time : %.6f s", dt);

        // ---- Mouse buttons ----
        {
            bool lmb = tuim_is_mouse_button(&ctx, TUIM_MOUSE_BUTTON_LEFT);
            bool rmb = tuim_is_mouse_button(&ctx, TUIM_MOUSE_BUTTON_RIGHT);
            bool mmb = tuim_is_mouse_button(&ctx, TUIM_MOUSE_BUTTON_MIDDLE);

            if (lmb) { strncpy(last_mouse, "LMB", sizeof(last_mouse) - 1); }
            else if (rmb) { strncpy(last_mouse, "RMB", sizeof(last_mouse) - 1); }
            else if (mmb) { strncpy(last_mouse, "MMB", sizeof(last_mouse) - 1); }

            tuim_text_format(&mouse_text, "mouse      : %s", last_mouse);
        }

        // ---- Character input ----
        {
            char c = tuim_get_char(&ctx);
            if (c) {
                last_char[0] = c;
                last_char[1] = '\0';
                MEB_LOGF("char input: '%c' (0x%02X)", c, (unsigned char)c);
            }
            tuim_text_format(&char_text, "char input : %s", c ? last_char : "-");
        }

        // ---- Key tests ----
        {
            // Modifier combos
            if (tuim_is_ctrl(&ctx, TUIM_KEY_Z)) {
                strncpy(last_key, "Ctrl+Z", sizeof(last_key) - 1);
                MEB_LOG("key: Ctrl+Z");
            }
            else if (tuim_is_alt(&ctx, TUIM_KEY_Z)) {
                strncpy(last_key, "Alt+Z", sizeof(last_key) - 1);
                MEB_LOG("key: Alt+Z");
            }
            else if (tuim_is_shift(&ctx, TUIM_KEY_Z)) {
                strncpy(last_key, "Shift+Z", sizeof(last_key) - 1);
                MEB_LOG("key: Shift+Z");
            }
            else if (tuim_is_ctrl(&ctx, TUIM_KEY_C)) {
                strncpy(last_key, "Ctrl+C", sizeof(last_key) - 1);
                MEB_LOG("key: Ctrl+C");
            }
            else if (tuim_is_ctrl(&ctx, TUIM_KEY_V)) {
                strncpy(last_key, "Ctrl+V", sizeof(last_key) - 1);
                MEB_LOG("key: Ctrl+V");
            }
            // Plain keys
            else if (tuim_is_key_down(&ctx, TUIM_KEY_A)) {
                strncpy(last_key, "A", sizeof(last_key) - 1);
                MEB_LOG("key: A");
            }
            else if (tuim_is_key_down(&ctx, TUIM_KEY_B)) {
                strncpy(last_key, "B", sizeof(last_key) - 1);
                MEB_LOG("key: B");
            }
            else if (tuim_is_key_down(&ctx, TUIM_KEY_C)) {
                strncpy(last_key, "C", sizeof(last_key) - 1);
                MEB_LOG("key: C");
            }
            // Arrow keys
            else if (tuim_is_key_down(&ctx, TUIM_KEY_UP)) {
                strncpy(last_key, "UP", sizeof(last_key) - 1);
            }
            else if (tuim_is_key_down(&ctx, TUIM_KEY_DOWN)) {
                strncpy(last_key, "DOWN", sizeof(last_key) - 1);
            }
            else if (tuim_is_key_down(&ctx, TUIM_KEY_LEFT)) {
                strncpy(last_key, "LEFT", sizeof(last_key) - 1);
            }
            else if (tuim_is_key_down(&ctx, TUIM_KEY_RIGHT)) {
                strncpy(last_key, "RIGHT", sizeof(last_key) - 1);
            }
            // Tab / Enter / Backspace
            else if (tuim_is_key_down(&ctx, TUIM_KEY_TAB)) {
                strncpy(last_key, "TAB", sizeof(last_key) - 1);
            }
            else if (tuim_is_key_down(&ctx, TUIM_KEY_ENTER)) {
                strncpy(last_key, "ENTER", sizeof(last_key) - 1);
            }
            else if (tuim_is_key_down(&ctx, TUIM_KEY_BACKSPACE)) {
                strncpy(last_key, "BACKSPACE", sizeof(last_key) - 1);
            }
            else if (tuim_is_key_down(&ctx, TUIM_KEY_DELETE)) {
                strncpy(last_key, "DELETE", sizeof(last_key) - 1);
            }
            // ESC -> quit
            else if (tuim_is_key_down(&ctx, TUIM_KEY_ESCAPE)) {
                strncpy(last_key, "ESCAPE", sizeof(last_key) - 1);
                MEB_LOG("ESC pressed - exiting loop");
                should_quit = true;
            }

            tuim_text_format(&key_text, "key        : %s", last_key);
        }

        // ---- Click count display ----
        tuim_text_format(&click_text, "btn clicks : %d", s_button_click_count);

        // ---- Update & draw ----
        tuim_window_update(&ctx, &w);
        tuim_window_draw(&ctx, &w);

        tuim_end_frame(&ctx);
    }

    // --------------------------------------------------------
    // 4.7  Teardown
    // --------------------------------------------------------
    MEB_LOG("=== exiting - destroying context ===");
    tuim_destroy_context(&ctx);

#ifdef _WIN32
    _CrtDumpMemoryLeaks();
#endif

    MEB_LOG("=== TUIM test suite finished ===");
    MEB_CLOSE();

    return 0;
}