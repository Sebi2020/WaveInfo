cmake_minimum_required (VERSION 2.8.11)
project(WaveInfo)
add_executable(WaveInfo main.c util.c)
target_compile_options(WaveInfo PUBLIC -Wall -std=c99 -O9)
target_include_directories (WaveInfo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

