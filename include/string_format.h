#ifndef TUIM_STRING_FORMAT_H
#define TUIM_STRING_FORMAT_H

#include <stdio.h>
#include <stdarg.h>

#ifndef TUIM_STRING_FORMAT_BUFFER_SIZE
#define TUIM_STRING_FORMAT_BUFFER_SIZE 256
#endif//TUIM_STRING_FORMAT_BUFFER_SIZE

char tuim_string_format[TUIM_STRING_FORMAT_BUFFER_SIZE];

const char* tuim_format(const char* str, ...);

#endif //TUIM_STRING_FORMAT_H