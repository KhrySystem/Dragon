#pragma once

#include "dragon_core.hpp"
/**
 * @defgroup gpu GPU
 * This section defines methods involving a GPU
 * @{
 */

/**
 * @struct dgQueueFamilies
 * @brief Structure that holds the queue families for graphics and presentation
 */
struct dgQueueFamilies {
    /**
     * @brief Graphics queue family index
     */
    std::optional<uint32_t> graphicsQueueFamily;
    /**
     * @brief Presentation queue family index
     */
    std::optional<uint32_t> presentationQueueFamily;
};

/**
 * @struct DgGPU
 * @brief Structure that holds all the data for a GPU
 */
struct DgGPU {
    /**
     * @brief Handle to the GPU
     */
    VkPhysicalDevice handle;
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
};

 /**
 * INTERNAL METHOD
 * @param pGPU The GPU to populate the queueFamilies of
 */
DGAPI DgResult _dgFindQueueFamilies(std::shared_ptr<DgGPU> pGPU);

/**
 * INTERNAL METHOD
 * @param pEngine The engine to find the extensions and layers from
 * @param pGPU The GPU to generate the Queue Buffers
 */
DGAPI DgResult _dgStartQueueBuffers(std::vector<const char*> vDeviceExtensions, std::vector<const char*> validationLayers, std::shared_ptr<DgGPU> pGPU);
DGAPI VkVertexInputBindingDescription _dgGenerateVertexBindingDescription();
DGAPI std::array<VkVertexInputAttributeDescription, DRAGON_VERTEX_ATTRIBUTE_COUNT> _dgGetAttributeDescriptions();
DGAPI void dgDestroyGPU(std::shared_ptr<DgGPU> pGPU) DRAGON_NOEXCEPT;
/** @} */



