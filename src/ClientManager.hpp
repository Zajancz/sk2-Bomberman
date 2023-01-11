#pragma once

#include <Godot.hpp>
#include <Input.hpp>
#include <Reference.hpp>
#include "client/Network.h"
#include "client/GameManager.h"
#include "client/NetworkAgent.h"
#include "client/MessageHandler.h"
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <cstdio>
#include <cstring>

class ClientManager : public godot::Reference {
	GODOT_CLASS(ClientManager, godot::Reference)

	godot::String data;

public:
	static void _register_methods();

	void _init();

	godot::String get_data() const;

	void connectToServer(godot::String _ip, godot::String _port);
	
	/// @brief Sets a position of a player, this also refreshes the game state (server responds with new data)
	void setPosition(godot::Vector2 position);
	// returns a list of enemies id's
	godot::Array getEnemies();
	// returns a position of a player with given id
	godot::Vector2 getEnemyPosition(int id);
	
};
