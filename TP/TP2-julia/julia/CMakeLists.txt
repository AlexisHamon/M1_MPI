cmake_minimum_required(VERSION 3.1.0)
enable_testing()

project(philosophes)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set(CMAKE_C_FLAGS "-Wall -Wunused -Wpointer-arith -Wchar-subscripts -Wcomment -Wformat -Wwrite-strings -Wno-unused-function -Wno-unused-parameter -Werror -g ${CMAKE_C_FLAGS}")

add_executable(julia julia.c compute_julia_pixel.c write_bmp_header.c)
target_link_libraries(julia PRIVATE m Threads::Threads)
