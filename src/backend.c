#include "backend.h"

TuimBackend tuim_default_backend() {
	TuimBackend backend;
	
	backend.attrib_supported = NULL;
	backend.data = NULL;
	backend.destroy = NULL;
	backend.get_char = NULL;
	backend.get_clipboard = NULL;
	backend.get_delta_time = NULL;
	backend.get_size = NULL;
	backend.init = NULL;
	backend.inp_rep = NULL;
	backend.pass_frame_buffer = NULL;
	backend.render = NULL;
	backend.set_attrib = NULL;
	backend.set_size = NULL;
	backend.update_input = NULL;

	return backend;
}
