#pragma once

struct GPU {
	VkPhysicalDevice handle;
	VkPhysicalDeviceProperties properties;
	VkPhysicalDeviceLimits limits;
	VkPhysicalDeviceFeatures features;
	#ifdef VK_VERSION_1_3
	    VkPhysicalDeviceToolProperties toolProperties;
	#else
	    VkPhysicalDeviceToolPropertiesEXT toolProperties;
	#endif
};

DGAPI int getGPUComputeScore(GPU* gpu);
DGAPI int getGPUGraphicsScore(GPU* gpu);