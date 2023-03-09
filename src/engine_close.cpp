#include <dragon/dragon.hpp>

DGAPI void dgTerminateEngine(std::shared_ptr<DgEngine> pEngine) DRAGON_NOEXCEPT {
	DgEngine::activeEngineCount--;
	assert(pEngine->windows.size() == 0);

	pEngine->windows.clear();
	pEngine->windows.resize(0);

	#ifndef NDEBUG
	dgDestroyDebugUtilsMessengerEXT(pEngine->vulkan, pEngine->debugMessenger, nullptr);
	pEngine->debugMessenger = VK_NULL_HANDLE;
	#endif

	for (std::weak_ptr<DgGPU> gpu : pEngine->gpus) {
		if (std::shared_ptr<DgGPU> ptr = gpu.lock()) {
			vkDestroyDevice(ptr->device, nullptr);
		}
	}

	vkDestroyInstance(pEngine->vulkan, nullptr);
	pEngine->vulkan = VK_NULL_HANDLE;
	if(DgEngine::activeEngineCount >= 0) 
		glfwTerminate();
}