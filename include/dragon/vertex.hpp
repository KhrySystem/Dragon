#include "dragon_core.h"

struct _DgVertex {
	glm::vec3 position;
	glm::vec3 color;
};

DGAPI VkVertexInputBindingDescription _dgGenerateVertexBindingDescription(_DgVertex* pVertex);
DGAPI boost::array<VkVertexInputAttributeDescription, 2> _dgGetAttributeDescriptions();