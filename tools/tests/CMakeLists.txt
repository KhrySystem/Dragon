macro(dragon_create_test TARGET SOURCES)
	add_executable(${TARGET} ${SOURCES})
	target_include_directories(${TARGET} PUBLIC 
		${Dragon_SOURCE_DIR}/include
		${Dragon_SOURCE_DIR}/external/glm
		${Dragon_SOURCE_DIR}/external/boost/predef/include
	)
	target_link_libraries(${TARGET} Dragon::Dragon)
	set_target_properties(${TARGET} PROPERTIES FOLDER ${TESTS_FOLDER})
endmacro()

dragon_create_test(window open_window.cpp)
dragon_create_test(dginfo dginfo.cpp)

dragon_create_test(unit_test unit_test.cpp)

add_test(NAME tests COMMAND $<TARGET_FILE:unit_test>)
if(DRAGON_INSTALL)
	install(TARGETS window DESTINATION ${CMAKE_INSTALL_BINDIR})
	install(TARGETS dginfo DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()