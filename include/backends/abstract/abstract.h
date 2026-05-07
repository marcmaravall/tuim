// This backend abstracts the backend implementation for different platforms

#ifndef TUIM_ABSTRACT_BACKEND_H
#define TUIM_ABSTRACT_BACKEND_H

#ifdef _WIN32
#include <backends/windows/windows_backend.h>
#define tuim_abstract_backend tuim_windows_backend

#elif __linux__
#include <backends/linux/linux_backend.h>
#define tuim_abstract_backend tuim_linux_backend

#else
#warning "No OS backend is supported for the current platform"
#endif

#endif // TUIM_ABSTRACT_BACKEND_H
