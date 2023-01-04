cmake_minimum_required(VERSION 3.20)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/dragon_core.h.in" "${Dragon_SOURCE_DIR}/include/dragon/dragon_core.h")

add_library(Dragon-Headers INTERFACE
	"dragon.hpp"
	"dragon_core.h.in"
	"engine.hpp"
	"init.hpp"
	"window.hpp"
)