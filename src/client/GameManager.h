#pragma once

#include <unordered_map>
#include <list>
#include "../server/Messages/Text.h"
#include "../game/Player.h"
#include "../game/Bomb.h"

namespace Client {
    /// @brief Stores the game state on client side
    class GameManager {
    public:
        // mapping client id onto Player
        static std::unordered_map<int, Game::Player> enemies;
        static std::list<Game::Bomb> bombs;

        static void addEnemy(int fd, PlayerPosition position);
        static void addBomb(Bomb bomb);
        static Bomb getBomb(); // return one bomb player doesnt know about
    };
}