#include <dragon/dragon.hpp>

DGAPI void _dgDestroyWindow(VkInstance instance, boost::shared_ptr<DgWindow> pWindow) DRAGON_NOEXCEPT {
	assert(pWindow != nullptr);
	assert(instance != nullptr);

	vkDeviceWaitIdle(pWindow->pGPU->device);
	if (!pWindow->imageAvailableSemaphores.empty())
		for (VkSemaphore semaphore : pWindow->imageAvailableSemaphores)
			vkDestroySemaphore(pWindow->pGPU->device, semaphore, nullptr);

	if (!pWindow->renderFinishedSemaphores.empty())
		for (VkSemaphore semaphore : pWindow->renderFinishedSemaphores)
		vkDestroySemaphore(pWindow->pGPU->device, semaphore, nullptr);

	if (!pWindow->inFlightFences.empty())
		for (VkFence fence : pWindow->inFlightFences)
			vkDestroyFence(pWindow->pGPU->device, fence, nullptr);

	if (!pWindow->shaderModules.empty())
		for (VkShaderModule shaderModule : pWindow->shaderModules)
			vkDestroyShaderModule(pWindow->pGPU->device, shaderModule, nullptr);

	vkDestroyPipelineLayout(pWindow->pGPU->device, pWindow->pipelineLayout, nullptr);
	vkDestroyPipeline(pWindow->pGPU->device, pWindow->graphicsPipeline, nullptr);
	vkDestroyRenderPass(pWindow->pGPU->device, pWindow->renderPass, nullptr);

	vkDestroyCommandPool(pWindow->pGPU->device, pWindow->commandPool, nullptr);

	_dgDestroySwapchain(pWindow);

	vkDestroySurfaceKHR(instance, pWindow->surface, nullptr);
	glfwDestroyWindow(pWindow->window);
}

DGAPI void _dgDestroySwapchain(boost::shared_ptr<DgWindow> pWindow) DRAGON_NOEXCEPT {
	vkDeviceWaitIdle(pWindow->pGPU->device);

	for (VkFramebuffer framebuffer : pWindow->swapChainFramebuffers) {
		vkDestroyFramebuffer(pWindow->pGPU->device, framebuffer, nullptr);
	}

	for (VkImageView imageView : pWindow->swapChainImageViews) {
		vkDestroyImageView(pWindow->pGPU->device, imageView, nullptr);
	}

	vkDestroySwapchainKHR(pWindow->pGPU->device, pWindow->swapChain, nullptr);
}
