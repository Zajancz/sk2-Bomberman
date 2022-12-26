#pragma once

// This structure represents a message to server/client console
struct Text {
    char content1[255];
    char content2[16];
};

struct PlayerPosition {};
struct NewBombPlacement {};
struct NewUser {};
struct NewLobby {};
struct LobbyList {};
struct Join {};
struct Ready {};