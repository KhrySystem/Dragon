#include <dragon/dragon.hpp>

DGAPI void dgFindQueueFamilies(DgGPU* pGPU) {
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

DGAPI void dgGeneratePresentationQueue(DgGPU* pGPU, DgWindow* pWindow) {
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

DGAPI void dgGetSwapChainSupport(DgGPU* gpu) {

}