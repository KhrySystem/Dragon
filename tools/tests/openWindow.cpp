#include <dragon/dragon.hpp>
#include <iostream>

#include "general.hpp"

using namespace Dragon;

int main(void) {
	CreateInfo createInfo{};
	createInfo.name = "openWindow.cpp";
	createInfo.refreshRate = 60;
	createInfo.verbosity = 2;

	Engine engine;

	if(createEngine(&engine, &createInfo, &mCallback) != DG_TRUE) {
		printf("createEngine failed.");
		// ensure engine doesn't have a stroke
		terminateEngine(&engine);
		return 1;
	} 
	printf("Engine created");

	while(!canEngineBeTerminated(&engine)) {
		updateEngine(&engine);
	}
	terminateEngine(&engine);
	printf("Engine terminated");
	return 0;
}