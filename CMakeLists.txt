# TODO: refactor for better readability

cmake_minimum_required (VERSION 3.10)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("tuim")

if (WIN32)
	file(GLOB tuim_src "src/*.c" "src/backends/windows/windows_backend.c")
  add_executable(tuim ${tuim_src})

elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")

  file(GLOB tuim_src "src/*.c" "src/backends/linux/linux_backend.c")

  add_executable(tuim ${tuim_src})

  target_compile_options(tuim PRIVATE
    -Wall -Wextra -pedantic -std=c11
    -fsanitize=address,undefined
    -fno-omit-frame-pointer -g

    # this should be commented
    -Wno-unused-variable
    -Wno-unused-parameter
  )

  target_link_options(tuim PRIVATE
    -fsanitize=address,undefined
  )

else()
	message(FATAL_ERROR "Unsupported platform")
endif()

target_include_directories (tuim PRIVATE include)
target_include_directories (tuim PRIVATE "thirdparty/meb")
