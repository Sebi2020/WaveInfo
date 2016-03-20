cmake_minimum_required (VERSION 2.8.11)
project(WaveInfo)
set (WaveInfo_VERSION 1.0.0)
add_definitions(-DCMAKE_BUILD)
configure_file (
	"${PROJECT_SOURCE_DIR}/WaveInfoConfig.h.in"
	"${PROJECT_BINARY_DIR}/WaveInfoConfig.h"
)
add_executable(WaveInfo main.c util.c)
target_compile_options(WaveInfo PUBLIC -Wall -std=c99 -O9)
target_include_directories (WaveInfo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
include_directories("${PROJECT_BINARY_DIR}")