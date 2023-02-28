#include <dragon/dragon.hpp>

#ifndef NDEBUG
	#include "windows.h"
	#define _CRTDBG_MAP_ALLOC 
	#include <stdlib.h>  
	#include <crtdbg.h> 
	#include <iostream>
#endif
std::vector<DgVertex> verts = {
	{{0.5, 0.5, 0.0}, {1.0, 0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}, 1.0},
	{{0.0, 0.5, 0.0}, {0.0, 1.0, 0.0, 1.0}, {0.0, 0.0, 1.0}, 1.0},
	{{0.0, 0.5, 0.5}, {0.0, 0.0, 1.0, 1.0}, {0.0, 0.0, 1.0}, 1.0}
};

int main(void) {
	#ifndef NDEBUG
		_CrtMemState sOld;
		_CrtMemState sNew;
		_CrtMemState sDiff;
		_CrtMemCheckpoint(&sOld); //take a snapshot
	#endif
	DgEngine engine;
	std::shared_ptr<DgEngine> engineRef(&engine);
	DgResult result = dgCreateEngine(engineRef);
	if (result != DG_SUCCESS) {
		std::cout << result << std::endl;
		dgTerminateEngine(engineRef);
		return result;
	}
	DgWindow window;
	std::shared_ptr<DgWindow> windowRef(&window);
	result = dgCreateWindow(engineRef, "", 800, 600, DG_TRUE, DG_FALSE, windowRef);
	if (result != DG_SUCCESS) {
		std::cout << result << std::endl;
		dgTerminateEngine(engineRef);
		return result;
	}

	dgAddRenderLayer(windowRef);
	DgModel model;
	std::shared_ptr<DgModel> modelRef(&model);
	result = dgCreateModel(windowRef, 0, verts, modelRef);

	dgUpdate(engineRef);

	dgTerminateEngine(engineRef);

	#ifndef _NDEBUG
		_CrtMemCheckpoint(&sNew); //take a snapshot 
		if (_CrtMemDifference(&sDiff, &sOld, &sNew)) // if there is a difference
		{
			OutputDebugString("-----------_CrtMemDumpStatistics ---------");
			_CrtMemDumpStatistics(&sDiff);
			OutputDebugString("-----------_CrtMemDumpAllObjectsSince ---------");
			_CrtMemDumpAllObjectsSince(&sOld);
			OutputDebugString("-----------_CrtDumpMemoryLeaks ---------");
			_CrtDumpMemoryLeaks();
		}
	#endif
	return 0;
}