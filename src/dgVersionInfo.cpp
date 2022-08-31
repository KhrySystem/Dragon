#include <dragon/Dragon.h>

#define PRINT(x) printf(#x)

void dgPrintVersionInfo() {
    printf("Dragon Engine Version ");
    printf("%d.", DRAGON_VERSION_MAJOR);
    printf("%d.", DRAGON_VERSION_MINOR);
    printf("%d.", DRAGON_VERSION_REVISION);
    printf("%d\n", DRAGON_VERSION_PATCH);
    printf("\t - Vulkan Version ");
    #if defined(VK_VERSION_1_3)
        printf("1.3.%d", VK_HEADER_VERSION);
    #elif defined(VK_VERSION_1_2)
        printf("1.2.%d", VK_HEADER_VERSION);
    #elif defined(VK_VERSION_1_1)
        printf("1.1.%d", VK_HEADER_VERSION);
    #elif defined(VK_VERSION_1_0)
        printf("1.0.%d", VK_HEADER_VERSION);
    #endif
    printf("%d", )
}