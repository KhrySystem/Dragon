cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)

project(Dragon)

add_compile_definitions(DRAGON_CXX_STANDARD 20)

message("Looking for a Vulkan installation...")
find_package(Vulkan)
if(Vulkan_FOUND)
	message("Vulkan - FOUND [${Vulkan_LIBRARY}]")
	add_compile_definitions(DRAGON_USE_VULKAN)

	list(APPEND DRAGON_INCLUDE_DIRECTORIES ${Vulkan_INCLUDE_DIRS})
	list(APPEND DRAGON_LINK_LIBRARIES ${Vulkan_LIBRARIES})
	

	if(Vulkan_glslc_FOUND)
		message("Vulkan GLSL Compiler Executable - FOUND [${Vulkan_GLSLC_EXECUTABLE}]")
		add_compile_definitions(DRAGON_VK_GLSLC_FOUND)
	else()
		message("Vulkan GLSL Compiler Executable - NOT FOUND")
	endif()

	if(Vulkan_glslang_FOUND)
		message("Vulkan GLSLang Library - FOUND [${Vulkan_glslang_LIBRARY}]")
		add_compile_definitions(DRAGON_VK_GLSLANG_FOUND)
	else()
		message("Vulkan GLSLang Library - NOT FOUND")
	endif()

	if(Vulkan_glslangValidator_FOUND)
		message("Vulkan GLSLang Validator Executable - FOUND [${Vulkan_GLSLANG_VALIDATOR_EXECUTABLE}]")
		add_compile_definitions(DRAGON_VK_GLSLANG_VALIDATOR_FOUND)
	else()
		message("Vulkan GLSLang Validator Executable - NOT FOUND")
	endif()

	if(Vulkan_shaderc_combined_FOUND)
		message("Vulkan ShaderC Combined Library - FOUND [${Vulkan_shaderc_combined_LIBRARY}]")
		add_compile_definitions(DRAGON_VK_SHADERC_COMBINED_FOUND)
	else()
		message("Vulkan ShaderC Combined Library - NOT FOUND")
	endif()

	if(Vulkan_SPIRV-Tools_FOUND)
		message("Vulkan SPIRV Tools Library - FOUND [${Vulkan_SPIRV-Tools_LIBRARY}]")
		add_compile_definitions(DRAGON_VK_SPIRV_TOOLS_FOUND)
	else()
		message("Vulkan SPIRV Tools Library - NOT FOUND")
	endif()

	if(Vulkan_MoltenVK_FOUND)
		message("Vulkan MoltenVK Library - FOUND [${Vulkan_MoltenVK_LIBRARY}]")
		add_compile_definitions(DRAGON_VK_MOLTENVK_FOUND)
	endif()

else()
	message("Failed to find a Vulkan installation.")
endif()

if(NOT Vulkan_FOUND)
	message(FATAL_ERROR "No Vulkan Installation found.")
endif()

find_package(OpenCL)
if(OpenCL_FOUND)
	message("OpenCL Library NOT FOUND")
	add_compile_definitions(DRAGON_USE_OPENCL)
	list(APPEND DRAGON_INCLDUE_DIRECTORIES ${OpenCL_INCLUDE_DIRS})
	list(APPEND DRAGON_LINK_LIBRARIES ${OpenCL_LIBRARIES})
else()
	message("OpenCL Libraries - NOT FOUND")
endif()

add_subdirectory(lib)

message("Looking for a Boost.hpp installation.")
if(CMAKE_BUILD_TYPE MATCHES RELEASE)
	message(" -- Looking for RELEASE Libraries")
	set(Boost_USE_DEBUG_LIBS OFF)
	set(Boost_USE_RELEASE_LIBS ON)
	set(Boost_USE_DEBUG_RUNTIME OFF)
	set(Boost_USE_DEBUG_PYTHON OFF)
else()
	message(" -- Defaulting to looking for DEBUG Libraries")
	set(Boost_USE_DEBUG_LIBS ON)
	set(Boost_USE_RELEASE_LIBS OFF)
	set(Boost_USE_DEBUG_RUNTIME ON)
	set(Boost_USE_DEBUG_PYTHON ON)
endif()

set(Boost_USE_MULTITHREADED ON)

find_package(Boost)
if(Boost_FOUND)
	message("Boost.hpp Libraries - FOUND [${Boost_VERSION_STRING}]")
	add_compile_definitions(DRAGON_BOOST_FOUND)
	list(APPEND DRAGON_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIRS})
	list(APPEND DRAGON_LINK_LIBRARIES ${Boost_LIBRARIES})
else()
	message("Boost.hpp Libraries - NOT FOUND")
endif()

list(APPEND DRAGON_INCLUDE_DIRECTORIES 
	headers/include lib/glfw lib/glfw/include lib/glfw/src lib/glfw/deps lib/glm 
)
list(APPEND DRAGON_LINK_LIBRARIES
	glfw OpenAL::OpenAL
)
include_directories(${DRAGON_INCLUDE_DIRECTORIES})
link_libraries(${DRAGON_LINK_LIBRARIES})
add_subdirectory(tools)

add_library(Dragon SHARED
	src/graphics/object.cpp
	src/graphics/window.cpp
	src/dgEngine.cpp 
	src/initialization.cpp
)

add_executable(OpenWindow tools/tests/openWindow.cpp)
target_link_libraries(OpenWindow Dragon)
add_executable(TriangleTest tools/tests/triangleTest.cpp)
target_link_libraries(TriangleTest Dragon)