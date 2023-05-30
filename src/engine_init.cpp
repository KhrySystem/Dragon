#include <dragon/dragon.h>

#include <vector>

#include "gpu_init.h"

DgResult __glfwSetup() {
    if(!glfwInit())             return DG_GLFW_INIT_FAILED;
    if(!glfwVulkanSupported())  return DG_GLFW_VULKAN_SUPPORT_FAILED;

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    return DG_SUCCESS;
}
constexpr VkApplicationInfo __createVkAppInfo() {
    // App info. Contains info about the Engine, the user app, etc.
    VkApplicationInfo appInfo;

    // Use the version that matches the shaders
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pNext = nullptr; // pNext is required to be null based on VkError 0x48eb8c19

	appInfo.apiVersion = VK_API_VERSION_1_2;
	appInfo.applicationVersion = APP_VERSION;
	appInfo.pApplicationName = APP_NAME;
	appInfo.pEngineName = "Dragon Engine";
	appInfo.engineVersion = DRAGON_VERSION;


    return appInfo;
}
VkInstanceCreateInfo __createVkInstanceCreateInfo(VkApplicationInfo appInfo, size_t enabledExtensionCount, const char** ppEnabledExtensionNames, size_t enabledLayerCount, const char** ppEnabledLayerNames) {
    // Instance Create Info. Used in the vkCreateInstance function
	VkInstanceCreateInfo createInfo;

	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

    createInfo.pNext = nullptr;

	createInfo.pApplicationInfo = &appInfo;
	createInfo.enabledExtensionCount = enabledExtensionCount;
	createInfo.ppEnabledExtensionNames = ppEnabledExtensionNames;

    #ifndef NDEBUG
		createInfo.enabledLayerCount = enabledLayerCount;
        createInfo.ppEnabledLayerNames = ppEnabledLayerNames;
    #else
        createInfo.enabledLayerCount = 0;
        createInfo.ppEnabledLayerNames = nullptr;
	#endif
    #if BOOST_OS_MACOS
        createInfo.flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
    #endif
    return createInfo;
}
DgResult __createVkInstance(VkInstance* pInstance, size_t extensionCount, const char** ppExtensions, size_t layerCount, const char** ppLayers) {
    VkApplicationInfo appInfo = __createVkAppInfo();
    VkInstanceCreateInfo createInfo = __createVkInstanceCreateInfo(
        appInfo, extensionCount, ppExtensions, layerCount, ppLayers
    );
    VkResult r = vkCreateInstance(&createInfo, nullptr, pInstance);
    if(r != VK_SUCCESS) {
        return DG_VULKAN_CREATE_INSTANCE_FAILED;
    }
    return DG_SUCCESS;
}
#if !defined(NDEBUG) || defined(_DEBUG)
DgResult __setupVulkanDebug(VkInstance instance, VkDebugUtilsMessengerEXT* pDebugMessenger) {
    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity =  VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = dgVulkanDebugCallback;
	createInfo.pUserData = nullptr;

    VkResult result = dgCreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, pDebugMessenger);
	if (result != VK_SUCCESS) {
		return DG_VULKAN_DEBUG_UTILS_INSTANCE_CREATION_FAILED;
	}
	return DG_SUCCESS;
}
#endif
DGAPI DgResult dgCreateEngine(DgEngineCreateInfo createInfo, DgEngine* pEngine) {
    DgResult r;
    static uint32_t glfwExtensionCount;
    static const char** ppGLFWExtensions;
    std::vector<const char*> vpExtensions;
    std::vector<const char*> vpLayers(createInfo.ppEnabledLayerNames, createInfo.ppEnabledLayerNames + createInfo.enabledLayerCount);
    if(dgActiveEngineCount <= 0) {
        r = __glfwSetup();
        if(r != DG_SUCCESS) return r;
    }

    ppGLFWExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    if (glfwExtensionCount == 0) return DG_GLFW_VULKAN_EXTENSIONS_NOT_FOUND;
    for(int i = 0; i < glfwExtensionCount; i++) {
        vpExtensions.push_back(ppGLFWExtensions[i]);
    }
    for(int i = 0; i < createInfo.enabledExtensionCount; i++) {
        vpExtensions.push_back(createInfo.ppEnabledExtensionNames[i]);
    }

    pEngine->pWindows = (DgWindow**)malloc(0); // Temporary placeholders
    pEngine->pGPUs = (DgGPU**)malloc(0);

    if(createInfo.enableVulkanDebug) {
        vpLayers.push_back("VK_LAYER_KHRONOS_validation");
        vpExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    #if BOOST_OS_MACOS
        vpExtensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
    #endif

    dgActiveEngineCount++;
    r = __createVkInstance(&pEngine->instance, vpExtensions.size(), vpExtensions.data(), vpLayers.size(), vpLayers.data());
    if(r != DG_SUCCESS) return r;
    #if !defined(NDEBUG) || defined(_DEBUG)
        r = __setupVulkanDebug(pEngine->instance, &pEngine->debugMessenger);
        if(r != DG_SUCCESS) return r;
    #endif
    r = __createGPUs(pEngine, createInfo.pGPURatingFunc, vpLayers);
    return r;
}