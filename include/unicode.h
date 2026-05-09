#ifndef TUIM_UNICODE_H
#define TUIM_UNICODE_H

#include <stdint.h>
#include "ascii.h"

// types: 
typedef unsigned char  tuim_utf8_t;
typedef unsigned short tuim_utf16_t;
// -----

int tuim_utf8_codepoint_len(const uint8_t* s);
int tuim_utf8_display_width(const uint8_t* s);

#endif // TUIM_UNICODE_H