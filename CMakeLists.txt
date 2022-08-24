cmake_minimum_required(VERSION 3.10)

include(.cmake/DragonGetVersionNumber.info)

find_package(OpenGL REQUIRED)

project(Dragon)

cmake_policy(SET CMP0079 NEW)
cmake_policy(SET CMP0002 OLD)

include_directories(src)

include_directories( ${OPENGL_INCLUDE_DIRS} )

include_directories(lib/glfw)
include_directories(lib/glfw/deps)
include_directories(lib/glfw/src)
include_directories(lib/glfw/include)

add_subdirectory(lib)
add_subdirectory(headers)
add_subdirectory(src)
add_subdirectory(tools)