#pragma once

#include "dragon_core.hpp"
#include "vertex.hpp"



struct DgModel {
	std::vector<DgVertex> verticies;
	DG_VEC4 rotation;
	std::array<VkCommandBuffer, DRAGON_RENDER_FRAME_MAX> buffers;
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
};

DGAPI void dgDestroyModel(std::shared_ptr<DgModel> pModel) DRAGON_NOEXCEPT;

