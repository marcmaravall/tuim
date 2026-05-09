#include "unicode.h"

int tuim_utf8_codepoint_len(const uint8_t* s) {
    if (!s || !*s) 
        return 0;
    if (*s < 0x80) 
        return 1;
    if (*s < 0xE0) 
        return 2;
    if (*s < 0xF0) 
        return 3;
    return 4;
}

// this is copied from stack ovweflow
int tuim_utf8_display_width(const uint8_t* s) {
    uint32_t cp = 0;
    int len = tuim_utf8_codepoint_len(s);
    if (len == 1) 
        cp = s[0];
    else if (len == 2) 
        cp = ((s[0] & 0x1F) << 6) | (s[1] & 0x3F);
    else if (len == 3) 
        cp = ((s[0] & 0x0F) << 12) | ((s[1] & 0x3F) << 6) | (s[2] & 0x3F);
    else if (len == 4) 
        cp = ((s[0] & 0x07) << 18) | ((s[1] & 0x3F) << 12) | ((s[2] & 0x3F) << 6) | (s[3] & 0x3F);

    if ((cp >= 0x0300 && cp <= 0x036F) ||
        (cp >= 0x1AB0 && cp <= 0x1AFF) ||
        (cp >= 0x1DC0 && cp <= 0x1DFF) ||
        (cp >= 0x20D0 && cp <= 0x20FF)) 
        return 0;

    if ((cp >= 0x1100 && cp <= 0x115F) ||
        (cp >= 0x2E80 && cp <= 0x303E) ||
        (cp >= 0x3041 && cp <= 0x33BF) ||
        (cp >= 0x4E00 && cp <= 0x9FFF) ||
        (cp >= 0xAC00 && cp <= 0xD7AF) ||
        (cp >= 0xF900 && cp <= 0xFAFF) ||
        (cp >= 0xFF01 && cp <= 0xFF60) ||
        (cp >= 0x1F300 && cp <= 0x1FAFF)) 
        return 2;

    return 1;
}
