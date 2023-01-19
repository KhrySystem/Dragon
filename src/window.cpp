#include <dragon/dragon.hpp>

DGAPI DgResult dgCreateWindow(DgEngine* pEngine, std::string title, unsigned int width, unsigned int height, DgBool32 isResizable) {
	glfwWindowHint(GLFW_RESIZABLE, isResizable);
	GLFWwindow* glfw = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	// Check if window is null 
	if (glfw == NULL) {
		return DG_GLFW_WINDOW_NULL_AFTER_CREATION;
	}

	DgWindow window;
	// Create the reference
	window.window = glfw;
	// Create window surface (the important thing)
	VkResult result = glfwCreateWindowSurface(pEngine->vulkan, window.window, nullptr, &window.surface);
	// The current primary GPU is now responsible for this window
	window.pGPU = pEngine->primaryGPU;
	
	// If for some reason it fails, destroy everything created by this function up to this point
	if (result != VK_SUCCESS) {
		dgDestroyWindow(pEngine->vulkan, &window);
		return DG_GLFW_WINDOW_SURFACE_CREATION_FAILED;
	}

	DgResult r;
	// if the current GPU doesn't have a presentation queue, generate one
	if (pEngine->primaryGPU->presentationQueue == nullptr) {
		 r = _dgGeneratePresentationQueue(&window);
		 if (r != DG_SUCCESS) {
			 // Destroy everything this function has created so far
			 dgDestroyWindow(pEngine->vulkan, &window);
			 return r;
		 }
	}

	// Generate this window's graphics pipeline
	r = _dgGenerateGraphicsPipeline(&window);
	if (r != DG_SUCCESS) {
		dgDestroyWindow(pEngine->vulkan, &window);
		return r;
	}

	r = _dgCreateFramebuffers(&window);
	r = _dgGenerateGraphicsPipeline(&window);
	if (r != DG_SUCCESS) {
		dgDestroyWindow(pEngine->vulkan, &window);
		return r;
	}

	pEngine->windows.push_back(window);
	return DG_SUCCESS;
}

DGAPI int dgGetWindowCount(DgEngine* pEngine) {
	return pEngine->windows.size();
}

DGAPI DgResult _dgChooseSwapSurfaceFormat(DgWindow* pWindow, const std::vector<VkSurfaceFormatKHR>& pFormats) {
	// Ensure window is not null
	if (pWindow == nullptr) {
		return DG_ARGUMENT_IS_NULL;
	}

	// Loop through all formats to find one that is good
	for (const VkSurfaceFormatKHR format : pFormats) {
		std::cout << "format:" << format.format << std::endl;
		if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			pWindow->surfaceFormat = format;
			return DG_SUCCESS;
		}
	}
	// return the default format
	pWindow->surfaceFormat = pFormats.at(0);
	return DG_SUCCESS;
}

