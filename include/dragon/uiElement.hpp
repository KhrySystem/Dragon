#pragma once

#include "dragon_core.h"
#include "vertex.hpp"

typedef struct {
	std::vector<DgVertex> verticies;
	glm::vec4 rotation;
	VkCommandBuffer buffer;
} DgUIElement;