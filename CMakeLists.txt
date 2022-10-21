cmake_minimum_required(VERSION 3.20)

project(Dragon VERSION 0.0.1.0 LANGUAGES CXX)

option(DRAGON_BUILD_STATIC "Build Dragon and its submodules as static libraries" OFF)
option(DRAGON_INSTALL_DIR "C:/DragonSDK")
set(BUILD_SHARED_LIBS ON)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)

option(Dragon_BUILD_TESTS "Build Dragon-specific tests" ON)
option(Dragon_BUILD_DOCS "Build Dragon Documentation" ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/tools/build")

find_package(Vulkan REQUIRED)
find_package(Boost REQUIRED COMPONENTS)
find_package(Python REQUIRED COMPONENTS Interpreter)
find_package(Doxygen)
find_package(OpenCL)
find_package(GTest)

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

macro(subdirlist RESULT CURDIR)
    file(GLOB CHILDREN ${CURDIR}/*) 
    set(DIRLIST "")
    foreach(CHILD ${CHILDREN})
        if(IS_DIRECTORY ${CHILD}) 
            list(APPEND DIRLIST ${CHILD})
        endif()
    endforeach()
    set(${RESULT} ${DIRLIST})
ENDMACRO()

subdirlist(GLFW_SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR}/headers/glfw)

set(Dragon_INCLUDE_DIRS 
    ${CMAKE_CURRENT_SOURCE_DIR}/headers/include 
    ${CMAKE_CURRENT_SOURCE_DIR}/headers/glm 
    ${CMAKE_CURRENT_SOURCE_DIR}/headers/openal/include 
    ${CMAKE_CURRENT_SOURCE_DIR}/headers/openxr/include
    ${Boost_INCLUDE_DIR} ${GLFW_SUBDIRS} ${Vulkan_INCLUDE_DIRS} ${OpenCL_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/extensions/firebreath/include
    ${CMAKE_CURRENT_SOURCE_DIR}/extensions/ironbreath/include
    ${CMAKE_CURRENT_SOURCE_DIR}/extensions/lightbreath/include
    ${CMAKE_CURRENT_SOURCE_DIR}/extensions/streambreath/include
    ${CMAKE_CURRENT_SOURCE_DIR}/extensions/thunderbreath/include
)
set(Dragon_LINK_LIBRARIES glfw ${Vulkan_LIBRARIES} ${Boost_LIBS} OpenAL::OpenAL)
set(DRAGON_LINK_INTERFACES Boost glm)

if(Dragon_BUILD_DOCS AND Doxygen_FOUND)
    add_subdirectory(docs)
endif()

add_subdirectory(headers)
add_subdirectory(extensions/firebreath)
add_subdirectory(extensions/ironbreath)
add_subdirectory(extensions/lightbreath)
add_subdirectory(extensions/streambreath)
if(OpenCL_FOUND)
    add_compile_definitions(DRAGON_OPENCL_FOUND)
	add_subdirectory(extensions/thunderbreath)
endif()

list(APPEND Dragon_EXTENSION_TARGETS Dragon::Firebreath Dragon::Ironbreath Dragon::Lightbreath Dragon::Streambreath Dragon::Thunderbreath)

add_subdirectory(src)

if(Dragon_BUILD_TESTS)
	add_executable(openWindow tools/tests/openWindow.cpp)
	target_include_directories(openWindow PUBLIC ${Dragon_INCLUDE_DIRS})
	target_link_libraries(openWindow PUBLIC Dragon::Dragon)
	target_link_libraries(openWindow INTERFACE ${Dragon_LINK_INTERFACES})
    if(GTest_FOUND)
        # GoogleTest targets
    endif()
endif()

add_custom_command(TARGET openWindow POST_BUILD
	COMMAND cd .. && ${Python_EXECUTABLE} "package_dragon.py" ${DRAGON_INSTALL_DIR} ${CMAKE_BUILD_TYPE}
	DEPENDS package_dragon.py
)
