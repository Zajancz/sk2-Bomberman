#include <unordered_map>
#include <unordered_set>
#include "../game/Player.h"
#include "../game/Bomb.h"

namespace Client {
    /// @brief Stores the game state on client side
    struct GameState {
        static std::unordered_map<int, Game::Player> enemies;
        static std::unordered_set<Game::Bomb> bombs;
    };
}