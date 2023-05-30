#include <dragon/dragon.h>

DGAPI void dgDestroyEngine(DgEngine* pEngine) noexcept {
    dgActiveEngineCount--;
    for(int i = 0; i < pEngine->gpuCount; i++) {
        vkDestroyDevice(pEngine->pGPUs[i]->device, nullptr);
    }

    #if !defined(NDEBUG) || defined(_DEBUG)
        dgDestroyDebugUtilsMessengerEXT(pEngine->instance, pEngine->debugMessenger, nullptr);
    #endif

    vkDestroyInstance(pEngine->instance, nullptr);
    if(dgActiveEngineCount <= 0) {
        glfwTerminate();
    }
}