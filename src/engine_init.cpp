 #include <dragon/dragon.hpp>

DGAPI DgResult dgAddLayerToEngine(std::shared_ptr<DgEngine> pEngine, std::string layerName) {
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

DgResult _dgAddVkExtensionToEngine(std::shared_ptr<DgEngine> pEngine, const char* extName) {
	for (const char* name : pEngine->vkExtensions) {
		if (strcmp(name, extName) == 0) {
			return DG_VK_EXTENSION_ALREADY_ADDED;
		}
	}

	pEngine->vkExtensions.push_back(extName);
	return DG_SUCCESS;
}

DGAPI void dgSetCallback(std::shared_ptr<DgEngine> pEngine, std::function<void(int, const char*, void*)> fCallback) {
	pEngine->fCallback = fCallback;
}

DgResult _dgSetupVulkan(std::shared_ptr<DgEngine> pEngine) {
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
	if (dgCreateVkInstance(&createInfo, nullptr, &pEngine->vulkan) != VK_SUCCESS) {
		return DG_VK_INSTANCE_CREATION_FAILED;
	}
	return DG_SUCCESS;
}
#ifndef NDEBUG
DgResult _dgSetupVulkanDebug(std::shared_ptr<DgEngine> pEngine) {
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
DgResult _dgSetupGPUs(std::shared_ptr<DgEngine> pEngine) {
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
		std::shared_ptr<DgGPU> gpuRef(new DgGPU);
		gpuRef->handle = dev;
		_dgFindQueueFamilies(gpuRef);
		pEngine->gpus.push_back(gpuRef);
	}

	if (!pEngine->gpus.at(0)->queueFamilies.graphicsQueueFamily.has_value()) {
		return DG_NO_GRAPHICS_QUEUE_FOUND;
	}
	pEngine->primaryGPU = pEngine->gpus.at(0);
	return DG_SUCCESS;
}

DGAPI DgResult dgCreateEngine(std::shared_ptr<DgEngine> pEngine, DgEngineCreateInfo createInfo) {
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

	pEngine->vkExtensions.insert(pEngine->vkExtensions.begin(), glfwExtensions.begin(), glfwExtensions.end());
	pEngine->vkExtensions.insert(pEngine->vkExtensions.begin(), createInfo.vkExtensions.begin(), createInfo.vkExtensions.end());
	
	std::vector<const char*>::iterator itr1 = pEngine->vkExtensions.begin();
	std::unordered_set<const char*> s1;

	for (auto curr = pEngine->vkExtensions.begin(); curr != pEngine->vkExtensions.end(); ++curr)
	{
		if (s1.insert(*curr).second) {
			*itr1++ = *curr;
		}
	}

	pEngine->vkExtensions.erase(itr1, pEngine->vkExtensions.end());

	pEngine->vkDeviceExtensions.insert(pEngine->vkDeviceExtensions.begin(), createInfo.vkDeviceExtensions.begin(), createInfo.vkDeviceExtensions.end());

	std::vector<const char*>::iterator itr = pEngine->vkDeviceExtensions.begin();
	std::unordered_set<const char*> s;

	for (auto curr = pEngine->vkDeviceExtensions.begin(); curr != pEngine->vkDeviceExtensions.end(); ++curr)
	{
		if (s.insert(*curr).second) {
			*itr++ = *curr;
		}
	}

	pEngine->vkDeviceExtensions.erase(itr, pEngine->vkDeviceExtensions.end());

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

