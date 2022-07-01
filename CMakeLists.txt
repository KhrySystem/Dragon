cmake_minimum_required(VERSION 3.22)
project(dgConfigure10)
option(BUILD_DRAGON_TESTS "Build the tests for Dragon." ON)
option(BUILD_SHARED_LIBS "Force-Build EVERYTHING as a DLL." OFF)

cmake_policy(SET CMP0077 NEW)

set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_EXAMPLES OFF)

set(BUILD_TESTS OFF)

set(DRAGON_LIBRARY_DIR ${CMAKE_SOURCE_DIR}/lib)
set(DRAGON_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(GLFW_VULKAN_STATIC ON)
set(SPIRV_TOOLS_BUILD_STATIC OFF)
set(SPIRV_HEADERS_SKIP_EXAMPLES OFF)

message(STATUS "Finding Python3")
find_package(Python3 QUIET)
if(Python3_FOUND)
	message(STATUS "Python3 - FOUND")

	if(Python3_Interpreter_FOUND)
		message(STATUS "Python3 Interpreter - FOUND" )
	else()
		message(STATUS "Python3 Interpreter - NOT FOUND")
	endif()

	if(Python3_Compiler_FOUND)
		message(STATUS "Python3 Compiler - FOUND")
	else()
		message(STATUS "Python3 Compiler - NOT FOUND")
	endif()

	if(Python3_Development_FOUND)
		message(STATUS "Python3 Development - FOUND")
	else()
		message(STATUS "Python3 Development -  NOT FOUND")
	endif()

else()
	message(WARNING "Python3 - NOT FOUND")
endif()

include(UseDragon.cmake)

add_subdirectory(src)
if(BUILD_DRAGON_TESTS)
	add_subdirectory(tests)
endif()