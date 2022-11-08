#pragma once

#if INTPTR_MAX == INT32_MAX
    #define DRAGON_BUS_SIZE 32
#elif INTPTR_MAX == INT64_MAX
    #define DRAGON_BUS_SIZE 64
#else
    #error "Environment bus size unknown."
#endif