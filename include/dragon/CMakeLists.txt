configure_file("${CMAKE_CURRENT_SOURCE_DIR}/dragon_core.hpp.in" "${Dragon_SOURCE_DIR}/include/dragon/dragon_core.hpp")

add_library(${Dragon_LIB_NAME}-Headers INTERFACE
    "camera.hpp"
    "config.hpp"
    "dragon_core.hpp"
    "dragon.hpp"
    "dyload.hpp"
    "engine.hpp"
    "frag.hpp"
    "gpu_queue_families.hpp"
    "gpu.hpp"
    "internal.hpp"
    "model_create_info.hpp"
    "model.hpp"
    "result.hpp"
    "swap_chain_support.hpp"
    "vert.hpp"
    "vulkan_debug.hpp"
    "window_create_info.hpp"
    "window.hpp"
)