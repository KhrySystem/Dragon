#include <dragon/dragon.hpp>

DgResult _dgGeneratePresentationQueue(std::shared_ptr<DgWindow> pWindow) {
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
	return DG_SUCCESS;
}

DgResult _dgChooseSwapSurfaceFormat(std::shared_ptr<DgWindow> pWindow, const std::vector<VkSurfaceFormatKHR>& pFormats) {
	// Ensure window is not null
	if (pWindow == nullptr) {
		return DG_ARGUMENT_IS_NULL;
	}

	// Loop through all formats to find one that is good
	for (const VkSurfaceFormatKHR format : pFormats) {
		if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			pWindow->surfaceFormat = format;
			return DG_SUCCESS;
		}
	}
	// return the default format
	pWindow->surfaceFormat = pFormats.at(0);
	return DG_SUCCESS;
}

DgResult _dgChooseSwapPresentMode(std::shared_ptr<DgWindow> pWindow, const std::vector<VkPresentModeKHR>& pPresentModes) {
	// Ensure window is not null
	if (pWindow == nullptr) {
		return DG_ARGUMENT_IS_NULL;
	}

#if (BOOST_OS_WINDOWS || BOOST_OS_LINUX || BOOST_OS_MACOS) && !defined(DRAGON_LOW_POWER) // Desktop platforms, high power mode active
	// loop through all modes 
	for (const VkPresentModeKHR availablePresentMode : pPresentModes) {
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) { // VK_PRESENT_MODE_MAILBOX_KHR is the highest power mode, triple-buffered. 
			pWindow->presentMode = availablePresentMode;
			return DG_SUCCESS;
		}
	}
#endif
	// return the default mode
	pWindow->presentMode = VK_PRESENT_MODE_FIFO_KHR; // VK_PRESENT_MODE_FIFO_KHR is the default double-bufferred mode. Best for mobile devices and devices without triple buffer capabilities.
	return DG_SUCCESS;
}

DgResult _dgChooseSwapExtent2D(std::shared_ptr<DgWindow> pWindow, const VkSurfaceCapabilitiesKHR* pCapabilities) {
	if (pWindow == nullptr) {
		return DG_ARGUMENT_IS_NULL;
	}
	pWindow->capabilities = *pCapabilities;
	if (pCapabilities->currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		pWindow->extent2D = pCapabilities->currentExtent;
	}
	else {
		int width, height;
		glfwGetFramebufferSize(pWindow->window, &width, &height);

		VkExtent2D actualExtent = {
			static_cast<uint32_t>(width),
			static_cast<uint32_t>(height)
		};

		actualExtent.width = std::clamp(actualExtent.width, pCapabilities->minImageExtent.width, pCapabilities->maxImageExtent.width);
		actualExtent.height = std::clamp(actualExtent.height, pCapabilities->minImageExtent.height, pCapabilities->maxImageExtent.height);

		pWindow->extent2D = actualExtent;
	}
	return DG_SUCCESS;
}

DgResult _dgCreateImageViews(std::shared_ptr<DgWindow> pWindow) {
	if (pWindow == nullptr) {
		return DG_ARGUMENT_IS_NULL;
	}

	pWindow->swapChainImageViews.resize(pWindow->swapChainImages.size());

	for (int i = 0; i < pWindow->swapChainImages.size(); i++) {
		VkImageViewCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = pWindow->swapChainImages[i];

		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = pWindow->surfaceFormat.format;

		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		VkResult result = vkCreateImageView(pWindow->pGPU->device, &createInfo, nullptr, &pWindow->swapChainImageViews[i]);

		if (result != VK_SUCCESS) {
			return DG_VK_IMAGE_VIEW_CREATION_FAILED;
		}
	}
	return DG_SUCCESS;
}

