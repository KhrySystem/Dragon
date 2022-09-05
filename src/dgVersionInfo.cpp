#include <dragon/Dragon.h>
#include <dragon/dgEngine.h>

DGAPI void dgPrintVersionInfo() {
    #ifdef DRAGON_CXX_VERSION
        printf("C++ Version %i", DRAGON_CXX_VERSION);
    #endif
    printf("Dragon Engine Version ");
    printf("%d.", DRAGON_VERSION_MAJOR);
    printf("%d.", DRAGON_VERSION_MINOR);
    printf("%d.", DRAGON_VERSION_REVISION);
    printf("%d\n", DRAGON_VERSION_PATCH);
    printf("\t - Vulkan Version ");
    printf("%d.", VK_API_VERSION_MAJOR(VK_HEADER_VERSION_COMPLETE));
    printf("%d.", VK_API_VERSION_MINOR(VK_HEADER_VERSION_COMPLETE));
    printf("%d.", VK_API_VERSION_PATCH(VK_HEADER_VERSION_COMPLETE));
    printf("%d\n", VK_API_VERSION_VARIANT(VK_HEADER_VERSION_COMPLETE));
    printf("\t - OpenAL Version ");
    #if defined(AL_VERSION_1_2)
        printf("1.2");
    #elif defined(AL_VERSION_1_1)
        printf("1.1");
    #elif defined(AL_VERSION_1_0)
        printf("1.0");
    #endif
    printf("\n");
    #if !defined(DRAGON_USE_OPENCL)
        printf("\t - OpenCL NOT AVAILIBLE");
    #elif defined(CL_VERSION_2_0)
        printf("\t - OpenCL Version 2.0");
    #elif defined(CL_VERSION_1_2)
        printf("\t - OpenCL Version 1.2");
    #elif defined(CL_VERSION_1_1)
        printf("\t - OpenCL Version 1.1");
    #else
        printf("\t - OpenCL Version 1.0");
    #endif
    printf("\n");
    printf("\t - GL Mathmatics Version ");
    printf("%d.", GLM_VERSION_MAJOR);
    printf("%d.", GLM_VERSION_MINOR);
    printf("%d.", GLM_VERSION_PATCH);
    printf("%d\n", GLM_VERSION_REVISION);
    printf("\t - GLFW Version ");
    printf("%d.", GLFW_VERSION_MAJOR);
    printf("%d.", GLFW_VERSION_MINOR);
    printf("%d\n", GLFW_VERSION_REVISION);
    printf("\t - Maximum Number of Concurrent Processes: %d\n", DRAGON_MAXIMUM_PROCESSES);
    std::vector<GPU*> gpus = dgGetGPUs(dgCreateEngine("dgPrintVersionInfo"));
    printf("\t - GPUs in System:\n");
    if(gpus.size() > 0) {
        for(int i = 0; i < gpus.size(); i++) {
            printf("\t\t - %i : %s\n", i, gpus.at(i)->deviceProperties.deviceName);
            printf("\t\t\t - General Evaluation Score: %i", dgEvaluateGPU(gpus.at(i)));
            printf("\t\t\t - Work Groups per Dispatch: %i", gpus.at(i)->deviceProperties.limits.maxComputeWorkGroupCount[3]);
            printf("\t\t\t - Thread Count per Workgroup: %i", gpus.at(i)->deviceProperties.limits.maxComputeWorkGroupInvocations);
            printf("\t\t\t - Maximum Thread Count: %i", gpus.at(i)->deviceProperties.limits.maxComputeWorkGroupSize[3]);
        }
    } else {
        printf("\t\t - NO DETECTED GPU IN SYSTEM");
    }
    
}