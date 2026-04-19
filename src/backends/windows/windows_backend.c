#include "backends/windows/windows_backend.h"

void tuim_windows_backend_init(void* backend_data) {
	MEB_ASSERT(backend_data);
	
	TuimWindowsBackendData* data = backend_data;
	data->window = GetConsoleWindow();
	data->handle = GetStdHandle(STD_OUTPUT_HANDLE);
	data->current_attributes = 0x07;
	data->resized = false;

	HWND input_handle = GetStdHandle(STD_INPUT_HANDLE);
	DWORD mode = 0;
	if (input_handle == INVALID_HANDLE_VALUE)
		return;
	if (!GetConsoleMode(data->handle, &mode))
		return;

	mode |= ENABLE_EXTENDED_FLAGS;
	mode |= ENABLE_MOUSE_INPUT;
	mode &= ~ENABLE_QUICK_EDIT_MODE;
	
	mode |= ENABLE_WINDOW_INPUT;

	SetConsoleMode(input_handle, mode);

	// remove resize:
	// this should be removed for certain backend user specs in the future
	/*LONG style = GetWindowLong(data->window, GWL_STYLE);

	style &= ~WS_SIZEBOX;
	style &= ~WS_MAXIMIZEBOX;

	SetWindowLong(data->window, GWL_STYLE, style);
	*/

	// remove scrollbar
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(data->handle, &csbi);

	data->current_attributes = csbi.wAttributes;

	COORD newSize;
	newSize.X = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	newSize.Y = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

	// TODO: remove cursor
}

