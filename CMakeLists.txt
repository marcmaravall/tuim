cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("tuim")

add_executable (tuim "src/main.c" "include/ansi.h" "src/ansi.c" "include/context.h" "src/context.c" "include/text.h" "src/text.c" "include/style.h" "src/style.c")

target_include_directories (tuim PRIVATE include)
