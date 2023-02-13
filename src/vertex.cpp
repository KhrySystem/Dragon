#include <dragon/dragon.hpp>

DGAPI VkVertexInputBindingDescription _dgGenerateVertexBindingDescription() {
	VkVertexInputBindingDescription bindingDescription{};

	bindingDescription.binding = 0;
	bindingDescription.stride = sizeof(DgVertex);
	bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	return bindingDescription;
}

DGAPI boost::array<VkVertexInputAttributeDescription, 3> _dgGetAttributeDescriptions() {
	boost::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

	attributeDescriptions.at(0).binding = 0;
	attributeDescriptions.at(0).location = 0;
	attributeDescriptions.at(0).format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions.at(0).offset = offsetof(DgVertex, position);

	attributeDescriptions.at(1).binding = 0;
	attributeDescriptions.at(1).location = 1;
	attributeDescriptions.at(1).format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions.at(1).offset = offsetof(DgVertex, color);

	attributeDescriptions.at(2).binding = 0;
	attributeDescriptions.at(2).location = 2;
	attributeDescriptions.at(2).format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions.at(2).offset = offsetof(DgVertex, normalVec);

	return attributeDescriptions;
}
