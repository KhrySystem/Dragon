macro(dragon_create_test TARGET SOURCES)
	add_executable(${TARGET} ${SOURCES})
	target_include_directories(${TARGET} PUBLIC ${Dragon_INCLUDE_DIRS})
	target_link_libraries(${TARGET} Dragon::Dragon)
	set_target_properties(${TARGET} PROPERTIES FOLDER ${TESTS_FOLDER})
endmacro()

dragon_create_test(test_dg_open_window open_window.cpp)
dragon_create_test(test_dg_compute compute_test.cpp)

add_executable(unit_test unit_test.cpp ${Dragon_SOURCE_DIR}/external/munit/munit.c)
target_include_directories(unit_test PUBLIC ${Dragon_INCLUDE_DIRS} ${Dragon_SOURCE_DIR}/external/munit)
target_link_libraries(unit_test Dragon::Dragon)

add_test(NAME tests COMMAND $<TARGET_FILE:unit_test>)