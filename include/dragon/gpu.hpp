#pragma once

#include <optional>

#include "dragon_core.h"
#include "engine.hpp"

typedef struct {
	std::optional<uint32_t> graphicsQueueFamily;
	std::optional<uint32_t> presentationQueueFamily;
} dgQueueFamilies;

typedef struct {
	VkPhysicalDevice handle;
	VkPhysicalDeviceProperties properties;
	VkPhysicalDeviceFeatures features;
	dgQueueFamilies queueFamilies;
	VkDevice device;
	VkQueue graphicsQueue;
	VkQueue presentationQueue;
} DgGPU;


DGAPI void dgFindQueueFamilies(DgGPU* gpu);