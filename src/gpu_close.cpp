#include <dragon/dragon.hpp>

DGAPI void dgDestroyGPU(std::shared_ptr<DgGPU> pGPU) DRAGON_NOEXCEPT {
	vkDestroyDevice(pGPU->device, nullptr);
}
