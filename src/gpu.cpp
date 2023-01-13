#pragma once

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

}

DGAPI void dgGetSwapChainSupport(DgGPU* gpu) {

}