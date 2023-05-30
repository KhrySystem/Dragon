#pragma once
// 0x<error level bit><system><id bits(2)>
// System:
//      GLFW: 0
//      VULKAN: 1
//      CGLM: 2
//      DRAGON: F
enum DgResult {
    DG_SUCCESS = 0x0,
    // Critical errors
    DG_GLFW_INIT_FAILED = 0xF000,
    DG_GLFW_VULKAN_SUPPORT_FAILED = 0xF001,
    DG_GLFW_VULKAN_EXTENSIONS_NOT_FOUND = 0xF002,
    DG_VULKAN_CREATE_INSTANCE_FAILED = 0xF100,
    DG_VULKAN_DEBUG_UTILS_INSTANCE_CREATION_FAILED = 0xF101,
    DG_VULKAN_SUPPORTED_GPUS_UNAVAILABLE = 0xF102,
    DG_VULKAN_CREATE_DEVICE_FAILED = 0xF103,
    // Errors, but mostly fixable
    DG_ARGUMENT_INVALID = 0x8F00
};

typedef DgResult DgResult;