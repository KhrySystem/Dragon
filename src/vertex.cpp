#include <dragon/dragon.hpp>

DGAPI VkVertexInputBindingDescription _dgGenerateVertexBindingDescription() {
	VkVertexInputBindingDescription bindingDescription{};

	bindingDescription.binding = 0;
	bindingDescription.stride = sizeof(DgVertex);
	bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	return bindingDescription;
}

DGAPI boost::array<VkVertexInputAttributeDescription, DRAGON_VERTEX_ATTRIBUTE_COUNT> _dgGetAttributeDescriptions() {
	boost::array<VkVertexInputAttributeDescription, DRAGON_VERTEX_ATTRIBUTE_COUNT> attributeDescriptions{};

	attributeDescriptions.at(0).binding = 0;
	attributeDescriptions.at(0).location = 0;
	attributeDescriptions.at(0).format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions.at(0).offset = offsetof(DgVertex, position);

	attributeDescriptions.at(1).binding = 0;
	attributeDescriptions.at(1).location = 1;
	attributeDescriptions.at(1).format = VK_FORMAT_R32G32B32A32_SFLOAT;
	attributeDescriptions.at(1).offset = offsetof(DgVertex, color);

	attributeDescriptions.at(2).binding = 0;
	attributeDescriptions.at(2).location = 2;
	attributeDescriptions.at(2).format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions.at(2).offset = offsetof(DgVertex, normalVec);

	attributeDescriptions.at(3).binding = 0;
	attributeDescriptions.at(3).location = 3;
	attributeDescriptions.at(3).format = VK_FORMAT_R64_SFLOAT;
	attributeDescriptions.at(3).offset = offsetof(DgVertex, shininess);

	attributeDescriptions.at(4).binding = 0;
	attributeDescriptions.at(4).location = 4;
	attributeDescriptions.at(4).format = VK_FORMAT_R32G32B32A32_SFLOAT;
	attributeDescriptions.at(4).offset = sizeof(DgVertex);

	return attributeDescriptions;
}
