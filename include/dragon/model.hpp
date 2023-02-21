#pragma once

#include "dragon_core.h"
#include "vertex.hpp"

typedef struct {
	std::vector<DgVertex> verticies;
	glm::vec4 rotation;
	boost::array<VkCommandBuffer, DRAGON_RENDER_FRAME_MAX> buffers;
} DgModel;