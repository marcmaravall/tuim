#ifndef TUIM_BOX_H
#define TUIM_BOX_H

#include "rect.h"

typedef enum {
	TUIM_BOX_TYPE_NONE,
	TUIM_BOX_TYPE_TEXT,
	TUIM_BOX_TYPE_BUTTON,
	TUIM_BOX_TYPE_LAYOUT,
} TuimBoxType;

typedef struct {
	TuimBoxType type;

} TuimBox;

#endif // TUIM_BOX_H