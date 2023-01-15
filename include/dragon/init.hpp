#pragma once

#include <string>

#include "dragon_core.h"
#include "engine.hpp"
#include "gpu.hpp"
#include "message.hpp"
#include "window.hpp"



/**
* Adds a VkValidationLayer to the DgEngine instance. When NDEBUG is defined, it will always return DG_FALSE, since no VkValidationLayers can exist in NDEBUG mode.
* @param pEngine: A pointer to the engine to add the layer to
* @param layerName the name of the Validation Layer to add to the engine
* @return A boolean on if adding the layer was successful
*/
DGAPI DgBool32 dgAddLayerToEngine(DgEngine* pEngine, std::string layerName);

// Creation functions
/**
* Initializes an engine instance. 
* Note that some member variables depend upon having a DgWindow instance as well, and so those will not be initialized until window creation.
* @param pEngine A pointer to the engine instance to initialize
* @return A boolean on if initializing the engine was successful
*/
DGAPI DgBool32 dgCreateEngine(DgEngine* pEngine);
/**
* Creates a window with the settings described according to the engine settings, and the other parameters of this function.
* @param pEngine A pointer to the engine to create the window with
* @param title The title of the window to display.
* @param width an unsigned int, stating how wide the window should be at creation, in pixels.
* @param height an unsigned int, stating how tall the window should be at creation, in pixels.
* @param isResizable A boolean, stating if the window's size is allowed to be changed by the user.
*/
DGAPI DgBool32 dgCreateWindow(DgEngine* pEngine, std::string title, unsigned int width, unsigned int height, DgBool32 isResizable);
/**
* INTERNAL METHOD
* @param pGPU The GPU to populate the queueFamilies of
*/
DGAPI void _dgFindQueueFamilies(DgGPU* pGPU);
/**
* INTERNAL METHOD
* @param pGPU The GPU to generate the presentQueue for
* @param pWindow The window to use for presentation
*/
DGAPI void _dgGeneratePresentationQueue(DgGPU* pGPU, DgWindow* pWindow);
/**
* INTERNAL METHOD
* @param pGPU the GPU to check for swap chain support
* @param pWindow the window to use for swap chain support checking
*/
DGAPI void _dgGetSwapChainSupport(DgGPU* pGPU, DgWindow* pWindow);
/**
* Adds a user-defined function to be used for debugging potential issues with the engine, including potentially unoptimal code order / function calls.
* @param pEngine the engine instance to set the callback for
* @param fCallback a function, of type void, that takes a pointer of a DgMessage
*/
DGAPI void dgSetCallback(DgEngine* pEngine, std::function<void(DgMessage*)> fCallback);
/**
* INTERNAL METHOD
* @param pWindow The window to generate a swap surface format for
* @param pSurfaceFormats The available surface formats
*/
DGAPI void _dgChooseSwapSurfaceFormat(DgWindow* pWindow, const std::vector<VkSurfaceFormatKHR>& pSurfaceFormats);
/**
* INTERNAL METHOD
* @param pWindow The window to generate a swap present mode for
* @param pPresentModes The available present modes
*/
DGAPI void _dgChooseSwapPresentMode(DgWindow* pWindow, const std::vector<VkPresentModeKHR>& pPresentModes);
/**
* INTERNAL METHOD
* @param pWindow The window to generate a swap extent 2D for
* @param pCapabilities The available capabilities of the window
*/
DGAPI void _dgChooseSwapExtent2D(DgWindow* pWindow, const VkSurfaceCapabilitiesKHR* pCapabilities);
/**
* Checks to see how many windows are on this specific engine instance. 
* Does not check for global window count created by *an* engine, just this specific one.
* @param pEngine A pointer to the engine instance to look for windows
* @return The number of windows attached to this engine
*/
DGAPI int dgGetWindowCount(DgEngine* pEngine);
/**
* Updates input buffers, renders images, and checks for queue completion. 
* If not called in the main loop, the program will crash.
* @param pEngine A pointer to the engine instance to update
*/
DGAPI void dgUpdate(DgEngine* pEngine);
/**
* Terminates an engine instance, immediately closing all still-open windows, the Vulkan instance, stopping queues, and rendering the whole engine null and void. 
* Only call when you're not using this instance again. 
* Engine destruction always succeeds.
* @param pEngine A pointer to the engine instance to destroy
*/
DGAPI void dgTerminateEngine(DgEngine* pEngine);