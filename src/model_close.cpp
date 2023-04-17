#include <dragon/dragon.hpp>

DGAPI void dgDestroyModel(std::shared_ptr<DgWindow> pWindow, std::shared_ptr<DgModel> pModel) DRAGON_NOEXCEPT {
	vkFreeCommandBuffers(pWindow->pGPU->device, pWindow->commandPool, pModel->buffers.size(), pModel->buffers.data());
	vkDestroyBuffer(pWindow->pGPU->device, pModel->vertexBuffer, nullptr);
	vkFreeMemory(pWindow->pGPU->device, pModel->vertexBufferMemory, nullptr);
	pModel->verticies.clear();
	pModel->verticies.shrink_to_fit();
}
