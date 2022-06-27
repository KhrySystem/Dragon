cmake_minimum_required(VERSION 3.22)

project(dgLibs)

set(CMAKE_POLICY_DEFAULT_CMP0002 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0072 NEW)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

set(GLFW_BUILD_TESTS OFF)
set(BUILD_TESTS OFF)
set(BUILD_UNIT_TESTS OFF)

add_library(
	${PROJECT_NAME} 

	SHARED 

	dragon_include_lib.hpp
)

if(BUILD_TESTS)
	add_subdirectory(googletest) # For Building tests. 
	set(Gtest_DIR C:/Dragon/bin/lib/googletest/googletest/generated)
endif()

add_subdirectory(Bullet3)
include(Bullet3/UseBullet.cmake)

add_subdirectory(GLFW)
target_link_libraries(${PROJECT_NAME} GLFW)

add_subdirectory(GLM)
target_link_libraries(${PROJECT_NAME} GLM)
target_include_directories(${PROJECT_NAME} PUBLIC GLM)


add_subdirectory(Robin-Hood-Hashing)
target_link_libraries(${PROJECT_NAME} robin_hood::robin_hood) 

add_subdirectory(SPIRV-Headers)
target_link_libraries(${PROJECT_NAME} SPIRV-Headers)

add_subdirectory(SPIRV-Tools)

add_subdirectory(GLSLang)
target_link_libraries(${PROJECT_NAME} glslang)
target_link_libraries(${PROJECT_NAME} HLSL)
target_link_libraries(${PROJECT_NAME} OSDependent)
target_link_libraries(${PROJECT_NAME} OGLCompiler)
target_link_libraries(${PROJECT_NAME} SPIRV)

add_subdirectory(Vulkan-Headers)

add_subdirectory(Vulkan-ExtensionLayer)

add_subdirectory(Vulkan-Loader)

add_subdirectory(Vulkan-ValidationLayers)

set(LIBS_BUILT ON)