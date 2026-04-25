#ifndef TUIM_ASCII_H
#define TUIM_ASCII_H

#include <stdbool.h>

typedef char tuim_ascii_t;

bool tuim_is_control_ascii	 (const tuim_ascii_t c);
bool tuim_is_printable_ascii (const tuim_ascii_t c);

#endif // TUIM_ASCII_H