#pragma once

#include <string>

#include "dragon_core.h"
#include "gpu.hpp"
#include "result.hpp"
#include "model.hpp"


/**
 * @brief Dynamic states for graphics pipeline
 */
const std::vector<VkDynamicState> dynamicStates = {
    VK_DYNAMIC_STATE_VIEWPORT,
    VK_DYNAMIC_STATE_SCISSOR
};

/**
 * @struct DgWindow
 * @brief Structure that holds all the data for a window
 */
struct DgWindow {
    /**
     * @brief Pointer to the GPU that renders this window
     */
    std::shared_ptr<DgGPU> pGPU;
    /**
     * @brief Pointer to the GLFW window
     */
    GLFWwindow* window;
    /**
     * @brief Handle to the vulkan surface of the window
     */
    VkSurfaceKHR surface;
    /**
     * @brief Surface format of the window
     */
    VkSurfaceFormatKHR surfaceFormat;
    /**
     * @brief Presentation mode of the window
     */
    VkPresentModeKHR presentMode;
    /**
     * @brief 2D Extent of the window
     */
    VkExtent2D extent2D;
    /**
     * @brief Capabilities of the window
     */
    VkSurfaceCapabilitiesKHR capabilities;
    /**
     * @brief Swap chain for the window
     */
    VkSwapchainKHR swapChain = VK_NULL_HANDLE;
    /**
     * @brief Images in the swap chain for the window
     */
    std::vector<VkImage> swapChainImages;
    /**
     * @brief Images views in the swap chain for the window
     */
    std::vector<VkImageView> swapChainImageViews;
    /**
     * @brief Shader modules for the graphics pipeline of the window
     */
    std::vector<VkShaderModule> shaderModules;
    /**
     * @brief Layout for the graphics pipeline of the window
     */
    VkPipelineLayout pipelineLayout;
    /**
     * @brief Render pass for the graphics pipeline of the window
     */
    VkRenderPass renderPass;
    /**
     * @brief Vulkan Pipeline object for the window
     */
    VkPipeline graphicsPipeline;
    std::vector<VkFramebuffer> swapChainFramebuffers;
    VkCommandPool commandPool;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    uint32_t currentFrame;
    std::vector < std::vector<std::weak_ptr<DgModel>>> models;
};
