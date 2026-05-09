#include "backends/windows/windows_backend.h"

void tuim_windows_backend_init(void* backend_data) {
	MEB_ASSERT(backend_data);
	
	TuimWindowsBackendData* data = backend_data;
	data->window = GetConsoleWindow();
	data->handle = GetStdHandle(STD_OUTPUT_HANDLE);
	data->current_attributes = 0x07;
	data->resized = false;

	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

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
	free(data->shadow_buffer);
	data->shadow_buffer = NULL;
}

#ifndef TUIM_MAX_FRAME_BUFFER_SIZE
#define TUIM_MAX_FRAME_BUFFER_SIZE (65535)
#endif // TUIM_MAX_BUFFER_SIZE

void tuim_windows_backend_pass_frame_buffer(void* backend_data, TuimFrameBuffer* frame_buffer) {
	MEB_ASSERT(backend_data && frame_buffer);
	TuimWindowsBackendData* data = backend_data;

	data->buffer = frame_buffer;

	if (data->resized) {
		size_t width = data->buffer_size.X;
		size_t height = data->buffer_size.Y;
		if (width * height == 0) return;

		tuim_windows_backend_resize_buffer(data, (SHORT)width, (SHORT)height);
		tuim_windows_backend_resize_console(data, width, height);
		tuim_frame_buffer_resize(frame_buffer, width, height);

		data->resized = false;
		return;
	}

	SHORT width = (SHORT)frame_buffer->width;
	SHORT height = (SHORT)frame_buffer->height;

	if (height != data->buffer_size.Y || width != data->buffer_size.X) {
		tuim_windows_backend_resize_buffer(data, width, height);
	}
}

// unicode helpers: 

static int utf8_to_utf16(const uint8_t* src, int byte_len, WCHAR out[2]) {
	int n = MultiByteToWideChar(
		CP_UTF8, 0,
		(LPCCH)src, byte_len,
		out, 2
	);
	return n;
}

// -----------------

// TODO: add support for complex unicode characters
void tuim_windows_backend_render(void* backend_data) {
	TuimWindowsBackendData* data = backend_data;
	TuimFrameBuffer* fb = data->buffer;
	if (!fb) return;

	const SHORT height = data->buffer_size.Y;
	const SHORT width = data->buffer_size.X;

	for (SHORT y = 0; y < height; y++) {

		SHORT first = -1, last = -1;

		for (SHORT x = 0; x < width; x++) {
			size_t i = (size_t)y * width + x;
			if (!tuim_frame_buffer_cell_equal (
				TUIM_FRAME_BUFFER_AT(fb, x, y),
				data->shadow_buffer[i])) {
				if (first == -1) 
					first = x;
				last = x;
			}
		}

		if (first == -1) 
			continue;

		WCHAR wline[4096];
		WORD  attrs[2048];
		int   wline_len = 0;
		int   attr_len = 0;

		for (SHORT x = first; x <= last; x++) {
			TuimFrameBufferCell* cell = &TUIM_FRAME_BUFFER_AT(fb, x, y);

			if (cell->display_width == 0 && cell->grapheme_length == 0) {
				attrs[attr_len++] = attrs[attr_len > 0 ? attr_len - 1 : 0];
				continue;
			}

			WORD attr = tuim_color_to_win32(cell->foreground_color)
				| (tuim_color_to_win32(cell->background_color) << 4);
			attrs[attr_len++] = attr;
			if (cell->display_width == 2) {
				attrs[attr_len++] = attr;
			}

			if (cell->grapheme_length == 0) {
				wline[wline_len++] = L' ';
			}
			else {
				WCHAR wch[2];
				int n = MultiByteToWideChar(
					CP_UTF8, 0,
					(LPCCH)cell->grapheme, cell->grapheme_length,
					wch, 2
				);

				for (int k = 0; k < n; k++)
					wline[wline_len++] = wch[k];
			}
		}

		COORD attr_coord = { first, y };
		DWORD written;
		WriteConsoleOutputAttribute(data->handle, attrs, (DWORD)attr_len, attr_coord, &written);
		WriteConsoleOutputCharacterW(data->handle, wline, (DWORD)wline_len, attr_coord, &written);
	}

	const size_t total = (size_t)width * height;
	memcpy(data->shadow_buffer, fb->cells, total * sizeof(TuimFrameBufferCell));
}

