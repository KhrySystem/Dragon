cmake_minimum_required(VERSION 3.20)

project(Dragon VERSION 1.0.0.1 LANGUAGES CXX)

find_package(Python REQUIRED COMPONENTS Interpreter)
find_package(Boost REQUIRED)
find_package(Vulkan REQUIRED)
find_package(OpenCL)
if(OpenCL_FOUND)
    add_compile_definitions(DRAGON_OPENCL_FOUND)
endif()
find_package(Doxygen)
find_package(GTest)

set(TOOLS_FOLDER tools)

option(Dragon_BUILD_SHARED "Set to OFF to build static, ON for shared." OFF)

set(Dragon_LIB_NAME dragon-${Dragon_VERSION_MAJOR}_${Dragon_VERSION_MINOR})

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)

option(Dragon_BUILD_TESTS "Build Dragon-specific tests" ON)
set(GLFW_BUILD_TESTS ${Dragon_BUILD_TESTS})
option(Dragon_BUILD_DOCS "Build Dragon Documentation" ON)
set(GLFW_BUILD_DOCS ${Dragon_BUILD_DOCS})

if(Dragon_BUILD_SHARED) 
    set(BUILD_SHARED_LIBS ON)
    set(DYNAMIC_LOADER ON)
    add_subdirectory(external/glfw)
    add_subdirectory(external/glm)
    add_subdirectory(external/openal)
    add_subdirectory(external/openxr)
    add_compile_definitions(DRAGON_BUILD_DLL)
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

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/dragon/predef/core.hpp.in ${CMAKE_CURRENT_SOURCE_DIR}/include/dragon/predef/core.hpp)

set(Dragon_INCLUDE_DIRS 
    ${Boost_INCLUDE_DIR} 
    ${Vulkan_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glfw/deps
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glfw/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glfw/src 
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glm
    ${CMAKE_CURRENT_SOURCE_DIR}/external/openal/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external/openxr/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include 
)
if(OpenCL_FOUND)
    list(APPEND Dragon_INCLUDE_DIRS ${OpenCL_INCLUDE_DIR})
endif()
if(Dragon_BUILD_DOCS AND Doxygen_FOUND)
    add_subdirectory(docs)
endif()
file(GLOB_RECURSE HPP_FILES ${Dragon_SOURCE_DIR}/include/dragon/*.hpp)
add_subdirectory(src)
if(NOT Dragon_INSTALL_DIR)
    set(Dragon_INSTALL_DIR "C:/DragonSDK/${Dragon_VERSION}")
endif()
if(Dragon_BUILD_TESTS)
    add_subdirectory(tools/tests)
endif()

add_custom_target(post_build ALL # Installer file                              Dragon root directory  Binary location      Path to install to  Path to install FindDragon.cmake
    COMMAND ${Python_EXECUTABLE} ${Dragon_SOURCE_DIR}/tools/installer/post_build.py ${Dragon_SOURCE_DIR} ${Dragon_BINARY_DIR} ${Dragon_INSTALL_DIR} ${CMAKE_COMMAND} ${CMAKE_VERSION}
    DEPENDS tools/installer/post_build.py
)
add_dependencies(post_build Dragon::Dragon)
if(Dragon_BUILD_TESTS)
    add_dependencies(post_build 
        openWindow audioTest
        boing gears heightmap offscreen particles sharing simple splitview wave
        alrecord altonegen ex-common openal-info
    )
endif()
if(Dragon_BUILD_DOCS AND Doxygen_FOUND)
    add_dependencies(post_build Dragon_Docs docs)
endif()
set_target_properties(post_build PROPERTIES FOLDER ${TOOLS_FOLDER})