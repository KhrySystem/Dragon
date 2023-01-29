#include "dragon_core.h"

struct _DgVertex {
	glm::vec3 position;
	glm::vec3 color;
};

DGAPI VkVertexInputBindingDescription dgGenerateVertexBindingDescription(_DgVertex* pVertex);
