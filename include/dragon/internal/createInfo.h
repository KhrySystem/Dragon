#pragma once

typedef struct DgCreateInfo {
    int8_t verbosity;
    uint8_t refreshRate;
    const char* name;
    DgBool32 xrEnabled;
} DgCreateInfo;