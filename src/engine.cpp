#include <iterator>
#include <vector>

#include <dragon/dragon.hpp>

DGAPI DgResult dgAddLayerToEngine(DgEngine* pEngine, std::string layerName) {
	#ifndef NDEBUG
		// Check Vulkan Layers
		uint32_t layerCount;
		dgEnumerateVkInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		dgEnumerateVkInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const auto& layerProperties : availableLayers) {
			if (strcmp(layerName.c_str(), layerProperties.layerName) == 0) {
				for (const char* ext : pEngine->vkExtensions) {
					if (strcmp(ext, layerName.c_str()) == 0) {
						return DG_VK_VALIDATION_LAYER_ALREADY_ADDED;
					}
				}

				pEngine->validationLayers.push_back(layerName.c_str());
				return DG_SUCCESS;
			}
		}
		return DG_VK_VALIDATION_LAYER_NOT_FOUND;
	#else
		return DG_IN_RELEASE_MODE;
	#endif
}

DGAPI DgResult dgAddVkExtensionToEngine(DgEngine* pEngine, const char* extName) {
	for (const char* name : pEngine->vkExtensions) {
		if (strcmp(name, extName) == 0) {
			return DG_VK_EXTENSION_ALREADY_ADDED;
		}
	}

	return DG_SUCCESS;
}

DGAPI void dgSetCallback(DgEngine* pEngine, std::function<void(int, const char*, void*)> fCallback) {
	pEngine->fCallback = fCallback;
}

