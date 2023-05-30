#pragma once

#include "dragon_core.h"
#include "gpu.h"
#include "window.h"

#ifdef __cplusplus
extern "C" {
#endif

struct DgEngineCreateInfo {
    size_t enabledLayerCount;
    const char** ppEnabledLayerNames;

    size_t enabledExtensionCount;
    const char** ppEnabledExtensionNames;

    DgBool32 enableVulkanDebug;
    void(*pGPURatingFunc)(DgGPU*) = NULL;
};

struct DgEngine {
    VkInstance instance;
    uint8_t windowCount;
    DgWindow** pWindows; // NOT one window, this is a pointer-array to all windows dependent on this engine
    #ifndef NDEBUG
    VkDebugUtilsMessengerEXT debugMessenger;
    #endif
    uint8_t gpuCount;
    DgGPU** pGPUs; // Same as the windows, could be more than one GPU here
};

static uint8_t dgActiveEngineCount = 0;

typedef struct DgEngineCreateInfo DgEngineCreateInfo;
typedef struct DgEngine DgEngine;

DGAPI DgResult dgCreateEngine(DgEngineCreateInfo createInfo, DgEngine* pEngine);
DGAPI DgBool32 dgCanEngineClose(DgEngine* pEngine);
DGAPI DgResult dgUpdateEngine(DgEngine* pEngine);
DGAPI void dgDestroyEngine(DgEngine* pEngine) noexcept;

#ifdef __cplusplus
}
#endif