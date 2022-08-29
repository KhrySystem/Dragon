#include <dragon/dragon.hpp>

DGAPI dgEngine* dgCreateEngine(const char* appName) {
	dgEngine* i = new dgEngine();
	#if defined(DRAGON_USE_VULKAN_GRAPHICS)
		i->appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		i->appInfo.pApplicationName = appName;
		i->appInfo.applicationVersion = VK_MAKE_API_VERSION(0, 0, 1, 0);
		i->appInfo.pEngineName = "DragonEngine";
		i->appInfo.engineVersion = VK_MAKE_API_VERSION(1, 3, 204, 0);
		i->appInfo.apiVersion = VK_API_VERSION_1_3;

		i->createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		i->createInfo.pApplicationInfo = &(i->appInfo);

		i->glfwExtensions = glfwGetRequiredInstanceExtensions(&(i->glfwExtensionCount));

		i->createInfo.enabledExtensionCount = i->glfwExtensionCount;
		i->createInfo.ppEnabledExtensionNames = i->glfwExtensions;

		i->createInfo.enabledLayerCount = 0;
		if(vkCreateInstance(&(i->createInfo), nullptr, &(i->vulkan)) != VK_SUCCESS)
			return NULL;

		vkEnumerateInstanceExtensionProperties(nullptr, &(i->extensionCount), nullptr);
		i->extensions.resize(i->extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &(i->extensionCount), i->extensions.data());
	#endif
	return i;
}