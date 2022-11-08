#include <dragon/dragon.h>
#include <iostream>

#include "general.hpp"

int main(void) {
	DgCreateInfo createInfo;
	createInfo.name = "openWindow.cpp";
	createInfo.refreshRate = 60;

	DgEngine engine;

	if(!dgCreateEngine(&engine, &createInfo, &mCallback, 2)) {
		printf("createEngine failed.");
		// ensure engine doesn't have a stroke
		dgTerminateEngine(&engine);
		return 1;
	} 
	printf("Engine created");

	while(!dgCanEngineBeTerminated(&engine)) {
		dgUpdateEngine(&engine);
	}
	dgTerminateEngine(&engine);
	printf("Engine terminated");
	return 0;
}