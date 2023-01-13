#pragma once

#include <string>

#include "dragon_core.h"
#include "engine.hpp"
#include "window.hpp"

// Control functions for before creation
DGAPI DgBool32 dgAddLayerToEngine(DgEngine* pEngine, std::string layerName);

// Creation functions
DGAPI DgBool32 dgCreateEngine(DgEngine* pEngine);
DGAPI DgBool32 dgCreateWindow(DgEngine* pEngine, std::string title, unsigned int width, unsigned int height, DgBool32 isResizable);

// Control functions for after creation
DGAPI void dgSetCallback(DgEngine* pEngine, std::function<void(DgEngine*)> optfCallback);

// Information Functions
DGAPI int dgGetWindowCount(DgEngine* pEngine);
DGAPI void dgUpdate(DgEngine* pEngine);

// Termination functions
DGAPI void dgTerminateEngine(DgEngine* pEngine);