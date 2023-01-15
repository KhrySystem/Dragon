
    

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

if (${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "AMD64")
    set(GLSL_VALIDATOR "$ENV{VULKAN_SDK}/Bin/glslangValidator.exe")
else()
    set(GLSL_VALIDATOR "$ENV{VULKAN_SDK}/Bin32/glslangValidator.exe")
endif()

file(GLOB_RECURSE GLSL_SOURCE_FILES
    "shaders/*.glsl"
)

foreach(GLSL ${GLSL_SOURCE_FILES})
    get_filename_component(FILE_NAME ${GLSL} NAME)
    set(SPIRV "${PROJECT_BINARY_DIR}/shaders/${FILE_NAME}.spv")
    add_custom_command(
        OUTPUT ${SPIRV}
            COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_BINARY_DIR}/shaders/"
            COMMAND ${GLSL_VALIDATOR} -V ${GLSL} -o ${SPIRV}
            DEPENDS ${GLSL}
    )
    list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(GLSL)

add_custom_target(
    Shaders 
    DEPENDS ${SPIRV_BINARY_FILES}
)

add_dependencies(${Dragon_LIB_NAME} Shaders)

add_custom_command(TARGET ${Dragon_LIB_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${Dragon_LIB_NAME}>/shaders/"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${Dragon_BINARY_DIR}/shaders"
        "$<TARGET_FILE_DIR:${Dragon_LIB_NAME}>/shaders"
)

# Specify the include directories for the library
target_include_directories(${Dragon_LIB_NAME} PUBLIC ${Dragon_INCLUDE_DIRS})

# Link the library with the vulkan library
target_link_libraries(${Dragon_LIB_NAME} PUBLIC Vulkan::Vulkan)

# Link the library with the glfw library
target_link_libraries(${Dragon_LIB_NAME} PUBLIC glfw)

# Create an alias for the library
add_library(Dragon::Dragon ALIAS ${Dragon_LIB_NAME})