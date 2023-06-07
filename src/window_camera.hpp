#include <dragon/dragon.hpp>
#include "internal.hpp"

DgResult createDescriptorSetLayout(std::shared_ptr<DgWindow>& pWindow) {
    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = &uboLayoutBinding;

    if (vkCreateDescriptorSetLayout(pWindow->pGPU->device, &layoutInfo, nullptr, &pWindow->descriptorSetLayout) != VK_SUCCESS) {
        return DG_VULKAN_CREATE_DESCRIPTOR_SET_LAYOUT_FAILED;
    }
    return DG_SUCCESS;
}

DgResult createUniformBuffers(std::shared_ptr<DgWindow>& pWindow) {
    return DG_SUCCESS;
}

DgResult createCamera(std::shared_ptr<DgWindow>& pWindow) {
    DgResult r;
    VkDeviceSize bufferSize = sizeof(DgCamera);

    pWindow->uniformBuffers.resize(DRAGON_RENDER_FRAME_MAX);
    pWindow->uniformDeviceMemory.resize(DRAGON_RENDER_FRAME_MAX);
    pWindow->uniformBuffersMapped.resize(DRAGON_RENDER_FRAME_MAX);
    pWindow->uniformAllocations.resize(DRAGON_RENDER_FRAME_MAX);

    r = createDescriptorSetLayout(pWindow);
    if(r != DG_SUCCESS) return r;

    for(size_t i = 0; i< DRAGON_RENDER_FRAME_MAX; i++) {
        r = createBuffer(
            pWindow, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, 
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
            pWindow->uniformBuffers[i], pWindow->uniformDeviceMemory[i], pWindow->uniformAllocations[i]
        );

        vkMapMemory(pWindow->pGPU->device, pWindow->uniformDeviceMemory[i], 0, bufferSize, 0, &pWindow->uniformBuffersMapped[i]);
    }
    r = createUniformBuffers(pWindow);
    return r;
}