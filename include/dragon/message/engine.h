// dragon/message/engine.h
// Defines 
// structs:
//     DgMessage
//     DgMessageEngine

#pragma once
#ifdef _cplusplus
extern "C" {
#endif

typedef struct DgMessage_T {
    const char* engineName;
    DgUInt64 code;
    char* message;
} DgMessage;

typedef struct DgMessageEngine_T {
    void((*pCallback)(DgMessage*)); 
} DgMessageEngine;


#ifdef _cplusplus
} // dragon/message/engine.h
#endif