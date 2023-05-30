#include <dragon/dragon.h>
#include <stdio.h>

VkResult dgCreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) glfwGetInstanceProcAddress(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void dgDestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}

VKAPI_ATTR VkBool32 VKAPI_CALL dgVulkanDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
    fprintf(stderr, "Vulkan Debug:\n\tSeverity: %x\n\tMessage: %s\n", messageSeverity, pCallbackData->pMessage);
    return VK_FALSE;
}

DGAPI const char** dgGetAvailableVulkanExtensions(uint32_t* count) {
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> extensions(extensionCount);
    static std::vector<const char*> extensionNames(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
    for (int i = 0; i < extensionCount; i++) {
        extensionNames[i] = extensions[i].extensionName;
    }
    *count = extensionCount;
    return extensionNames.data();
}

DGAPI const char** dgGetAvailableVulkanLayers(uint32_t* count) {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    static std::vector<const char*> layerNames(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for(int i = 0; i < layerCount; i++) {
        layerNames[i] = availableLayers[i].layerName;
    }

    *count = layerCount;
    return layerNames.data();
}