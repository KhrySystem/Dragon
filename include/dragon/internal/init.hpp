#pragma once 

DGAPI DgBool32 createEngine(Engine* pEngine, CreateInfo* pCreateInfo);
DGAPI DgBool32 createEngine(Engine* pEngine, CreateInfo* pCreateInfo, std::function<void(Message::Message*)> pCallback);
DGAPI void updateEngine(Engine* engine);
DGAPI DgBool32 canEngineBeTerminated(Engine* engine);
DGAPI void terminateEngine(Engine* engine);