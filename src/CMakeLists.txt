# Check if the library is to be built as static or shared
if(${Dragon_BUILD_STATIC})
	# Create a static library with the given name, and specify the sources to be compiled
	add_library(${Dragon_LIB_NAME} STATIC
		"init.cpp"
		"window.cpp"
		"gpu.cpp"
		"validation.cpp"
	)
else() 
	# Create a shared library with the given name, and specify the sources to be compiled
	add_library(${Dragon_LIB_NAME} SHARED 
		"init.cpp"
		"window.cpp"
		"gpu.cpp"
		"validation.cpp"
	)
	# Define the preprocessor symbol for shared library build
	target_compile_definitions(${Dragon_LIB_NAME} PUBLIC -DDRAGON_BUILD_DLL)
endif()

# Specify the include directories for the library
target_include_directories(${Dragon_LIB_NAME} PUBLIC ${Dragon_INCLUDE_DIRS})

# Link the library with the vulkan library
target_link_libraries(${Dragon_LIB_NAME} PUBLIC Vulkan::Vulkan)

# Link the library with the glfw library
target_link_libraries(${Dragon_LIB_NAME} PUBLIC glfw)

# Create an alias for the library
add_library(Dragon::Dragon ALIAS ${Dragon_LIB_NAME})