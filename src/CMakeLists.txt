cmake_minimum_required(VERSION 3.22)

project(Dragon VERSION ${DRAGON_VERSION_MAJOR}.${DRAGON_VERSION_MINOR})

add_library(${PROJECT_NAME} ${DRAGON_BUILD_TYPE}

	${DRAGON_LIBRARY_DIR}/dragon_include_lib.hpp

	include/dragon/dragon${DRAGON_VERSION_MAJOR}${DRAGON_VERSION_MINOR}.hpp
	include/dragon/internal.hpp

	object/dgObject.hpp
	video/dgWindow.hpp video/dgWindow.cpp
	video/dgAsyncWindow.hpp
)

