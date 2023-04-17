#pragma once

#include "dragon_core.hpp"
#include "gpu.hpp"
#include "window.hpp"

/**
 * @defgroup engine Engine
 * This section is for engine functions / classes
 * @{
 */


/**
 * @struct DgEngine
 * @brief Structure that holds all the data for the engine instance
 */
struct DgEngine {
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
    std::weak_ptr<DgGPU> primaryGPU;
    /**
     * @brief Vector of all available GPUs
     */
    std::vector<std::shared_ptr<DgGPU>> gpus;
    /**
     * @brief Vector of all created windows
     */
    std::vector<std::weak_ptr<DgWindow>> windows;
    /**
     * @brief Callback function for handling messages
     */
    std::function<void(int, const char*, void*)> fCallback;
    static inline uint32_t activeEngineCount = 0;
};

 /**
 * Adds a VkValidationLayer to the DgEngine instance. When NDEBUG is defined, it will always return DG_FALSE, since no VkValidationLayers can exist in NDEBUG mode.
 * @param pEngine: A pointer to the engine to add the layer to
 * @param layerName the name of the Validation Layer to add to the engine
 * @return A boolean on if adding the layer was successful
 */
DGAPI DgResult dgAddLayerToEngine(std::shared_ptr<DgEngine> pEngine, std::string layerName);

/**
* Adds a user-defined function to be used for debugging potential issues with the engine, including potentially unoptimal code order / function calls.
* @param pEngine the engine instance to set the callback for
* @param fCallback a function, of type void, that takes a pointer of a DgMessage
*/
DGAPI void dgSetCallback(std::shared_ptr<DgEngine> pEngine, std::function<void(int, const char*, void*)> fCallback);

/**
* Checks to see how many windows are on this specific engine instance.
* Does not check for global window count created by *an* engine, just this specific one.
* To check if a program is safe to exit, all engine instances must have been terminated, and all windows destroyed.
* @param pEngine A pointer to the engine instance to look for windows
* @return The number of windows attached to this engine
*/
DGAPI int dgGetWindowCount(std::shared_ptr<DgEngine> pEngine);

/**
* Updates input buffers, renders images, and checks for queue completion.
* If not called in the main loop, the program will crash.
* @param pEngine A pointer to the engine instance to update
*/
DGAPI DgResult dgUpdate(std::shared_ptr<DgEngine> pEngine);

// Destruction functions
/**
* Terminates an engine instance, immediately closing all still-open windows, the Vulkan instance, stopping queues, and rendering the whole engine null and void.
* Only call when you're not using this instance again.
* Engine destruction always succeeds.
* @param pEngine A pointer to the engine instance to destroy
*/
DGAPI void dgTerminateEngine(std::shared_ptr<DgEngine> pEngine) DRAGON_NOEXCEPT;

/** @} */