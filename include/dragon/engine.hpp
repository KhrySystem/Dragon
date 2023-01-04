#pragma once

#include <string>
#include <vector>

#include "dragon_core.h"
#include "gpu.hpp"
#include "window.hpp"

typedef struct {
	VkInstance vulkan;
	std::vector<std::string> validationLayers;
	#ifndef NDEBUG
	VkDebugUtilsMessengerEXT debugMessenger;
	#endif
	DgGPU* primaryGPU;
	std::vector<DgGPU> gpus;
	std::vector<DgWindow> windows;
} DgEngine;



