#ifndef DRAGON_10_HPP
	#define DRAGON_10_HPP
	#include "internal.hpp"
	#include <dragon_include_lib.hpp>
	
	bool dgInit() {
		std::cout << "Initialized Dragon. " << std::endl;
		if(!glfwInit())
			return false;
		return true;
	}

	struct any { template<typename T> any(const T&){} };

	#include <object/dgObject.hpp>
	#include <video/dgWindow.hpp>

	void dgTerminate() {
		std::cout << "Terminated Dragon. " << std::endl;
		glfwTerminate();
	}

#endif