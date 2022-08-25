#pragma once
#ifdef _cplusplus
extern "C" {
#endif

#ifndef VULKAN_H_
#include <vulkan/vulkan.h>
#endif

#define DGBOOL bool
#define DGTRUE 1
#define DGFALSE 0

#define dgInstance dgInstance_internal
#define dgCreateInstance dgCreateInstance_internal

#ifdef _cplusplus
}
#endif