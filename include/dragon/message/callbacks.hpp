#pragma once
namespace Message {
	DGAPI std::string VkResultAsString(VkResult result);
	DGAPI void sendMessage(Dragon::Engine* pEngine, Message* pMessage);
}
