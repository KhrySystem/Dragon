#pragma once

typedef struct DgGPU_T {
	VkPhysicalDevice handle;
	VkPhysicalDeviceProperties properties;
	VkPhysicalDeviceLimits limits;
	VkPhysicalDeviceFeatures features;
	#ifdef VK_VERSION_1_3
	    VkPhysicalDeviceToolProperties toolProperties;
	#else
	    VkPhysicalDeviceToolPropertiesEXT toolProperties;
	#endif
} DgGPU;

DGAPI int dgGetGPUComputeScore(DgGPU* gpu);
DGAPI int dgGetGPUGraphicsScore(DgGPU* gpu);