cmake_minimum_required(VERSION 3.20)

set(LIBRARY_OUTPUT_PATH "${CMAKE_CURRENT_BINARY_DIR}")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_BINARY_DIR}")

if(POLICY CMP0077)
    cmake_policy(SET CMP0077 NEW)
endif()

set(TOOLS_FOLDER tools)
set(TESTS_FOLDER tests)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

project(Dragon VERSION 1.0.1.1 LANGUAGES CXX C)

include(CTest)
include(tools/cmake/build_vars.cmake)

find_package(Python REQUIRED COMPONENTS Interpreter)
find_package(Vulkan REQUIRED)
find_package(Doxygen)
find_package(GTest)

if(NOT BUILD_SHARED_LIBS)
	option(Dragon_BUILD_SHARED "Set to OFF to build static, ON for shared." OFF)
else()
	set(Dragon_BUILD_SHARED "Set to OFF to build static, ON for shared." ON)
endif()
set(BUILD_SHARED_LIBS ${Dragon_BUILD_SHARED})
set(DYNAMIC_LOADER ${Dragon_BUILD_SHARED})
set(USE_MSVC_RUNTIME_LIBRARY_DLL ${Dragon_BUILD_SHARED})

option(Dragon_BUILD_TESTS "Build Dragon-specific tests" ON)
set(GLFW_BUILD_TESTS ${Dragon_BUILD_TESTS})
set(GLFW_BUILD_EXAMPLES ${Dragon_BUILD_TESTS})

option(Dragon_BUILD_DOCS "Build Dragon Documentation" ON)
set(GLFW_BUILD_DOCS ${Dragon_BUILD_DOCS})

set(Dragon_LIB_NAME dragon-${Dragon_VERSION})

add_subdirectory(external/glfw)

add_subdirectory(include/dragon)
add_subdirectory(src)

if(Dragon_BUILD_TESTS)
	add_subdirectory(tools/tests)
endif()

include(tools/cmake/install.cmake)