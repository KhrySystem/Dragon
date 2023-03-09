#include <dragon/dragon.hpp>

DgResult _dgRecordCommandBuffer(std::shared_ptr<DgWindow> pWindow, uint32_t imageIndex, std::shared_ptr<DgModel> pModel) {
	VkCommandBuffer buffer = pModel->buffers[imageIndex];

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	if (vkBeginCommandBuffer(buffer, &beginInfo) != VK_SUCCESS) {
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

	vkCmdBeginRenderPass(buffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pWindow->graphicsPipeline);

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)pWindow->extent2D.width;
	viewport.height = (float)pWindow->extent2D.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(buffer, 0, 1, &viewport);

	VkBuffer vertexBuffers[] = { pModel->vertexBuffer };
	VkDeviceSize offsets[] = {0};
	vkCmdBindVertexBuffers(buffer, 0, 1, vertexBuffers, offsets);

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = pWindow->extent2D;
	vkCmdSetScissor(buffer, 0, 1, &scissor);


	vkCmdDraw(buffer, 3, 1, 0, 0);


	vkCmdEndRenderPass(buffer);

	if (vkEndCommandBuffer(buffer) != VK_SUCCESS) {
		return DG_VK_COMMAND_BUFFER_RECORD_FAILED;
	}
	return DG_SUCCESS;
}

DGAPI DgResult _dgRecreateSwapchain(std::shared_ptr<DgWindow> pWindow) {
	std::cout << "Recreating swapchain" << std::endl;
	_dgDestroySwapchain(pWindow);
	DgResult r = _dgCreateSwapchain(pWindow);
	if (r != DG_SUCCESS) {
		return r;
	}
	return DG_SUCCESS;
}

DGAPI DgResult _dgRenderWindow(std::shared_ptr<DgWindow> pWindow) {
	vkWaitForFences(pWindow->pGPU->device, 1, &pWindow->inFlightFences.at(pWindow->currentFrame), VK_TRUE, UINT64_MAX);

	uint32_t imageIndex;
	VkResult result = vkAcquireNextImageKHR(pWindow->pGPU->device, pWindow->swapChain, UINT64_MAX, pWindow->imageAvailableSemaphores.at(pWindow->currentFrame), VK_NULL_HANDLE, &imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		DgResult r = _dgRecreateSwapchain(pWindow);
		return r;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		return DG_VK_ACQUIRE_NEXT_IMAGE_FAILED;
	}

	vkResetFences(pWindow->pGPU->device, 1, &pWindow->inFlightFences.at(pWindow->currentFrame));

	std::vector<VkCommandBuffer> activeBuffers;

	for (std::vector<std::weak_ptr<DgModel>> layer : pWindow->models) {
		for (std::weak_ptr<DgModel> weak : layer) {
			if (std::shared_ptr<DgModel> model = weak.lock()) {
				vkResetCommandBuffer(model->buffers[pWindow->currentFrame], /*VkCommandBufferResetFlagBits*/ 0);
				_dgRecordCommandBuffer(pWindow, imageIndex, model);
				activeBuffers.push_back(model->buffers[pWindow->currentFrame]);
			}
		}
	}

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { pWindow->imageAvailableSemaphores.at(pWindow->currentFrame) };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = activeBuffers.data();

	VkSemaphore signalSemaphores[] = { pWindow->renderFinishedSemaphores.at(pWindow->currentFrame) };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	if (vkQueueSubmit(pWindow->pGPU->graphicsQueue, 1, &submitInfo, pWindow->inFlightFences.at(pWindow->currentFrame)) != VK_SUCCESS) {
		return DG_VK_QUEUE_SUBMISSION_FAILED;
	}

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { pWindow->swapChain };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;

	presentInfo.pImageIndices = &imageIndex;

	result = vkQueuePresentKHR(pWindow->pGPU->presentationQueue, &presentInfo);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
		_dgRecreateSwapchain(pWindow);
	}
	else if (result != VK_SUCCESS) {
		return DG_VK_QUEUE_PRESENT_FAILED;
	}

	pWindow->currentFrame = (pWindow->currentFrame + 1) % DRAGON_RENDER_FRAME_MAX;
	return DG_SUCCESS;
}

DGAPI void dgAddRenderLayer(std::shared_ptr<DgWindow> pWindow) {
	pWindow->models.push_back(std::vector<std::weak_ptr<DgModel>>());
}
