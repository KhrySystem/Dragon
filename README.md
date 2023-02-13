![example event parameter](https://github.com/KhrySystem/Dragon/actions/workflows/cmake.yml/badge.svg?event=push)
# DragonEngine
DragonEngine is a C++, code-first, thin and lightweight Vulkan 3D game engine designed with gaming, AR, and VR in mind; designed to keep functions tucked away when you don't want to see them, and give access to the APIs beneath it when you want it. 

## Documentation
 - The authoritative home for the HTML documentation is found [here](https://khrysystem.dev/docs/dragon), along with example usage.
 - Documentation can be generated using Doxygen locally.
 - GitHub Issues is a good way to ask questions.

## SDK Structure
### Pre-Build
```txt
 - docs/                             Doxygen build scripts for Dragon
 - external/
      - glfw/                        GLFW submodule for the window
 - include/dragon/                   Dragon Headers for the main library.
 - src/                              C++ Source for Dragon
 - tools/                            Useful scripts not built into the library binary
      - cmake/                       Files and scripts for CMake to find Dragon
           - FindDragon.cmake     !! CMake script that gets placed in your CMake modules directory !!
      - installer/                   Installer source code
      - tests/                       Assorted tests from Dragon
```

### Post-Build
```txt
 - bin/                              Test, debug, and info executables, may also be bin32/ on Linux / MacOS
 - lib/                              Dragon library binaries
 - docs/                             Documentation for Dragon, and GLFW interface through the engine
 - include/                          Single include directory for Dragon
      - dragon/                      Dragon Headers
      - glfw/                        GLFW Headers
```

## Build Dependencies
Several libraries are required for DragonEngine to be built on your computer. 

### Required Dependencies
 - [Vulkan](https://khronos.org/vulkan): Dragon requires a version of Vulkan 1, and supports up to Vulkan 1.3, tested up to Vulkan 1.2. 

### Optional Dependencies
 - [OpenXR](https://khronos.org/openxr): Dragon's XR capability extension module requires OpenXR to support the AR and VR capabilities of this engine. If it can't find it at compile time / install time, no worries. Currently OpenXR is not production-ready, so it will not be used by the engine. We shall wait for Khronos to release an official version.

### Test and Documentation Dependencies
 - Doxygen: Doxygen is required when building the documentation for Dragon.  

## Using Dragon
Dragon uses CMake to build, and provides a FindDragon.cmake file to easily include Dragon into your project. If needed, Dragon can be added as a subdirectory in your project.  

### Building From Source
Dragon will reconfigure its entire structure after finalizing all of the targets in Dragon. By default, this value is "C:\DragonSDK\" on Windows, "~/usr/local/libdragon-dev" on Linux, and "/Applications/DragonSDK" on MacOS. Using CMake, add 
```sh
cmake -DDragon_INSTALL_DIR:String=${PREFERRED_INSTALL}
cmake --build
```

### Minimal CMake Example with Binaries
```CMake
find_package(Dragon REQUIRED)
add_executable(MyApp main.cpp)
target_include_directories(MyApp PUBLIC ${Dragon_INCLUDE_DIR})
target_link_libraries(MyApp PUBLIC ${Dragon_LINK_LIBRARIES})
```

### CMake Example with Dragon as a subdirectory
```CMake
add_subdirectory(external/dragon)
add_executable(MyApp main.cpp)
target_include_directories(MyApp PUBLIC ${Dragon_INCLUDE_DIR})
target_link_libraries(MyApp PUBLIC Dragon::Dragon)
```

### IMPORTANT NOTE
Building Dragon from Source, or using the installer, will automatically attempt to add "FindDragon.cmake" to your CMake modules. Downloading a precompiled source package WILL NOT add this file. You will have to manually move it to your CMake modules path.
