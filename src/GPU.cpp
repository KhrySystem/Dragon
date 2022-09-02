#include <dragon/Dragon.h>

DGAPI GPU* dgCreateGPU(VkPhysicalDevice gpuHandle) {
	if(gpuHandle == DG_NULL) return DG_NULL;
	GPU* gpu = new GPU();
	vkGetPhysicalDeviceProperties(gpuHandle, &gpu->deviceProperties);
	vkGetPhysicalDeviceFeatures(gpuHandle, &gpu->deviceFeatures);
	return gpu;
}

DGAPI int dgEvaluateGPU(GPU* gpu) {
	int eval = 1000;
	switch(gpu->deviceProperties.deviceType) {
		case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
			eval += 1000;
			break;
		case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
			eval += 5000;
			break;
		case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
			eval -= 500;
			break;
		case VK_PHYSICAL_DEVICE_TYPE_CPU:
			return 0;
		default:
			eval -= 100;
	}
	eval += (int)(gpu->deviceProperties.limits.maxImageDimension2D);
	eval += (int)(gpu->deviceProperties.limits.maxComputeWorkGroupCount[3] * 
			gpu->deviceProperties.limits.maxComputeWorkGroupInvocations / 
			1000
	);
	if (!gpu->deviceFeatures.geometryShader) {
        return 0;
    }
	return eval;
}