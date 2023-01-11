#pragma once
#include "../server/Messages/Text.h"

namespace Game {
    struct Player {
        int fd;
        PlayerPosition position;
    };
}