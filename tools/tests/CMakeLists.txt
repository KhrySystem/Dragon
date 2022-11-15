cmake_minimum_required(VERSION 3.20)

macro(dragon_create_test TARGET SOURCES)
	add_executable(${TARGET} ${SOURCES})
	target_include_directories(${TARGET} PUBLIC ${Dragon_INCLUDE_DIRS})
	target_link_libraries(${TARGET} Dragon::Dragon)
	set_target_properties(${TARGET} PROPERTIES FOLDER ${TESTS_FOLDER})
endmacro()

dragon_create_test(test_dragon_open_window dragon_open_window.cpp)
dragon_create_test(test_dragon_audio_test dragon_audio_test.cpp)