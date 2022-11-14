#pragma once

#include <dragon/dragon.h>

extern "C" {
    void mCallback(DgMessage* pMessage) {
        printf("Message Recieved:\n\tEngine:\t\t%s\n\tCode:\t\t%llx\n\tMessage:\t%s", pMessage->engineName, pMessage->code, pMessage->message);
    }
}