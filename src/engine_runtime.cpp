#include <dragon/dragon.hpp>

DGAPI DgBool32 dgCanEngineClose(std::unique_ptr<DgEngine>& pEngine) {
    for(std::weak_ptr<DgWindow> pWindow : pEngine->vpWindows) {
        if(std::shared_ptr<DgWindow> window = pWindow.lock()) {
            if(!glfwWindowShouldClose(window->pWindow)) {
                return DG_FALSE;
            }
        }
        
    }
    return DG_TRUE;
}

DGAPI DgResult dgUpdateEngine(std::unique_ptr<DgEngine>& pEngine) {
    DgResult r;
    glfwPollEvents();
    for(std::weak_ptr<DgWindow> pWindow : pEngine->vpWindows) {
        if(std::shared_ptr<DgWindow> ptr = pWindow.lock()) {
            r = dgUpdateWindow(ptr, pEngine);
            if(r != DG_SUCCESS && r != DG_SWAPCHAIN_RECREATED) {
                return r;
            }
        }
    }
    return DG_SUCCESS;
}

DGAPI DgResult dgWaitForEvents(std::unique_ptr<DgEngine>& pEngine) {
    glfwWaitEvents();
    return DG_SUCCESS;
}