#include <dragon/dragon.hpp>

DGAPI DgBool32 dgCreateWindow(DgEngine* pEngine, std::string title, unsigned int width, unsigned int height, DgBool32 isResizable) {
	glfwWindowHint(GLFW_RESIZABLE, isResizable);
	GLFWwindow* glfw = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	assert(glfw != NULL);
	DgWindow window;
	window.window = glfw;
	VkResult result = glfwCreateWindowSurface(pEngine->vulkan, window.window, nullptr, &window.surface);

	assert(result == VK_SUCCESS);

	if (pEngine->primaryGPU->presentationQueue == nullptr) {
		_dgGeneratePresentationQueue(pEngine->primaryGPU, &window);
	}


	return DG_TRUE;
}

DGAPI int dgGetWindowCount(DgEngine* pEngine) {
	return pEngine->windows.size();
}

DGAPI void _dgChooseSwapSurfaceFormat(DgWindow* pWindow, const std::vector<VkSurfaceFormatKHR>& pFormats) {
	// Ensure window is not null
	assert(pWindow != nullptr);

	// Loop through all formats to find one that is good
	for (const VkSurfaceFormatKHR format : pFormats) {
		if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			pWindow->surfaceFormat = format;
			return;
		}
	}
	// return the default format
	pWindow->surfaceFormat = pFormats.at(0);
}

DGAPI void _dgChooseSwapPresentMode(DgWindow* pWindow, const std::vector<VkPresentModeKHR>& pPresentModes) {
	// Ensure window is not null
	assert(pWindow != nullptr);

	#if (BOOST_OS_WINDOWS || BOOST_OS_LINUX || BOOST_OS_MACOS) && !defined(DRAGON_LOW_POWER) // Desktop platforms, high power mode active
		// loop through all modes 
		for (const VkPresentModeKHR availablePresentMode : pPresentModes) {
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) { // VK_PRESENT_MODE_MAILBOX_KHR is the highest power mode, triple-buffered. 
				pWindow->presentMode = availablePresentMode;
				return;
			}
		}
	#endif
	// return the default mode
	pWindow->presentMode = VK_PRESENT_MODE_FIFO_KHR; // VK_PRESENT_MODE_FIFO_KHR is the default double-bufferred mode. Best for mobile devices and devices without triple buffer capabilities.
}

DGAPI void _dgChooseSwapExtent2D(DgWindow* pWindow, const VkSurfaceCapabilitiesKHR* pCapabilities) {
	pWindow->capabilities = *pCapabilities;
	if (pCapabilities->currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		pWindow->extent2D = pCapabilities->currentExtent;
		return;
	} else {
		int width, height;
		glfwGetFramebufferSize(pWindow->window, &width, &height);

		VkExtent2D actualExtent = {
			static_cast<uint32_t>(width),
			static_cast<uint32_t>(height)
		};

		actualExtent.width = std::clamp(actualExtent.width, pCapabilities->minImageExtent.width, pCapabilities->maxImageExtent.width);
		actualExtent.height = std::clamp(actualExtent.height, pCapabilities->minImageExtent.height, pCapabilities->maxImageExtent.height);

		pWindow->extent2D = actualExtent;
	}
}