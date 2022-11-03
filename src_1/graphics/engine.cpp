#include <dragon/dragon.hpp>

DGAPI DgBool32 Dragon::Graphics::createWindow(Dragon::Engine* pEngine, uint8_t width, uint8_t height, std::string title) {
    GLFWwindow* window;
    if(pEngine->graphics.windows.size() != 0) {
        window = glfwCreateWindow(width, height, title.c_str(), NULL, pEngine->graphics.windows.at(0));
    } else {
        window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    }
    if(window != NULL) {
        pEngine->graphics.windows.emplace_back(window);
        return DG_TRUE;
    } 
    
    Message::Message message;
    message.code = 0x8F1200000000FFFF + (width << 24) + (height << 16);
    message.message = "GLFWwindow* creation failed when called with (" + width, ", " + height + title + ")";
    Message::sendMessage(pEngine, &message);
    return DG_FALSE;
}