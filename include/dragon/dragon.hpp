#pragma once

#include "dragon.h"

namespace Dragon {
    // Classes
    typedef DgEngine Engine;
    typedef DgWindow Window;
    typedef DgGPU GPU;

    // Enums and Structures
    typedef DgResult Result;
    typedef DgEngineCreateInfo EngineCreateInfo;

    // Creation Functions
    constexpr auto createEngine = dgCreateEngine;
    // Runtime Functions
    // Destruction Functions
    constexpr auto destroyEngine = dgDestroyEngine;
};