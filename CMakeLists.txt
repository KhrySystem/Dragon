cmake_minimum_required(VERSION 3.20)

if(POLICY CMP0077)
    cmake_policy(SET CMP0077 NEW)
endif()

set(TOOLS_FOLDER tools)
set(TESTS_FOLDER tests)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)

project(Dragon VERSION 1.0.1.0)

option(Dragon_BUILD_SHARED "Set to OFF to build static, ON for shared." ON)
option(Dragon_BUILD_TESTS "Build Dragon-specific tests" ON)
option(Dragon_BUILD_DOCS "Build Dragon Documentation" ON)

set(Dragon_LIB_NAME dragon-${Dragon_VERSION_MAJOR}_${Dragon_VERSION_MINOR})
set(GLFW_BUILD_TESTS ${Dragon_BUILD_TESTS})
set(GLFW_BUILD_EXAMPLES ${Dragon_BUILD_TESTS})
set(GLFW_BUILD_DOCS ${Dragon_BUILD_DOCS})

find_package(Python REQUIRED COMPONENTS Interpreter)
find_package(Boost REQUIRED)
find_package(Vulkan REQUIRED)
find_package(Doxygen)
find_package(GTest)

set(BUILD_STATIC_LIBS ON)
if(Dragon_BUILD_SHARED) 
    message("Building submodules as shared libraries")
    set(DYNAMIC_LOADER ON)
    add_subdirectory(external/glm)
    set(BUILD_SHARED_LIBS ON)
    add_subdirectory(external/glfw)
    add_subdirectory(external/openal)
    add_subdirectory(external/openxr)
    add_compile_definitions(DRAGON_DLL)
else()
    message("Building submodules as static libraries")
    set(USE_MSVC_RUNTIME_LIBRARY_DLL OFF)
    add_subdirectory(external/glfw)
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
        COMMENT "Building SPIR-V shader ${OUTPUT} from ${INPUT}"
    )

    list(APPEND SPV_BINARY_FILES)
endmacro()
endif()

add_subdirectory(include/dragon)

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
    ${CMAKE_CURRENT_BINARY_DIR}/include/dragon
)
if(Dragon_BUILD_DOCS AND Doxygen_FOUND)
    add_subdirectory(docs)
endif()
add_subdirectory(src)
if(NOT Dragon_INSTALL_DIR)
    set(Dragon_INSTALL_DIR "C:/DragonSDK/${Dragon_VERSION}")
endif()
if(Dragon_BUILD_TESTS)
    add_subdirectory(tools/tests)
endif()

install(TARGETS Dragon-Headers
    DESTINATION "include"
)

install(TARGETS ${Dragon_LIB_NAME} audioTest
        DESTINATION "bin/tests"
)
