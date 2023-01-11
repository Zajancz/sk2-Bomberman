#pragma once
#include "../server/Messages/Text.h"

namespace Game {
    struct Bomb {
        int fd;
        PlayerPosition position;
    };
}