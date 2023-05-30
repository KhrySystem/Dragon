#pragma once

#include "dragon_core.h"
#include "gpu_queue_families.h"

struct DgGPU {
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkPhysicalDeviceFeatures physicalDeviceFeatures;
    VkPhysicalDeviceProperties physicalDeviceProperties;
    DgGPUQueueFamilies gpuQueueFamilies;
    VkDevice device;
    VkQueue graphicsQueue;
    int32_t score;
};

typedef DgGPU DgGPU;