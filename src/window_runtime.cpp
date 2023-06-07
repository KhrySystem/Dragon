#include <dragon/dragon.hpp>

DgResult updateUniformBuffer(std::shared_ptr<DgWindow> pWindow, uint32_t currentImage) {
    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
    if(std::shared_ptr<DgCamera> camera = pWindow->pCamera.lock()) {
        camera->model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        camera->view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        camera->proj = glm::perspective(glm::radians(45.0f), pWindow->swapChainExtent.width / (float) pWindow->swapChainExtent.height, 0.1f, 10.0f);
        camera->proj[1][1] *= -1;
        memcpy(pWindow->uniformBuffersMapped[currentImage], &camera, sizeof(DgCamera));
    } else {
        return DG_CAMERA_UNLOCKED;
    }
    return DG_SUCCESS;
}

DGAPI DgResult dgRecordCommandBuffer(std::shared_ptr<DgWindow> pWindow, std::shared_ptr<DgModel> pModel, VkCommandBuffer commandBuffer, uint32_t imageIndex) {
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        return DG_VULKAN_BEGIN_COMMAND_BUFFER_FAILED;
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = pWindow->renderPass;
    renderPassInfo.framebuffer = pWindow->swapChainFramebuffers[imageIndex];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = pWindow->swapChainExtent;

    VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};

    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pWindow->graphicsPipeline);

    VkBuffer vertexBuffers[] = {pModel->vertexBuffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
    vkCmdBindIndexBuffer(commandBuffer, pModel->indexBuffer, 0, VK_INDEX_TYPE_UINT32);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float) pWindow->swapChainExtent.width;
    viewport.height = (float) pWindow->swapChainExtent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = pWindow->swapChainExtent;
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);            

    vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(pModel->indices.size()), 1, 0, 0, 0);

    vkCmdEndRenderPass(commandBuffer);

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        return DG_VULKAN_END_COMMAND_BUFFER_FAILED;
    }
    return DG_SUCCESS;
}

DGAPI DgResult dgUpdateWindow(std::shared_ptr<DgWindow>& pWindow, std::unique_ptr<DgEngine>& pEngine) {
    vkWaitForFences(pWindow->pGPU->device, 1, &pWindow->inFlightFences[pWindow->currentFrame], VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    VkResult r = vkAcquireNextImageKHR(pWindow->pGPU->device, pWindow->swapChain, UINT64_MAX, pWindow->imageAvailableSemaphores[pWindow->currentFrame], VK_NULL_HANDLE, &imageIndex);
    if (r == VK_ERROR_OUT_OF_DATE_KHR || pWindow->framebufferResized) {
        pWindow->framebufferResized = DG_FALSE;
        dgRecreateSwapchain(pWindow);
        return DG_SWAPCHAIN_RECREATED;
    } else if (r != VK_SUCCESS && r != VK_SUBOPTIMAL_KHR) {
        return DG_UNKNOWN_ERROR;
    }

    vkResetFences(pWindow->pGPU->device, 1, &pWindow->inFlightFences[pWindow->currentFrame]);

    VkSemaphore signalSemaphores[] = {pWindow->renderFinishedSemaphores[pWindow->currentFrame]};

    std::vector<VkCommandBuffer> activeBuffers;
    for(std::vector<std::weak_ptr<DgModel>> layer : pWindow->vvpModels) {
        for(std::weak_ptr<DgModel> weak : layer) {
            if(auto pModel = weak.lock()) {
                vkResetCommandBuffer(pModel->commandBuffers[pWindow->currentFrame], 0);
                dgRecordCommandBuffer(pWindow, pModel, pModel->commandBuffers[pWindow->currentFrame], imageIndex);
                activeBuffers.push_back(pModel->commandBuffers[pWindow->currentFrame]);
            }
        }
    }

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {pWindow->imageAvailableSemaphores[pWindow->currentFrame]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = activeBuffers.data();

    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(pWindow->pGPU->graphicsQueue, 1, &submitInfo, pWindow->inFlightFences[pWindow->currentFrame]) != VK_SUCCESS) {
        return DG_VULKAN_QUEUE_SUBMIT_FAILED;
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {pWindow->swapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;

    presentInfo.pResults = nullptr; // Optional

    vkQueuePresentKHR(pWindow->presentQueue, &presentInfo);

    pWindow->currentFrame = (pWindow->currentFrame + 1) % DRAGON_RENDER_FRAME_MAX;

    return DG_SUCCESS;
}