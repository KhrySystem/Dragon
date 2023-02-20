#include <dragon/dragon.hpp>

DGAPI DgResult dgCreateModel(boost::shared_ptr<DgWindow> pWindow, uint32_t layer, std::vector<DgVertex> verts, boost::shared_ptr<DgModel> pModel) {
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

    pWindow->models.at(layer).push_back(pModel);
    return DG_SUCCESS;
}