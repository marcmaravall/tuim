#include "string_format.h"

const char* tuim_format(const char* str, ...)
{
    va_list args;

    va_start(args, str);
    vsnprintf(tuim_string_format, TUIM_STRING_FORMAT_BUFFER_SIZE, str, args);
    va_end(args);

    return tuim_string_format;
}