DGAPI DgResult _dgChooseSwapPresentMode(DgWindow* pWindow, const std::vector<VkPresentModeKHR>& pPresentModes) {
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

DGAPI DgResult _dgChooseSwapExtent2D(DgWindow* pWindow, const VkSurfaceCapabilitiesKHR* pCapabilities) {
	if (pWindow == nullptr) {
		return DG_ARGUMENT_IS_NULL;
	}
	pWindow->capabilities = *pCapabilities;
	if (pCapabilities->currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		pWindow->extent2D = pCapabilities->currentExtent;
	} else {
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

DGAPI DgResult _dgCreateImageViews(DgWindow* pWindow) {
	if (pWindow == nullptr) {
		return DG_ARGUMENT_IS_NULL;
	}

	pWindow->swapChainImages.resize(pWindow->swapChainImages.size());

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
			#ifndef NDEBUG
			std::cerr << "vkCreateImageView failed with " << dgConvertVkResultToString(result) << std::endl;
			#endif
			return DG_VK_IMAGE_VIEW_CREATION_FAILED;
		}
	}
	return DG_SUCCESS;
}

DGAPI DgResult _dgGenerateGraphicsPipeline(DgWindow* pWindow) {
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
		#ifndef NDEBUG
		std::cerr << "Dragon: vkCreateShaderModule returned " << dgConvertVkResultToString(result) << std::endl;
		#endif
		vkDestroyShaderModule(pWindow->pGPU->device, vertModule, nullptr);
		return DG_VK_SHADER_MODULE_CREATION_FAILED;
	}
	pWindow->shaderModules.push_back(vertModule);

	VkShaderModuleCreateInfo fragCreateInfo{};
	fragCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	fragCreateInfo.codeSize = DRAGON_FRAG_SHADER_SIZE;
	fragCreateInfo.pCode = reinterpret_cast<const uint32_t*>(DRAGON_FRAG_SHADER);

	VkShaderModule fragModule = nullptr;
	result = vkCreateShaderModule(pWindow->pGPU->device, &fragCreateInfo, nullptr, &fragModule);
	if (result != VK_SUCCESS) {
		#ifndef NDEBUG
		std::cerr << "Dragon: vkCreateShaderModule returned " << dgConvertVkResultToString(result) << std::endl;
		#endif
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

	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = 0;
	vertexInputInfo.vertexAttributeDescriptionCount = 0;

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
		#ifndef NDEBUG
		std::cerr << "vkCreatePipelineLayout failed with " << dgConvertVkResultToString(result) << std::endl;
		#endif
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

	result = vkCreateGraphicsPipelines(pWindow->pGPU->device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pWindow->graphicsPipeline);

	if (result != VK_SUCCESS) {
		return DG_VK_GRAPHICS_PIPELINE_CREATION_FAILED;
	}

	return DG_SUCCESS;
}

DGAPI DgResult _dgCreateFramebuffers(DgWindow* pWindow) {
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

DGAPI DgResult _dgCreateCommandPool(DgWindow* pWindow) {
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

DGAPI DgResult _dgCreateCommandBuffer(DgWindow* pWindow) {
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = pWindow->commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = 1;

	VkResult result = vkAllocateCommandBuffers(pWindow->pGPU->device, &allocInfo, &pWindow->commandBuffer);

	if (result != VK_SUCCESS) {
		return DG_VK_COMMAND_BUFFER_CREATION_FAILED;
	}
	return DG_SUCCESS;
}

DGAPI DgResult _dgRecordCommandBuffer(DgWindow* pWindow, uint32_t imageIndex) {
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	if (vkBeginCommandBuffer(pWindow->commandBuffer, &beginInfo) != VK_SUCCESS) {
		return DG_VK_COMMAND_BUFFER_FAILED_RECORD_START;
	}

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = pWindow->renderPass;
	renderPassInfo.framebuffer = pWindow->swapChainFramebuffers[imageIndex];
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = pWindow->extent2D;

	VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;

	vkCmdBeginRenderPass(pWindow->commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	vkCmdBindPipeline(pWindow->commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pWindow->graphicsPipeline);

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)pWindow->extent2D.width;
	viewport.height = (float)pWindow->extent2D.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(pWindow->commandBuffer, 0, 1, &viewport);

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = pWindow->extent2D;
	vkCmdSetScissor(pWindow->commandBuffer, 0, 1, &scissor);

	vkCmdDraw(pWindow->commandBuffer, 3, 1, 0, 0);

	vkCmdEndRenderPass(pWindow->commandBuffer);

	if (vkEndCommandBuffer(pWindow->commandBuffer) != VK_SUCCESS) {
		return DG_VK_COMMAND_BUFFER_RECORD_FAILED;
	}
	return DG_SUCCESS;
}

DGAPI DgResult _dgCreateSyncObjects(DgWindow* pWindow) {
	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

	VkResult result = vkCreateSemaphore(pWindow->pGPU->device, &semaphoreInfo, nullptr, &pWindow->imageAvailableSemaphore);

	if (result != VK_SUCCESS)
		return DG_VK_SEMAPHORE_CREATION_FAILED;


	result = vkCreateSemaphore(pWindow->pGPU->device, &semaphoreInfo, nullptr, &pWindow->renderFinishedSemaphore);

	if (result != VK_SUCCESS)
		return DG_VK_SEMAPHORE_CREATION_FAILED;

	result = vkCreateFence(pWindow->pGPU->device, &fenceInfo, nullptr, &pWindow->inFlightFence);

	if (result != VK_SUCCESS) {
		return DG_VK_FENCE_CREATION_FAILED;
	}
	return DG_SUCCESS;
}

DGAPI DgResult _dgRenderWindow(DgWindow* pWindow) {
	vkWaitForFences(pWindow->pGPU->device, 1, &pWindow->inFlightFence, VK_TRUE, UINT64_MAX);
	vkResetFences(pWindow->pGPU->device, 1, &pWindow->inFlightFence);
	return DG_SUCCESS;
}

DGAPI void dgDestroyWindow(VkInstance instance, DgWindow* pWindow) {
	vkDestroySemaphore(pWindow->pGPU->device, pWindow->imageAvailableSemaphore, nullptr);
	vkDestroySemaphore(pWindow->pGPU->device, pWindow->renderFinishedSemaphore, nullptr);
	vkDestroyFence(pWindow->pGPU->device, pWindow->inFlightFence, nullptr);

	vkDestroyCommandPool(pWindow->pGPU->device, pWindow->commandPool, nullptr);

	for (VkFramebuffer framebuffer : pWindow->swapChainFramebuffers) {
		vkDestroyFramebuffer(pWindow->pGPU->device, framebuffer, nullptr);
	}

	if(pWindow->graphicsPipeline != VK_NULL_HANDLE)
		vkDestroyPipeline(pWindow->pGPU->device, pWindow->graphicsPipeline, nullptr);
	if(pWindow->graphicsPipeline != VK_NULL_HANDLE)
		vkDestroyPipelineLayout(pWindow->pGPU->device, pWindow->pipelineLayout, nullptr);
	if(pWindow->renderPass != VK_NULL_HANDLE)
		vkDestroyRenderPass(pWindow->pGPU->device, pWindow->renderPass, nullptr);

	for (VkImageView imageView : pWindow->swapChainImageViews) {
		vkDestroyImageView(pWindow->pGPU->device, imageView, nullptr);
	}

	vkDestroySwapchainKHR(pWindow->pGPU->device, pWindow->swapChain, nullptr);
	vkDestroySurfaceKHR(instance, pWindow->surface, nullptr);
	glfwDestroyWindow(pWindow->window);
}
