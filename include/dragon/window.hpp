#pragma once

#include <string>

#include "dragon_core.h"
#include "gpu.hpp"

/**
 * @struct DgWindow
 * @brief Structure that holds all the data for a window
 */
typedef struct DgWindow {
    /**
     * @brief Pointer to the GPU that renders this window
     */
    DgGPU* pGPU;
    /**
     * @brief Pointer to the GLFW window
     */
    GLFWwindow* window = nullptr;
    /**
     * @brief Handle to the vulkan surface of the window
     */
    VkSurfaceKHR surface = nullptr;
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
    VkSwapchainKHR swapChain = nullptr;
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
     * @brief Dynamic states for graphics pipeline
     */
    std::vector<VkDynamicState> dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };
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
    std::vector<VkCommandBuffer> commandBuffers;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    uint32_t currentFrame;
} DgWindow;

