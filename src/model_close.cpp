#include <dragon/dragon.hpp>

DGAPI void dgDestroyModel(std::shared_ptr<DgModel> pModel, std::shared_ptr<DgWindow> pWindow) {
    vkDeviceWaitIdle(pWindow->pGPU->device);
    
    vmaDestroyBuffer(pWindow->pGPU->allocator, pModel->vertexBuffer, pModel->vertexAllocation);
    vkFreeMemory(pWindow->pGPU->device, pModel->vertexDeviceMemory, nullptr);
    vmaDestroyBuffer(pWindow->pGPU->allocator, pModel->indexBuffer, pModel->indexAllocation);
    vkFreeMemory(pWindow->pGPU->device, pModel->indexDeviceMemory, nullptr);
}