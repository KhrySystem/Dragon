#pragma once

#include "dragon_core.hpp"
#include "internal.hpp"

DGAPI std::vector<std::string> dgGetAvailableVulkanExtensions();
DGAPI std::vector<std::string> dgGetAvailableVulkanLayers();

// VULKAN CREATION FUNCTIONS
//VkResult dgCreateInstance(VkInstance instance);
VkResult dgCreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
void dgDestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
void dgDestroyDevice(VkInstance instance, VkDevice device, const VkAllocationCallbacks* pAllocator);

VKAPI_ATTR VkBool32 VKAPI_CALL dgVulkanDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);