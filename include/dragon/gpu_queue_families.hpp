#pragma once

#include "dragon_core.hpp"

struct DgGPUQueueFamilies_T {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;
};

typedef struct DgGPUQueueFamilies_T DgGPUQueueFamilies;
