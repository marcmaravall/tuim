#include "size_hint.h"

TuimSizeHint tuim_size_hint_dynamic (const TuimRect rect) {
	TuimSizeHint sh;

	sh.min_width  = 1;
	sh.min_height = 1;

	sh.preferred_width  = rect.width;
	sh.preferred_height = rect.height;

	sh.max_width  = TUIM_SIZE_HINT_MAX;
	sh.max_height = TUIM_SIZE_HINT_MAX;

	return sh;
}

TuimSizeHint tuim_size_hint_static  (const TuimRect rect) {
	TuimSizeHint sh;

	sh.min_width = rect.width;
	sh.min_height = rect.height;
	
	sh.preferred_width = rect.width;
	sh.preferred_height = rect.height;
	
	sh.max_width = rect.width;
	sh.max_height = rect.height;

	return sh;
}