DgResult _dgSetupVulkan(DgEngine* pEngine) {
	// App info. Contains info about the Engine, the user app, etc.
	VkApplicationInfo appInfo{};
	// Use the version that matches the shaders
	appInfo.apiVersion = VK_API_VERSION_1_2;
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = APP_NAME;
	appInfo.applicationVersion = APP_VERSION;
	appInfo.pEngineName = "Dragon Engine";
	appInfo.engineVersion = DRAGON_VERSION;

	// Instance Create Info. Used in the vkCreateInstance function
	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;
	createInfo.enabledExtensionCount = pEngine->vkExtensions.size();
	createInfo.ppEnabledExtensionNames = pEngine->vkExtensions.data();

	// If we're not in debug mode, no validation layers should be added to the engine
	#ifndef NDEBUG
		createInfo.enabledLayerCount = pEngine->validationLayers.size();
		createInfo.ppEnabledLayerNames = pEngine->validationLayers.data();
	#else
		createInfo.enabledLayerCount = 0;
		createInfo.ppEnabledLayerNames = nullptr;
	#endif
	// Officially create the VkInstance
		VkResult result = dgCreateVkInstance(&createInfo, nullptr, &pEngine->vulkan);
	if (result != VK_SUCCESS) {
		#ifndef NDEBUG
		std::cerr << "Dragon: vkCreateInstance returned " << dgConvertVkResultToString(result) << std::endl;
		#endif
		return DG_VK_INSTANCE_CREATION_FAILED;
	}
	return DG_SUCCESS;
}
#ifndef NDEBUG
DgResult _dgSetupVulkanDebug(DgEngine* pEngine) {
	VkDebugUtilsMessengerCreateInfoEXT messengerCreateInfo{};
	messengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	messengerCreateInfo.messageSeverity =  VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	messengerCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	messengerCreateInfo.pfnUserCallback = dgVulkanDebugCallback;
	messengerCreateInfo.pUserData = nullptr;

	VkResult result = dgCreateVkDebugUtilsMessengerEXT(pEngine->vulkan, &messengerCreateInfo, nullptr, &(pEngine->debugMessenger));
	if (result != VK_SUCCESS) {
		std::cerr << "Dragon: dgCreateDebugUtilsMessengerEXT returned " << dgConvertVkResultToString(result) << std::endl;
		return DG_VK_DEBUG_UTILS_INSTANCE_CREATION_FAILED;
	}
	return DG_SUCCESS;
}
#endif
DgResult _dgSetupGPUs(DgEngine* pEngine) {
	uint32_t deviceCount = 0;
	dgEnumerateVkPhysicalDevices(pEngine->vulkan, &deviceCount, nullptr);

	if (deviceCount == 0) {
		#ifndef NDEBUG
		std::cerr << "Dragon: vkEnumeratePhysicalDevices gave 0 devices available for Vulkan" << std::endl;
		#endif
		return DG_NO_VK_PHYSICAL_DEVICES_FOUND;
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	dgEnumerateVkPhysicalDevices(pEngine->vulkan, &deviceCount, devices.data());

	for (VkPhysicalDevice dev : devices) {
		DgGPU gpu;
		gpu.handle = dev;
		vkGetPhysicalDeviceFeatures(dev, &(gpu.features));
		vkGetPhysicalDeviceProperties(dev, &(gpu.properties));
		_dgFindQueueFamilies(&gpu);
		pEngine->gpus.push_back(gpu);
	}
	if (!pEngine->gpus.at(0).queueFamilies.graphicsQueueFamily.has_value()) {
		return DG_NO_GRAPHICS_QUEUE_FOUND;
	}
	pEngine->primaryGPU = &pEngine->gpus.at(0);
	return DG_SUCCESS;
}

DGAPI DgResult dgCreateEngine(DgEngine* pEngine) {
	if (pEngine == nullptr) {
		return DG_ARGUMENT_IS_NULL;
	}
	DgResult r;

	if (!glfwInit()) {
		return DG_GLFW_INITIALIZATION_FAILED;
	}

	// Vulkan Initialization
	if (!glfwVulkanSupported()) {
		return DG_GLFW_VULKAN_NOT_SUPPORTED;
	}

	uint32_t count;
	const char** extensions = glfwGetRequiredInstanceExtensions(&count);
	std::vector<const char*> glfwExtensions(extensions, extensions + count);

	for (const char* required : glfwExtensions) {
		dgAddVkExtensionToEngine(pEngine, required);
	}
	r = _dgSetupVulkan(pEngine);
	if (r != DG_SUCCESS) {
		return r;
	}

	// VkDebugMessenger Initialization
	#ifndef NDEBUG
	r = _dgSetupVulkanDebug(pEngine);
	if (r != DG_SUCCESS) {
		return r;
	}
	#endif

	// Get GPUs and start compute API
	r = _dgSetupGPUs(pEngine);
	if (r != DG_SUCCESS) {
		return r;
	}

	return DG_SUCCESS;
}

DGAPI DgResult dgUpdate(DgEngine* pEngine) {
	if (pEngine == nullptr) {
		return DG_ARGUMENT_IS_NULL;
	}
	glfwPollEvents();
	for (int i = 0; i < pEngine->windows.size(); i++) {
		DgWindow window = pEngine->windows.at(i);
		if (_dgRenderWindow(&window) != DG_SUCCESS) {
			dgDestroyWindow(pEngine->vulkan, &window);
		}

		if(glfwWindowShouldClose(window.window)) {
			dgDestroyWindow(pEngine->vulkan, &window);
		}
	}
}

DGAPI void dgTerminateEngine(DgEngine* pEngine) {
	#ifndef NDEBUG
	dgDestroyDebugUtilsMessengerEXT(pEngine->vulkan, pEngine->debugMessenger, nullptr);
	#endif
	
	for (DgWindow window : pEngine->windows) {
		vkDeviceWaitIdle(window.pGPU->device);
		dgDestroyWindow(pEngine->vulkan, &window);
	}

	for (DgGPU gpu : pEngine->gpus) {
		if (gpu.device != nullptr) {
			vkDestroyDevice(gpu.device, nullptr);
		}
	}
	
	vkDestroyInstance(pEngine->vulkan, nullptr);
	glfwTerminate();
}