# DragonEngine
DragonEngine is a C++, code-first, thin and lightweight Vulkan 3D game engine designed with gaming, AR, and VR in mind. Designed to keep functions tucked away when you don't want to see them, and give access to the APIs beneath it when you want it. 

## Build Dependencies
Several libraries are required for DragonEngine to be built on your computer. 

### Required Dependencies
 - Boost: Dragon requires an pre-installed version of Boost. Dragon looks for Boost::exception, Boost::Filesystem, Boost::Predef, and Boost::Thread.
 - GLFW: Dragon packages its own version of GLFW within the engine, so no further installation required. 
 - GLM: Dragon includes a modified copy of OpenGL Mathematics in the engine.
 - OpenAL: Dragon packages OpenAL-soft inside of the core library. No further installation required. 
 - Vulkan: Dragon requires a version of Vulkan 1, and supports up to Vulkan 1.3, tested up to Vulkan 1.2. 

### Optional Dependencies
 - OpenCL: Dragon's Thunderbreath extension module requires OpenCL to run its acceleration functions. 
 - OpenXR: Dragon's Airbreath extension module requires OpenXR to support the AR and VR capabilities of this engine.

### Test and Documentation Dependencies
 - GTest: Dragon asks for GoogleTest to run certain test scripts.
 - Doxygen: Doxygen is required whn building the documentation for Dragon.  
