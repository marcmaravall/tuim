# TUIM

Tuim is an open-source Terminal User Interface (TUI) and terminal engine made in C99, designed for terminal users interfaces (TUIs).

It's designed to be simple and customizable, with a custom backend architecture, so you can create your own tuim backend.

# Usage
Tuim is designed to be easy to integrate with your application. This project uses CMake as main build system, but you can use any buid system you want.
At this moment, tuim only has support for a windows backend, wich uses features from the win32 api. You can develop your own custom backend for any platform/environment (See the section Backend).

# Custom Backend creation
You can create your own backend creating these functions:
```C
void init(void* data);  
void destroy(void* data);   
void render (void* data, const TuimFrameBuffer* frame_buffer);

void get_size(void* data, size_t* x, size_t* y);
void pass_frame_buffer(void* data, const TuimFrameBuffer* frame_buffer);

void update_input(void* data, TuimInputState* input_state);
```
and then you can create the backend like this:
```
TuimBackend my_custom_backend() {
    TuimBackend backend;

    backend.init = my_custom_start;
    backend.destroy = my_custom_destroy;

    // ...

    return backend;
}
```

# License
Tuim is licensed under the MIT license, see LICENSE.txt for more information.
