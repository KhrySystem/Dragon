#include <dragon/dragon.hpp>



DgResult createBuffer(std::shared_ptr<DgWindow>& pWindow, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory, VmaAllocation& allocation);