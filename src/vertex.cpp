#include <dragon/dragon.hpp>

DGAPI VkVertexInputBindingDescription _dgGenerateVertexBindingDescription(_DgVertex* pVertex) {
	VkVertexInputBindingDescription bindingDescription{};

	bindingDescription.binding = 0;
	bindingDescription.stride = sizeof(_DgVertex);
	bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	return bindingDescription;
}

DGAPI boost::array<VkVertexInputAttributeDescription, 2> _dgGetAttributeDescriptions() {
	boost::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

	return attributeDescriptions;
}
