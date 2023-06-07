#pragma once

#include "dragon_core.hpp"
#include "vertex.hpp"

struct DgModel_T {
    std::vector<DgVertex> vertices;
    std::vector<uint32_t> indices;
    VmaAllocation vertexAllocation;
    VmaAllocation indexAllocation;
    VkBuffer vertexBuffer;
    VkBuffer indexBuffer;
    VkDeviceMemory vertexDeviceMemory;
    VkDeviceMemory indexDeviceMemory;
    std::vector<VkCommandBuffer> commandBuffers;
};

typedef struct DgModel_T DgModel;

