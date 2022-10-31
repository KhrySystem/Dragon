cmake_minimum_required(VERSION 3.20)

project(Dragon VERSION 1.0.0.0 LANGUAGES CXX)

option(DRAGON_BUILD_SHARED "Set to OFF to build static, ON for shared." OFF)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)

option(Dragon_BUILD_TESTS "Build Dragon-specific tests" ON)
option(Dragon_BUILD_DOCS "Build Dragon Documentation" ON)

find_package(Vulkan REQUIRED)
find_package(Boost REQUIRED COMPONENTS)
find_package(Python REQUIRED COMPONENTS Interpreter)
find_package(Doxygen)
find_package(OpenCL)
find_package(GTest)

if(Dragon_BUILD_SHARED) 
    set(BUILD_SHARED_LIBS ON)
    add_subdirectory(external/glfw)
    add_subdirectory(external/glm)
    add_subdirectory(external/openal)
    set(DYNAMIC_LOADER ON)
    add_subdirectory(external/openxr)
else()
    add_subdirectory(external/glfw)
    set(BUILD_STATIC_LIBS ON)
    add_subdirectory(external/glm)
    add_subdirectory(external/openal)
    add_subdirectory(external/openxr)
endif()

if(Vulkan_glslangValidator_FOUND)
macro(compile_shader OUTPUT INPUT)
    get_filename_component(FNAME ${INPUT} NAME)

    set(SPV_NAME ${CMAKE_BINARY_DIR}/shaders/${FNAME}.spv)

    add_custom_command(
        OUTPUT ${OUTPUT}
        COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_BINARY_DIR}/shaders/"
        COMMAND ${Vulkan_GLSLANG_VALIDATOR_EXECUTABLE} -V ${INPUT} -o ${SPV}
        DEPENDS ${INPUT}
    )

    list(APPEND SPV_BINARY_FILES)
endmacro()
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/headers/include/dragon/predef/core.hpp.in ${CMAKE_CURRENT_SOURCE_DIR}/headers/include/dragon/predef/core.hpp)

set(Dragon_INCLUDE_DIRS 
    ${Boost_INCLUDE_DIR} 
    ${Vulkan_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glfw/deps
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glfw/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glfw/src 
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glm
    ${CMAKE_CURRENT_SOURCE_DIR}/external/openal/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external/openxr/include
    ${CMAKE_CURRENT_SOURCE_DIR}/headers/include 
)

if(OpenCL_FOUND)
    list(APPEND Dragon_INCLUDE_DIRS ${OpenCL_INCLUDE_DIR})
endif()

if(Dragon_BUILD_DOCS AND Doxygen_FOUND)
    add_subdirectory(docs)
endif()

file(GLOB_RECURSE HPP_FILES ${Dragon_SOURCE_DIR}/headers/include/dragon/*.hpp)

add_subdirectory(src)

if(NOT Dragon_INSTALL_DIR)
    set(Dragon_INSTALL_DIR "C:/DragonSDK")
endif()

# The only test that is always build due to the directory reconfiguration requiring that a target is built in this script.
add_executable(openWindow tools/tests/openWindow.cpp)
target_include_directories(openWindow PUBLIC ${Dragon_INCLUDE_DIRS})
target_link_libraries(openWindow Dragon::Dragon)

if(Dragon_BUILD_TESTS)
    add_subdirectory(tools/tests)
endif()

add_custom_command(TARGET openWindow POST_BUILD
	COMMAND cd .. && ${Python_EXECUTABLE} package_dragon.py ${Dragon_INSTALL_DIR}
	DEPENDS package_dragon.py
)
