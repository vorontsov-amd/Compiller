cmake_minimum_required(VERSION 3.2) 

project(Compiller)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O2")
set(CMAKE_BUILD_TYPE RELEASE)

set(STDLIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/stdlib/stdlib.bin")
set(OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/include/stdlibPath.h")
configure_file(./src/include/stdlibPath.h.in ${OUTPUT_PATH} @ONLY)

add_subdirectory(src)
