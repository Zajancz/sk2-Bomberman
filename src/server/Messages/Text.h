#pragma once

// This structure represents a message to server/client console
struct Text {
    char content1[255];
    int content2;
    char content3[10];
};

struct PlayerPosition {
    int x;
    int y;
};

struct AllPlayersPositions {
    int ids[32];
    PlayerPosition position[32];
};

struct NewBombPlacement {};
struct NewUser {};
struct NewLobby {};
struct LobbyList {};
struct Join {};
struct Ready {};