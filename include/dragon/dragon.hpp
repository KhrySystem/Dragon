#pragma once

#include "createInfos.hpp"
#include "dragon_core.hpp"
#include "engine.hpp"
#include "frag.hpp"
#include "gpu.hpp"
#include "result.hpp"
#include "model.hpp"
#include "validation.hpp"
#include "vert.hpp"
#include "vertex.hpp"
#include "window.hpp"

/**
* Initializes an engine instance.
* Note that some member variables depend upon having a DgWindow instance as well, and so those will not be initialized until window creation.
* @param pEngine A pointer to the engine instance to initialize
* @return A boolean on if initializing the engine was successful
*/
DGAPI DgResult dgCreateEngine(std::shared_ptr<DgEngine> pEngine, DgEngineCreateInfo createInfo);

/**
 * Creates a window with the settings described according to the engine settings, and the other parameters of this function.
 * @param pEngine A pointer to the engine to create the window with
 * @param title The title of the window to display.
 * @param width an unsigned int, stating how wide the window should be at creation, in pixels.
 * @param height an unsigned int, stating how tall the window should be at creation, in pixels.
 * @param isResizable A boolean, stating if the window's size is allowed to be changed by the user.
 * @param isFullscreen A boolean, stating if the window should take up the entire screen.
 */
DGAPI DgResult dgCreateWindow(std::shared_ptr<DgWindow> pWindow, DgWindowCreateInfo createInfo);
