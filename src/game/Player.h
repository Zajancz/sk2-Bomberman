#pragma once
#include "../server/Messages/Text.h"
#include "Bomb.h"
#include <list>

namespace Game {
    struct Player {
        int fd;
        PlayerPosition position;
        std::list<Game::Bomb> bombs;
    };
}