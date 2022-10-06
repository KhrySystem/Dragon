cmake_minimum_required(VERSION 3.24)

project(Dragon)

option(DG_BUILD_STATIC "Build Dragon and its submodules as static libraries" OFF)
set(BUILD_SHARED_LIBS ON)

set(DRAGON_DG_BACKEND_H headers/include/dragon/dg_backend.h)
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



include(UseDragon.cmake)

add_subdirectory(headers)
add_subdirectory(src)
add_subdirectory(extensions/ironbreath)
add_subdirectory(extensions/streambreath)

if(DG_BUILD_TESTS)
	add_subdirectory(tools)
endif()