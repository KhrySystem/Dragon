#include <dragon/dragon.hpp>

DGAPI void dgDestroySwapchain(std::shared_ptr<DgWindow>& pWindow) {
    for(auto framebuffer : pWindow->swapChainFramebuffers) {
        vkDestroyFramebuffer(pWindow->pGPU->device, framebuffer, nullptr);
    }
    for(auto imageView : pWindow->swapChainImageViews) {
        vkDestroyImageView(pWindow->pGPU->device, imageView, nullptr);
    }
    vkDestroySwapchainKHR(pWindow->pGPU->device, pWindow->swapChain, nullptr);

    for(size_t i = 0; i < DRAGON_RENDER_FRAME_MAX; i++) {
        vkDestroySemaphore(pWindow->pGPU->device, pWindow->imageAvailableSemaphores[i], nullptr);
        vkDestroySemaphore(pWindow->pGPU->device, pWindow->renderFinishedSemaphores[i], nullptr);
        vkDestroyFence(pWindow->pGPU->device, pWindow->inFlightFences[i], nullptr);
    }
}

DGAPI void dgDestroyWindow(std::shared_ptr<DgWindow>& pWindow, std::unique_ptr<DgEngine>& pEngine) {
    vkDeviceWaitIdle(pWindow->pGPU->device);

    for(int i = 0; i < DRAGON_RENDER_FRAME_MAX; i++) {
        vmaDestroyBuffer(pWindow->pGPU->allocator, pWindow->uniformBuffers[i], pWindow->uniformAllocations[i]);
        vkFreeMemory(pWindow->pGPU->device, pWindow->uniformDeviceMemory[i], nullptr);
    }

    dgDestroySwapchain(pWindow);
    vkDestroyDescriptorSetLayout(pWindow->pGPU->device, pWindow->descriptorSetLayout, nullptr);
    vkDestroyCommandPool(pWindow->pGPU->device, pWindow->commandPool, nullptr);
    vkDestroyPipeline(pWindow->pGPU->device, pWindow->graphicsPipeline, nullptr);
    vkDestroyPipelineLayout(pWindow->pGPU->device, pWindow->pipelineLayout, nullptr);
    vkDestroyRenderPass(pWindow->pGPU->device, pWindow->renderPass, nullptr);
    vkDestroyShaderModule(pWindow->pGPU->device, pWindow->fragShaderModule, nullptr);
    vkDestroyShaderModule(pWindow->pGPU->device, pWindow->vertShaderModule, nullptr);
    vkDestroySurfaceKHR(pEngine->instance, pWindow->surface, nullptr);
    glfwDestroyWindow(pWindow->pWindow);
}