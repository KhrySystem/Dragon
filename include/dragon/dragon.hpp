#pragma once

#include "camera.hpp"
#include "config.hpp"
#include "dragon_core.hpp"
//#include "dyload.hpp"
#include "engine.hpp"
#include "frag.hpp"
#include "gpu_queue_families.hpp"
#include "gpu.hpp"
#include "internal.hpp"
#include "model_create_info.hpp"
#include "model.hpp"
#include "result.hpp"
#include "swap_chain_support.hpp"
#include "vert.hpp"
#include "vertex.hpp"
#include "vulkan_debug.hpp"
#include "window_create_info.hpp"
#include "window.hpp"

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
    constexpr auto createWindow = dgCreateWindow;
    // Runtime Functions
    constexpr auto canEngineClose = dgCanEngineClose;
    constexpr auto updateEngine = dgUpdateEngine;
    // Destruction Functions
    constexpr auto destroyEngine = dgDestroyEngine;
    constexpr auto destroyWindow = dgDestroyWindow;
};