#include <iterator>
#include <vector>

#include <dragon/dragon.hpp>

DGAPI DgResult dgAddLayerToEngine(DgEngine* pEngine, std::string layerName) {
	#ifndef NDEBUG
		// Check Vulkan Layers
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const auto& layerProperties : availableLayers) {
			if (strcmp(layerName.c_str(), layerProperties.layerName) == 0) {
				for (const char* ext : pEngine->vkExtensions) {
					if (strcmp(ext, layerName.c_str()) == 0) {
						#ifndef NDEBUG
						std::cout << "VkValidationLayer " << layerName << " already added to DgEngine instance located at " << pEngine << std::endl;
						#endif
						return DG_VK_VALIDATION_LAYER_ALREADY_ADDED;
					}
				}

				pEngine->validationLayers.push_back(layerName.c_str());
				return DG_SUCCESS;
			}
		}
		#ifndef NDEBUG
			std::cout << "VkValidationLayer " << layerName << " not found in available extensions for DgEngine instance located at " << pEngine << std::endl;
		#endif
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
	pEngine->vkExtensions.push_back(extName);
	return DG_SUCCESS;
}

DGAPI void dgSetCallback(DgEngine* pEngine, std::function<void(DgMessage*)> fCallback) {
	pEngine->fCallback = fCallback;
}

DgResult _dgSetupVulkan(DgEngine* pEngine) {
	// App info. Contains info about the Engine, the user app, etc.
	VkApplicationInfo appInfo{};
	// Use the highest Vulkan version available, up to 1.3
	appInfo.apiVersion = VK_API_VERSION_1_3;
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
	VkResult result = vkCreateInstance(&createInfo, nullptr, &(pEngine->vulkan));
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

	VkResult result = dgCreateDebugUtilsMessengerEXT(pEngine->vulkan, &messengerCreateInfo, nullptr, &(pEngine->debugMessenger));
	if (result != VK_SUCCESS) {
		std::cerr << "Dragon: dgCreateDebugUtilsMessengerEXT returned " << dgConvertVkResultToString(result) << std::endl;
		return DG_VK_DEBUG_UTILS_INSTANCE_CREATION_FAILED;
	}
	return DG_SUCCESS;
}
#endif
DgResult _dgSetupGPUs(DgEngine* pEngine) {
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(pEngine->vulkan, &deviceCount, nullptr);

	if (deviceCount == 0) {
		#ifndef NDEBUG
		std::cerr << "Dragon: vkEnumeratePhysicalDevices gave 0 devices available for Vulkan" << std::endl;
		#endif
		return DG_NO_VK_PHYSICAL_DEVICES_FOUND;
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(pEngine->vulkan, &deviceCount, devices.data());

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

DgResult _dgStartQueueBuffers(DgEngine* pEngine) {
	
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

	VkDeviceQueueCreateInfo graphicsQueueCreateInfo{};
	graphicsQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	graphicsQueueCreateInfo.queueFamilyIndex = pEngine->primaryGPU->queueFamilies.graphicsQueueFamily.value();
	graphicsQueueCreateInfo.queueCount = 1;
	float queuePriority = 1.0f;
	graphicsQueueCreateInfo.pQueuePriorities = &queuePriority;

	queueCreateInfos.push_back(graphicsQueueCreateInfo);

	VkDeviceCreateInfo deviceCreateInfo{};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
	deviceCreateInfo.queueCreateInfoCount = queueCreateInfos.size();

	deviceCreateInfo.pEnabledFeatures = &pEngine->primaryGPU->features;

	deviceCreateInfo.enabledExtensionCount = pEngine->vkDeviceExtensions.size();
	deviceCreateInfo.ppEnabledExtensionNames = pEngine->vkDeviceExtensions.data();

	#ifndef NDEBUG
		deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(pEngine->validationLayers.size());
		deviceCreateInfo.ppEnabledLayerNames = pEngine->validationLayers.data();
	#else
		deviceCreateInfo.enabledLayerCount = 0;
	#endif
	VkResult result = vkCreateDevice((pEngine->primaryGPU->handle), &deviceCreateInfo, nullptr, &(pEngine->primaryGPU->device));

	if (result != VK_SUCCESS) {
		return DG_VK_DEVICE_CREATION_FAILED;
	}

	vkGetDeviceQueue(pEngine->primaryGPU->device, pEngine->primaryGPU->queueFamilies.graphicsQueueFamily.value(), 0, &pEngine->primaryGPU->graphicsQueue);
	return DG_SUCCESS;
}

DGAPI DgResult dgCreateEngine(DgEngine* pEngine) {
	DgResult r;

	if (!glfwInit()) {
		return DG_GLFW_INITIALIZATION_FAILED;
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	// Vulkan Initialization
	glfwSetErrorCallback(dgGLFWErrorCallback);

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
	// Start Graphics Queue Buffers
	r = _dgStartQueueBuffers(pEngine);
	if (r != DG_SUCCESS) {
		return r;
	}

	return DG_SUCCESS;
}

DGAPI void dgUpdate(DgEngine* pEngine) {
	glfwPollEvents();
	for (int i = 0; i < pEngine->windows.size(); i++) {
		DgWindow window = pEngine->windows[i];
		if(glfwWindowShouldClose(window.window)) {
			for (VkImageView imageView : window.swapChainImageViews) {
				vkDestroyImageView(pEngine->primaryGPU->device, imageView, nullptr);
			}
			vkDestroySwapchainKHR(pEngine->primaryGPU->device, window.swapChain, nullptr);
			vkDestroySurfaceKHR(pEngine->vulkan, window.surface, nullptr);
			glfwDestroyWindow(window.window);
			pEngine->windows.erase(pEngine->windows.begin() + i);
		}
	}
}

DGAPI std::vector<char> _dgLoadShaderSPV(const std::string& filename) {
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open()) {
		return std::vector<char>(0);
	}

	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();

	return buffer;
}

DGAPI void dgTerminateEngine(DgEngine* pEngine) {
	#ifndef NDEBUG
	dgDestroyDebugUtilsMessengerEXT(pEngine->vulkan, pEngine->debugMessenger, nullptr);
	#endif
	
	for (DgWindow window : pEngine->windows) {
		for (VkImageView imageView : window.swapChainImageViews) {
			vkDestroyImageView(window.pGPU->device, imageView, nullptr);
			for (VkShaderModule module : window.shaderModules) {
				vkDestroyShaderModule(window.pGPU->device, module, nullptr);
			}
			vkDestroyPipelineLayout(window.pGPU->device, window.pipelineLayout, nullptr);
		}
		
		vkDestroySwapchainKHR(window.pGPU->device, window.swapChain, nullptr);
		vkDestroySurfaceKHR(pEngine->vulkan, window.surface, nullptr);
		glfwDestroyWindow(window.window);
	}

	for (DgGPU gpu : pEngine->gpus) {
		if (gpu.device != nullptr) {
			vkDestroyDevice(gpu.device, nullptr);
		}
	}
	
	vkDestroyInstance(pEngine->vulkan, nullptr);
	glfwTerminate();
}