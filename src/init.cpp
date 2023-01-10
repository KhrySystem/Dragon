#include <iterator>
#include <vector>

#include <dragon/dragon.hpp>

DGAPI DgBool32 dgAddLayerToEngine(DgEngine* pEngine, std::string layerName) {
	// Check Vulkan Layers
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const auto& layerProperties : availableLayers) {
		if (strcmp(layerName.c_str(), layerProperties.layerName) == 0) {
			pEngine->validationLayers.push_back(layerName);
			return DG_TRUE;
			break;
		}
	}
	return DG_FALSE;
}

DGAPI DgBool32 dgAddVkExtensionToEngine(DgEngine* pEngine, std::string extName) {
	return DG_FALSE;
}

DGAPI DgBool32 dgCreateEngine(DgEngine* pEngine) {
	// Must initialize GLFW first
	if (!glfwInit())
		return DG_FALSE;
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	// Vulkan Initialization
	glfwSetErrorCallback(dgGLFWErrorCallback);
	if (!glfwVulkanSupported())
		return DG_FALSE;

	uint32_t count;
	const char** extensions = glfwGetRequiredInstanceExtensions(&count);
	int i;
	for (i = 0; i <= (sizeof(extensions) / sizeof(extensions[0])) + 1; i++) {
		bool found = false;
		for (const char* ext : pEngine->vkExtensions) {
			if (strcmp(extensions[i], ext)) {
				found = true;
			}

		}

		if (!found)
			pEngine->vkExtensions.push_back(extensions[i]);
	}

	for (const char* ext : pEngine->vkExtensions) {
		std::cout << ext << std::endl;
	}
	#pragma region
	VkApplicationInfo appInfo{};
	// Use the highest Vulkan version available, up to 1.3
	#if defined(VK_API_VERSION_1_3)
	appInfo.apiVersion = VK_API_VERSION_1_3;
	#elif defined(VK_API_VERSION_1_2)
	appInfo.apiVersion = VK_API_VERSION_1_2;
	#elif defined(VK_API_VERSION_1_1)
	appInfo.apiVersion = VK_API_VERSION_1_1;
	#elif defined(VK_API_VERSION_1_0)
	appInfo.apiVersion = VK_API_VERSION_1_0;
	#endif
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "dgEngine";
	appInfo.applicationVersion = APP_VERSION;
	appInfo.pEngineName = "Dragon Engine";
	appInfo.engineVersion = DRAGON_VERSION;

	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;
	createInfo.enabledExtensionCount = pEngine->vkExtensions.size();
	createInfo.ppEnabledExtensionNames = pEngine->vkExtensions.data();

	#ifndef NDEBUG
	std::vector<const char*> layers(pEngine->validationLayers.size());
	for (int i = 0; i < pEngine->validationLayers.size(); i++) {
		layers.at(i) = pEngine->validationLayers.at(i).c_str();
	}

	createInfo.enabledLayerCount = pEngine->validationLayers.size();
	createInfo.ppEnabledLayerNames = layers.data();
	#else
	createInfo.enabledLayerCount = 0;
	createInfo.ppEnabledLayerNames = nullptr;
	#endif

	VkResult result = vkCreateInstance(&createInfo, nullptr, &(pEngine->vulkan));

	if (result != VK_SUCCESS)
		return DG_FALSE;
	#pragma endregion

	// VkDebugMessenger Initialization
	#ifndef NDEBUG
	VkDebugUtilsMessengerCreateInfoEXT messengerCreateInfo{};
	messengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	messengerCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	messengerCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	messengerCreateInfo.pfnUserCallback = dgDebugCallback;
	messengerCreateInfo.pUserData = nullptr; 
	
	result = dgCreateDebugUtilsMessengerEXT(pEngine->vulkan, &messengerCreateInfo, nullptr, &(pEngine->debugMessenger));

	if (result != VK_SUCCESS)
		return DG_FALSE;
	#endif

	// Get GPUs and start compute API
	#pragma region
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(pEngine->vulkan, &deviceCount, nullptr);

	if (deviceCount == 0) {
		return DG_FALSE;
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(pEngine->vulkan, &deviceCount, devices.data());

	for (VkPhysicalDevice dev : devices) {
		DgGPU gpu;
		gpu.handle = dev;
		vkGetPhysicalDeviceFeatures(dev, &(gpu.features));
		vkGetPhysicalDeviceProperties(dev, &(gpu.properties));
		dgFindQueueFamilies(&gpu);
		pEngine->gpus.push_back(gpu);
	}
	assert(pEngine->gpus.at(0).queueFamilies.graphicsQueueFamily.has_value() && "No GraphicsQueue found on GPU 0.");
	pEngine->primaryGPU = &pEngine->gpus.at(0);
	#pragma endregion
	// Start Graphics Queue Buffers
	#pragma region
	VkDeviceQueueCreateInfo queueCreateInfo{};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = pEngine->primaryGPU->queueFamilies.graphicsQueueFamily.value();
	queueCreateInfo.queueCount = 1;
	float queuePriority = 1.0f;
	queueCreateInfo.pQueuePriorities = &queuePriority;

	VkDeviceCreateInfo deviceCreateInfo{};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
	deviceCreateInfo.queueCreateInfoCount = 1;

	deviceCreateInfo.pEnabledFeatures = &pEngine->primaryGPU->features;

	deviceCreateInfo.enabledExtensionCount = 0;

	#ifndef NDEBUG
	deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());
	deviceCreateInfo.ppEnabledLayerNames = layers.data();
	#else
	deviceCreateInfo.enabledLayerCount = 0;
	#endif
	result = vkCreateDevice((pEngine->primaryGPU->handle), &deviceCreateInfo, nullptr, &(pEngine->primaryGPU->device));

	if (result != VK_SUCCESS) {
		return DG_FALSE;
	}

	vkGetDeviceQueue(pEngine->primaryGPU->device, pEngine->primaryGPU->queueFamilies.graphicsQueueFamily.value(), 0, &pEngine->primaryGPU->graphicsQueue);
	#pragma endregion
	return DG_TRUE;
}

DGAPI void dgUpdate(DgEngine* pEngine) {
	glfwPollEvents();
	GLFWwindow* window;
	for (int i = 0; i < pEngine->windows.size(); i++) {
		window = pEngine->windows.at(i).window;
		if (glfwWindowShouldClose(window)) {
			glfwDestroyWindow(window);
			vkDestroySurfaceKHR(pEngine->vulkan, pEngine->windows.at(i).surface, nullptr);
			pEngine->windows.erase(pEngine->windows.begin() + i);
		}
	}
}

DGAPI void dgTerminateEngine(DgEngine* pEngine) {
	#ifndef NDEBUG
	dgDestroyDebugUtilsMessengerEXT(pEngine->vulkan, pEngine->debugMessenger, nullptr);
	#endif
	for (DgGPU gpu : pEngine->gpus) {
		if (gpu.device != nullptr) {
			vkDestroyDevice(gpu.device, nullptr);
		}
	}

	for (DgWindow window : pEngine->windows) {
		vkDestroySurfaceKHR(pEngine->vulkan, window.surface, nullptr);
	}
	vkDestroyInstance(pEngine->vulkan, nullptr);
	glfwTerminate();
}