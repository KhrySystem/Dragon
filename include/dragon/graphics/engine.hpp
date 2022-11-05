#pragma once

namespace Graphics {
	struct Engine {
		VkInstance vkInstance;
		std::vector<GLFWwindow*> windows;
	};
}
