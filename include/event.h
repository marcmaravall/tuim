#ifndef TUIM_EVENT_H
#define TUIM_EVENT_H

#include <meb.h>
#include <stdio.h>

typedef void (*TuimEventCallback)(void* data);

typedef enum TuimEventType {
	TUIM_EVENT_NULL = 0,
	TUIM_EVENT_RESIZE = 1,
	// TODO: continue adding types: 
	// ...
	TUIM_EVENT_CUSTOM = 2,
} TuimEventType;

typedef struct TuimEvent {
	TuimEventType type;
	TuimEventCallback callback;
} TuimEvent;

TuimEvent tuim_default_event();
TuimEvent tuim_event(TuimEventCallback callback, const TuimEventType type);

#endif // TUIM_EVENT_H