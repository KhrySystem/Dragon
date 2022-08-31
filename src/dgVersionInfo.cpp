#include <dragon/Dragon.h>

void dgPrintVersionInfo() {
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
    #if defined(AL_VERSION_1_1)
        printf("1.1\n");
    #elif defined(AL_VERSION_1_0)
        printf("1.0\n");
    #endif
    printf("\t - GL Mathmatics Version ");
    printf("%d.", GLM_VERSION_MAJOR);
    printf("%d.", GLM_VERSION_MINOR);
    printf("%d.", GLM_VERSION_PATCH);
    printf("%d\n", GLM_VERSION_REVISION);
    printf("\t - GLFW Version ");
    printf("%d.", GLFW_VERSION_MAJOR);
    printf("%d.", GLFW_VERSION_MINOR);
    printf("%d\n", GLFW_VERSION_REVISION);

}