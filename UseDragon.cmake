list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/tools/build")

macro(subdirlist RESULT CURDIR)
    file(GLOB CHILDREN ${CURDIR}/*) # This was changed
    set(DIRLIST "")
    foreach(CHILD ${CHILDREN})
        if(IS_DIRECTORY ${CHILD}) # This was changed
            list(APPEND DIRLIST ${CHILD})
        endif()
    endforeach()
    set(${RESULT} ${DIRLIST})
ENDMACRO()

subdirlist(GLFW_SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR}/headers/glfw)

find_package(Vulkan)
find_package(Boost)
find_package(Python REQUIRED COMPONENTS Interpreter)
find_package(Doxygen)

set(Dragon_INCLUDE_DIRS 
    ${CMAKE_CURRENT_SOURCE_DIR}/headers/include 
    ${CMAKE_CURRENT_SOURCE_DIR}/headers/glm 
    ${CMAKE_CURRENT_SOURCE_DIR}/headers/openal/include 
    ${Boost_INCLUDE_DIR} ${GLFW_SUBDIRS} ${Vulkan_INCLUDE_DIRS} 
    ${CMAKE_CURRENT_SOURCE_DIR}/extensions/ironbreath/include
    ${CMAKE_CURRENT_SOURCE_DIR}/extensions/lightbreath/include
    ${CMAKE_CURRENT_SOURCE_DIR}/extensions/streambreath/include
)
set(Dragon_LINK_LIBRARIES glfw ${Vulkan_LIBRARIES})
set(DRAGON_LINK_INTERFACES Boost glm)