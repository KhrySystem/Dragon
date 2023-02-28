#include <dragon/dragon.hpp>

DGAPI DgResult _dgFindQueueFamilies(std::shared_ptr<DgGPU> pGPU) {
	if (pGPU == nullptr) {
		return DG_ARGUMENT_IS_NULL;
	}

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
	return DG_SUCCESS;
}

DGAPI DgResult _dgStartQueueBuffers(std::shared_ptr<DgEngine> pEngine, std::shared_ptr<DgGPU> pGPU) {
	if (pEngine == nullptr || pGPU == nullptr) {
		return DG_ARGUMENT_IS_NULL;
	}

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	float queuePriority = 1.0f;

	VkDeviceQueueCreateInfo graphicsQueueCreateInfo{};
	graphicsQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	graphicsQueueCreateInfo.queueFamilyIndex = pGPU->queueFamilies.graphicsQueueFamily.value();
	graphicsQueueCreateInfo.queueCount = 1;
	graphicsQueueCreateInfo.pQueuePriorities = &queuePriority;

	VkDeviceQueueCreateInfo presentQueueCreateInfo{};
	presentQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	presentQueueCreateInfo.queueFamilyIndex = pGPU->queueFamilies.presentationQueueFamily.value();
	presentQueueCreateInfo.queueCount = 1;
	presentQueueCreateInfo.pQueuePriorities = &queuePriority;

	queueCreateInfos.push_back(graphicsQueueCreateInfo);
	queueCreateInfos.push_back(presentQueueCreateInfo);

	VkDeviceCreateInfo deviceCreateInfo{};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
	deviceCreateInfo.queueCreateInfoCount = queueCreateInfos.size();

	VkPhysicalDeviceFeatures features;
	vkGetPhysicalDeviceFeatures(pGPU->handle, &features);

	deviceCreateInfo.pEnabledFeatures = &features;

	deviceCreateInfo.enabledExtensionCount = pEngine->vkDeviceExtensions.size();
	deviceCreateInfo.ppEnabledExtensionNames = pEngine->vkDeviceExtensions.data();

	#ifndef NDEBUG
		deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(pEngine->validationLayers.size());
		deviceCreateInfo.ppEnabledLayerNames = pEngine->validationLayers.data();
	#else
		deviceCreateInfo.enabledLayerCount = 0;
	#endif
	VkResult result = vkCreateDevice((pGPU->handle), &deviceCreateInfo, nullptr, &(pGPU->device));

	if (result != VK_SUCCESS) {
		return DG_VK_DEVICE_CREATION_FAILED;
	}

	vkGetDeviceQueue(pGPU->device, pGPU->queueFamilies.graphicsQueueFamily.value(), 0, &pGPU->graphicsQueue);
	vkGetDeviceQueue(pGPU->device, pGPU->queueFamilies.presentationQueueFamily.value(), 0, &pGPU->presentationQueue);
	return DG_SUCCESS;
}
