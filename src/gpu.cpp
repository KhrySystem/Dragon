#include <dragon/dragon.hpp>

DGAPI DgResult _dgFindQueueFamilies(DgGPU* pGPU) {
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

DGAPI DgResult _dgGeneratePresentationQueue(DgWindow* pWindow) {
	if (pWindow == nullptr) {
		return DG_ARGUMENT_IS_NULL;
	}

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(pWindow->pGPU->handle, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(pWindow->pGPU->handle, &queueFamilyCount, queueFamilies.data());

	for (int i = 0; i < queueFamilies.size(); i++) {
		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(pWindow->pGPU->handle, i, pWindow->surface, &presentSupport);

		if (presentSupport)
			pWindow->pGPU->queueFamilies.presentationQueueFamily = i;
	}

	VkDeviceQueueCreateInfo presentQueueCreateInfo{};
	presentQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	presentQueueCreateInfo.queueFamilyIndex = pWindow->pGPU->queueFamilies.presentationQueueFamily.value();
	presentQueueCreateInfo.queueCount = 1;
	float queuePriority = 1.0f;
	presentQueueCreateInfo.pQueuePriorities = &queuePriority;
	return DG_SUCCESS;
}

DGAPI DgResult _dgGetSwapChainSupport(DgWindow* pWindow) {
	if (pWindow == nullptr) {
		return DG_ARGUMENT_IS_NULL;
	}

	VkSurfaceCapabilitiesKHR capabilities;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(pWindow->pGPU->handle, pWindow->surface, &capabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(pWindow->pGPU->handle, pWindow->surface, &formatCount, nullptr);
	if (formatCount == 0) {
		return DG_NO_VK_SURFACE_FORMATS_AVAILABLE;
	}
	std::vector<VkSurfaceFormatKHR> formats(formatCount);
	vkGetPhysicalDeviceSurfaceFormatsKHR(pWindow->pGPU->handle, pWindow->surface, &formatCount, formats.data());

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(pWindow->pGPU->handle, pWindow->surface, &presentModeCount, nullptr);
	if (presentModeCount == 0) {
		return DG_NO_VK_PRESENT_MODES_AVAILABLE;
	}
	std::vector<VkPresentModeKHR> presentModes(presentModeCount);
	vkGetPhysicalDeviceSurfacePresentModesKHR(pWindow->pGPU->handle, pWindow->surface, &presentModeCount, presentModes.data());

	DgResult r = _dgChooseSwapSurfaceFormat(pWindow, formats);
	if (r != DG_SUCCESS) {
		return DG_VK_SURFACE_FORMAT_SELECTION_FAILED;
	}
	r = _dgChooseSwapPresentMode(pWindow, presentModes);
	if (r != DG_SUCCESS) {
		return DG_VK_PRESENT_MODE_SELECTION_FAILED;
	}
	r = _dgChooseSwapExtent2D(pWindow, &capabilities);
	if (r != DG_SUCCESS) {
		return DG_VK_EXTENT_2D_SELECTION_FAILED;
	}

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

	uint32_t queueFamilyIndices[] = { pWindow->pGPU->queueFamilies.graphicsQueueFamily.value(), pWindow->pGPU->queueFamilies.presentationQueueFamily.value() };

	if (pWindow->pGPU->queueFamilies.graphicsQueueFamily != pWindow->pGPU->queueFamilies.presentationQueueFamily) {
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
	VkResult result = vkCreateSwapchainKHR(pWindow->pGPU->device, &createInfo, nullptr, &pWindow->swapChain);
	if (result != VK_SUCCESS) {
		#ifndef NDEBUG
		std::cerr << "vkCreateSwapchainKHR failed with " << dgConvertVkResultToString(result) << std::endl;
		#endif
		return DG_VK_SWAPCHAIN_CREATION_FAILED;
	}

	vkGetSwapchainImagesKHR(pWindow->pGPU->device, pWindow->swapChain, &imageCount, nullptr);
	pWindow->swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(pWindow->pGPU->device, pWindow->swapChain, &imageCount, pWindow->swapChainImages.data());
	_dgCreateImageViews(pWindow);
	return DG_SUCCESS;
}

