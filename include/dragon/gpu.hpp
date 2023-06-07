#pragma once

#include "dragon_core.hpp"
#include "gpu_queue_families.hpp"

struct DgGPU_T {
    VkPhysicalDevice physicalDevice;
    VkPhysicalDeviceFeatures physicalDeviceFeatures;
    VkPhysicalDeviceProperties physicalDeviceProperties;
    DgGPUQueueFamilies gpuQueueFamilies;
    VkDevice device;
    std::vector<VkQueueFamilyProperties> vQueueFamilyProperties;
    VkQueue graphicsQueue;
    int32_t score;
    VmaAllocator allocator;
};

typedef struct DgGPU_T DgGPU;
