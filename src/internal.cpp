#include <dragon/dragon.hpp>
#include "internal.hpp"

__DgTypes __dghints;

uint32_t findMemoryType(std::shared_ptr<DgWindow>& pWindow, uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(pWindow->pGPU->physicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }
    return -1;
}

VkBufferCreateInfo getBufferCreateInfo(VkDeviceSize size, VkBufferUsageFlags usage) {
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    return bufferInfo;
}

DGAPI void dgGLFWFramebufferSizeCallback(GLFWwindow* pWindow, int width, int height) {
    auto p = reinterpret_cast<DgWindow*>(glfwGetWindowUserPointer(pWindow));
    p->framebufferResized = DG_TRUE;
}

DgResult createBuffer(std::shared_ptr<DgWindow>& pWindow, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory, VmaAllocation& allocation) {
    auto bufferInfo = getBufferCreateInfo(size, usage);

    VmaAllocationCreateInfo allocationInfo = {};
    allocationInfo.usage = VMA_MEMORY_USAGE_CPU_ONLY;

    if(vmaCreateBuffer(pWindow->pGPU->allocator, &bufferInfo, &allocationInfo, &buffer, &allocation, nullptr) != VK_SUCCESS) {
        return DG_VKMEMALLOC_CREATE_BUFFER_FAILED;
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(pWindow->pGPU->device, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(pWindow, memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(pWindow->pGPU->device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        return DG_VULKAN_ALLOCATE_MEMORY_FAILED;
    }
    return DG_SUCCESS;
}