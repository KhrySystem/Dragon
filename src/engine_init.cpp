 #include <dragon/dragon.hpp>

DGAPI DgResult dgAddLayerToEngine(boost::shared_ptr<DgEngine> pEngine, std::string layerName) {
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

DGAPI DgResult dgAddVkExtensionToEngine(boost::shared_ptr<DgEngine> pEngine, const char* extName) {
	for (const char* name : pEngine->vkExtensions) {
		if (strcmp(name, extName) == 0) {
			return DG_VK_EXTENSION_ALREADY_ADDED;
		}
	}

	pEngine->vkExtensions.push_back(extName);
	return DG_SUCCESS;
}

DGAPI void dgSetCallback(boost::shared_ptr<DgEngine> pEngine, std::function<void(int, const char*, void*)> fCallback) {
	pEngine->fCallback = fCallback;
}

DgResult _dgSetupVulkan(boost::shared_ptr<DgEngine> pEngine) {
	// App info. Contains info about the Engine, the user app, etc.
	VkApplicationInfo appInfo{};
	// Use the version that matches the shaders
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
DgResult _dgSetupVulkanDebug(boost::shared_ptr<DgEngine> pEngine) {
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
DgResult _dgSetupGPUs(boost::shared_ptr<DgEngine> pEngine) {
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
		boost::shared_ptr<DgGPU> gpuRef(&gpu);
		gpu.handle = dev;
		vkGetPhysicalDeviceFeatures(dev, &(gpu.features));
		vkGetPhysicalDeviceProperties(dev, &(gpu.properties));
		_dgFindQueueFamilies(gpuRef);
		pEngine->gpus.push_back(gpu);
		gpuRef.reset();
	}

	if (!pEngine->gpus.at(0).queueFamilies.graphicsQueueFamily.has_value()) {
		return DG_NO_GRAPHICS_QUEUE_FOUND;
	}
	pEngine->primaryGPU = boost::shared_ptr<DgGPU>(&pEngine->gpus.at(0));
	return DG_SUCCESS;
}

DGAPI DgResult dgCreateEngine(boost::shared_ptr<DgEngine> pEngine) {
	if (pEngine == nullptr) {
		return DG_ARGUMENT_IS_NULL;
	}
	DgResult r;

	if (DgEngine::activeEngineCount <= 0) {
		if (!glfwInit()) {
			return DG_GLFW_INITIALIZATION_FAILED;
		}
	}

	glfwSetErrorCallback(_dgGlfwCallback);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	// Vulkan Initialization
	if (!glfwVulkanSupported()) {
		return DG_GLFW_VULKAN_NOT_SUPPORTED;
	}

	uint32_t count;
	const char** extensions = glfwGetRequiredInstanceExtensions(&count);
	if (count == 0) {
		return DG_GLFW_NO_INSTANCE_EXTENSIONS_FOUND;
	}
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
