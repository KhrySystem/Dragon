#include <dragon/dragon.hpp>
#include "internal.hpp"

void copyBuffer(std::shared_ptr<DgWindow>& pWindow, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = pWindow->commandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(pWindow->pGPU->device, &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(pWindow->pGPU->graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(pWindow->pGPU->graphicsQueue);

    vkFreeCommandBuffers(pWindow->pGPU->device, pWindow->commandPool, 1, &commandBuffer);
}

DgResult createVertexBuffer(std::shared_ptr<DgWindow>& pWindow, std::shared_ptr<DgModel>& pModel) {
    VkDeviceSize size = sizeof(pModel->vertices[0]) * pModel->vertices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    VmaAllocation stagingBufferAllocation;
    DgResult r = createBuffer(pWindow, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
        stagingBuffer, stagingBufferMemory, stagingBufferAllocation
    );
    if(r != DG_SUCCESS) return r;

    void* data;
    vmaMapMemory(pWindow->pGPU->allocator, stagingBufferAllocation, &data);
    memcpy(data, pModel->vertices.data(), (size_t) size);
    vmaUnmapMemory(pWindow->pGPU->allocator, stagingBufferAllocation);

    r = createBuffer(pWindow, size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
        pModel->vertexBuffer, pModel->vertexDeviceMemory, pModel->vertexAllocation);
    if(r != DG_SUCCESS) return r;

    copyBuffer(pWindow, stagingBuffer, pModel->vertexBuffer, size);

    vmaDestroyBuffer(pWindow->pGPU->allocator, stagingBuffer, stagingBufferAllocation);
    vkFreeMemory(pWindow->pGPU->device, stagingBufferMemory, nullptr);
    return DG_SUCCESS;
}

DgResult createIndexBuffer(std::shared_ptr<DgWindow>& pWindow, std::shared_ptr<DgModel>& pModel) {
    VkDeviceSize size = sizeof(pModel->indices[0]) * pModel->indices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    VmaAllocation stagingBufferAllocation;
    DgResult r = createBuffer(pWindow, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
        stagingBuffer, stagingBufferMemory, stagingBufferAllocation
    );
    if(r != DG_SUCCESS) return r;

    void* data;
    vmaMapMemory(pWindow->pGPU->allocator, stagingBufferAllocation, &data);
    memcpy(data, pModel->indices.data(), (size_t) size);
    vmaUnmapMemory(pWindow->pGPU->allocator, stagingBufferAllocation);

    r = createBuffer(pWindow, size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
        pModel->indexBuffer, pModel->indexDeviceMemory, pModel->indexAllocation);
    if(r != DG_SUCCESS) return r;

    copyBuffer(pWindow, stagingBuffer, pModel->indexBuffer, size);

    vmaDestroyBuffer(pWindow->pGPU->allocator, stagingBuffer, stagingBufferAllocation);
    vkFreeMemory(pWindow->pGPU->device, stagingBufferMemory, nullptr);
    return DG_SUCCESS;
}

DGAPI DgResult dgCreateModel(DgModelCreateInfo createInfo, std::shared_ptr<DgModel> pModel) {
    if(createInfo.pWindow->vvpModels.size() < createInfo.renderLayer) return DG_ARGUMENT_INVALID;
    createInfo.pWindow->vvpModels[createInfo.renderLayer].push_back(pModel);
    pModel->commandBuffers.resize(DRAGON_RENDER_FRAME_MAX);
    pModel->vertices = createInfo.vertices;
    pModel->indices = createInfo.indices;

    VkCommandBufferAllocateInfo bufAllocInfo{};
    bufAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    bufAllocInfo.commandPool = createInfo.pWindow->commandPool;
    bufAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    bufAllocInfo.commandBufferCount = pModel->commandBuffers.size();

    if (vkAllocateCommandBuffers(createInfo.pWindow->pGPU->device, &bufAllocInfo, pModel->commandBuffers.data()) != VK_SUCCESS) {
        return DG_VULKAN_ALLOCATE_COMMAND_BUFFERS_FAILED;
    }

    DgResult r = createVertexBuffer(createInfo.pWindow, pModel);
    if(r != DG_SUCCESS) return r;
    r = createIndexBuffer(createInfo.pWindow, pModel);

    return r;
}