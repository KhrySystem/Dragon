#pragma once

#include "dragon_core.h"

struct DgGPUQueueFamilies {
    DgBool32 graphicsFamilyFound;
    uint32_t graphicsFamily;
};

typedef DgGPUQueueFamilies DgGPUQueueFamilies;