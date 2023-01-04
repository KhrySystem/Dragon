if(${Dragon_BUILD_STATIC})
	add_library(${Dragon_LIB_NAME} STATIC
		"init.cpp"
		"window.cpp"
		"gpu.cpp"
		"validation.cpp"
	)
else() 
	add_library(${Dragon_LIB_NAME} SHARED 
		"init.cpp"
		"window.cpp"
		"gpu.cpp"
		"validation.cpp"
	)
	target_compile_definitions(${Dragon_LIB_NAME} PUBLIC -DDRAGON_BUILD_DLL)
endif()

target_include_directories(${Dragon_LIB_NAME} PUBLIC ${Dragon_INCLUDE_DIRS})
target_link_libraries(${Dragon_LIB_NAME} PUBLIC Vulkan::Vulkan)
target_link_libraries(${Dragon_LIB_NAME} PUBLIC glfw)

add_library(Dragon::Dragon ALIAS ${Dragon_LIB_NAME})