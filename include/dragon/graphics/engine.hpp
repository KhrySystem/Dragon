#pragma once

struct Engine {
	VkInstance vkInstance;
	std::vector<GLFWwindow*> windows;
};
