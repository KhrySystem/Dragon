
    

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

if(${Vulkan_glslc_FOUND})
	add_custom_command(
		OUTPUT "${Dragon_BINARY_DIR}/shaders/frag.spv" "${Dragon_BINARY_DIR}/shaders/vert.spv"
		COMMAND ${CMAKE_COMMAND} -E make_directory ${Dragon_BINARY_DIR}/shaders/
		COMMAND ${Vulkan_GLSLC_EXECUTABLE} ${Dragon_SOURCE_DIR}/src/shaders/shader.frag -o ${Dragon_BINARY_DIR}/shaders/frag.spv --target-env=vulkan1.2
		COMMAND ${Vulkan_GLSLC_EXECUTABLE} ${Dragon_SOURCE_DIR}/src/shaders/shader.vert -o ${Dragon_BINARY_DIR}/shaders/vert.spv --target-env=vulkan1.2
		DEPENDS "${Dragon_SOURCE_DIR}/src/shaders/shader.frag" "${Dragon_SOURCE_DIR}/src/shaders/shader.vert"
	)

	add_custom_target(Shaders
		DEPENDS "${Dragon_BINARY_DIR}/shaders/frag.spv" "${Dragon_BINARY_DIR}/shaders/vert.spv"
	)

	add_dependencies(${Dragon_LIB_NAME} Shaders)
endif()

# Specify the include directories for the library
target_include_directories(${Dragon_LIB_NAME} PUBLIC ${Dragon_INCLUDE_DIRS})

# Link the library with the vulkan library
target_link_libraries(${Dragon_LIB_NAME} PUBLIC Vulkan::Vulkan)

# Link the library with the glfw library
target_link_libraries(${Dragon_LIB_NAME} PUBLIC glfw)

# Create an alias for the library
add_library(Dragon::Dragon ALIAS ${Dragon_LIB_NAME})