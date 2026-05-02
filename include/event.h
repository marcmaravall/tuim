// TODO: delete this shit, it's completely useless

#ifndef TUIM_EVENT_H
#define TUIM_EVENT_H

#include <meb.h>
#include <stdio.h>

#include "input.h"

typedef void (*TuimEventCallback)(void* data);

typedef enum TuimEventType {
	TUIM_EVENT_NULL = 0,
    TUIM_EVENT_FOCUS_GAINED = 2,
    TUIM_EVENT_FOCUS_LOST = 3,
	TUIM_EVENT_CUSTOM = 10,
} TuimEventType;

typedef struct TuimEvent {
	TuimEventType type;
	union {
		struct {
			bool focus_gained;
		};

		struct {
			bool focus_lost;
		};
	};
} TuimEvent;

TuimEvent tuim_default_event();
TuimEvent tuim_event(TuimEventCallback callback, const TuimEventType type);

#endif // TUIM_EVENT_H