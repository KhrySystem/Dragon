#pragma once

#include <string>

#include "dragon_core.h"
#include "engine.hpp"
#include "window.hpp"

DGAPI DgBool32 dgAddLayerToEngine(DgEngine* pEngine, std::string layerName);
DGAPI DgBool32 dgCreateEngine(DgEngine* pEngine);
DGAPI void dgSetCallback(DgEngine* pEngine, std::function<void>(DgEngine*));
DGAPI void dgUpdate(DgEngine* pEngine);
DGAPI void dgTerminateEngine(DgEngine* pEngine);
DGAPI DgBool32 dgCreateWindow(DgEngine* pEngine, std::string title, unsigned int width, unsigned int height, DgBool32 isResizable);
DGAPI int dgGetWindowCount(DgEngine* pEngine);