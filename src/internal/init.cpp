#include <dragon/dragon.hpp>

using namespace Dragon;

DGAPI DgBool32 Dragon::createEngine(Engine* engine, CreateInfo* createInfo) {
	return Dragon::createEngine(engine, createInfo, nullptr);
}

DGAPI DgBool32 Dragon::createEngine(Engine* pEngine, CreateInfo* pCreateInfo, std::function<void(Message::Message*)> pCallback) {
	
	// Message Init
	if(DRAGON_MESSAGE_ENABLED) {
		pEngine->message.pCallback = pCallback;
	}

	if(!glfwInit()) {
		Message::Message message;
		message.code = 0xFF12000000000000;
		message.message = "GLFW failed to be initialized";
		Message::sendMessage(pEngine, &message);
		return DG_FALSE;
	}

	pEngine->name = pCreateInfo->name;
	// Graphics Init
	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.apiVersion = VK_HEADER_VERSION_COMPLETE;
	appInfo.pApplicationName = pEngine->name.c_str();
	appInfo.pEngineName = "DragonEngine";
	appInfo.engineVersion = DRAGON_VERSION;

	uint32_t glfwExtensionCount = 0;
	char** extensionNames = const_cast<char**>(glfwGetRequiredInstanceExtensions(&glfwExtensionCount));
	std::vector<const char*> extensions(extensionNames, *(&extensionNames + 1));

	#ifdef DG_PLAT_MACOS

	#endif

	#ifndef VK_VERSION_1_3
	    extensions.emplace_back("VK_EXT_tooling_info");
		if(pCreateInfo->verbosity >= 1) {
			Message::Message message;
			message.code = 0x1111000000000000;
			message.message = "Vulkan Version is below 1.3, adding VK_EXT_tooling_info";
			Message::sendMessage(pEngine, &message);
		}
	#endif
	
	VkInstanceCreateInfo instanceCreateInfo{};
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.ppEnabledExtensionNames = extensions.data();
	instanceCreateInfo.enabledExtensionCount = extensions.size();
	instanceCreateInfo.enabledLayerCount = 0;
	instanceCreateInfo.ppEnabledLayerNames = nullptr;
	VkResult result = vkCreateInstance(&instanceCreateInfo, nullptr, &pEngine->graphics.vkInstance);
	if(result != VK_SUCCESS) {
		Message::Message message{};
		message.code = 0xFF11000000000000 + result;
		message.message = "vkCreateInstance failed with result " + Message::VkResultAsString(result);
		Message::sendMessage(pEngine, &message);
	}

	return DG_TRUE;
}

DGAPI void Dragon::updateEngine(Engine* pEngine) {
	glfwPollEvents();
}

DGAPI DgBool32 Dragon::canEngineBeTerminated(Engine* engine) {
	if(!engine->graphics.windows.empty()) {
		return DG_FALSE;
	}
	return DG_TRUE;
}

DGAPI void Dragon::terminateEngine(Engine* engine) {
	vkDestroyInstance(engine->graphics.vkInstance, nullptr);
	glfwTerminate();
}