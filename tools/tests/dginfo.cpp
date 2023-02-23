#include <dragon/dragon.hpp>

int main() {
	std::cout << "DragonEngine Version " <<
	#ifdef VK_MAKE_API_VERSION
		VK_API_VERSION_MAJOR(DRAGON_VERSION) << "." << VK_API_VERSION_MINOR(DRAGON_VERSION) << "." << VK_API_VERSION_PATCH(DRAGON_VERSION) << "." << VK_API_VERSION_VARIANT(DRAGON_VERSION) << std::endl;
	#else
		VK_VERSION_MAJOR(DRAGON_VERSION) << "." << VK_VERSION_MINOR(DRAGON_VERSION) << "." << VK_VERSION_PATCH(DRAGON_VERSION) << std::endl;
	#endif
	std::cout << "\tSettings" << std::endl;
	std::cout << "\t\tC Tweak: " <<
	#ifdef DG_C
		"Enabled"
	#else
		"Disabled"
	#endif
		<< std::endl;
	std::cout << "\t\tMax Rendered Frames: " << DRAGON_RENDER_FRAME_MAX << std::endl;
	std::cout << "\t\tBuild Mode:" <<
	#ifdef DRAGON_DLL
		"Shared"
	#else
		"Static"
	#endif
		<< std::endl;
	std::cout << "\t\tMax Point Lights: " << DRAGON_MAX_POINT_LIGHTS << std::endl;
	std::cout << "\t\tMax Directional Lights: " << DRAGON_MAX_DIRECTIONAL_LIGHTS << std::endl;
	std::cout << "\t\tData Per Vertex:" << std::endl;
	std::cout << "\t\t\tPosition (24 bytes)" << std::endl;
	std::cout << "\t\t\tColor    (32 bytes)" << std::endl;
	std::cout << "\t\t\tNormal   (24 bytes)" << std::endl;
	std::cout << "\t\tData Per Model:" << std::endl;
	std::cout << "\t\t\tRotation (32 bytes)" << std::endl;
	std::cout << "\tInternal" << std::endl;
	std::cout << "\t\tGLFW   Version " << GLFW_VERSION_MAJOR << "." << GLFW_VERSION_MINOR << "." << GLFW_VERSION_REVISION << std::endl;
	std::cout << "\t\tVulkan Version " <<
	#ifdef VK_MAKE_API_VERSION
		VK_API_VERSION_MAJOR(VK_HEADER_VERSION_COMPLETE) << "." << VK_API_VERSION_MINOR(VK_HEADER_VERSION_COMPLETE) << "." << VK_API_VERSION_PATCH(VK_HEADER_VERSION_COMPLETE) << "." << VK_API_VERSION_VARIANT(VK_HEADER_VERSION_COMPLETE)
	#else
		VK_VERSION_MAJOR(VK_HEADER_VERSION_COMPLETE) << "." << VK_VERSION_MINOR(VK_HEADER_VERSION_COMPLETE) << "." << VK_VERSION_PATCH(VK_HEADER_VERSION_COMPLETE)
	#endif
	<< std::endl;
	std::cout << "\t\tGLM    Version " << GLM_VERSION_MAJOR << "." << GLM_VERSION_MINOR << "." << GLM_VERSION_REVISION << std::endl;
	std::cout << std::endl << "Press any key to exit... ";
	char x;
	std::cin >> x;
	return;
}
