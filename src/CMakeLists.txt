cmake_minimum_required(VERSION 3.22)

project(Dragon VERSION 0.1.0)



add_library(${PROJECT_NAME} SHARED

	${DRAGON_LIBRARY_DIR}/dragon_include_lib.hpp

	include/dragon/dragon10.hpp

	object/dgObject.hpp
	video/dgWindow.hpp video/dgWindow.cpp
)

