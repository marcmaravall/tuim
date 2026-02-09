#include <tuim.h>
#include <context.h>
#include <os.h>

#include <backend.h>
#include <backends/ansi/ansi_backend.h>

#include <text.h>
#include <stdio.h>

int main(void) {
    while (1) {
        TuimBackend ansi_backend = tuim_ansi_backend();

        ansi_backend.init(NULL);
        ansi_backend.clear(NULL);

        ansi_backend.set_cursor_pos(NULL, 5, 3);
        ansi_backend.set_foreground_color(NULL, TUIM_COLOR_BRIGHT_YELLOW);
        ansi_backend.set_background_color(NULL, TUIM_COLOR_RED);
        ansi_backend.render_text(NULL, "Hola desde Tuim");

        printf("\x1b[0m");
        ansi_backend.destroy(NULL);
    }

    return 0;
}
