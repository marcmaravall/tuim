#include "event.h"

TuimEvent tuim_default_event() {
	TuimEvent event;

	event.callback = NULL;
	event.type = TUIM_EVENT_NULL;

	return event;
}

TuimEvent tuim_event(TuimEventCallback callback, const TuimEventType type) {
	MEB_ASSERT(callback);
	TuimEvent event;
	event.callback = callback;
	event.type = type;
	return event;
}
