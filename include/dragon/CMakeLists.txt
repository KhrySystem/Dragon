cmake_minimum_required(VERSION 3.20)

file(GLOB_RECURSE Dragon_H_FILES ./*.h)
file(GLOB_RECURSE Dragon_HPP_FILES ./*.hpp)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/dragon_core.h.in ${CMAKE_CURRENT_SOURCE_DIR}/dragon_core.h)

add_library(Dragon-Headers INTERFACE
	${Dragon_H_FILES} ${Dragon_HPP_FILES}
)