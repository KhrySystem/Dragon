#pragma once
#ifdef _cplusplus
extern "C" {
#endif


#ifdef DRAGON_USE_VULKAN
	#define GLFW_INCLUDE_VULKAN
	#ifndef VULKAN_H_
		#include <vulkan/vulkan.h>
	#endif
#endif

#ifdef DRAGON_USE_OPENGL
	#define GLFW_INCLUDE_OPENGL
#endif

#include <glfw/glfw3.h>


#ifdef DRAGON_STATIC
	#define DGAPI
#else
	#define DGAPI __declspec(dllexport)
#endif

#define DGBOOL bool
#define DGTRUE 1
#define DGFALSE 0

#include <dragon/dgEngine.h>
#include <dragon/graphics/WindowCreateParams.h>

#define dgWindow GLFWwindow*

#define dgInit dgInit_internal
#define dgTerminate dgTerminate_internal
#define dgCreateInstance dgCreateInstance_internal


#ifdef _cplusplus
}
#endif