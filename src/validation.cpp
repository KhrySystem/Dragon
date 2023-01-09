#include <dragon/dragon.hpp>

VkResult dgCreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)glfwGetInstanceProcAddress(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    }
    else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

static VKAPI_ATTR VkBool32 VKAPI_CALL dgVulkanDebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) {

    std::cerr << "Vulkan Debug:" << std::endl;
    std::cerr << "\tSeverity: " << messageSeverity << std::endl;
    std::cerr << "\tMessage Type" << messageType << std::endl;
    std::cerr << "\tCallback Data:" << std::endl;
    std::cerr << "\t\tcmdBufLabelCount" << pCallbackData->cmdBufLabelCount << std::endl;
    std::cerr << "\t\tflags" << pCallbackData->flags << std::endl;
    std::cerr << "\t\tmessageIdNumber" << pCallbackData->messageIdNumber << std::endl;
    std::cerr << "\t\tobjectCount" << pCallbackData->objectCount << std::endl;
    std::cerr << "\t\tcmdBufLabels" << pCallbackData->pCmdBufLabels << std::endl;
    std::cerr << "\t\t\tcolor" << pCallbackData->pCmdBufLabels->color << std::endl;
    std::cerr << "\t\t\tpLabelName" << pCallbackData->pCmdBufLabels->pLabelName << std::endl;
    std::cerr << "\t\tpMessage" << pCallbackData->pMessage << std::endl;
    std::cerr << "\t\tpMessageIdName" << pCallbackData->pMessageIdName << std::endl;
    std::cerr << "\t\tqueueLabelCount" << pCallbackData->queueLabelCount << std::endl;

    return VK_FALSE;
}

void dgDestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)glfwGetInstanceProcAddress(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}

void dgGLFWErrorCallback(int code, const char* description) {
    std::cerr << "GLFW Error:" << std::endl;
    std::cerr << "\tCode:" << code << std::endl;
    std::cerr << "\tMessage:" << description << std::endl;
}
