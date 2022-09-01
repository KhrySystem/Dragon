#pragma once

#include <dragon/predef.h>

struct GPU
{
	VkPhysicalDevice handle;
	VkPhysicalDeviceProperties deviceProperties;
	VkPhysicalDeviceFeatures deviceFeatures;
};

DGAPI GPU* dgCreateGPU(VkPhysicalDevice gpuHandle);
DGAPI int dgEvaluateGPU(GPU* gpu);