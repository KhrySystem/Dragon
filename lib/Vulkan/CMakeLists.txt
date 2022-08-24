cmake_minimum_required(VERSION 3.10)

add_subdirectory(Vulkan-Headers)
add_subdirectory(Vulkan-Loader)
add_subdirectory(Vulkan-Tools)
#add_subdirectory(MoltenVK)
add_custom_target(MoltenVK COMMAND make run
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/MoltenVK
)
add_subdirectory(Vulkan-Profiles)
add_subdirectory(Vulkan-ValidationLayers)