cmake_minimum_required(VERSION 3.24)

project(Dragon)

option(DG_BUILD_STATIC "Build Dragon and its submodules as static libraries" OFF)
set(BUILD_SHARED_LIBS ON)

set(DRAGON_DG_BACKEND_H Dragon-Headers/include/dragon/dg_backend.h)
	if(EXISTS ${DRAGON_DG_BACKEND_H})
		file(STRINGS ${DRAGON_DG_BACKEND_H} DragonHeaderVersionLine REGEX "^#define DG_HEADER_VERSION")
		string(REGEX MATCHALL "[0-9]+" DragonHeaderVersion "${DragonHeaderVersionLine}")
		file(STRINGS  ${DRAGON_DG_BACKEND_H} DragonHeaderVersionLine2 REGEX "^#define DG_HEADER_VERSION_COMPLETE ")
    	string(REGEX MATCHALL "[0-9]+" DragonHeaderVersion2 "${DragonHeaderVersionLine2}")
		list(LENGTH DragonHeaderVersion2 _len)
		# versions follow the Vulkan standard, with an additional number in front of e.g. '0, 1, 2' instead of '1, 2'
		if(_len EQUAL 3)
			list(REMOVE_AT DragonHeaderVersion2 0)
		endif()
		list(APPEND DragonHeaderVersion2 ${DragonHeaderVersion})
		list(JOIN DragonHeaderVersion2 "." Dragon_VERSION)
endif()

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)

option(DG_BUILD_TESTS "Build Dragon-specific tests" ON)

find_package(Vulkan REQUIRED)

include(Dragon-Tools/build/DragonMacros.cmake)

subdirlist(BOOST_SUBDIR_ROOTS ./Dragon-Headers/boost/libs)
subdirlist(GLFW_SUBDIRS ./Dragon-Headers/glfw)

set(BOOST_SUBDIRS "")
foreach(subdir ${BOOST_SUBDIR_ROOTS})
	set(subdir_include ${subdir}/include)
	list(APPEND BOOST_SUBDIRS ${subdir_include})
endforeach()

set(Dragon_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/Dragon-Headers/include ${CMAKE_CURRENT_SOURCE_DIR}/Dragon-Headers/glm ${BOOST_SUBDIRS} ${GLFW_SUBDIRS} ${Vulkan_INCLUDE_DIRS})
set(Dragon_LINK_LIBRARIES glfw ${Vulkan_LIBRARIES})
set(DRAGON_LINK_INTERFACES Boost glm)

add_subdirectory(Dragon-Headers)
add_subdirectory(Dragon-Ironbreath)
add_subdirectory(Dragon-Source)

if(DG_BUILD_TESTS)
	add_subdirectory(Dragon-Tools)
endif()