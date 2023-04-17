#pragma once

#include "dragon_core.hpp"
/**
 * @brief Function for handling debug messages for the vulkan validation layers
 *
 * @param messageSeverity Severity of the message
 * @param messageType Type of the message
 * @param pCallbackData Data of the message
 * @param pUserData User data passed to the callback
 *
 * @return VkBool32
 */
VKAPI_ATTR VkBool32 VKAPI_CALL dgVulkanDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

/**
 * @brief Create a debug messenger for the given vulkan instance
 *
 * @param instance Handle to the vulkan instance
 * @param pCreateInfo pointer to the debug messenger creation info
 * @param pAllocator Pointer to the memory allocation callbacks
 * @param pDebugMessenger Pointer to the created debug messenger
 *
 * @return VkResult
 */
DGAPI VkResult dgCreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);

/**
 * @brief Error callback function for GLFW
 *
 * @param code Error code
 * @param description Description of the error
 */
DGAPI void dgGLFWErrorCallback(int code, const char* description);

/**
 * @brief Destroy the given debug messenger
 *
 * @param instance Handle to the vulkan instance
 * @param debugMessenger Handle to the debug messenger to be destroyed
 * @param pAllocator Pointer to the memory allocation callbacks
 */
DGAPI void dgDestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

DGAPI void _dgGlfwCallback(int code, const char* description);
/**
 * @brief Send a message to the user-defined callback in this engine instance
 *
 * @param pEngine Pointer to the engine
 * @param message Pointer to the message to be sent
 */
DGAPI void _dgSendMessage(int code, const char* pMessage, void* pCallbackData);

/**
 * @brief Convert a VkResult to a string representation
 *
 * @param result VkResult to be converted
 *
 * @return std::string
 */
DGAPI std::string dgConvertVkResultToString(VkResult result);
/**
 * @brief Convert a DgResult to a string representation
 * 
 * @param result DgResult to be converted
 * 
 * @return std::string
 */
DGAPI std::string dgConvertDgResultToString(DgResult result);
