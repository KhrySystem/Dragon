cmake_minimum_required(VERSION 3.24)

project(Dragon)

option(DG_BUILD_STATIC "Build Dragon and its submodules as static libraries" OFF)
set(BUILD_SHARED_LIBS ON)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)

option(DG_BUILD_TESTS "Build Dragon-specific tests" ON)
option(DG_BUILD_DOCS "Build Dragon Documentation" ON)

include(UseDragon.cmake)

add_subdirectory(headers)
add_subdirectory(src)
add_subdirectory(extensions/ironbreath)
add_subdirectory(extensions/streambreath)

if(DG_BUILD_TESTS)
	add_executable(openWindow tools/tests/openWindow.cpp)
	target_include_directories(openWindow PUBLIC ${Dragon_INCLUDE_DIRS})
	target_link_libraries(openWindow PUBLIC Dragon::Dragon)
	target_link_libraries(openWindow INTERFACE ${Dragon_LINK_INTERFACES})
endif()

add_custom_command(TARGET openWindow POST_BUILD
	COMMAND cd .. && ${Python_EXECUTABLE} "package_dragon.py" "E:/DragonSDK"
	DEPENDS package_dragon.py
)