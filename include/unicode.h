#ifndef TUIM_UNICODE_H
#define TUIM_UNICODE_H

#include "ascii.h"

// types: 
typedef unsigned char  tuim_utf8_t;
typedef unsigned short tuim_utf16_t;
typedef unsigned int   tuim_utf32_t;
// -----

bool tuim_is_printable_utf8 (const tuim_utf8_t  c);
bool tuim_is_printable_utf16(const tuim_utf16_t c);
bool tuim_is_printable_utf32(const tuim_utf32_t c);

#endif // TUIM_UNICODE_H