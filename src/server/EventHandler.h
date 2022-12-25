#pragma once
#include <stdint.h>

struct EventHandler {
    virtual ~EventHandler(){}
    virtual void handleEventEpollin(uint32_t events) = 0;
    virtual void handleEventEpollout(uint32_t events) = 0;
};