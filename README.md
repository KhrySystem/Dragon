# DragonEngine
DragonEngine is a C++, code-first, thin and lightweight Vulkan 3D game engine designed with gaming, AR, and VR in mind; designed to keep functions tucked away when you don't want to see them, and give access to the APIs beneath it when you want it. 

## SDK Structure
### Pre-Build
```txt
 - docs/                             Doxygen build scripts for Dragon
 - external/
      - glfw/                        GLFW submodule for the window
      - glm/                         GLM submodule for shader interface
      - openal/                      OpenAL-Soft for audio
      - openxr/                      OpenXR for XR
 - include/dragon/                   Dragon Headers for the main library.
 - src/                              C++ Source for Dragon
 - tools/                            Useful scripts not built into the library binary
      - build/                       Files and scripts for CMake to find Dragon
           - FindDragon.cmake     !! CMake script that gets placed in your CMake modules directory !!
      - installer/                   Installer source code
      - tests/                       Assorted tests from Dragon
```

### Post-Build
```txt
 - bin/                              Test, Debug, and info executables
 - docs/                             Documentation for Dragon and all submodules
 - include/                          Single include directory for Dragon
      - boost/                       Only if Boost was not pre-installed
      - dragon/                      Dragon Headers
      - glfw/                        GLFW Headers
      - glm/                         GLM Headers
      - openal/                      OpenAL Headers
      - opencl/                      Only if OpenCL was not pre-installed
      - openxr/                      OpenXR Headers
      - vulkan/                      Only if Vulkan was not pre-installed
 - src/
      - boost/                       Only if Boost was not pre-installed
      - dragon/                      Dragon Source
      - glfw/                        GLFW Source
      - glm/                         GLM Source
      - openal/                      OpenAL Source
      - opencl/                      Only if OpenCL was not pre-installed
      - openxr/                      OpenXR Source
      - vulkan/                      Only if Vulkan was not pre-installed
```

## Build Dependencies
Several libraries are required for DragonEngine to be built on your computer. 

### Required Dependencies
 - [Boost](https://boost.org): Dragon requires an pre-installed version of Boost. Dragon looks for Boost::exception, Boost::Filesystem, Boost::Predef, and Boost::Thread.
 - GLFW: Dragon packages its own version of GLFW within the engine, so no further installation required. 
 - GLM: Dragon includes a modified copy of OpenGL Mathematics in the engine.
 - OpenAL: Dragon packages OpenAL-soft inside of the core library. No further installation required. 
 - [Vulkan](https://khronos.org/vulkan): Dragon requires a version of Vulkan 1, and supports up to Vulkan 1.3, tested up to Vulkan 1.2. 

### Optional Dependencies
 - [OpenCL](https://khronos.org/opencl): Dragon's Thunderbreath extension module requires OpenCL to run its acceleration functions.
 - [OpenXR](https://khronos.org/openxr): Dragon's Firebreath extension module requires OpenXR to support the AR and VR capabilities of this engine.

### Test and Documentation Dependencies
 - GTest: Dragon asks for GoogleTest to run certain test scripts.
 - Doxygen: Doxygen is required whn building the documentation for Dragon.  

## Using Dragon
Dragon uses CMake to build, and provides a FindDragon.cmake file to easily include Dragon into your project. 

### Building From Source
Dragon will reconfigure it's entire structure after finalizing all of the targets in Dragon. By default, this value is "C:\DragonSDK\" on Windows, "~\DragonSDK\" on Linux, and "\Applications\" on MacOS. Using CMake, add 
```sh
cmake -DDragon_INSTALL_DIR:String=${PREFERRED_INSTALL}
```

### Minimal CMake Example with Binaries
```CMake
find_package(Dragon REQUIRED)
add_executable(MyApp main.cpp)
target_include_directories(MyApp PUBLIC ${Dragon_INCLUDE_DIR})
target_link_libraries(MyApp PUBLIC ${Dragon_LINK_LIBRARIES})
```

### IMPORTANT NOTE
Building Dragon from Source, or using the installer, will automatically add "FindDragon.cmake" to your CMake modules. Downloading a precompiled source package WILL NOT add this file. You will have to manually move it to your CMake modules path.
