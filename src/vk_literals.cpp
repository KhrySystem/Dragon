#include <dragon/dragon.hpp>

VkResult dgCreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
    PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT) glfwGetInstanceProcAddress(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void dgDestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
    PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT) glfwGetInstanceProcAddress(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    } else {
        fprintf(stderr, "PFN_vkDestroyDebugUtilsMessengerEXT (dgDestroyDebugUtilsMessengerEXT) was a NULL function\n");
    }
}

void dgDestroyDevice(VkInstance instance, VkDevice device, const VkAllocationCallbacks* pAllocator) {
    PFN_vkDestroyDevice func = (PFN_vkDestroyDevice) glfwGetInstanceProcAddress(instance, "vkDestroyDevice");
   if (func != nullptr) {
        func(device, pAllocator);
   } else {
        fprintf(stderr, "PFN_vkDestroyDevice (dgDestroyDevice) was a NULL function\n");
    }
}

VKAPI_ATTR VkBool32 VKAPI_CALL dgVulkanDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
    fprintf(stderr, "Vulkan Debug:\n\tSeverity: 0x%x\n\tMessage: %s\n", messageSeverity, pCallbackData->pMessage);
    return VK_FALSE;
}

DGAPI std::vector<std::string> dgGetAvailableVulkanExtensions() {
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

    __dghints.vAvailableExtensions.resize(extensionCount);
    for (int i = 0; i < extensionCount; i++) {
        __dghints.vAvailableExtensions[i] = extensions[i].extensionName;
    }

    return __dghints.vAvailableExtensions;
}

DGAPI std::vector<std::string> dgGetAvailableVulkanLayers() {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, NULL);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
    __dghints.vAvailableLayers.resize(layerCount);
    for(int i = 0; i < layerCount; i++) {
        __dghints.vAvailableLayers[i] = availableLayers[i].layerName;
    }

    return __dghints.vAvailableLayers;
}