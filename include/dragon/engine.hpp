#pragma once

#include <string>
#include <vector>
#include <functional>

#include "dragon_core.h"
#include "gpu.hpp"
#include "message.hpp"
#include "window.hpp"

/**
 * @struct DgEngine
 * @brief Structure that holds all the data for the engine instance
 */
typedef struct DgEngine {
    /**
     * @brief boolean on if the engine was initialized successfully
     */
    DgBool32 initialized;
    /**
     * @brief Vulkan object instance for the engine
     */
    VkInstance vulkan;
    /**
     * @brief Vector of required vulkan instance extensions
     */
    std::vector<const char*> vkExtensions = { VK_EXT_DEBUG_UTILS_EXTENSION_NAME };
    /**
     * @brief Vector of required vulkan device extensions
     */
    std::vector<const char*> vkDeviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
    /**
     * @brief Vector of required validation layers
     */
    #ifndef NDEBUG
        std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
        VkDebugUtilsMessengerEXT debugMessenger;
    #endif
     /**
      * @brief Pointer to the primary GPU
      */
    DgGPU* primaryGPU;
    /**
     * @brief Vector of all available GPUs
     */
    std::vector<DgGPU> gpus;
    /**
     * @brief Vector of all created windows
     */
    std::vector<DgWindow> windows;
    /**
     * @brief Callback function for handling messages
     */
    std::function<void(DgMessage*)> fCallback;
    
} DgEngine;


