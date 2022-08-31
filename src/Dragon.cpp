#include <Dragon/Dragon.h>

DGAPI DG_BOOL dgInit() {
    #ifndef DRAGON_MAXIMUM_PROCESSES
        #if (DRAGON_CXX_STANDARD >= 11)
            #include <thread>
            const unsigned int DRAGON_MAXIMUM_PROCESSES = std::thread::hardware_concurrency();
        #else
            #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
                SYSTEM_INFO sysInfo;
                GetSystemInfo(&sysInfo);
                const unsigned int DRAGON_MAXIMUM_PROCESES = sysInfo.dwNumberOfProcessors;
            #elif __APPLE__
            #endif
        #endif
    #endif
    if(!glfwInit()) return DG_FALSE;
    return DG_TRUE;
}

DGAPI void dgDestroyWindow(dgEngine* e) {
    glfwDestroyWindow(e->window);
}