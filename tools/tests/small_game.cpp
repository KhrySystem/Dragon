// Define certain overridable things BEFORE including dragon header
#define APP_NAME "Small Test Game"

#include <dragon/dragon.hpp>
#include <exception>
#include <vector>
#include <iostream>
#include <csignal>

DgEngine* pEngine = (DgEngine*)malloc(sizeof(DgEngine));
DgWindow* pWindow = (DgWindow*)malloc(sizeof(DgWindow));

void printVkExtLayerData() {
    uint32_t extensionCount;
    const char** extensions = dgGetAvailableVulkanExtensions(&extensionCount);
    std::cout << "Extensions Available: " << extensionCount << '\n';
    for(int i = 0; i < extensionCount; i++) {
        std::cout << extensions[i] << '\n';
    }
    uint32_t layerCount;
    const char** layers = dgGetAvailableVulkanLayers(&layerCount);
    std::cout << "Layers Available: " << layerCount << '\n';
    for(int i = 0; i < layerCount; i++) {
        std::cout << layers[i] << '\n';
    }
}

void initEngine() {
    pEngine = (DgEngine*)malloc(sizeof(DgEngine));
    DgEngineCreateInfo createInfo{};
    createInfo.enableVulkanDebug = DG_TRUE;

    DgResult r = dgCreateEngine(createInfo, pEngine);
    if(r != DG_SUCCESS) throw std::exception();
}

void initWindow() {
    pWindow = (DgWindow*)malloc(sizeof(DgWindow));
    DgWindowCreateInfo createInfo{};
    createInfo.width = 200;
    createInfo.height = 200;
    createInfo.pTitle = APP_NAME;
    createInfo.pEngine = pEngine;
    DgResult r = dgCreateWindow(createInfo, pWindow);
    if(r != DG_SUCCESS) throw std::exception();
}

void initModels() {

}

int main() {
    // Check Extensions and Layers available
    // printVkExtLayerData();
    // engine creation
    initEngine();
    // window creation
    initWindow();
    // model creation
    initModels();
    // loop
    while(!dgCanEngineClose(pEngine)) {
        // inputs (should come from GLFW?)
        // math things
        // move models
        // animation
        // update window
        dgUpdateEngine(pEngine);
    }
    // endloop

    // model destruction
    // window destruction
    dgDestroyWindow(pWindow, pEngine);
    // engine destruction
    dgDestroyEngine(pEngine);
    return 0;
}