#pragma once

#include "dragon_core.hpp"
#include "gpu.hpp"
#include "model.hpp"
#include "swap_chain_support.hpp"
#include "camera.hpp"

struct DgWindow_T {
    DgBool32 framebufferResized;
    DgGPU* pGPU;
    DgSwapChainSupportDetails swapChainSupportDetails;

    std::vector<std::vector<std::weak_ptr<DgModel>>> vvpModels;
    std::weak_ptr<DgCamera> pCamera;

    GLFWwindow* pWindow;

    std::vector<VmaAllocation> uniformAllocations;

    VkCommandPool commandPool;
    VkDescriptorSetLayout descriptorSetLayout;
    VkExtent2D swapChainExtent;
    VkFormat swapChainImageFormat;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;
    VkRenderPass renderPass;
    VkShaderModule vertShaderModule;
    VkShaderModule fragShaderModule;
    VkSurfaceKHR surface;
    VkSwapchainKHR swapChain;
    VkQueue presentQueue;

    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkImage> swapChainImages;
    std::vector<VkImageView> swapChainImageViews;
    std::vector<VkFramebuffer> swapChainFramebuffers;
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    std::vector<VkDeviceMemory> uniformDeviceMemory;

    std::function<VkSurfaceFormatKHR(const std::vector<VkSurfaceFormatKHR>& availableFormats)> PFN_chooseSwapSurfaceFormat;
    std::function<VkPresentModeKHR(const std::vector<VkPresentModeKHR>& availablePresentModes)> PFN_chooseSwapPresentMode;
    std::function<VkExtent2D(GLFWwindow*,const VkSurfaceCapabilitiesKHR& capabilities)> PFN_chooseSwapExtent;
    std::vector<void*> uniformBuffersMapped;

    uint32_t currentFrame = 0;

};

typedef struct DgWindow_T DgWindow;