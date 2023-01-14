#pragma once

#include <optional>

#include "dragon_core.h"
#include "window.hpp"

typedef struct {
	std::optional<uint32_t> graphicsQueueFamily;
	std::optional<uint32_t> presentationQueueFamily;
} dgQueueFamilies;


typedef struct {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
} dgSwapChainSupportDetails;

typedef struct {
	VkPhysicalDevice handle;
	VkPhysicalDeviceProperties properties;
	VkPhysicalDeviceFeatures features;
	dgQueueFamilies queueFamilies;
	VkDevice device;
	VkQueue graphicsQueue;
	VkQueue presentationQueue;
	dgSwapChainSupportDetails swapChainSupportDetails;

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
} DgGPU;

// Helper creation/destruction methods
DGAPI void dgFindQueueFamilies(DgGPU* gpu);
DGAPI void dgGeneratePresentationQueue(DgGPU* pGPU, DgWindow* window);
DGAPI void dgGetSwapChainSupport(DgGPU* gpu);