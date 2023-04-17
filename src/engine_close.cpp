#include <dragon/dragon.hpp>

DGAPI void dgTerminateEngine(std::shared_ptr<DgEngine> pEngine) DRAGON_NOEXCEPT {
	DgEngine::activeEngineCount--;

	for (std::weak_ptr<DgWindow> window : pEngine->windows) {
		if ( std::shared_ptr<DgWindow> ptr = window.lock()) {	
			_dgDestroyWindow(pEngine->vulkan, ptr);
		}
		window.reset();
		std::cout << "Window Ref Count: " << window.use_count() << std::endl;
		
	}
	pEngine->windows.clear();
	pEngine->windows.resize(0);

	#ifndef NDEBUG
	dgDestroyDebugUtilsMessengerEXT(pEngine->vulkan, pEngine->debugMessenger, nullptr);
	#endif

	pEngine->primaryGPU.reset();
	
	for (int i = 0; i < pEngine->gpus.size(); i++) {
		dgDestroyGPU(pEngine->gpus[i]);
		pEngine->gpus[i].reset();
		std::cout << "GPU Ref Count: " << pEngine->gpus[i].use_count() << std::endl;
	}

	vkDestroyInstance(pEngine->vulkan, nullptr);
	pEngine->vulkan = VK_NULL_HANDLE;
	if(DgEngine::activeEngineCount >= 0) 
		glfwTerminate();
}