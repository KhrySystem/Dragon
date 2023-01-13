#pragma once

#include <string>
#include <vector>
#include <functional>

#include "dragon_core.h"
#include "gpu.hpp"
#include "message.hpp"
#include "window.hpp"

typedef struct DgEngine {
	VkInstance vulkan;
	std::vector<const char*> vkExtensions = { VK_KHR_SURFACE_EXTENSION_NAME, VK_EXT_DEBUG_UTILS_EXTENSION_NAME};
	std::vector<const char*> vkDeviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	#ifndef NDEBUG
	std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
	VkDebugUtilsMessengerEXT debugMessenger;
	#else
	#endif
	DgGPU* primaryGPU;
	std::vector<DgGPU> gpus;
	std::vector<DgWindow> windows;
	std::function<void(DgMessage*)> fCallback;
} DgEngine;



