#include <dragon/dragon.hpp>

DGAPI void _dgFindQueueFamilies(DgGPU* pGPU) {
	dgQueueFamilies indices;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(pGPU->handle, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(pGPU->handle, &queueFamilyCount, queueFamilies.data());

	for (int i = 0; i < queueFamilies.size(); i++) {
		if (queueFamilies.at(i).queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			indices.graphicsQueueFamily = i;
		}

	}

	pGPU->queueFamilies = indices;
}

DGAPI void _dgGeneratePresentationQueue(DgGPU* pGPU, DgWindow* pWindow) {
	assert(pGPU != nullptr && pWindow != nullptr);

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(pGPU->handle, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(pGPU->handle, &queueFamilyCount, queueFamilies.data());

	for (int i = 0; i < queueFamilies.size(); i++) {
		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(pGPU->handle, i, pWindow->surface, &presentSupport);

		if (presentSupport)
			pGPU->queueFamilies.presentationQueueFamily = i;
	}

	VkDeviceQueueCreateInfo presentQueueCreateInfo{};
	presentQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	presentQueueCreateInfo.queueFamilyIndex =pGPU->queueFamilies.presentationQueueFamily.value();
	presentQueueCreateInfo.queueCount = 1;
	float queuePriority = 1.0f;
	presentQueueCreateInfo.pQueuePriorities = &queuePriority;
}

DGAPI void _dgGetSwapChainSupport(DgGPU* pGPU, DgWindow* pWindow) {
	dgSwapChainSupportDetails details;
	assert(pGPU != nullptr && pWindow != nullptr);

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(pGPU->handle, pWindow->surface, &details.capabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(pGPU->handle, pWindow->surface, &formatCount, nullptr);
	
	assert(formatCount != 0);

	details.formats.resize(formatCount);
	vkGetPhysicalDeviceSurfaceFormatsKHR(pGPU->handle, pWindow->surface, &formatCount, details.formats.data());

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(pGPU->handle, pWindow->surface, &presentModeCount, nullptr);

	assert(presentModeCount != 0);

	details.presentModes.resize(presentModeCount);
	vkGetPhysicalDeviceSurfacePresentModesKHR(pGPU->handle, pWindow->surface, &presentModeCount, details.presentModes.data());
}

