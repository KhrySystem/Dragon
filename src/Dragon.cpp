#include <Dragon/Dragon.h>

DGAPI DG_BOOL dgInit() {
    
    if(!glfwInit()) return DG_FALSE;
    return DG_TRUE;
}

DGAPI void dgDestroyEngine(dgEngine* e) {
    glfwDestroyWindow(e->window);
    vkDestroyInstance(e->instance, DG_NULL);
    delete e;
}

DGAPI unsigned int __dgGetMaximumProcesses__() {
    #if (DRAGON_CXX_STANDARD >= 11)
        #include <thread>
        return std::thread::hardware_concurrency();
    #else
        #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
            SYSTEM_INFO sysInfo;
            GetSystemInfo(&sysInfo);
            return sysInfo.dwNumberOfProcessors;
        #elif defined(__APPLE__)
            int mib[4];
            int numCPU;
            std::size_t len = sizeof(numCPU); 
            mib[0] = CTL_HW;
            mib[1] = HW_AVAILCPU;
            sysctl(mib, 2, &numCPU, &len, NULL, 0);
            if (numCPU < 1) {
                mib[1] = HW_NCPU;
                sysctl(mib, 2, &numCPU, &len, NULL, 0);
                if (numCPU < 1)
                    numCPU = 1;
            }
            return numCPU;

        #endif
    #endif
}