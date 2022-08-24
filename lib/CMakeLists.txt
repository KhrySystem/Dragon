cmake_minimum_required(VERSION 3.10)

set(SKIP_PERFORMANCE_COMPARISON ON)


add_subdirectory(Robin-Hood-Hashing)
add_subdirectory(Boost)
add_subdirectory(GoogleTest)
add_subdirectory(GLSLang)
add_subdirectory(Cereal)
add_subdirectory(OpenAL-Soft)
add_subdirectory(glfw)
add_subdirectory(glm)

add_subdirectory(SPIRV)
add_subdirectory(Vulkan)