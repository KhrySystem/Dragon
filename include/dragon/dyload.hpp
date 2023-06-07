#pragma once

#ifdef DRAGON_BUILD_DLL
    #if BOOST_OS_WINDOWS
        BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved);
    #else
        __attribute__((constructor)) void dllMain();
    #endif
#endif