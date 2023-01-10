#pragma once

#include <string>
#include <vector>

#include "dragon_core.h"
#include "gpu.hpp"
#include "window.hpp"

typedef struct DgEngine {
	VkInstance vulkan;
	std::vector<const char*> vkExtensions;
	#ifndef NDEBUG
	std::vector<std::string> validationLayers;
	VkDebugUtilsMessengerEXT debugMessenger;
	#endif
	DgGPU* primaryGPU;
	std::vector<DgGPU> gpus;
	std::vector<DgWindow> windows;
} DgEngine;



