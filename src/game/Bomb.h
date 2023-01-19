#pragma once
#include "../server/Messages/Text.h"

namespace Game {
    struct Bomb {
        int fd; // placer id
        PlayerPosition position; // bomb's position
    };
}