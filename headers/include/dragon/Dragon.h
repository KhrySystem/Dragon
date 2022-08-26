#pragma once
#ifdef _cplusplus
extern "C" {
#endif

#ifdef DRAGON_USE_VULKAN
	#ifndef VULKAN_H_
		#include <vulkan/vulkan.h>
	#endif
#endif

#ifdef DRAGON_STATIC
	#define DGAPI
#else
	#define DGAPI __declspec(dllexport)
#endif

#include <glfw/glfw3.h>

#define DGBOOL bool
#define DGTRUE 1
#define DGFALSE 0

#define dgInit dgInit_internal
#define dgCreateInstance dgCreateInstance_internal

#ifdef _cplusplus
}
#endif