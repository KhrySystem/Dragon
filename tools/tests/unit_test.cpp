#include <dragon/dragon.hpp>

#ifndef NDEBUG
    #if BOOST_OS_WINDOWS
	    #include "windows.h"
	    #define _CRTDBG_MAP_ALLOC 
	    #include <stdlib.h>  
	    #include <crtdbg.h> s
    #endif
	#include <iostream>
#endif
std::vector<DgVertex> verts = {
	{{0.5, 0.5, 0.0}, {1.0, 0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}, 1.0},
	{{0.0, 0.5, 0.0}, {0.0, 1.0, 0.0, 1.0}, {0.0, 0.0, 1.0}, 1.0},
	{{0.0, 0.5, 0.5}, {0.0, 0.0, 1.0, 1.0}, {0.0, 0.0, 1.0}, 1.0}
};

int main(void) {
	#if !defined(NDEBUG) && BOOST_OS_WINDOWS
		_CrtMemState sOld;
		_CrtMemState sNew;
		_CrtMemState sDiff;
		_CrtMemCheckpoint(&sOld); //take a snapshot
	#endif
	DgEngine engine;
	std::shared_ptr<DgEngine> engineRef(&engine);
	DgEngineCreateInfo eci{};
	DgResult result = dgCreateEngine(engineRef, eci);
	if (result != DG_SUCCESS) {
		#ifndef _NDEBUG
			std::cout << result << std::endl;
		#endif
		dgTerminateEngine(engineRef);
		return result;
	}
	DgWindow window;
	std::shared_ptr<DgWindow> windowRef(&window);
	DgWindowCreateInfo wci{};
	wci.pEngine = engineRef;
	wci.title = "Unit Test";
	wci.width = 100;
	wci.height = 100;
	result = dgCreateWindow(windowRef, wci);
	if (result != DG_SUCCESS) {
		#ifndef _NDEBUG
			std::cout << result << std::endl;
		#endif
		dgTerminateEngine(engineRef);
		return result;
	}

	dgAddRenderLayer(windowRef);
	DgModel model;
	std::shared_ptr<DgModel> modelRef(&model);
	result = dgCreateModel(windowRef, 0, verts, modelRef);

	dgUpdate(engineRef);

	dgTerminateEngine(engineRef);

	#if !defined(_NDEBUG) && BOOST_OS_WINDOWS
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