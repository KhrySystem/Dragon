#include <dragon/dragon.hpp>

DGAPI DgBool32 dgCreateWindow(DgEngine* pEngine, std::string title, unsigned int width, unsigned int height, DgBool32 isResizable) {
	glfwWindowHint(GLFW_RESIZABLE, isResizable);
	GLFWwindow* glfw = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	assert(glfw != NULL);
	DgWindow window;
	window.window = glfw;
	VkResult result = glfwCreateWindowSurface(pEngine->vulkan, window.window, nullptr, &window.surface);
	switch (result) {
		case VK_ERROR_INITIALIZATION_FAILED:
			#ifndef NDEBUG
			std::cerr << "VK_ERR_INITIALIZATION_FAILED from glfwCreateWindowSurface" << std::endl;
			#endif
			return DG_FALSE;
		case VK_ERROR_EXTENSION_NOT_PRESENT:
			#ifndef NDEBUG
			std::cerr << "VK_ERROR_EXTENSION_NOT_PRESENT from glfwCreateWindowSurface" << std::endl;
			#endif
			return DG_FALSE;
		case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
			#ifndef NDEBUG
			std::cerr << "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR from glfwCreateWindowSurface" << std::endl;
			#endif
			return DG_FALSE;
		default:
			pEngine->windows.push_back(window);
			return DG_TRUE;
	}

	
}

DGAPI int dgGetWindowCount(DgEngine* pEngine) {
	return pEngine->windows.size();
}