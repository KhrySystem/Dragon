#pragma once

#include <iostream>

#include "dragon_core.h"
#include "message.hpp"

VKAPI_ATTR VkBool32 VKAPI_CALL dgVulkanDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,void* pUserData);
DGAPI VkResult dgCreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
DGAPI void dgGLFWErrorCallback(int code, const char* description);
DGAPI void dgDestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
DGAPI void dgSendMessage(DgEngine* pEngine, DgMessage* message);
DGAPI std::string dgConvertVkResultToString(VkResult result);