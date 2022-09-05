#pragma once

#include <dragon/predef.h>

struct GPU
{
	int loadIndex;
	int score;
	VkPhysicalDevice handle;
	VkPhysicalDeviceProperties deviceProperties;
	VkPhysicalDeviceFeatures deviceFeatures;
};

DGAPI GPU* dgCreateGPU(VkPhysicalDevice gpuHandle);
DGAPI int dgEvaluateGPU(GPU* gpu);