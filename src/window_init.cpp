#include <dragon/dragon.hpp>

#include "window_camera.hpp"
#include "window_pipeline.hpp"

DgResult createWindowSurface(VkInstance instance, GLFWwindow* pWindow, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) {
    if(glfwCreateWindowSurface(instance, pWindow, pAllocator, pSurface)) {
        return DG_GLFW_CREATE_WINDOW_SURFACE_FAILED;
    }
    return DG_SUCCESS;
}

DgResult createSwapChain(std::shared_ptr<DgWindow>& pWindow) {
    VkSurfaceFormatKHR surfaceFormat = pWindow->PFN_chooseSwapSurfaceFormat(pWindow->swapChainSupportDetails.formats);
    VkPresentModeKHR presentMode = pWindow->PFN_chooseSwapPresentMode(pWindow->swapChainSupportDetails.presentModes);
    VkExtent2D extent = pWindow->PFN_chooseSwapExtent(pWindow->pWindow, pWindow->swapChainSupportDetails.capabilities);

    uint32_t imageCount = pWindow->swapChainSupportDetails.capabilities.minImageCount + 1;

    if (pWindow->swapChainSupportDetails.capabilities.maxImageCount > 0 && imageCount > pWindow->swapChainSupportDetails.capabilities.maxImageCount) {
        imageCount = pWindow->swapChainSupportDetails.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = pWindow->surface;

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    createInfo.oldSwapchain = pWindow->swapChain;

    uint32_t queueFamilyIndices[] = {pWindow->pGPU->gpuQueueFamilies.graphicsFamily.value(), pWindow->pGPU->gpuQueueFamilies.presentFamily.value()};

    if (pWindow->pGPU->gpuQueueFamilies.graphicsFamily != pWindow->pGPU->gpuQueueFamilies.presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    createInfo.preTransform = pWindow->swapChainSupportDetails.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;
    if (vkCreateSwapchainKHR(pWindow->pGPU->device, &createInfo, nullptr, &pWindow->swapChain) != VK_SUCCESS) {
        return DG_VULKAN_SWAP_CHAIN_CREATION_FAILED;
    }

    vkGetSwapchainImagesKHR(pWindow->pGPU->device, pWindow->swapChain, &imageCount, nullptr);
    pWindow->swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(pWindow->pGPU->device, pWindow->swapChain, &imageCount, pWindow->swapChainImages.data());

    pWindow->swapChainImageFormat = surfaceFormat.format;
    pWindow->swapChainExtent = extent;

    return DG_SUCCESS;
}

DgResult createImageViews(std::shared_ptr<DgWindow>& pWindow) {
    pWindow->swapChainImageViews.resize(pWindow->swapChainImages.size());
    for (size_t i = 0; i < pWindow->swapChainImages.size(); i++) {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = pWindow->swapChainImages[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = pWindow->swapChainImageFormat;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;
        if (vkCreateImageView(pWindow->pGPU->device, &createInfo, nullptr, &pWindow->swapChainImageViews[i]) != VK_SUCCESS) {
            return DG_VULKAN_CREATE_IMAGE_VIEW_FAILED;
        }
    }
    return DG_SUCCESS;
}

DgResult findPresentQueue(DgGPU* pGPU, VkSurfaceKHR& surface, VkQueue* pPresentQueue) {
    uint32_t i = 0;
    for(VkQueueFamilyProperties queueFamily : pGPU->vQueueFamilyProperties) {
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(pGPU->physicalDevice, i, surface, &presentSupport);
        if(presentSupport) {
            pGPU->gpuQueueFamilies.presentFamily = i;
            #if !defined(NDEBUG) || defined(_DEBUG)
                std::cout << "Found queue " << i << " for Presentation Queue Support.\n";
            #endif
        }
        i++;
    }
    if(pGPU->gpuQueueFamilies.presentFamily.has_value()) {
        vkGetDeviceQueue(pGPU->device, pGPU->gpuQueueFamilies.presentFamily.value(), 0, pPresentQueue);
    } else {
        return DG_VULKAN_PRESENT_QUEUE_NOT_FOUND;
    }
    
    return DG_SUCCESS;
}

DgResult createCommandPool(std::shared_ptr<DgWindow>& pWindow) {
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = pWindow->pGPU->gpuQueueFamilies.graphicsFamily.value();
    if (vkCreateCommandPool(pWindow->pGPU->device, &poolInfo, nullptr, &pWindow->commandPool) != VK_SUCCESS) {
        return DG_VULKAN_CREATE_COMMAND_POOL_FAILED;
    }
    return DG_SUCCESS;
}

DgResult createSyncObjects(std::shared_ptr<DgWindow>& pWindow) {
    pWindow->imageAvailableSemaphores.resize(DRAGON_RENDER_FRAME_MAX);
    pWindow->renderFinishedSemaphores.resize(DRAGON_RENDER_FRAME_MAX);
    pWindow->inFlightFences.resize(DRAGON_RENDER_FRAME_MAX);
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for(size_t i = 0; i < DRAGON_RENDER_FRAME_MAX; i++) {
        if (vkCreateSemaphore(pWindow->pGPU->device, &semaphoreInfo, nullptr, &pWindow->imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(pWindow->pGPU->device, &semaphoreInfo, nullptr, &pWindow->renderFinishedSemaphores[i]) != VK_SUCCESS) {
            return DG_VULKAN_CREATE_SEMAPHORE_FAILED;
        }

        if(vkCreateFence(pWindow->pGPU->device, &fenceInfo, nullptr, &pWindow->inFlightFences[i]) != VK_SUCCESS) {
            return DG_VULKAN_CREATE_FENCE_FAILED;
        }
    }
    

    return DG_SUCCESS;
}
 
DGAPI DgResult dgCreateWindow(DgWindowCreateInfo createInfo, std::shared_ptr<DgWindow>& pWindow, std::unique_ptr<DgEngine>& pEngine, std::shared_ptr<DgCamera> pCamera) {
    DgResult r;
    if(pEngine == nullptr) return DG_ARGUMENT_INVALID;

    pWindow->pGPU = &pEngine->vGPUs[0];
    pWindow->vvpModels.resize(createInfo.renderLayers);

    pWindow->pWindow = glfwCreateWindow(createInfo.width, createInfo.height, createInfo.pTitle, nullptr, nullptr);
    glfwSetWindowUserPointer(pWindow->pWindow, pWindow.get());
    glfwSetFramebufferSizeCallback(pWindow->pWindow, dgGLFWFramebufferSizeCallback);
    pWindow->framebufferResized = DG_FALSE;


    pEngine->vpWindows.push_back(pWindow);

    r = createWindowSurface(pEngine->instance, pWindow->pWindow, nullptr, &pWindow->surface);
    if(r != DG_SUCCESS) return r;
    r = findPresentQueue(pWindow->pGPU, pWindow->surface, &pWindow->presentQueue);
    if(r != DG_SUCCESS) return r;
    assert(pWindow->presentQueue != nullptr);
    r = getSwapChainSupportDetails(pWindow->pGPU, pWindow);

    if(createInfo.PFN_chooseSwapSurfaceFormat != nullptr)
        pWindow->PFN_chooseSwapSurfaceFormat = createInfo.PFN_chooseSwapSurfaceFormat;
    else
        pWindow->PFN_chooseSwapSurfaceFormat = chooseSwapSurfaceFormat;
    if(createInfo.PFN_chooseSwapPresentMode != nullptr)
        pWindow->PFN_chooseSwapPresentMode = createInfo.PFN_chooseSwapPresentMode;
    else
        pWindow->PFN_chooseSwapPresentMode = chooseSwapPresentMode;
    if(createInfo.PFN_chooseSwapExtent != nullptr)
        pWindow->PFN_chooseSwapExtent = createInfo.PFN_chooseSwapExtent;
    else
        pWindow->PFN_chooseSwapExtent = chooseSwapExtent;

    pWindow->pCamera = pCamera;
    r = createCamera(pWindow);
    if(r != DG_SUCCESS) return r;
    r = createSwapChain(pWindow);
    if(r != DG_SUCCESS) return r;
    r = createImageViews(pWindow);
    if(r != DG_SUCCESS) return r;
    r = createRenderPass(pWindow);
    if(r != DG_SUCCESS) return r;
    r = createGraphicsPipeline(pWindow);
    if(r != DG_SUCCESS) return r;
    r = createFramebuffers(pWindow);
    if(r != DG_SUCCESS) return r;
    r = createCommandPool(pWindow);
    if(r != DG_SUCCESS) return r;
    r = createSyncObjects(pWindow);
    if(r != DG_SUCCESS) return r;


    return r;
}

DGAPI DgResult dgRecreateSwapchain(std::shared_ptr<DgWindow>& pWindow) {
    vkDeviceWaitIdle(pWindow->pGPU->device);
    
    #if !defined(NDEBUG) || defined(_DEBUG)
        std::cout << "Recreating Swap Chain\n" << std::flush;
    #endif

    DgResult r;
    dgDestroySwapchain(pWindow);

    r = createSwapChain(pWindow);
    if(r != DG_SUCCESS) return r;
    r = createImageViews(pWindow);
    if(r != DG_SUCCESS) return r;
    r = createFramebuffers(pWindow);
    if(r != DG_SUCCESS) return r;
    r = createSyncObjects(pWindow);
    return r;
}
