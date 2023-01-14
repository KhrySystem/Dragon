#include <iterator>
#include <vector>

#include <dragon/dragon.hpp>

DGAPI DgBool32 dgAddLayerToEngine(DgEngine* pEngine, std::string layerName) {
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
					return DG_TRUE;
				}
			}

			pEngine->validationLayers.push_back(layerName.c_str());
			return DG_TRUE;
		}
	}
	#ifndef NDEBUG
	std::cout << "VkValidationLayer " << layerName << " not found in available extensions for DgEngine instance located at " << pEngine << std::endl;
	#endif
	return DG_FALSE;
#else
	return DG_FALSE;
#endif
}


DGAPI DgBool32 dgAddVkExtensionToEngine(DgEngine* pEngine, const char* extName) {
	pEngine->vkExtensions.push_back(extName);
	return DG_TRUE;
}

DGAPI void dgSetCallback(DgEngine* pEngine, std::function<void(DgMessage*)> optfCallback) {
	pEngine->fCallback = optfCallback;
}

void _dgSetupVulkan(DgEngine* pEngine) {
	// App info. Contains info about the Engine, the user app, etc.
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

	// Ensure result is ok
	assert(result == VK_SUCCESS);
}
#ifndef NDEBUG
void _dgSetupVulkanDebug(DgEngine* pEngine) {
	VkDebugUtilsMessengerCreateInfoEXT messengerCreateInfo{};
	messengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	messengerCreateInfo.messageSeverity =  VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	messengerCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	messengerCreateInfo.pfnUserCallback = dgVulkanDebugCallback;
	messengerCreateInfo.pUserData = nullptr;

	VkResult result = dgCreateDebugUtilsMessengerEXT(pEngine->vulkan, &messengerCreateInfo, nullptr, &(pEngine->debugMessenger));
	assert(result == VK_SUCCESS);
}
#endif
void _dgSetupGPUs(DgEngine* pEngine) {
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(pEngine->vulkan, &deviceCount, nullptr);

	assert(deviceCount != 0);

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
	assert(pEngine->gpus.at(0).queueFamilies.graphicsQueueFamily.has_value());
	pEngine->primaryGPU = &pEngine->gpus.at(0);
}

void _dgStartQueueBuffers(DgEngine* pEngine) {
	
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

	assert(result == VK_SUCCESS);

	vkGetDeviceQueue(pEngine->primaryGPU->device, pEngine->primaryGPU->queueFamilies.graphicsQueueFamily.value(), 0, &pEngine->primaryGPU->graphicsQueue);
}


DGAPI DgBool32 dgCreateEngine(DgEngine* pEngine) {
	// Must initialize GLFW first
	#if BOOST_OS_WINDOWS
	pEngine->vkExtensions.push_back("VK_KHR_win32_surface");
	#endif

	assert(glfwInit());
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	// Vulkan Initialization
	glfwSetErrorCallback(dgGLFWErrorCallback);

	assert(glfwVulkanSupported());

	_dgSetupVulkan(pEngine);

	// VkDebugMessenger Initialization
	#ifndef NDEBUG
	_dgSetupVulkanDebug(pEngine);
	#endif

	// Get GPUs and start compute API
	_dgSetupGPUs(pEngine);

	// Start Graphics Queue Buffers
	_dgStartQueueBuffers(pEngine);

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
		
	for (int i = 0; i < pEngine->windows.size(); i++) {
		glfwDestroyWindow(pEngine->windows.at(i).window);
		vkDestroySurfaceKHR(pEngine->vulkan, pEngine->windows.at(i).surface, nullptr);
		pEngine->windows.erase(pEngine->windows.begin() + i);
	}
	
	vkDestroyInstance(pEngine->vulkan, nullptr);
	glfwTerminate();
}