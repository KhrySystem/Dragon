list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/tools/build")

message(WARNING ${CMAKE_MODULE_PATH})
find_package(Dragon)

set(BOOST_SUBDIRS "")
foreach(subdir ${BOOST_SUBDIR_ROOTS})
	set(subdir_include ${subdir}/include)
	list(APPEND BOOST_SUBDIRS ${subdir_include})
endforeach()

set(Dragon_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/headers/include ${CMAKE_CURRENT_SOURCE_DIR}/headers/glm ${CMAKE_CURRENT_SOURCE_DIR}/headers/openal/include ${BOOST_SUBDIRS} ${GLFW_SUBDIRS} ${Vulkan_INCLUDE_DIRS})
set(Dragon_LINK_LIBRARIES glfw ${Vulkan_LIBRARIES})
set(DRAGON_LINK_INTERFACES Boost glm)