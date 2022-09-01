#include <dragon/Dragon.h>

DGAPI dgEngine* dgCreateEngine(const char* appName) {
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	dgEngine* i = new dgEngine();
	i->appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	i->appInfo.pApplicationName = appName;
	i->appInfo.applicationVersion = VK_MAKE_API_VERSION(DRAGON_VERSION_PATCH, DRAGON_VERSION_MAJOR, DRAGON_VERSION_MINOR, DRAGON_VERSION_REVISION);
	i->appInfo.pEngineName = "DragonEngine";
	i->appInfo.engineVersion = VK_HEADER_VERSION_COMPLETE;
	i->appInfo.apiVersion = VK_API_VERSION_1_3;

	i->createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	i->createInfo.pApplicationInfo = &(i->appInfo);

	i->glfwExtensions = glfwGetRequiredInstanceExtensions(&(i->glfwExtensionCount));

	i->createInfo.enabledExtensionCount = i->glfwExtensionCount;
	i->createInfo.ppEnabledExtensionNames = i->glfwExtensions;

	i->createInfo.enabledLayerCount = 0;
	if(vkCreateInstance(&(i->createInfo), nullptr, &(i->instance)) != VK_SUCCESS)
		return NULL;

	vkEnumerateInstanceExtensionProperties(nullptr, &(i->extensionCount), nullptr);
	i->extensions.resize(i->extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &(i->extensionCount), i->extensions.data());
	#ifndef DRAGON_FULL_POWER
		vkEnumerateInstanceLayerProperties(&i->layerCount, nullptr);
		i->availableLayers.resize(i->layerCount);
		vkEnumerateInstanceLayerProperties(&(i->layerCount), i->availableLayers.data());
		for (const char* layerName : i->requestedValidationLayers) {
			bool layerFound = false;

			for (const auto& layerProperties : i->availableLayers) {
				if (strcmp(layerName, layerProperties.layerName) == 0) {
					layerFound = true;
					break;
				}
			}

			if (!layerFound) {
				printf("Layer %s not found.", layerName);
			}
		}
	#endif
	i->appName = appName;
	std::vector<GPU*> gpus = dgGetGPUs(i);
	return i;
}

DGAPI std::vector<GPU*> dgGetGPUs(dgEngine* engine) {
	vkEnumeratePhysicalDevices(engine->instance, &engine->deviceCount, DG_NULL);
	std::vector<VkPhysicalDevice> devices(engine->deviceCount);
	std::vector<GPU*> out;
	if(engine->deviceCount == 0) return out;
	vkEnumeratePhysicalDevices(engine->instance, &engine->deviceCount, devices.data());
	for(int i = 0; i >= engine->deviceCount; i++) {
		out.push_back(dgCreateGPU(devices.at(i)));
	}
	return out;
}

DGAPI DG_BOOL dgShareResources(dgEngine* e1, dgEngine* e2) {
	if(e1->window != NULL || e2->window != NULL) {
		return DG_FALSE;
	}
	e1->shared = e2;
	e2->shared = e1;
	return DG_TRUE;
}

DGAPI void dgUpdateEngine(dgEngine* engine) {
	return;
}