void tuim_windows_backend_get_size(void* backend_data, size_t* x, size_t* y) {
	MEB_ASSERT(backend_data != NULL);
	
	TuimWindowsBackendData* data = backend_data;
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	if (!GetConsoleScreenBufferInfo(data->handle, &csbi)) {
		return;
	}

	*x = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	*y = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

void tuim_windows_backend_destroy(void* backend_data) {
	TuimWindowsBackendData* data = backend_data;
	free(data->buffer);
	free(data->shadow_buffer);
}

#ifndef TUIM_MAX_FRAME_BUFFER_SIZE
#define TUIM_MAX_FRAME_BUFFER_SIZE (65535)
#endif // TUIM_MAX_BUFFER_SIZE

void tuim_windows_backend_pass_frame_buffer(void* backend_data, TuimFrameBuffer* frame_buffer) {
	MEB_ASSERT(backend_data && frame_buffer);

	TuimWindowsBackendData* data = backend_data;

	if (data->resized) {
		size_t width = data->buffer_size.X;
		size_t height = data->buffer_size.Y;

		if (width * height == 0)
			return;

		CHAR_INFO* new_buffer = realloc(data->buffer, width * height * sizeof(CHAR_INFO));
		data->buffer = new_buffer;

		new_buffer = realloc(data->shadow_buffer, width * height * sizeof(CHAR_INFO));
		data->shadow_buffer = new_buffer;

		tuim_windows_backend_resize_console (
			data,
			(SHORT)width,
			(SHORT)height
		);

		tuim_frame_buffer_resize(
			frame_buffer,
			width,
			height
		);

		data->resized = false;
	}

	SHORT width = (SHORT)frame_buffer->width;
	SHORT height = (SHORT)frame_buffer->height;
	if (height != data->buffer_size.Y || width != data->buffer_size.X) {

		tuim_windows_backend_resize_buffer(data, width, height);
	}
	else {
		MEB_ASSERT(data->buffer);
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

	const SHORT height = data->buffer_size.Y;
	const SHORT width  = data->buffer_size.X;

	for (SHORT y = 0; y < height; y++) {
		SHORT first = -1, last = -1;

		for (SHORT x = 0; x < width; x++) {
			size_t i = y * width + x;

			if (memcmp(&data->buffer[i], &data->shadow_buffer[i], sizeof(CHAR_INFO)) != 0) {
				if (first == -1) first = x;
				last = x;
			}
		}

		if (first == -1) continue;

		SMALL_RECT region = { first, y, last, y };
		COORD bufferCoord = { first, y };

		WriteConsoleOutput(data->handle, data->buffer, data->buffer_size, bufferCoord, &region);
	}

	memcpy(data->shadow_buffer, data->buffer, width * height * sizeof(CHAR_INFO));

	/*WriteConsoleOutput(
		data->handle,
		data->buffer,
		data->buffer_size,
		bufferCoord,
		&region
	);*/
}

// TODO: do with table:
static WORD tuim_color_to_win32(const TuimColor color) {
	WORD win_color = 0;
	if (color.type == TUIM_COLOR_TYPE_INDEXED) {
		switch (color.color.indexed_color) {
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
				win_color = color.color.indexed_color % 16;
				break;
		}
	} else if (color.type == TUIM_COLOR_TYPE_RGB) {
		win_color |= (color.color.rgb_color.red > 128) ? FOREGROUND_RED : 0;
		win_color |= (color.color.rgb_color.green > 128) ? FOREGROUND_GREEN : 0;
		win_color |= (color.color.rgb_color.blue > 128) ? FOREGROUND_BLUE : 0;
		if (color.color.rgb_color.red > 128 || color.color.rgb_color.green > 128 || color.color.rgb_color.blue > 128)
			win_color |= FOREGROUND_INTENSITY;
	}
	return win_color;
}

void tuim_windows_backend_input_record_to_input_state(const INPUT_RECORD* record, TuimKeyboardState* input_state) {
	MEB_ASSERT(record);
	MEB_ASSERT(input_state);

	if (record->EventType == KEY_EVENT) {
		const KEY_EVENT_RECORD* key = &record->Event.KeyEvent;

		uint8_t key_code = (uint8_t)key->wVirtualKeyCode;
		size_t to_tuim = (size_t)tuim_windows_backend_win32_to_virtual_key(key_code);

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

// TODO: implement virtual keys for windows backend
void tuim_windows_backend_update_input(void* backend_data, TuimInputState* input_state) {
	MEB_ASSERT(backend_data && input_state);

	TuimWindowsBackendData* data = backend_data;
	data->char_pressed = 0;

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
			MOUSE_EVENT_RECORD mouse = record.Event.MouseEvent;

			COORD pos = mouse.dwMousePosition;

			input_state->mouse_state.mouse_x = pos.X;
			input_state->mouse_state.mouse_y = pos.Y;

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

		else if (record.EventType == WINDOW_BUFFER_SIZE_EVENT) {
			CONSOLE_SCREEN_BUFFER_INFO csbi;
			if (GetConsoleScreenBufferInfo(data->handle, &csbi)) {
				SHORT w = csbi.srWindow.Right - csbi.srWindow.Left + 1;
				SHORT h = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

				data->buffer_size.X = w;
				data->buffer_size.Y = h;
				
				data->resized = true;
			}
		}

		// TODO: combine with record to input state:
		// and implement unicode...
		else if (record.EventType == KEY_EVENT) {
			if (record.Event.KeyEvent.bKeyDown)
				data->char_pressed = record.Event.KeyEvent.uChar.AsciiChar;
		}

		tuim_windows_backend_input_record_to_input_state(
			&record,
			&input_state->keyboard_state
		);
	}
}

void tuim_windows_backend_resize_console(TuimWindowsBackendData* data, SHORT width, SHORT height) {
	COORD currentSize;
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	GetConsoleScreenBufferInfo(data->handle, &csbi);
	currentSize = csbi.dwSize;

	SMALL_RECT rect = { 0, 0, width - 1, height - 1 };
	COORD newSize = { width, height };
	
	// solved by chatgpt:
	if (width < currentSize.X || height < currentSize.Y) {
		SetConsoleWindowInfo(data->handle, TRUE, &rect);
		(data->handle, newSize);
	}
	else {
		(data->handle, newSize);
		SetConsoleWindowInfo(data->handle, TRUE, &rect);
	}
	// ---------
}

void tuim_windows_backend_resize_buffer(TuimWindowsBackendData* data, const SHORT width, const SHORT height) {
	MEB_ASSERT(data);

	data->buffer_size.X = (SHORT)width;
	data->buffer_size.Y = (SHORT)height;
	
	const size_t size = width * height * sizeof(CHAR_INFO);
	if (size == 0)
		return;

	CHAR_INFO* new_buffer = realloc(data->buffer, size);
	MEB_ASSERT(new_buffer);
	data->buffer = new_buffer;

	CHAR_INFO* new_shadow = realloc(data->shadow_buffer, size);
	MEB_ASSERT(new_shadow);
	data->shadow_buffer = new_shadow;
}

TuimBackend tuim_windows_backend() {
	TuimBackend backend;

	TuimWindowsBackendData* data = malloc(sizeof(TuimWindowsBackendData));
	MEB_ASSERT(data);

	// this should be changed to init
	data->buffer = NULL;
	data->shadow_buffer = NULL;
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
	backend.set_size = tuim_windows_backend_resize_console;
	backend.pass_frame_buffer = tuim_windows_backend_pass_frame_buffer;
	backend.update_input = tuim_windows_backend_update_input;

	backend.set_attrib = tuim_windows_backend_set_attrib;
	backend.attrib_supported = tuim_windows_backend_attrib_supported;
	backend.get_char = tuim_windows_backend_get_char;

	return backend;
}

// this shit dont works on Windows 11
void tuim_windows_backend_set_attrib(TuimWindowsBackendData* data, const tuim_backend_attrib_t attrib, const char* value) {
	MEB_ASSERT(data && attrib && value);

	if (attrib == TUIM_BACKEND_ATTRIB_SIZE_FIXED) {
		bool val = STR_TO_BOOL(value);
		LONG style = GetWindowLong(data->window, GWL_STYLE);

		if (val) {
			style &= ~WS_SIZEBOX;
			style &= ~WS_MAXIMIZEBOX;

		} else {

			style |= WS_SIZEBOX;
			style |= WS_MAXIMIZEBOX;
		}

		SetWindowLong(data->window, GWL_STYLE, style);
	}

	else {
		
	}
}

bool tuim_windows_backend_attrib_supported(TuimWindowsBackendData* data, const tuim_backend_attrib_t attrib) {
	MEB_ASSERT(data && attrib);

	return attrib < 2;	// currently only one attribute
}

char tuim_windows_backend_get_char(TuimWindowsBackendData* data) {
	MEB_ASSERT(data);

	return data->char_pressed;
}
