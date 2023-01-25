#pragma once

#include <optional>

#include "dragon_core.h"

/**
 * @struct dgQueueFamilies
 * @brief Structure that holds the queue families for graphics and presentation
 */
typedef struct {
    /**
     * @brief Graphics queue family index
     */
    std::optional<uint32_t> graphicsQueueFamily;
    /**
     * @brief Presentation queue family index
     */
    std::optional<uint32_t> presentationQueueFamily;
} dgQueueFamilies;

/**
 * @struct DgGPU
 * @brief Structure that holds all the data for a GPU
 */
typedef struct DgGPU {
    /**
     * @brief Handle to the GPU
     */
    VkPhysicalDevice handle;
    /**
     * @brief Properties of the GPU
     */
    VkPhysicalDeviceProperties properties;
    /**
     * @brief Features of the GPU
     */
    VkPhysicalDeviceFeatures features;
    /**
     * @brief Queue families of the GPU
     */
    dgQueueFamilies queueFamilies;
    /**
     * @brief Logical device of the GPU
     */
    VkDevice device = VK_NULL_HANDLE;
    /**
     * @brief Graphics queue of the GPU
     */
    VkQueue graphicsQueue;
    /**
     * @brief Presentation queue of the GPU
     */
    VkQueue presentationQueue;
} DgGPU;
