#include <dragon/dragon.hpp>

DGAPI void _dgFindQueueFamilies(DgGPU* pGPU) {
	assert(pGPU != nullptr);

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
	assert(pGPU != nullptr && pWindow != nullptr);

	VkSurfaceCapabilitiesKHR capabilities;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(pGPU->handle, pWindow->surface, &capabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(pGPU->handle, pWindow->surface, &formatCount, nullptr);
	assert(formatCount != 0);
	std::vector<VkSurfaceFormatKHR> formats(formatCount);
	vkGetPhysicalDeviceSurfaceFormatsKHR(pGPU->handle, pWindow->surface, &formatCount, formats.data());

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(pGPU->handle, pWindow->surface, &presentModeCount, nullptr);
	assert(presentModeCount != 0);
	std::vector<VkPresentModeKHR> presentModes(presentModeCount);
	vkGetPhysicalDeviceSurfacePresentModesKHR(pGPU->handle, pWindow->surface, &presentModeCount, presentModes.data());

	_dgChooseSwapSurfaceFormat(pWindow, formats);
	_dgChooseSwapPresentMode(pWindow, presentModes);
	_dgChooseSwapExtent2D(pWindow, &capabilities);

	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = pWindow->surface;

	uint32_t imageCount = pWindow->capabilities.minImageCount + 1;
	if (pWindow->capabilities.maxImageCount > 0 && imageCount > pWindow->capabilities.maxImageCount) {
		imageCount = pWindow->capabilities.maxImageCount;
	}

	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = pWindow->surfaceFormat.format;
	createInfo.imageColorSpace = pWindow->surfaceFormat.colorSpace;
	createInfo.imageExtent = pWindow->extent2D;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	uint32_t queueFamilyIndices[] = { pGPU->queueFamilies.graphicsQueueFamily.value(), pGPU->queueFamilies.presentationQueueFamily.value() };

	if (pGPU->queueFamilies.graphicsQueueFamily != pGPU->queueFamilies.presentationQueueFamily) {
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else {
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	}

	createInfo.preTransform = pWindow->capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = pWindow->presentMode;
	createInfo.clipped = VK_TRUE;

	createInfo.oldSwapchain = VK_NULL_HANDLE;
	VkResult result = vkCreateSwapchainKHR(pGPU->device, &createInfo, nullptr, &pWindow->swapChain);
	if (result != VK_SUCCESS) {
		#ifndef NDEBUG
		std::cerr << "vkCreateSwapchainKHR failed with " << dgConvertVkResultToString(result) << std::endl;
		#endif
		return;
	}

	vkGetSwapchainImagesKHR(pGPU->device, pWindow->swapChain, &imageCount, nullptr);
	pWindow->swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(pGPU->device, pWindow->swapChain, &imageCount, pWindow->swapChainImages.data());
}

