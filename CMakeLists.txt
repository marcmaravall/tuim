cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("tuim")

file(GLOB tuim_src "src/*.c" "src/backends/ansi/ansi_backend.c")
add_executable(tuim ${tuim_src} "src/backends/windows/windows_backend.c")

target_include_directories (tuim PRIVATE include)
