include(${Dragon_SOURCE_DIR}/cmake/bin2h.cmake) 

# Check if the library is to be built as static or shared
if(Dragon_BUILD_STATIC)
	# Create a static library with the given name, and specify the sources to be compiled
	add_library(${Dragon_LIB_NAME} STATIC 
        "engine_close.cpp"
        "engine_init.cpp"
        "engine_runtime.cpp"
        "gpu_init.h"
        "vk_literals.cpp"
        "window_close.cpp"
        "window_init.cpp"
	)
else() 
	# Create a shared library with the given name, and specify the sources to be compiled
	add_library(${Dragon_LIB_NAME} SHARED 
        "engine_close.cpp"
		"engine_init.cpp"
        "engine_runtime.cpp"
        "gpu_init.h"
        "vk_literals.cpp"
        "window_close.cpp"
        "window_init.cpp"
	)
	# Define the preprocessor symbol for shared library build
	target_compile_definitions(${Dragon_LIB_NAME} PUBLIC -DDRAGON_BUILD_DLL)
	add_compile_definitions(DRAGON_DLL)
endif()

configure_file(${Dragon_SOURCE_DIR}/src/shaders/shader.frag.in ${Dragon_BINARY_DIR}/src/shaders/shader.frag)
configure_file(${Dragon_SOURCE_DIR}/src/shaders/shader.vert.in ${Dragon_BINARY_DIR}/src/shaders/shader.vert)

if(${Vulkan_glslc_FOUND})
    message(STATUS "Using glslc to build SPIR-V Binaries")
    execute_process(
        COMMAND ${Vulkan_GLSLC_EXECUTABLE} ${Dragon_BINARY_DIR}/src/shaders/shader.frag -o ${Dragon_BINARY_DIR}/src/shaders/frag.spv --target-env=vulkan1.2
        COMMAND ${Vulkan_GLSLC_EXECUTABLE} ${Dragon_BINARY_DIR}/src/shaders/shader.vert -o ${Dragon_BINARY_DIR}/src/shaders/vert.spv --target-env=vulkan1.2
    )

    file(REMOVE "${Dragon_SOURCE_DIR}/include/dragon/frag.h")
    file(REMOVE "${Dragon_SOURCE_DIR}/include/dragon/vert.h")
	bin2h(SOURCE_FILE "${Dragon_BINARY_DIR}/src/shaders/frag.spv" HEADER_FILE "${Dragon_SOURCE_DIR}/include/dragon/frag.h" VARIABLE_NAME DRAGON_FRAG_SHADER APPEND NULL_TERMINATE)
	bin2h(SOURCE_FILE "${Dragon_BINARY_DIR}/src/shaders/vert.spv" HEADER_FILE "${Dragon_SOURCE_DIR}/include/dragon/vert.h" VARIABLE_NAME DRAGON_VERT_SHADER APPEND NULL_TERMINATE)
elseif(${Vulkan_glslangValidator_FOUND})
    message(STATUS "Using glslangValidator to build SPIR-V Binaries")

    execute_process(
        COMMAND ${Vulkan_GLSLANG_VALIDATOR_EXECUTABLE} -V ${Dragon_BINARY_DIR}/src/shaders/shader.frag -o ${Dragon_BINARY_DIR}/src/shaders/frag.spv --target-env vulkan1.2
        COMMAND ${Vulkan_GLSLANG_VALIDATOR_EXECUTABLE} -V ${Dragon_BINARY_DIR}/src/shaders/shader.vert -o ${Dragon_BINARY_DIR}/src/shaders/vert.spv --target-env vulkan1.2
    )

    file(REMOVE "${Dragon_SOURCE_DIR}/include/dragon/frag.h")
    file(REMOVE "${Dragon_SOURCE_DIR}/include/dragon/vert.h")
	bin2h(SOURCE_FILE "${Dragon_BINARY_DIR}/src/shaders/frag.spv" HEADER_FILE "${Dragon_SOURCE_DIR}/include/dragon/frag.h" VARIABLE_NAME DRAGON_FRAG_SHADER APPEND NULL_TERMINATE)
	bin2h(SOURCE_FILE "${Dragon_BINARY_DIR}/src/shaders/vert.spv" HEADER_FILE "${Dragon_SOURCE_DIR}/include/dragon/vert.h" VARIABLE_NAME DRAGON_VERT_SHADER APPEND NULL_TERMINATE)
else()
    message(WARNING "Using prebuilt SPIR-V Binaries")
endif()

# Specify the include directories for the library
target_include_directories(${Dragon_LIB_NAME} PRIVATE
	${Dragon_SOURCE_DIR}/include
    ${Dragon_SOURCE_DIR}/external/cglm/include
    ${Dragon_SOURCE_DIR}/external/boost/predef/include
)

target_link_libraries(${Dragon_LIB_NAME}
    PUBLIC
        Vulkan::Vulkan
        glfw
    INTERFACE
        Boost::predef
)

# Create an alias for the library
add_library(Dragon::Dragon ALIAS ${Dragon_LIB_NAME})
