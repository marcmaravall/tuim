#include "backends/windows/windows_backend.h"

void tuim_windows_backend_init(void* backend_data) {
	assert(backend_data);
	
	TuimWindowsBackendData* data = backend_data;
	data->window = GetConsoleWindow();
	data->handle = GetStdHandle(STD_OUTPUT_HANDLE);
	data->current_attributes = 0x07;

	HWND input_handle = GetStdHandle(STD_INPUT_HANDLE);
	DWORD mode = 0;
	if (input_handle == INVALID_HANDLE_VALUE)
		return;
	if (!GetConsoleMode(data->handle, &mode))
		return;

	mode |= ENABLE_EXTENDED_FLAGS;
	mode |= ENABLE_MOUSE_INPUT;
	mode &= ~ENABLE_QUICK_EDIT_MODE;

	SetConsoleMode(input_handle, mode);

	// remove resize:
	LONG style = GetWindowLong(data->window, GWL_STYLE);

	style &= ~WS_SIZEBOX;
	style &= ~WS_MAXIMIZEBOX;

	SetWindowLong(data->window, GWL_STYLE, style);

	// remove scrollbar
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(data->handle, &csbi);

	data->current_attributes = csbi.wAttributes;

	COORD newSize;
	newSize.X = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	newSize.Y = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

	SetConsoleScreenBufferSize(data->handle, newSize);

	// TODO: remove cursor
}

void tuim_windows_backend_get_size(void* backend_data, size_t* x, size_t* y) {
	assert(backend_data != NULL);
	
	TuimWindowsBackendData* data = backend_data;
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	if (!GetConsoleScreenBufferInfo(data->handle, &csbi)) {
		return;
	}

	*x = csbi.dwSize.X;
	*y = csbi.dwSize.Y;
}

void tuim_windows_backend_destroy(void* data) {

}

void tuim_windows_backend_pass_frame_buffer(void* backend_data, const TuimFrameBuffer* frame_buffer) {
	assert(backend_data != NULL);
	assert(frame_buffer != NULL);

	TuimWindowsBackendData* data = backend_data;

	size_t width = frame_buffer->width;
	size_t height = frame_buffer->height;

	if (height != data->buffer_size.Y || width != data->buffer_size.X) {

		CHAR_INFO* new_buffer = realloc(
			data->buffer,
			width * height * sizeof(CHAR_INFO)
		);

		assert(new_buffer != NULL);

		data->buffer = new_buffer;

		data->buffer_size.X = (SHORT)width;
		data->buffer_size.Y = (SHORT)height;
	}

	size_t total = width * height;

	for (size_t i = 0; i < total; i++) {
		TuimFrameBufferCell cell = frame_buffer->cells[i];
		data->buffer[i].Char.AsciiChar = cell.ascii_char;

		data->buffer[i].Attributes = tuim_color_to_win32(cell.foreground_color)
			| (tuim_color_to_win32(cell.background_color) << 4);
	}
}

void tuim_windows_backend_render(void* backend_data) {
	TuimWindowsBackendData* data = backend_data;

	COORD bufferCoord = { 0,0 };
	SMALL_RECT region = { 0,0, data->buffer_size.X - 1, data->buffer_size.Y- 1 };

	WriteConsoleOutput(
		data->handle,
		data->buffer,
		data->buffer_size,
		bufferCoord,
		&region
	);
}

// TODO: do with table:
static WORD tuim_color_to_win32(const TuimColor color) {
	WORD win_color = 0;
	if (color.type == TUIM_COLOR_TYPE_INDEXED) {
		switch (color.indexed_color) {
			case TUIM_BLACK: win_color = 0; break;
			case TUIM_RED: win_color = 4; break;
			case TUIM_GREEN: win_color = 2; break;
			case TUIM_YELLOW: win_color = 6; break;
			case TUIM_BLUE: win_color = 1; break;
			case TUIM_MAGENTA: win_color = 5; break;
			case TUIM_CYAN: win_color = 3; break;
			case TUIM_WHITE: win_color = 7; break;
			case TUIM_BRIGHT_BLACK: win_color = 8; break;
			case TUIM_BRIGHT_RED: win_color = 12; break;
			case TUIM_BRIGHT_GREEN: win_color = 10; break;
			case TUIM_BRIGHT_YELLOW: win_color = 14; break;
			case TUIM_BRIGHT_BLUE: win_color = 9; break;
			case TUIM_BRIGHT_MAGENTA: win_color = 13; break;
			case TUIM_BRIGHT_CYAN: win_color = 11; break;
			case TUIM_BRIGHT_WHITE: win_color = 15; break;
			default:
				win_color = color.indexed_color % 16;
				break;
		}
	} else if (color.type == TUIM_COLOR_TYPE_RGB) {
		win_color |= (color.rgb_color.red > 128) ? FOREGROUND_RED : 0;
		win_color |= (color.rgb_color.green > 128) ? FOREGROUND_GREEN : 0;
		win_color |= (color.rgb_color.blue > 128) ? FOREGROUND_BLUE : 0;
		if (color.rgb_color.red > 128 || color.rgb_color.green > 128 || color.rgb_color.blue > 128)
			win_color |= FOREGROUND_INTENSITY;
	}
	return win_color;
}

