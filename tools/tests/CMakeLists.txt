macro(dragon_create_test TARGET SOURCES)
	add_executable(${TARGET} ${SOURCES})
	target_include_directories(${TARGET} PUBLIC ${Dragon_INCLUDE_DIRS})
	target_link_libraries(${TARGET} Dragon::Dragon)
	set_target_properties(${TARGET} PROPERTIES FOLDER ${TESTS_FOLDER})
endmacro()

dragon_create_test(test_dg_open_window open_window.cpp)
dragon_create_test(test_dg_compute compute_test.cpp)
dragon_create_test(unit_eval_functions unit_test_eval_functions.cpp)

add_test(NAME unit_eval_functions COMMAND $<TARGET_FILE:unit_eval_functions>)