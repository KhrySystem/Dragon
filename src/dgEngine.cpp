#include <dragon/Dragon.h>

DGAPI dgEngine* dgCreateEngine(char* appName) {
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	dgEngine* i = new dgEngine();
	i->appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	i->appInfo.pApplicationName = appName;
	i->appInfo.applicationVersion = VK_MAKE_API_VERSION(DRAGON_VERSION_MAJOR, DRAGON_VERSION_MINOR, DRAGON_VERSION_REVISION, DRAGON_VERSION_PATCH);
	i->appInfo.pEngineName = "DragonEngine";
	i->appInfo.engineVersion = VK_MAKE_API_VERSION(1, 3, 204, 0);
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

	return i;
}