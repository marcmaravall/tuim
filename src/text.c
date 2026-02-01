#include <text.h>

void tuim_draw_text(TuimContext* context, char* msg) {
	context->cursor_x += strlen(msg);
	context->cursor_y++;
	printf(msg);
	printf("\n");
}
