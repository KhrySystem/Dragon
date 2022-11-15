cmake_minimum_required(VERSION 3.20)

if("${Dragon_BUILD_SHARED}" STREQUAL "ON")
	add_library(${Dragon_LIB_NAME} SHARED 
		graphics/engine.c
		internal/gpu.c
		internal/init.c
		internal/vector.c
		message/callbacks.c
	)
	target_compile_definitions(${Dragon_LIB_NAME} PRIVATE DRAGON_BUILD_DLL)
	message("Building Dragon as a shared library")
else() 
	add_library(${Dragon_LIB_NAME} STATIC 
		graphics/engine.c
		internal/gpu.c
		internal/init.c
		internal/vector.c
		message/callbacks.c
	)
	message("Building Dragon as a static library")
endif()
target_include_directories(${Dragon_LIB_NAME} PUBLIC ${Dragon_INCLUDE_DIRS})
target_link_libraries(${Dragon_LIB_NAME} PUBLIC glfw openxr_loader OpenAL ${Vulkan_LIBRARIES})
add_library(Dragon::Dragon ALIAS ${Dragon_LIB_NAME})