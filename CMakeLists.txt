cmake_minimum_required(VERSION 3.22)

option(BUILD_TESTS "Build the tests for Dragon." OFF)

add_subdirectory(src)
add_subdirectory(tests)