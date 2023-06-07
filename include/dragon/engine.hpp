#pragma once

#include "dragon_core.hpp"
#include "gpu.hpp"
#include "window.hpp"

struct DgEngineCreateInfo_T {
    std::vector<std::string> vEnabledLayerNames;
    std::vector<std::string> vEnabledExtensionNames;

    DgBool32 enableVulkanDebug;
    std::function<int(DgGPU)> PFN_GPURatingFunc;
};

struct DgEngine_T {
    VkInstance instance;
    std::vector<std::weak_ptr<DgWindow>> vpWindows;
    #ifndef NDEBUG
    VkDebugUtilsMessengerEXT debugMessenger;
    #endif
    std::vector<DgGPU> vGPUs;
};

static std::atomic<unsigned int> dgActiveEngineCount = 0;

typedef struct DgEngineCreateInfo_T DgEngineCreateInfo;
typedef struct DgEngine_T DgEngine;

DGAPI DgResult dgCreateEngine(DgEngineCreateInfo createInfo, std::unique_ptr<DgEngine>& pEngine);
DGAPI DgBool32 dgCanEngineClose(std::unique_ptr<DgEngine>& pEngine);
DGAPI DgResult dgUpdateEngine(std::unique_ptr<DgEngine>& pEngine);
DGAPI void dgDestroyEngine(std::unique_ptr<DgEngine>& pEngine);
