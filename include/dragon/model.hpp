#pragma once

#include "dragon_core.h"
#include "vertex.hpp"

typedef struct {
	std::vector<DgVertex> verticies;
	glm::vec4 rotation;
	std::array<VkCommandBuffer, DRAGON_RENDER_FRAME_MAX> buffers;
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
} DgModel;