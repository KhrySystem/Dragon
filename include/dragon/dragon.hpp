#pragma once

/**
 *  @defgroup initialization Initialization
 *  This section is for initialization functions / structs / classes / predefs
 */



  

   /**
	*  @defgroup debug Debugging
	*  This section is for debugging functions / structs / classes / predefs
	*/

#include <cassert>

#include "dragon_core.h"
#include "engine.hpp"
#include "frag.hpp"
#include "gpu.hpp"
#include "result.hpp"
#include "uiElement.hpp"
#include "validation.hpp"
#include "vert.hpp"
#include "vertex.hpp"
#include "window.hpp"


/**
 * @defgroup engine Engine
 * This section is for engine functions / classes
 * @{
 */

 /**
 * Adds a VkValidationLayer to the DgEngine instance. When NDEBUG is defined, it will always return DG_FALSE, since no VkValidationLayers can exist in NDEBUG mode.
 * @param pEngine: A pointer to the engine to add the layer to
 * @param layerName the name of the Validation Layer to add to the engine
 * @return A boolean on if adding the layer was successful
 */
DGAPI DgResult dgAddLayerToEngine(DgEngine* pEngine, std::string layerName);

/**
* Adds a user-defined function to be used for debugging potential issues with the engine, including potentially unoptimal code order / function calls.
* @param pEngine the engine instance to set the callback for
* @param fCallback a function, of type void, that takes a pointer of a DgMessage
*/
DGAPI void dgSetCallback(DgEngine* pEngine, std::function<void(int, const char*, void*)> fCallback);

/**
* Initializes an engine instance.
* Note that some member variables depend upon having a DgWindow instance as well, and so those will not be initialized until window creation.
* @param pEngine A pointer to the engine instance to initialize
* @return A boolean on if initializing the engine was successful
*/
DGAPI DgResult dgCreateEngine(DgEngine* pEngine);

/**
* Checks to see how many windows are on this specific engine instance.
* Does not check for global window count created by *an* engine, just this specific one.
* To check if a program is safe to exit, all engine instances must have been terminated, and all windows destroyed.
* @param pEngine A pointer to the engine instance to look for windows
* @return The number of windows attached to this engine
*/
DGAPI int dgGetWindowCount(DgEngine* pEngine);

/**
* Updates input buffers, renders images, and checks for queue completion.
* If not called in the main loop, the program will crash.
* @param pEngine A pointer to the engine instance to update
*/
DGAPI DgResult dgUpdate(DgEngine* pEngine);

// Destruction functions
/**
* Terminates an engine instance, immediately closing all still-open windows, the Vulkan instance, stopping queues, and rendering the whole engine null and void.
* Only call when you're not using this instance again.
* Engine destruction always succeeds.
* @param pEngine A pointer to the engine instance to destroy
*/
DGAPI void dgTerminateEngine(DgEngine* pEngine);

/** @} */

/**
 *  @defgroup window Window
 *  This section is for window functions / structs / classes
 *  @{
 */

 /**
  * Creates a window with the settings described according to the engine settings, and the other parameters of this function.
  * @param pEngine A pointer to the engine to create the window with
  * @param title The title of the window to display.
  * @param width an unsigned int, stating how wide the window should be at creation, in pixels.
  * @param height an unsigned int, stating how tall the window should be at creation, in pixels.
  * @param isResizable A boolean, stating if the window's size is allowed to be changed by the user.
  * @param isFullscreen A boolean, stating if the window should take up the entire screen.
  */
DGAPI DgResult dgCreateWindow(DgEngine* pEngine, std::string title, unsigned int width, unsigned int height, DgBool32 isResizable, DgBool32 isFullscreen, DgWindow* pWindow);
DGAPI DgResult dgCreateUIElement(DgWindow* pWindow, std::vector<DgVertex> verts);
/**
 * INTERNAL METHOD
 * @param pWindow The window to use for presentation
 * @return A DgResult stating if the window creation was successful, or the error during creation.
 */
DGAPI DgResult _dgGeneratePresentationQueue(DgWindow* pWindow);


/**
* INTERNAL METHOD
* @param pWindow the window to generate a swapchain for
* @return A DgResult stating if the swapchain creation was successful, or the error during creation.
*/
DGAPI DgResult _dgCreateSwapchain(DgWindow* pWindow);


