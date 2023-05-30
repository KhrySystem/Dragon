#pragma once
#include <dragon/dragon.h>
#include <vector>
#include <iostream>

void autoRate(DgGPU* pGPU) {
    pGPU->score = 0;

    // Discrete GPUs have a significant performance advantage
    if (pGPU->physicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
        pGPU->score += 1000;
    }

    // Maximum possible size of textures affects graphics quality
    pGPU->score += pGPU->physicalDeviceProperties.limits.maxImageDimension2D;

    // Application can't function without geometry shaders or queue families
    if (!(pGPU->physicalDeviceFeatures.geometryShader || pGPU->gpuQueueFamilies.graphicsFamilyFound)) {
        pGPU->score = -1;
    }
}

void __getQueueFamilies(DgGPU* pGPU) {
    DgGPUQueueFamilies queueFamilies;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(pGPU->physicalDevice, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> vqueueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(pGPU->physicalDevice, &queueFamilyCount, vqueueFamilies.data());

    int i = 0;
    queueFamilies.graphicsFamilyFound = DG_FALSE;
    for (const auto& queueFamily: vqueueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            queueFamilies.graphicsFamilyFound = DG_TRUE;
            queueFamilies.graphicsFamily = i;
        }

        i++;
    }

    pGPU->gpuQueueFamilies = queueFamilies;
}

DgResult __createGPUs(DgEngine* pEngine, void(*pGPURatingFunc)(DgGPU*), std::vector<const char*> layers) {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(pEngine->instance, &deviceCount, nullptr);
    if(deviceCount == 0) {
        return DG_VULKAN_SUPPORTED_GPUS_UNAVAILABLE;
    }
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(pEngine->instance, &deviceCount, devices.data());

    std::vector<DgGPU*> gpus(pEngine->pGPUs, pEngine->pGPUs + deviceCount);

    for (int i = 0; i < deviceCount; i++) {
        gpus[i] = (DgGPU*)malloc(sizeof(DgGPU));
        gpus[i]->physicalDevice = devices[i];
        vkGetPhysicalDeviceProperties(gpus[i]->physicalDevice, &gpus[i]->physicalDeviceProperties);
        vkGetPhysicalDeviceFeatures(gpus[i]->physicalDevice, &gpus[i]->physicalDeviceFeatures);
        __getQueueFamilies(gpus[i]);
        if(pGPURatingFunc == nullptr) {
            autoRate(gpus[i]);
        } else {
            pGPURatingFunc(gpus[i]);
        }
        if(gpus[i]->score >= 0) {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = gpus[i]->gpuQueueFamilies.graphicsFamily;
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
                createInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());
                createInfo.ppEnabledLayerNames = layers.data();
            #else
                createInfo.enabledLayerCount = 0;
            #endif
            #if BOOST_OS_MACOS
                std::vector<const char*> extensions(1);
                extensions[0] = "VK_KHR_portability_subset";
                createInfo.enabledExtensionCount = extensions.size();
                createInfo.ppEnabledExtensionNames = extensions.data();
            #else
                createInfo.enabledExtensionCount = 0;
            #endif

            if (vkCreateDevice(gpus[i]->physicalDevice, &createInfo, nullptr, &gpus[i]->device) != VK_SUCCESS) {
                return DG_VULKAN_CREATE_DEVICE_FAILED;
            }
            vkGetDeviceQueue(gpus[i]->device, gpus[i]->gpuQueueFamilies.graphicsFamily, 0, &gpus[i]->graphicsQueue);
        }

    }

    pEngine->pGPUs = gpus.data();
    pEngine->gpuCount = deviceCount;

    return DG_SUCCESS;
}