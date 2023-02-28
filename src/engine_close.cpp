#include <dragon/dragon.hpp>

DGAPI void dgTerminateEngine(std::shared_ptr<DgEngine> pEngine) DRAGON_NOEXCEPT {
	DgEngine::activeEngineCount--;

	for (std::shared_ptr<DgWindow> pWindow : pEngine->windows) {
		vkDeviceWaitIdle(pWindow->pGPU->device);
		_dgDestroyWindow(pEngine->vulkan, pWindow);
	}

	#ifndef NDEBUG
	dgDestroyDebugUtilsMessengerEXT(pEngine->vulkan, pEngine->debugMessenger, nullptr);
	#endif

	for (std::shared_ptr<DgGPU> gpu : pEngine->gpus) {
		if (gpu->device != VK_NULL_HANDLE) {
			vkDestroyDevice(gpu->device, nullptr);
		}
	}

	vkDestroyInstance(pEngine->vulkan, nullptr);
	if(DgEngine::activeEngineCount >= 0) 
		glfwTerminate();
}