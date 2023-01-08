#include "Player.h"

using namespace Server;

Player::Player(int _fd, PlayerPosition _pos) {
    fd = _fd;
    position = _pos;
}