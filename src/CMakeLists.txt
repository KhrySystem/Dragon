cmake_minimum_required(VERSION 3.10)

set(DRAGON_SOURCE_DIR ${CMAKE_SYSTEM_PROCESSOR}/${CMAKE_SYSTEM_NAME})

add_library(Dragon SHARED 
	${DRAGON_SOURCE_DIR}/dgEngine.cpp ${DRAGON_SOURCE_DIR}/initialization.cpp
	${DRAGON_SOURCE_DIR}/graphics/window.cpp
)