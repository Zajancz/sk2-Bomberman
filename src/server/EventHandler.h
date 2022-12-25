#pragma once
#include <stdint.h>

struct EventHandler {
    virtual ~EventHandler(){}
    virtual void handleEvent(uint32_t events) = 0;
};