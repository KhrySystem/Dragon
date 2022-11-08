cmake_minimum_required(VERSION 3.20)

if(Dragon_IS_TOP_LEVEL AND Dragon_BUILD_SHARED)
	remove_definitions(-DDRAGON_BUILD_DLL)
	add_compile_definitions(DRAGON_CALL_DLL)
endif()

add_executable(openWindow openWindow.cpp)
target_include_directories(openWindow PUBLIC ${Dragon_INCLUDE_DIRS})
target_link_libraries(openWindow Dragon::Dragon)
set_target_properties(openWindow PROPERTIES FOLDER ${TESTS_FOLDER})

add_executable(audioTest audioTest.cpp)
target_include_directories(audioTest PUBLIC ${Dragon_INCLUDE_DIRS})
target_link_libraries(audioTest Dragon::Dragon)
set_target_properties(audioTest PROPERTIES FOLDER ${TESTS_FOLDER})
