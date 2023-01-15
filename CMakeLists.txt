cmake_minimum_required(VERSION 3.20)

# Set the output directories for the library and executable
set(LIBRARY_OUTPUT_PATH "${CMAKE_CURRENT_BINARY_DIR}")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_BINARY_DIR}")

# Check if the CMP0077 policy is set and if so, set it to NEW
if(POLICY CMP0077)
    cmake_policy(SET CMP0077 NEW)
endif()

# Set the names of the tools and test folders
set(TOOLS_FOLDER tools)
set(TESTS_FOLDER tests)

# Set the C and C++ standards for the project
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

# Define the project and its version and languages
project(Dragon VERSION 1.0.2.0 LANGUAGES CXX C)

# Include the CTest and build_vars modules
include(CTest)
include(tools/cmake/build_vars.cmake)

# Find and include the Python, Vulkan, Doxygen, and GTest packages
find_package(Python REQUIRED COMPONENTS Interpreter)
find_package(Vulkan REQUIRED)
find_package(Doxygen)
find_package(GTest)

# Check if the library is to be built as shared or static
if(NOT BUILD_SHARED_LIBS)
	# Create an option to build as shared or static
	option(Dragon_BUILD_SHARED "Set to OFF to build static, ON for shared." OFF)
else()
	# Set the option to build as shared directly
	set(Dragon_BUILD_SHARED "Set to OFF to build static, ON for shared." ON)
endif()
# Backload variabled based on Dragon_BUILD_SHARED
set(BUILD_SHARED_LIBS ${Dragon_BUILD_SHARED})
set(DYNAMIC_LOADER ${Dragon_BUILD_SHARED})
set(USE_MSVC_RUNTIME_LIBRARY_DLL ${Dragon_BUILD_SHARED})

# Create an option to build Dragon specific tests
option(Dragon_BUILD_TESTS "Build Dragon-specific tests" ON)
# Backload variables based on Dragon_BUILD_TESTS
set(GLFW_BUILD_TESTS ${Dragon_BUILD_TESTS})
set(GLFW_BUILD_EXAMPLES ${Dragon_BUILD_TESTS})

# Create an option to build Dragon documentation
option(Dragon_BUILD_DOCS "Build Dragon Documentation" ON)
# Backload variables based on Dragon_BUILD_DOCS
set(GLFW_BUILD_DOCS ${Dragon_BUILD_DOCS})

# Set the name of the library to "dragon-{version}"
set(Dragon_LIB_NAME dragon-${Dragon_VERSION})

# Add the GLFW library as a subdirectory
add_subdirectory(external/glfw)

# Add subdirectories for the engine
add_subdirectory(include/dragon)
add_subdirectory(src)

# If Dragon_BUILD_TESTS is true, add the tools/tests directory as a subdirectory
if(Dragon_BUILD_TESTS)
	add_subdirectory(tools/tests)
endif()

# Include the tools/cmake/install.cmake file
include(tools/cmake/install.cmake)