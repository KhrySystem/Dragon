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
		return 1;
	} 
	printf("Engine created");
	dgTerminateEngine(&engine);
}