static WORD tuim_color_to_win32(const TuimColor color) {
	WORD win_color = 0;
	if (color.type == TUIM_COLOR_TYPE_INDEXED) {
		win_color = tuim_color_to_win32_table[color.color.indexed_color];
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
	data->unicode_pressed = 0;
	data->vk_pressed = 0;

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
			if (record.Event.KeyEvent.bKeyDown) {
				data->char_pressed = record.Event.KeyEvent.uChar.AsciiChar;
				data->unicode_pressed = record.Event.KeyEvent.uChar.UnicodeChar;
				data->vk_pressed = record.Event.KeyEvent.wVirtualKeyCode;
			}
		}

		tuim_windows_backend_input_record_to_input_state(
			&record,
			&input_state->keyboard_state
		);
	}
}

void tuim_windows_backend_resize_console(TuimWindowsBackendData* data, size_t width, size_t height) {
	COORD currentSize;
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	GetConsoleScreenBufferInfo(data->handle, &csbi);
	currentSize = csbi.dwSize;

	SMALL_RECT rect = { 0, 0, (WORD)width - 1, (WORD)height - 1 };
	COORD newSize = {(WORD)width, (WORD)height };
	
	// solved by chatgpt:
	if ((WORD)width < currentSize.X || (WORD)height < currentSize.Y) {
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

	data->buffer_size.X = width;
	data->buffer_size.Y = height;

	const size_t count = (size_t)width * height;
	if (count == 0) 
		return;

	TuimFrameBufferCell* new_shadow = realloc(data->shadow_buffer, count * sizeof(TuimFrameBufferCell));
	MEB_ASSERT(new_shadow);
	data->shadow_buffer = new_shadow;

	memset(data->shadow_buffer, 0xFF, count * sizeof(TuimFrameBufferCell));
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
	backend.inp_rep = tuim_windows_backend_inp_rep;
	backend.get_clipboard = tuim_windows_backend_get_clipboard;
	backend.get_delta_time = tuim_windows_backend_get_delta_time;

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

bool tuim_windows_backend_inp_rep(TuimWindowsBackendData* data, const tuim_key_code_t key) {
	MEB_ASSERT(data);

	if (data->vk_pressed != 0)
		return data->vk_pressed == tuim_to_win32_vk[key];
	return false;
}

// TODO: support unicode and solve errors:
char* tuim_windows_backend_get_clipboard(TuimWindowsBackendData* data) {
	MEB_ASSERT(data);

	if (!OpenClipboard(NULL)) {
		MEB_LOG_ERROR("Cannot open clipboard on win32 backend!");
		return NULL;
	}

	HANDLE h_data = GetClipboardData(CF_TEXT);
	if (!h_data) {
		CloseClipboard();
		MEB_LOG_ERROR("Error getting clipboard data!");
		return NULL;
	}

	char* p_text = (char*)GlobalLock(h_data);
	if (!p_text) {
		MEB_LOG_ERROR("p_text is null!");
		CloseClipboard();
		return NULL;
	}

	char* result = _strdup(p_text);

	GlobalUnlock(h_data);
	CloseClipboard();

	MEB_LOG_INFO (result);

	return result;
}

static LARGE_INTEGER frequency;
static LARGE_INTEGER last_time;
static bool initialized = false;

double tuim_windows_backend_get_delta_time(TuimWindowsBackendData* data) {
	if (!initialized) {
		QueryPerformanceFrequency(&frequency);
		QueryPerformanceCounter(&last_time);
		initialized = true;

		return 0.0f;
	}

	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);

	double elapsedTicks = (double)(currentTime.QuadPart - last_time.QuadPart);
	float deltaTime = (float)(elapsedTicks / (double)frequency.QuadPart);
	last_time = currentTime;

	return deltaTime;
}
