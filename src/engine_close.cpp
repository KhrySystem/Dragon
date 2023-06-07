#include <dragon/dragon.hpp>

DGAPI void dgDestroyEngine(std::unique_ptr<DgEngine>& pEngine) {
    dgActiveEngineCount--;

    for(DgGPU gpu : pEngine->vGPUs) {
        vmaDestroyAllocator(gpu.allocator);
        vkDestroyDevice(gpu.device, nullptr);
    }

    #if !defined(NDEBUG) || defined(_DEBUG)
        dgDestroyDebugUtilsMessengerEXT(pEngine->instance, pEngine->debugMessenger, nullptr);
    #endif 

    vkDestroyInstance(pEngine->instance, nullptr);
    if(dgActiveEngineCount <= 0) {
        glfwTerminate();
    }
}