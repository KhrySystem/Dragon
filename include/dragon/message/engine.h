#pragma once

typedef struct DgMessage {
    const char* engineName;
    DgUInt64 code;
    const char* message;
} DgMessage;

typedef struct DgMessageEngine {
    void((*pCallback)(DgMessage*)); 
} DgMessageEngine;


