include(${Dragon_SOURCE_DIR}/cmake/bin2h.cmake) 

# Check if the library is to be built as static or shared

# Create a shared library with the given name, and specify the sources to be compiled
add_library(${Dragon_LIB_NAME} 
    "engine_close.cpp"
    "engine_init.cpp"
    "engine_runtime.cpp"
    "gpu_init.hpp"
    "internal.cpp"
    "model_close.cpp"
    "model_init.cpp"
    "vertex.cpp"
    "vk_literals.cpp"
    "window_camera.hpp"
    "window_close.cpp"
    "window_init.cpp"
    "window_pipeline.hpp"
    "window_runtime.cpp"
)
if(BUILD_SHARED_LIBS)
	# Define the preprocessor symbol for shared library build
	target_compile_definitions(${Dragon_LIB_NAME} PUBLIC -DDRAGON_BUILD_DLL)
	add_compile_definitions(DRAGON_DLL)
endif()

# get shader files
if(Dragon_2D_RENDER)
    set(Dragon_VERT_SHADER ${Dragon_SOURCE_DIR}/src/shaders/2d.vert.in)
    set(Dragon_FRAG_SHADER ${Dragon_SOURCE_DIR}/src/shaders/2d.simple.frag.in)
else()
    set(Dragon_VERT_SHADER ${Dragon_SOURCE_DIR}/src/shaders/3d.simple.vert.in)
    set(Dragon_FRAG_SHADER ${Dragon_SOURCE_DIR}/src/shaders/3d.simple.frag.in)
endif()


configure_file(${Dragon_FRAG_SHADER} ${Dragon_BINARY_DIR}/src/shaders/shader.frag)
configure_file(${Dragon_VERT_SHADER} ${Dragon_BINARY_DIR}/src/shaders/shader.vert)

if(OFF)
    message(STATUS "Using glslc to build SPIR-V Binaries")
    execute_process(
        COMMAND ${Vulkan_GLSLC_EXECUTABLE} ${Dragon_BINARY_DIR}/src/shaders/shader.frag -o ${Dragon_BINARY_DIR}/src/shaders/frag.spv --target-env=vulkan1.2
        COMMAND ${Vulkan_GLSLC_EXECUTABLE} ${Dragon_BINARY_DIR}/src/shaders/shader.vert -o ${Dragon_BINARY_DIR}/src/shaders/vert.spv --target-env=vulkan1.2
        OUTPUT_QUIET
    )

    file(REMOVE "${Dragon_SOURCE_DIR}/include/dragon/frag.hpp")
    file(REMOVE "${Dragon_SOURCE_DIR}/include/dragon/vert.hpp")
	bin2h(SOURCE_FILE "${Dragon_BINARY_DIR}/src/shaders/frag.spv" HEADER_FILE "${Dragon_SOURCE_DIR}/include/dragon/frag.hpp" VARIABLE_NAME DRAGON_FRAG_SHADER APPEND NULL_TERMINATE)
	bin2h(SOURCE_FILE "${Dragon_BINARY_DIR}/src/shaders/vert.spv" HEADER_FILE "${Dragon_SOURCE_DIR}/include/dragon/vert.hpp" VARIABLE_NAME DRAGON_VERT_SHADER APPEND NULL_TERMINATE)
elseif(${Vulkan_glslangValidator_FOUND})
    message(STATUS "Using glslangValidator to build SPIR-V Binaries")

    execute_process(
        COMMAND ${Vulkan_GLSLANG_VALIDATOR_EXECUTABLE} -V ${Dragon_BINARY_DIR}/src/shaders/shader.frag -o ${Dragon_BINARY_DIR}/src/shaders/frag.spv --target-env vulkan1.2
        COMMAND ${Vulkan_GLSLANG_VALIDATOR_EXECUTABLE} -V ${Dragon_BINARY_DIR}/src/shaders/shader.vert -o ${Dragon_BINARY_DIR}/src/shaders/vert.spv --target-env vulkan1.2
        OUTPUT_QUIET
    )

    file(REMOVE "${Dragon_SOURCE_DIR}/include/dragon/frag.hpp")
    file(REMOVE "${Dragon_SOURCE_DIR}/include/dragon/vert.hpp")
	bin2h(SOURCE_FILE "${Dragon_BINARY_DIR}/src/shaders/frag.spv" HEADER_FILE "${Dragon_SOURCE_DIR}/include/dragon/frag.hpp" VARIABLE_NAME DRAGON_FRAG_SHADER APPEND NULL_TERMINATE)
	bin2h(SOURCE_FILE "${Dragon_BINARY_DIR}/src/shaders/vert.spv" HEADER_FILE "${Dragon_SOURCE_DIR}/include/dragon/vert.hpp" VARIABLE_NAME DRAGON_VERT_SHADER APPEND NULL_TERMINATE)
else()
    message(WARNING "Using prebuilt SPIR-V Binaries")
endif()

# Specify the include directories for the library
target_include_directories(${Dragon_LIB_NAME} PRIVATE
	${Dragon_SOURCE_DIR}/include
    ${Dragon_SOURCE_DIR}/external/glm
    ${Dragon_SOURCE_DIR}/external/boost/predef/include
    ${Dragon_SOURCE_DIR}/external/vulkan_memory_allocator/include
)

target_link_libraries(${Dragon_LIB_NAME}
    PUBLIC
        Vulkan::Vulkan
        glfw
        VulkanMemoryAllocator
    INTERFACE
        Boost::predef
)

# Create an alias for the library
add_library(Dragon::Dragon ALIAS ${Dragon_LIB_NAME})
