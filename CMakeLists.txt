cmake_minimum_required(VERSION 3.20)

project(Dragon)

include(.cmake/DragonGetVersionNumber.info)

message("Looking for a Vulkan installation...")
find_package(Vulkan)
if(Vulkan_FOUND)
	add_compile_definitions(DRAGON_USE_VULKAN)

	set(DRAGON_INCLUDE_DIRECTORIES ${Vulkan_INCLUDE_DIRS})

	if(Vulkan_glslc_FOUND)
		set(DRAGON_LINK_LIBRARIES Vulkan::glslc)
	endif()

	if(Vulkan_glslangValidator_FOUND)
		set(DRAGON_LINK_LIBRARIES ${DRAGON_LINK_LIBRARIES} Vulkan::glslangValidator)
	endif()

	if(Vulkan_glslang_FOUND)
		set(DRAGON_LINK_LIBRARIES ${DRAGON_LINK_LIBRARIES} Vulkan::glslang)
	endif()

	if(Vulkan_shaderc_combined_FOUND)
		set(DRAGON_LINK_LIBRARIES ${DRAGON_LINK_LIBRARIES} Vulkan::shaderc_combined)
	endif()

	if(Vulkan_SPIRV-Tools_FOUND)
		set(DRAGON_LINK_LIBRARIES ${DRAGON_LINK_LIBRARIES} Vulkan::SPIRV-Tools)
	endif()

	if(Vulkan_MoltenVK_FOUND)
		set(DRAGON_LINK_LIBRARIES ${DRAGON_LINK_LIBRARIES} Vulkan::MoltenVK)
	endif()

else()
	message("Failed to find a Vulkan installation.")
	message("Looking for an OpenGL installation...")
	find_package(OpenGL REQUIRED)
	if(OpenGL_FOUND)
		add_compile_definitions(DRAGON_USE_OPENGL)

		set(DRAGON_INCLUDE_DIRECTORIES ${OPENGL_INCLUDE_DIR})
		set(DRAGON_LINK_LIBRARIES ${OPENGL_LIBRARIES})
	endif()
endif()


add_subdirectory(lib)
set(DRAGON_INCLUDE_DIRECTORIES ${DRAGON_INCLUDE_DIRECTORIES} 
	headers/include ${GLFW_INCLUDE_DIRECTORIES} ${GLM_INCLUDE_DIRECTORIES} ${BOOST_INCLUDE_DIRECTORIES}
)

include_directories(${DRAGON_INCLUDE_DIRECTORIES})
add_subdirectory(headers)
add_subdirectory(src)
add_subdirectory(tools)