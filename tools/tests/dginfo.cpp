#include <chrono>
#include <dragon/dragon.hpp>

int main() {
	auto t1 = std::chrono::high_resolution_clock::now();
	std::cout << "DragonEngine Version " <<
	#ifdef VK_MAKE_API_VERSION
		VK_API_VERSION_MAJOR(DRAGON_VERSION) << "." << VK_API_VERSION_MINOR(DRAGON_VERSION) << "." << VK_API_VERSION_PATCH(DRAGON_VERSION) << "." << VK_API_VERSION_VARIANT(DRAGON_VERSION) << "\n";
	#else
		VK_VERSION_MAJOR(DRAGON_VERSION) << "." << VK_VERSION_MINOR(DRAGON_VERSION) << "." << VK_VERSION_PATCH(DRAGON_VERSION) << "\n";
	#endif
	std::cout << "\tSettings" << "\n";
	std::cout << "\t\tC Tweak: " <<
	#ifdef DG_C
		"Enabled"
	#else
		"Disabled"
	#endif
		 << "\n";
	std::cout << "\t\tShader Precision: " <<
#ifndef DRAGON_DOUBLE_PRECISION_SHADER
		"Single Precision"
#else
		"Double Precision"
#endif
		<< "\n";
	std::cout << "\t\tMax Rendered Frames: " << DRAGON_RENDER_FRAME_MAX  << "\n";
	std::cout << "\t\tBuild Mode:" <<
	#ifdef DRAGON_DLL
		"Shared"
	#else
		"Static"
	#endif
		 << "\n";
	std::cout << "\t\tMax Point Lights: " << DRAGON_MAX_POINT_LIGHTS  << "\n";
	std::cout << "\t\tMax Directional Lights: " << DRAGON_MAX_DIRECTIONAL_LIGHTS  << "\n";
	std::cout << "\t\tData Per Vertex:"  << "\n";
	std::cout << "\t\t\tPosition  (" << sizeof(DgVertex::position) << " bytes, offset " << offsetof(DgVertex, position) << ")"  << "\n";
	std::cout << "\t\t\tColor     (" << sizeof(DgVertex::color) << " bytes, offset " << offsetof(DgVertex, color) << ")"  << "\n";
	std::cout << "\t\t\tNormal    (" << sizeof(DgVertex::normalVec) << " bytes, offset " << offsetof(DgVertex, normalVec) << ")"  << "\n";
	std::cout << "\t\t\tShininess (" << sizeof(DgVertex::shininess) << " bytes, offset " << offsetof(DgVertex, shininess) << ")"  << "\n";
	std::cout << "\t\tData Per Model:"  << "\n";
	std::cout << "\t\t\tRotation  (" << sizeof(DgModel::rotation) << " bytes, offset " << sizeof(DgVertex) << ")"  << "\n";
	std::cout << "\tInternal"  << "\n";
	std::cout << "\t\tGLFW   Version " << GLFW_VERSION_MAJOR << "." << GLFW_VERSION_MINOR << "." << GLFW_VERSION_REVISION  << "\n";
	std::cout << "\t\tVulkan Version " <<
	#ifdef VK_MAKE_API_VERSION
		VK_API_VERSION_MAJOR(VK_HEADER_VERSION_COMPLETE) << "." << VK_API_VERSION_MINOR(VK_HEADER_VERSION_COMPLETE) << "." << VK_API_VERSION_PATCH(VK_HEADER_VERSION_COMPLETE) << "." << VK_API_VERSION_VARIANT(VK_HEADER_VERSION_COMPLETE)
	#else
		VK_VERSION_MAJOR(VK_HEADER_VERSION_COMPLETE) << "." << VK_VERSION_MINOR(VK_HEADER_VERSION_COMPLETE) << "." << VK_VERSION_PATCH(VK_HEADER_VERSION_COMPLETE)
	#endif
	 << "\n";
	std::cout << "\t\tGLM    Version " << GLM_VERSION_MAJOR << "." << GLM_VERSION_MINOR << "." << GLM_VERSION_REVISION  << "\n";
	auto t2 = std::chrono::high_resolution_clock::now();
	std::cout << "Took " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << "ms to print out information\n";
	DgEngine engine;
	std::shared_ptr<DgEngine> pEngine(&engine);
	DgEngineCreateInfo createInfo{};
	createInfo.vkDeviceExtensions = {};
	createInfo.vkExtensions = {};
	createInfo.vkValidationLayers = {};

	dgCreateEngine(pEngine, createInfo);
	std::cout << "Engine reference count:" << pEngine.use_count() << "\n";
	t1 = std::chrono::high_resolution_clock::now();
	std::cout << "Took " << std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t2).count() << "ms to create DgEngine instance\n";
	dgTerminateEngine(pEngine);
	std::cout << "Engine reference count:" << pEngine.use_count() << "\n";
	t2 = std::chrono::high_resolution_clock::now();
	std::cout << "Took " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << "ms to destroy DgEngine instance\n";
	std::cout << "\nPress enter to exit... " << std::endl;
	std::cin.ignore();
	std::cout << "Engine reference count:" << pEngine.use_count() << "\n";
	pEngine.reset();
	std::cout << "Engine reference count:" << pEngine.use_count() << "\n";
}
