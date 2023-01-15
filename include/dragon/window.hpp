#pragma once

#include <string>

#include "dragon_core.h"

/**
 * @struct DgWindow
 * @brief Structure that holds all the data for a window
 */
typedef struct {
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
     * Capabilities of the window
     */
    VkSurfaceCapabilitiesKHR capabilities;
    /**
     * Swap chain for the window
     */
    VkSwapchainKHR swapChain;
    /**
     * Images in the swap chain for the window
     */
    std::vector<VkImage> swapChainImages;
} DgWindow;