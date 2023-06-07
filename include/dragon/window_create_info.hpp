#pragma once

#include <memory>

#include "engine.hpp"
#include "window.hpp"

struct DgWindowCreateInfo_T {
    unsigned int width;
    unsigned int height;
    size_t renderLayers;
    const char* pTitle;
    DgBool32 isFullscreen;
    DgBool32 isBorderless;
    std::function<VkSurfaceFormatKHR(const std::vector<VkSurfaceFormatKHR>& availableFormats)> PFN_chooseSwapSurfaceFormat;
    std::function<VkPresentModeKHR(const std::vector<VkPresentModeKHR>& availablePresentModes)> PFN_chooseSwapPresentMode;
    std::function<VkExtent2D(GLFWwindow*,const VkSurfaceCapabilitiesKHR& capabilities)> PFN_chooseSwapExtent;
};
typedef struct DgWindowCreateInfo_T DgWindowCreateInfo;

DGAPI DgResult dgCreateWindow(DgWindowCreateInfo createInfo, std::shared_ptr<DgWindow>& pWindow, std::unique_ptr<DgEngine>& pEngine, std::shared_ptr<DgCamera> pCamera);
DGAPI DgResult dgRecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
DGAPI DgResult dgUpdateWindow(std::shared_ptr<DgWindow>& pWindow, std::unique_ptr<DgEngine>& pEngine);
DGAPI DgResult dgRecreateSwapchain(std::shared_ptr<DgWindow>& pWindow);
DGAPI void dgDestroySwapchain(std::shared_ptr<DgWindow>& pWindow);
DGAPI void dgDestroyWindow(std::shared_ptr<DgWindow>& pWindow, std::unique_ptr<DgEngine>& pEngine);