/**
* INTERNAL METHOD
* @param pWindow The window to generate a swap surface format for
* @param pSurfaceFormats The available surface formats
* @return A DgResult stating if a valid swap surface format was found, or the error during creation.
*/
DGAPI DgResult _dgChooseSwapSurfaceFormat(DgWindow* pWindow, const std::vector<VkSurfaceFormatKHR>& pSurfaceFormats);

/**
* INTERNAL METHOD
* @param pWindow The window to generate a swap present mode for
* @param pPresentModes The available present modes
* @return A DgResult stating if a valid swap present mode was found, or the error during creation.
*/
DGAPI DgResult _dgChooseSwapPresentMode(DgWindow* pWindow, const std::vector<VkPresentModeKHR>& pPresentModes);

/**
* INTERNAL METHOD
* @param pWindow The window to generate a swap extent 2D for
* @param pCapabilities The available capabilities of the window
* @return A DgResult stating if a valid swap extent was found, or the error during creation.
*/
DGAPI DgResult _dgChooseSwapExtent2D(DgWindow* pWindow, const VkSurfaceCapabilitiesKHR* pCapabilities);

/**
 *INTERNAL METHOD
 * @param pWindow The window to geterate image views for
 * @return A DgResult stating if the image view creation was successful, or the error during creation.
 */
DGAPI DgResult _dgCreateImageViews(DgWindow* pWindow);

/**
 * INTERNAL METHOD
 * @param pWindow The window to generate a graphics pipeline for
 * @return A DgResult stating if the graphics pipeline creation was successful, or the error during creation.
 */
DGAPI DgResult _dgGenerateGraphicsPipeline(DgWindow* pWindow);

/**
 * INTERNAL METHOD
 * @param pWindow The window to generate framebuffers for
 * @return A DgResult stating if framebuffer creation was successful, or the error during creation.
 */
DGAPI DgResult _dgCreateFramebuffers(DgWindow* pWindow);

/**
 * INTERNAL METHOD
 * @param pWindow The window to generate a command pool for
 * @return A DgResult stating if the command pool creation was successful, or the error during creation.
 */
DGAPI DgResult _dgCreateCommandPool(DgWindow* pWindow);

/**
 * INTERNAL METHOD
 * @param pWindow The window to generate the command buffers for
 * @return A DgResult stating if command buffer creation was successful, or the error during creation.
 */
DGAPI DgResult _dgCreateCommandBuffer(DgWindow* pWindow);

/**
 * INTERNAL METHOD
 * @param pWindow The window to generate the sync objects for
 * @return A DgResult stating if the sync object creation was successful, or the error during creation.
 */
DGAPI DgResult _dgCreateSyncObjects(DgWindow* pWindow);
/**
 * INTERNAL METHOD
 * @param pWindow The window to regenerate a swapchain for
 * @return A DgResult stating if the swapchain recreation was successful, or the error during creation.
 */
DGAPI DgResult _dgRecreateSwapchain(DgWindow* pWindow);

/**
 * INTERNAL METHOD
 * @param pWindow The window to render
 * @return A DgResult stating if rendering the window was successful, or the error during rendering.
 */
DGAPI DgResult _dgRenderWindow(DgWindow* pWindow);

/**
 * INTERNAL METHOD
 * @param pWindow The window to destroy the swapchain of
 */
DGAPI void _dgDestroySwapchain(DgWindow* pWindow);

/**
* INTERNAL METHOD
* @param instance The VkInstance that this window was created with
* @param pWindow The window to destroy
*/
DGAPI void _dgDestroyWindow(VkInstance instance, DgWindow* pWindow);
/** @} */


/**
 * @defgroup gpu GPU
 * This section defines methods involving a GPU
 * @{
 */

/**
* INTERNAL METHOD
* @param pGPU The GPU to populate the queueFamilies of
*/
DGAPI DgResult _dgFindQueueFamilies(DgGPU* pGPU);

/**
 * INTERNAL METHOD
 * @param pEngine The engine to find the extensions and layers from
 * @param pGPU The GPU to generate the Queue Buffers
 */
DGAPI DgResult _dgStartQueueBuffers(DgEngine* pEngine, DgGPU* pGPU);
DGAPI VkVertexInputBindingDescription _dgGenerateVertexBindingDescription();
DGAPI boost::array<VkVertexInputAttributeDescription, DRAGON_VERTEX_ATTRIBUTE_COUNT> _dgGetAttributeDescriptions();
/** @} */
