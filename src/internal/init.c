#include <dragon/dragon.h>

DGAPI DgBool32 dgCreateEngine(DgEngine* pEngine, DgCreateInfo* pCreateInfo, void((*pCallback)(DgMessage*)), int verbosity) {
	pEngine->name = pCreateInfo->name;

	// Message Init
	if(DRAGON_MESSAGE_ENABLED) {
		pEngine->message->pCallback = pCallback;
	}

	if(verbosity >= 1) {
		DgMessage message;
		message.code = 0x11100000FFFFFFFF;
		message.message = "Dragon::createEngine called";
		dgSendMessage(pEngine, &message);
		free(&message);
	}

	if(!glfwInit()) {
		DgMessage message;
		message.code = 0xFF12000000000000;
		message.message = "GLFW failed to be initialized";
		dgSendMessage(pEngine, &message);
		free(&message);
		return DG_FALSE;
	}

	assert(glfwVulkanSupported());

	// Graphics Init
	VkApplicationInfo appInfo;
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.apiVersion = VK_HEADER_VERSION_COMPLETE;
	appInfo.pApplicationName = pEngine->name;
	appInfo.pEngineName = "DragonEngine";
	appInfo.engineVersion = DRAGON_VERSION;

	DgVector extensions;
	VECTOR_ADD(extensions, "VK_KHR_surface");

	#ifdef DG_PLAT_MACOS
	VECTOR_ADD(extensions, VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
	#endif

	#ifndef VK_VERSION_1_3
	VECTOR_ADD(extensions, "VK_EXT_tooling_info");
	if(pCreateInfo->verbosity >= 1) {
		DgMessage message;
		message.code = 0x1111000000000000;
		message.message = "Vulkan Version is below 1.3, adding VK_EXT_tooling_info";
		dgSendMessage(pEngine, &message);
		free(&message);
	}
	#endif
	
	VkInstanceCreateInfo instanceCreateInfo;
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.ppEnabledExtensionNames = (const char**) extensions.items;
	instanceCreateInfo.enabledExtensionCount = VECTOR_TOTAL(extensions);
	instanceCreateInfo.enabledLayerCount = 0;
	instanceCreateInfo.ppEnabledLayerNames = NULL;
	VkResult result = vkCreateInstance(&instanceCreateInfo, NULL, &pEngine->vkInstance);
	if(result != VK_SUCCESS) {
		DgMessage message;
		message.code = 0xFF11000000000000 + result;
		message.message = "%s%s";
		sprintf(message.message, "vkCreateInstance failed with result ", dgVkResultAsString(result));
		dgSendMessage(pEngine, &message);
		free(&message);
	}

	return DG_TRUE;
}

DGAPI void dgWaitForInputs() {
	glfwWaitEvents();
}

DGAPI void dgUpdateEngine(DgEngine* pEngine) {
	glfwPollEvents();
}

DGAPI DgBool32 dgCanEngineBeTerminated(DgEngine* pEngine) {
	if(VECTOR_EMPTY(pEngine->graphics->windows)) {
		return DG_FALSE;
	}
	return DG_TRUE;
}

DGAPI void dgTerminateEngine(DgEngine* pEngine) {
	vkDestroyInstance(pEngine->vkInstance, NULL);
	glfwTerminate();
}