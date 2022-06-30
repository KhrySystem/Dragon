cmake_minimum_required(VERSION 3.22)

project(Dragon VERSION 0.1.0)



add_library(${PROJECT_NAME} SHARED

	${DRAGON_LIBRARY_DIR}/dragon_include_lib.hpp

	video/dgVideoInclude.hpp
	video/dgWindow.hpp video/dgWindow.cpp
)

