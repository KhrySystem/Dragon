#pragma once
#include <dragon/dragon.hpp>

int autoRate(DgGPU gpu) {
    int score = 0;

    // Discrete GPUs have a significant performance advantage
    if (gpu.physicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
        score += 1000;
    }

    // Maximum possible size of textures affects graphics quality
    score += gpu.physicalDeviceProperties.limits.maxImageDimension2D;

    // Application can't function without geometry shaders or queue families
    if (!(gpu.physicalDeviceFeatures.geometryShader || gpu.gpuQueueFamilies.graphicsFamily.has_value())) {
        score = -1;
    }
    return score;
}

void getQueueFamilies(DgGPU* pGPU) {
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(pGPU->physicalDevice, &queueFamilyCount, nullptr);

    pGPU->vQueueFamilyProperties.resize(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(pGPU->physicalDevice, &queueFamilyCount, pGPU->vQueueFamilyProperties.data());

    uint32_t i = 0;
    for (const VkQueueFamilyProperties queueFamily: pGPU->vQueueFamilyProperties) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            pGPU->gpuQueueFamilies.graphicsFamily = i;
            #if !defined(NDEBUG) || defined(_DEBUG)
                std::cout << "Found queue " << i << " for Graphics Queue Support.\n";
            #endif
        }
        DgBool32 presentSupport = DG_FALSE;
        i++;
    }
}

DgResult createGPUs(std::unique_ptr<DgEngine>& pEngine, std::function<int(DgGPU)>& PFN_GPURatingFunc, std::vector<const char*>& vLayers) {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(pEngine->instance, &deviceCount, nullptr);
    if(deviceCount == 0) {
        return DG_VULKAN_SUPPORTED_GPUS_UNAVAILABLE;
    }
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(pEngine->instance, &deviceCount, devices.data());

    for (int i = 0; i < deviceCount; i++) {
        DgGPU gpu{};

        gpu.physicalDevice = devices[i];
        vkGetPhysicalDeviceProperties(gpu.physicalDevice, &gpu.physicalDeviceProperties);
        vkGetPhysicalDeviceFeatures(gpu.physicalDevice, &gpu.physicalDeviceFeatures);
        getQueueFamilies(&gpu);

        assert(gpu.physicalDevice != NULL);
        assert(gpu.gpuQueueFamilies.graphicsFamily.has_value());

        if(PFN_GPURatingFunc == nullptr) {
            gpu.score = autoRate(gpu);
        } else {
            gpu.score = PFN_GPURatingFunc(gpu);
        }
        if(gpu.score >= 0) {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = gpu.gpuQueueFamilies.graphicsFamily.value();
            queueCreateInfo.queueCount = 1;
            float queuePriority = 1.0f;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            VkPhysicalDeviceFeatures deviceFeatures{};
            VkDeviceCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
            createInfo.pQueueCreateInfos = &queueCreateInfo;
            createInfo.queueCreateInfoCount = 1;

            createInfo.pEnabledFeatures = &deviceFeatures;

            #if !defined(NDEBUG) || defined(_DEBUG)
                createInfo.enabledLayerCount = static_cast<uint32_t>(vLayers.size());
                createInfo.ppEnabledLayerNames = vLayers.data();
            #else
                createInfo.enabledLayerCount = 0;
            #endif
            std::vector<const char*> extensions(2);
            extensions[0] = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
            #if BOOST_OS_MACOS
                extensions[1] = "VK_KHR_portability_subset";
                createInfo.enabledExtensionCount = extensions.size();
                createInfo.ppEnabledExtensionNames = extensions.data();
            #else
                createInfo.enabledExtensionCount = 0;
            #endif

            if (vkCreateDevice(gpu.physicalDevice, &createInfo, nullptr, &gpu.device) != VK_SUCCESS) {
                return DG_VULKAN_CREATE_DEVICE_FAILED;
            }
            if(gpu.gpuQueueFamilies.graphicsFamily.has_value())
                vkGetDeviceQueue(gpu.device, gpu.gpuQueueFamilies.graphicsFamily.value(), 0, &gpu.graphicsQueue);
            else
                return DG_VULKAN_GRAPHICS_QUEUE_NOT_FOUND;

            VmaAllocatorCreateInfo allocInfo{};
            allocInfo.flags = VMA_ALLOCATOR_CREATE_EXTERNALLY_SYNCHRONIZED_BIT;
            allocInfo.physicalDevice = gpu.physicalDevice;
            allocInfo.device = gpu.device;
            allocInfo.instance = pEngine->instance;
            if(vmaCreateAllocator(&allocInfo, &gpu.allocator) != VK_SUCCESS) {
                return DG_VKMEMALLOC_CREATE_ALLOCATOR_FAILED;
            }

            assert(gpu.physicalDevice != VK_NULL_HANDLE);
            assert(gpu.device != VK_NULL_HANDLE);
            assert(gpu.graphicsQueue != VK_NULL_HANDLE);
            pEngine->vGPUs.push_back(gpu);
        }

    }

    return DG_SUCCESS;
}