#include "ascii.h"

bool tuim_is_control_ascii(const tuim_ascii_t c) {
	return c <= 31 || c == 127;
}

bool tuim_is_printable_ascii(const tuim_ascii_t c) {
	return c >= 32 && c <= 126;
}
