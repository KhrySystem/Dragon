# DragonEngine
DragonEngine is a C++, code-first, thin and lightweight Vulkan 3D game engine designed with gaming, AR, and VR in mind; designed to keep functions tucked away when you don't want to see them, and give access to the APIs beneath it when you want it. 

## Structure
```txt
 - docs/                             Doxygen build scripts for Dragon.
 - extensions/                       All the optional Dragon Extensions.
      - firebreath/                  See https://github.com/KhrySystem/Dragon-Firebreath
      - ironbreath/                  See https://github.com/KhrySystem/Dragon-Ironbreath
      - lightbreath/                 See https://github.com/KhrySystem/Dragon-Lightbreath
      - streambreath/                See https://github.com/KhrySystem/Dragon-Streambreath
      - thunderbreath/               See https://github.com/KhrySystem/Dragon-Thunderbreath
 - headers/                          See https://github.com/KhrySystem/Dragon-Headers
      - glfw/                        GLFW submodule.
      - glm/                         GLM submodule.
      - include/dragon/              Dragon Headers for the main library.
      - openal/                      OpenAL-Soft submodule.
      - openxr/                      OpenXR submodule
 - src/dg/                           See https://github.com/KhrySystem/Dragon-Source
      - audio/                       
      - *.cpp
 - tools/
      - build/
           - FindDragon.cmake
      - installer/
      - tests/
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
Dragon will reconfigure it's entire structure after finalizing all of the targets in Dragon. By default, this value is "C:\DragonSDK\" on Windows, "~\DragonSDK\" on Linux, and "\Applications\". Using CMake, add 
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
