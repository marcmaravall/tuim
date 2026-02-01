#include <tuim.h>
#include <context.h>
#include <os.h>
#include <text.h>
#include <stdio.h>

int main(int argc, char** argv) {
	tuim_os_init();

	TuimContext* ctx;
	ctx = malloc(sizeof(TuimContext));

	tuim_init_context(ctx);

	TuimOsConsoleBufferInfo buffer_info;
	tuim_os_get_console_buffer_info(&buffer_info);
	
	printf("Console Size { WIDTH: %d HEIGHT: %d } Cursor Position { X: %d Y: %d }",
		buffer_info.width, buffer_info.height,
		buffer_info.cursor_x, buffer_info.cursor_y);

	tuim_destroy_context(ctx);
	while (1) {}
	return 0;
}
