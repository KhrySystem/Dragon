cmake_minimum_required(VERSION 3.20)

project(Dragon VERSION 0.0.1.0 LANGUAGES CXX)

option(DG_BUILD_STATIC "Build Dragon and its submodules as static libraries" OFF)
set(DRAGON_INSTALL_DIR "E:/DragonSDK")
set(BUILD_SHARED_LIBS ON)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)

option(DG_BUILD_TESTS "Build Dragon-specific tests" ON)
option(DG_BUILD_DOCS "Build Dragon Documentation" ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/tools/build")

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

find_package(Vulkan REQUIRED)
find_package(Boost REQUIRED COMPONENTS)
find_package(Python REQUIRED COMPONENTS Interpreter)
find_package(Doxygen REQUIRED)
find_package(OpenCL)

set(Dragon_INCLUDE_DIRS 
    ${CMAKE_CURRENT_SOURCE_DIR}/headers/include 
    ${CMAKE_CURRENT_SOURCE_DIR}/headers/glm 
    ${CMAKE_CURRENT_SOURCE_DIR}/headers/openal/include 
    ${Boost_INCLUDE_DIR} ${GLFW_SUBDIRS} ${Vulkan_INCLUDE_DIRS} ${OpenCL_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/extensions/firebreath/include
    ${CMAKE_CURRENT_SOURCE_DIR}/extensions/ironbreath/include
    ${CMAKE_CURRENT_SOURCE_DIR}/extensions/lightbreath/include
    ${CMAKE_CURRENT_SOURCE_DIR}/extensions/streambreath/include
    ${CMAKE_CURRENT_SOURCE_DIR}/extensions/thunderbreath/include
)
set(Dragon_LINK_LIBRARIES glfw ${Vulkan_LIBRARIES} ${Boost_LIBS})
set(DRAGON_LINK_INTERFACES Boost glm)

add_subdirectory(headers)
add_subdirectory(src)
add_subdirectory(extensions/firebreath)
add_subdirectory(extensions/ironbreath)
add_subdirectory(extensions/lightbreath)
add_subdirectory(extensions/streambreath)
if(OpenCL_FOUND)
	add_subdirectory(extensions/thunderbreath)
endif()

if(DG_BUILD_TESTS)
	add_executable(openWindow tools/tests/openWindow.cpp)
	target_include_directories(openWindow PUBLIC ${Dragon_INCLUDE_DIRS})
	target_link_libraries(openWindow PUBLIC Dragon::Dragon)
	target_link_libraries(openWindow INTERFACE ${Dragon_LINK_INTERFACES})
endif()

add_custom_command(TARGET openWindow POST_BUILD
	COMMAND cd .. && ${Python_EXECUTABLE} "package_dragon.py" ${DRAGON_INSTALL_DIR} ${CMAKE_BUILD_TYPE}
	DEPENDS package_dragon.py
)