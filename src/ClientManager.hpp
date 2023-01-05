#pragma once

#include <Godot.hpp>
#include <Input.hpp>
#include <Reference.hpp>
#include "client/Network.h"
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
	// godot::Vector3[] getPosition();
};
