#include <tuim.h>
#include <ansi.h>
#include <stdio.h>

int main(int argc, char** argv) {
	tuim_ansi_init();
	tuim_ansi_set_cursor_pos(5, 1);
	printf("hello, world!\n");
	while (1) {}
	return 0;
}