void tuim_windows_backend_input_record_to_input_state(const INPUT_RECORD* record, TuimKeyboardState* input_state) {
	assert(record);
	assert(input_state);

	if (record->EventType == KEY_EVENT) {
		const KEY_EVENT_RECORD* key = &record->Event.KeyEvent;

		size_t key_code = key->wVirtualKeyCode;
		size_t to_tuim = tuim_windows_backend_win32_to_virtual_key(key_code);

		if (to_tuim != TUIM_KEY_UNKNOWN && key_code < TUIM_KEY_COUNT) {
			input_state->current[to_tuim] = key->bKeyDown;
		}
		else if (key_code < TUIM_KEY_COUNT) {
			input_state->current[key_code] = key->bKeyDown;
		}
	}
}

TuimVirtualKey tuim_windows_backend_win32_to_virtual_key(const uint8_t key) {
	if (key < 256)
		return tuim_win32_vk_to_tuim[key];

	return TUIM_KEY_UNKNOWN;
}

void tuim_windows_backend_update_input(void* backend_data, TuimInputState* input_state) {
	assert(backend_data);
	assert(input_state);

	HANDLE input = GetStdHandle(STD_INPUT_HANDLE);

	DWORD events = 0;
	GetNumberOfConsoleInputEvents(input, &events);

	if (events == 0)
		return;

	INPUT_RECORD record;
	DWORD read = 0;

	while (events--) {
		ReadConsoleInput(input, &record, 1, &read);

		if (record.EventType == MOUSE_EVENT) {
			if (record.Event.MouseEvent.dwEventFlags == 0) {
				if (record.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) {
					input_state->mouse_state.current |= TUIM_MOUSE_BUTTON_LEFT;
				} else {
					input_state->mouse_state.current &= ~TUIM_MOUSE_BUTTON_LEFT;
				}
				if (record.Event.MouseEvent.dwButtonState & RIGHTMOST_BUTTON_PRESSED) {
					input_state->mouse_state.current |= TUIM_MOUSE_BUTTON_RIGHT;
				} else {
					input_state->mouse_state.current &= ~TUIM_MOUSE_BUTTON_RIGHT;
				}
				if (record.Event.MouseEvent.dwButtonState & FROM_LEFT_2ND_BUTTON_PRESSED) {
					input_state->mouse_state.current |= TUIM_MOUSE_BUTTON_MIDDLE;
				} else {
					input_state->mouse_state.current &= ~TUIM_MOUSE_BUTTON_MIDDLE;
				}
			}
		}

		tuim_windows_backend_input_record_to_input_state(
			&record,
			&input_state->keyboard_state
		);
	}
}

TuimBackend tuim_windows_backend() {
	TuimBackend backend;

	TuimWindowsBackendData* data = malloc(sizeof(TuimWindowsBackendData));
	if (!data) return backend;

	data->buffer = NULL;
	data->buffer_size.X = 0;
	data->buffer_size.Y = 0;
	data->window = NULL;
	data->handle = NULL;
	data->current_attributes = 0;

	backend.data = data;

	backend.init = tuim_windows_backend_init;
	backend.destroy = tuim_windows_backend_destroy;
	backend.render = tuim_windows_backend_render;
	backend.get_size = tuim_windows_backend_get_size;
	backend.pass_frame_buffer = tuim_windows_backend_pass_frame_buffer;
	backend.update_input = tuim_windows_backend_update_input;

	return backend;
}
