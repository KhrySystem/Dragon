#pragma once

#include <string>
#include <vector>
#include <functional>

#include "dragon_core.h"
#include "gpu.hpp"
#include "message.hpp"
#include "window.hpp"

typedef struct {
	VkInstance vulkan;
	std::vector<const char*> vkExtensions;
	std::vector<const char*> vkDeviceExtensions;
	#ifndef NDEBUG
	std::vector<std::string> validationLayers;
	VkDebugUtilsMessengerEXT debugMessenger;
	#endif
	DgGPU* primaryGPU;
	std::vector<DgGPU> gpus;
	std::vector<DgWindow> windows;
	std::function<void(DgMessage*)> optfCallback;
} DgEngine;



