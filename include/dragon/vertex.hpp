#pragma once 

#include "dragon_core.hpp"

struct DgVertex_T {
    glm::vec2 pos;
    glm::vec3 color;
};

typedef struct DgVertex_T DgVertex;

DGAPI constexpr VkVertexInputBindingDescription dgGetVertexBindingDescription() {
    VkVertexInputBindingDescription bindingDescription{};
    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(DgVertex);
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return bindingDescription;
}

DGAPI constexpr std::array<VkVertexInputAttributeDescription, 2> dgGetVertexAttributeDescriptions() {
    std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(DgVertex, pos);

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(DgVertex, color);

    return attributeDescriptions;
}