DGAPI DgResult _dgCreateSwapchain(std::shared_ptr<DgWindow> pWindow) {
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

	if (_dgChooseSwapSurfaceFormat(pWindow, formats) != DG_SUCCESS) {
		return DG_VK_SURFACE_FORMAT_SELECTION_FAILED;
	}

	if (_dgChooseSwapPresentMode(pWindow, presentModes) != DG_SUCCESS) {
		return DG_VK_PRESENT_MODE_SELECTION_FAILED;
	}

	if (_dgChooseSwapExtent2D(pWindow, &capabilities) != DG_SUCCESS) {
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

	createInfo.oldSwapchain = pWindow->swapChain;
	if (vkCreateSwapchainKHR(pWindow->pGPU->device, &createInfo, nullptr, &pWindow->swapChain) != VK_SUCCESS) {
		return DG_VK_SWAPCHAIN_CREATION_FAILED;
	}

	if (vkGetSwapchainImagesKHR(pWindow->pGPU->device, pWindow->swapChain, &imageCount, nullptr) != VK_SUCCESS) {
		return DG_VK_GET_SWAPCHAIN_IMAGES_FAILED;
	}

	pWindow->swapChainImages.resize(imageCount);
	if (vkGetSwapchainImagesKHR(pWindow->pGPU->device, pWindow->swapChain, &imageCount, pWindow->swapChainImages.data()) != VK_SUCCESS) {
		return DG_VK_GET_SWAPCHAIN_IMAGES_FAILED;
	}
	if (_dgCreateImageViews(pWindow) != DG_SUCCESS) {
		return DG_VK_IMAGE_VIEW_CREATION_FAILED;
	}
	return DG_SUCCESS;
}

DgResult _dgGenerateGraphicsPipeline(std::shared_ptr<DgWindow> pWindow) {
	if (pWindow == nullptr) {
		return DG_ARGUMENT_IS_NULL;
	}

	VkShaderModuleCreateInfo vertCreateInfo{};
	vertCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	vertCreateInfo.codeSize = DRAGON_VERT_SHADER_SIZE;
	vertCreateInfo.pCode = reinterpret_cast<const uint32_t*>(DRAGON_VERT_SHADER);

	VkShaderModule vertModule;
	VkResult result = vkCreateShaderModule(pWindow->pGPU->device, &vertCreateInfo, nullptr, &vertModule);
	if (result != VK_SUCCESS) {
		vkDestroyShaderModule(pWindow->pGPU->device, vertModule, nullptr);
		return DG_VK_SHADER_MODULE_CREATION_FAILED;
	}
	pWindow->shaderModules.push_back(vertModule);

	VkShaderModuleCreateInfo fragCreateInfo{};
	fragCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	fragCreateInfo.codeSize = DRAGON_FRAG_SHADER_SIZE;
	fragCreateInfo.pCode = reinterpret_cast<const uint32_t*>(DRAGON_FRAG_SHADER);

	VkShaderModule fragModule;
	result = vkCreateShaderModule(pWindow->pGPU->device, &fragCreateInfo, nullptr, &fragModule);
	if (result != VK_SUCCESS) {
		vkDestroyShaderModule(pWindow->pGPU->device, fragModule, nullptr);
		return DG_VK_SHADER_MODULE_CREATION_FAILED;
	}
	pWindow->shaderModules.push_back(fragModule);

	VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = vertModule;
	vertShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragModule;
	fragShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

	VkVertexInputBindingDescription bindingDescription = _dgGenerateVertexBindingDescription();
	std::array<VkVertexInputAttributeDescription, DRAGON_VERTEX_ATTRIBUTE_COUNT> attributeDescriptions = _dgGetAttributeDescriptions();

	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	vertexInputInfo.vertexAttributeDescriptionCount = DRAGON_VERTEX_ATTRIBUTE_COUNT;
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	VkPipelineViewportStateCreateInfo viewportState{};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.scissorCount = 1;

	VkPipelineRasterizationStateCreateInfo rasterizer{};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;

	VkPipelineMultisampleStateCreateInfo multisampling{};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	VkPipelineColorBlendAttachmentState colorBlendAttachment{};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;

	VkPipelineColorBlendStateCreateInfo colorBlending{};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY;
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f;
	colorBlending.blendConstants[1] = 0.0f;
	colorBlending.blendConstants[2] = 0.0f;
	colorBlending.blendConstants[3] = 0.0f;

	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

	result = vkCreatePipelineLayout(pWindow->pGPU->device, &pipelineLayoutInfo, nullptr, &pWindow->pipelineLayout);

	if (result != VK_SUCCESS) {
		vkDestroyPipelineLayout(pWindow->pGPU->device, pWindow->pipelineLayout, nullptr);
		return DG_VK_PIPELINE_CREATION_FAILED;
	}

	VkAttachmentDescription colorAttachment{};
	colorAttachment.format = pWindow->surfaceFormat.format; // Does not fall within range and is not added by an extension? Does not contain VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference colorAttachmentRef{};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef; // Validation Error references the above

	VkRenderPassCreateInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = 1;
	renderPassInfo.pAttachments = &colorAttachment;
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;

	std::vector<VkDynamicState> dynamicStates = {
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR
	};
	VkPipelineDynamicStateCreateInfo dynamicState{};
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
	dynamicState.pDynamicStates = dynamicStates.data();

	result = vkCreateRenderPass(pWindow->pGPU->device, &renderPassInfo, nullptr, &pWindow->renderPass);
	if (result != VK_SUCCESS) {
		vkDestroyRenderPass(pWindow->pGPU->device, pWindow->renderPass, nullptr);
		return DG_VK_RENDER_PASS_CREATION_FAILED;
	}

	VkGraphicsPipelineCreateInfo pipelineInfo{};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pDepthStencilState = nullptr; // Optional
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDynamicState = &dynamicState;
	pipelineInfo.layout = pWindow->pipelineLayout;
	pipelineInfo.renderPass = pWindow->renderPass;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
	pipelineInfo.basePipelineIndex = -1; // Optional

	result = vkCreateGraphicsPipelines(pWindow->pGPU.get()->device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pWindow->graphicsPipeline);

	if (result != VK_SUCCESS) {
		return DG_VK_GRAPHICS_PIPELINE_CREATION_FAILED;
	}

	return DG_SUCCESS;
}

DgResult _dgCreateFramebuffers(std::shared_ptr<DgWindow> pWindow) {
	pWindow->swapChainFramebuffers.resize(pWindow->swapChainImageViews.size());

	for (size_t i = 0; i < pWindow->swapChainImageViews.size(); i++) {
		VkImageView attachments[] = {
			pWindow->swapChainImageViews[i]
		};

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = pWindow->renderPass;
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments = attachments;
		framebufferInfo.width = pWindow->extent2D.width;
		framebufferInfo.height = pWindow->extent2D.height;
		framebufferInfo.layers = 1;

		VkResult result = vkCreateFramebuffer(pWindow->pGPU->device, &framebufferInfo, nullptr, &pWindow->swapChainFramebuffers[i]);

		if (result != VK_SUCCESS) {
			return DG_VK_FRAMEBUFFER_CREATION_FAILED;
		}
	}


	return DG_SUCCESS;
}

DgResult _dgCreateCommandPool(std::shared_ptr<DgWindow> pWindow) {
	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = pWindow->pGPU->queueFamilies.graphicsQueueFamily.value();

	VkResult result = vkCreateCommandPool(pWindow->pGPU->device, &poolInfo, nullptr, &pWindow->commandPool);
	if (result != VK_SUCCESS) {
		return DG_VK_COMMAND_POOL_CREATION_FAILED;
	}
	return DG_SUCCESS;
}

DgResult _dgCreateSyncObjects(std::shared_ptr<DgWindow> pWindow) {
	pWindow->imageAvailableSemaphores.resize(DRAGON_RENDER_FRAME_MAX);
	pWindow->renderFinishedSemaphores.resize(DRAGON_RENDER_FRAME_MAX);
	pWindow->inFlightFences.resize(DRAGON_RENDER_FRAME_MAX);

	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (int i = 0; i < pWindow->imageAvailableSemaphores.size(); i++)
		if (vkCreateSemaphore(pWindow->pGPU->device, &semaphoreInfo, nullptr, &pWindow->imageAvailableSemaphores.at(i)) != VK_SUCCESS) {
			return DG_VK_SEMAPHORE_CREATION_FAILED;
		}
	for (int i = 0; i < pWindow->renderFinishedSemaphores.size(); i++)
		if (vkCreateSemaphore(pWindow->pGPU->device, &semaphoreInfo, nullptr, &pWindow->renderFinishedSemaphores.at(i)) != VK_SUCCESS) {
			return DG_VK_SEMAPHORE_CREATION_FAILED;
		}
	for (int i = 0; i < pWindow->inFlightFences.size(); i++)
		if (vkCreateFence(pWindow->pGPU->device, &fenceInfo, nullptr, &pWindow->inFlightFences.at(i)) != VK_SUCCESS) {
			return DG_VK_FENCE_CREATION_FAILED;
		}

	return DG_SUCCESS;
}

DGAPI DgResult dgCreateWindow(std::shared_ptr<DgEngine> pEngine, std::string title, unsigned int width, unsigned int height, DgBool32 isResizable, DgBool32 isFullscreen, std::shared_ptr<DgWindow> pWindow) {
	if (pEngine == nullptr || pWindow == nullptr) {
		return DG_ARGUMENT_IS_NULL;
	}

	GLFWmonitor* monitor;
	if (isFullscreen) {
		monitor = glfwGetPrimaryMonitor();
		if (monitor != NULL) {
			const GLFWvidmode* mode = glfwGetVideoMode(monitor);
			width = mode->width;
			height = mode->height;
			glfwWindowHint(GLFW_RED_BITS, mode->redBits);
			glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
			glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
			glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
		}

	}
	else {
		monitor = NULL;
	}

	glfwWindowHint(GLFW_RESIZABLE, isResizable);
	GLFWwindow* glfw = glfwCreateWindow(width, height, title.c_str(), monitor, NULL);
	if (glfw == NULL) {
		std::cout << "Window was null" << std::endl;
		return DG_GLFW_WINDOW_NULL_AFTER_CREATION;
	}

	pWindow->window = glfw;
	pWindow->pGPU = std::shared_ptr<DgGPU>(pEngine->primaryGPU);
	pWindow->imageAvailableSemaphores.resize(DRAGON_RENDER_FRAME_MAX);
	pWindow->renderFinishedSemaphores.resize(DRAGON_RENDER_FRAME_MAX);
	pWindow->inFlightFences.resize(DRAGON_RENDER_FRAME_MAX);

	if (glfwCreateWindowSurface(pEngine->vulkan, pWindow->window, NULL, &pWindow->surface) != VK_SUCCESS) {
		return DG_GLFW_WINDOW_SURFACE_CREATION_FAILED;
	}

	DgResult r;
	if (!pWindow.get()->pGPU->queueFamilies.presentationQueueFamily.has_value()) {
		r = _dgGeneratePresentationQueue(pWindow);
		if (r != DG_SUCCESS) {
			_dgDestroyWindow(pEngine->vulkan, pWindow);
			return r;
		}
		r = _dgStartQueueBuffers(pEngine, pWindow->pGPU);
		if (r != DG_SUCCESS) {
			_dgDestroyWindow(pEngine->vulkan, pWindow);
			return r;
		}
	}

	r = _dgCreateSwapchain(pWindow);
	if (r != DG_SUCCESS) {
		_dgDestroyWindow(pEngine->vulkan, pWindow);
		return r;
	}

	r = _dgGenerateGraphicsPipeline(pWindow);
	if (r != DG_SUCCESS) {
		_dgDestroyWindow(pEngine->vulkan, pWindow);
		return r;
	}

	r = _dgCreateFramebuffers(pWindow);
	if (r != DG_SUCCESS) {
		_dgDestroyWindow(pEngine->vulkan, pWindow);
		return r;
	}

	r = _dgCreateCommandPool(pWindow);
	if (r != DG_SUCCESS) {
		_dgDestroyWindow(pEngine->vulkan, pWindow);
		return r;
	}

	r = _dgCreateSyncObjects(pWindow);
	if (r != DG_SUCCESS) {
		_dgDestroyWindow(pEngine->vulkan, pWindow);
		return r;
	}

	pWindow->currentFrame = 0;
	pEngine->windows.push_back(pWindow);
	return DG_SUCCESS;
}






