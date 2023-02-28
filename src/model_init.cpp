#include <dragon/dragon.hpp>

uint32_t findSuitableMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    return 0;
}

DGAPI DgResult dgCreateModel(std::shared_ptr<DgWindow> pWindow, uint32_t layer, std::vector<DgVertex> verts, std::shared_ptr<DgModel> pModel) {
    if (pWindow == nullptr) {
        return DG_ARGUMENT_IS_NULL;
    }
    pModel->verticies = verts;

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.pNext = NULL;
    allocInfo.commandBufferCount = DRAGON_RENDER_FRAME_MAX;
    allocInfo.commandPool = pWindow->commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;


    if(vkAllocateCommandBuffers(pWindow->pGPU->device, &allocInfo, pModel->buffers.data()) != VK_SUCCESS) {
        return DG_VK_COMMAND_BUFFER_CREATION_FAILED;
    }

    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = sizeof(DgVertex) * verts.size();
    bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(pWindow->pGPU->device, &bufferInfo, nullptr, &pModel->vertexBuffer) != VK_SUCCESS) {
        return DG_VK_VERTEX_BUFFER_CREATION_FAILED;
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(pWindow->pGPU->device, pModel->vertexBuffer, &memRequirements);

    VkMemoryAllocateInfo memAllocInfo{};
    memAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memAllocInfo.allocationSize = memRequirements.size;
    memAllocInfo.memoryTypeIndex = findSuitableMemoryType(pWindow->pGPU->handle, memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    if (vkAllocateMemory(pWindow->pGPU->device, &memAllocInfo, nullptr, &pModel->vertexBufferMemory) != VK_SUCCESS) {
        return DG_VK_MEMORY_ALLOCATION_FAILED;
    }

    vkBindBufferMemory(pWindow->pGPU->device, pModel->vertexBuffer, pModel->vertexBufferMemory, 0);

    void* data;
    vkMapMemory(pWindow->pGPU->device, pModel->vertexBufferMemory, 0, bufferInfo.size, 0, &data);
    memcpy(data, pModel->verticies.data(), (size_t)bufferInfo.size);
    vkUnmapMemory(pWindow->pGPU->device, pModel->vertexBufferMemory);

    pWindow->models.at(layer).push_back(pModel);
    return DG_SUCCESS;
}