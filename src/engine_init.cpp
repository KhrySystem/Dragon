#include <dragon/dragon.hpp>

#include "gpu_init.hpp"

DgResult glfwSetup() {
    if(!glfwInit())             return DG_GLFW_INIT_FAILED;
    if(!glfwVulkanSupported())  return DG_GLFW_VULKAN_SUPPORT_FAILED;

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    return DG_SUCCESS;
}
constexpr VkApplicationInfo createVkAppInfo() {
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
constexpr VkInstanceCreateInfo createVkInstanceCreateInfo(std::vector<const char*>& vExtensions, std::vector<const char*>& vLayers) {
    // Instance Create Info. Used in the vkCreateInstance function
	VkInstanceCreateInfo createInfo;

	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pNext = nullptr;

	createInfo.enabledExtensionCount = vExtensions.size();
	createInfo.ppEnabledExtensionNames = vExtensions.data();

    #ifndef NDEBUG
		createInfo.enabledLayerCount = vLayers.size();
        createInfo.ppEnabledLayerNames = vLayers.data();
    #else
        createInfo.enabledLayerCount = 0;
        createInfo.ppEnabledLayerNames = nullptr;
	#endif
    #if BOOST_OS_MACOS
        createInfo.flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
    #endif
    return createInfo;
}
DgResult createVkInstance(VkInstance& instance, std::vector<std::string>& vExtensions, std::vector<const char*>& vLayers) {
    // Instance Create Info. Used in the vkCreateInstance function
    std::vector<const char*> ext(vExtensions.size(), nullptr);

    for(int i = 0; i < vExtensions.size(); i++) {
        ext[i] = vExtensions[i].c_str();
    }
    VkApplicationInfo appInfo = createVkAppInfo();
    VkInstanceCreateInfo createInfo = createVkInstanceCreateInfo(ext, vLayers);
    createInfo.pApplicationInfo = &appInfo;

    VkResult r = vkCreateInstance(&createInfo, nullptr, &instance);
    if(r != VK_SUCCESS) {
        return DG_VULKAN_CREATE_INSTANCE_FAILED;
    }
    return DG_SUCCESS;
}
#if !defined(NDEBUG) || defined(_DEBUG)
DgResult setupVulkanDebug(VkInstance& instance, VkDebugUtilsMessengerEXT* pDebugMessenger) {
    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
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
DGAPI DgResult dgCreateEngine(DgEngineCreateInfo createInfo, std::unique_ptr<DgEngine>& pEngine) {
    DgResult r;
    static uint32_t glfwExtensionCount;
    static const char** ppGLFWExtensions;
    if(dgActiveEngineCount <= 0) {
        r = glfwSetup();
        if(r != DG_SUCCESS) return r;
    }
    std::vector<const char*> lay(createInfo.vEnabledLayerNames.size(), nullptr);
    for(int i = 0; i < createInfo.vEnabledLayerNames.size(); i++) {
        lay[i] = createInfo.vEnabledLayerNames[i].c_str();
    }

    ppGLFWExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    if (glfwExtensionCount == 0) return DG_GLFW_VULKAN_EXTENSIONS_NOT_FOUND;
    for(int i = 0; i < glfwExtensionCount; i++) {
        createInfo.vEnabledExtensionNames.push_back(ppGLFWExtensions[i]);
    }

    if(createInfo.enableVulkanDebug) {
        createInfo.vEnabledLayerNames.push_back("VK_LAYER_KHRONOS_validation");
        createInfo.vEnabledExtensionNames.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    #if BOOST_OS_MACOS
        createInfo.vEnabledExtensionNames.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
    #endif

    dgActiveEngineCount++;
    r = createVkInstance(pEngine->instance, createInfo.vEnabledExtensionNames, lay);
    if(r != DG_SUCCESS) return r;
    #if !defined(NDEBUG) || defined(_DEBUG)
        r = setupVulkanDebug(pEngine->instance, &pEngine->debugMessenger);
        if(r != DG_SUCCESS) return r;
    #endif
    r = createGPUs(pEngine, createInfo.PFN_GPURatingFunc, lay);
    return r;
}