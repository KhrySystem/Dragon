cmake_minimum_required(VERSION 3.24)

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
project(Dragon VERSION 1.0.2.3 LANGUAGES CXX C)

# Include the CTest and build_vars modules
include(CTest)
include(tools/cmake/build_vars.cmake)

# Find and include the Python, Vulkan, Doxygen, and GTest packages
find_package(Python REQUIRED COMPONENTS Interpreter)
find_package(Vulkan REQUIRED)
find_package(Doxygen)

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

if(NOT DEFINED Dragon_MAX_POINT_LIGHTS)
	set(Dragon_MAX_POINT_LIGHTS 128)
endif()

if(NOT DEFINED Dragon_MAX_DIRECTIONAL_LIGHTS)
	set(Dragon_MAX_DIRECTIONAL_LIGHTS 4)
endif()

# Create an option to build Dragon specific tests
option(Dragon_BUILD_TESTS "Build Dragon-specific tests" ON)
# Create an option to build Dragon documentation
option(Dragon_BUILD_DOCS "Build Dragon Documentation" ON)
# Set the name of the library to "dragon-{version}"
set(Dragon_LIB_NAME dragon-${Dragon_VERSION_MAJOR})

#Remove GLFW options
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_DOCS OFF)
# Add all the third-party code repos
add_subdirectory(external/boost/algorithm)
add_subdirectory(external/boost/align)
add_subdirectory(external/boost/array)
add_subdirectory(external/boost/assert)
add_subdirectory(external/boost/atomic)
add_subdirectory(external/boost/bind)
add_subdirectory(external/boost/chrono)
add_subdirectory(external/boost/concept_check)
add_subdirectory(external/boost/config)
add_subdirectory(external/boost/container)
add_subdirectory(external/boost/container_hash)
add_subdirectory(external/boost/conversion)
add_subdirectory(external/boost/core)
add_subdirectory(external/boost/date_time)
add_subdirectory(external/boost/describe)
add_subdirectory(external/boost/detail)
add_subdirectory(external/boost/exception)
add_subdirectory(external/boost/function)
add_subdirectory(external/boost/function_types)
add_subdirectory(external/boost/functional)
add_subdirectory(external/boost/fusion)
add_subdirectory(external/boost/integer)
add_subdirectory(external/boost/intrusive)
add_subdirectory(external/boost/io)
add_subdirectory(external/boost/iterator)
add_subdirectory(external/boost/lexical_cast)
add_subdirectory(external/boost/move)
add_subdirectory(external/boost/mp11)
add_subdirectory(external/boost/mpl)
add_subdirectory(external/boost/numeric_conversion)
add_subdirectory(external/boost/optional)
add_subdirectory(external/boost/predef)
add_subdirectory(external/boost/preprocessor)
add_subdirectory(external/boost/range)
add_subdirectory(external/boost/ratio)
add_subdirectory(external/boost/rational)
add_subdirectory(external/boost/regex)
add_subdirectory(external/boost/smart_ptr)
add_subdirectory(external/boost/static_assert)
add_subdirectory(external/boost/system)
add_subdirectory(external/boost/thread)
add_subdirectory(external/boost/throw_exception)
add_subdirectory(external/boost/tokenizer)
add_subdirectory(external/boost/tuple)
add_subdirectory(external/boost/type_index)
add_subdirectory(external/boost/type_traits)
add_subdirectory(external/boost/typeof)
add_subdirectory(external/boost/unordered)
add_subdirectory(external/boost/utility)
add_subdirectory(external/boost/variant2)
add_subdirectory(external/boost/winapi)
add_subdirectory(external/glfw)
add_subdirectory(external/glm)

# Add subdirectories for the engine
add_subdirectory(include/dragon)
add_subdirectory(src)

# If Dragon_BUILD_TESTS is true, add the tools/tests directory as a subdirectory
if(Dragon_BUILD_TESTS)
	add_subdirectory(external/googletest)
	add_subdirectory(tools/tests)
endif()

# Include the tools/cmake/install.cmake file
include(tools/cmake/install.cmake)