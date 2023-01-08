#include "ClientManager.hpp"

void ClientManager::_register_methods() {
	godot::register_method("get_data", &ClientManager::get_data);
	godot::register_method("connectToServer", &ClientManager::connectToServer);
	godot::register_method("getPlayers", &ClientManager::getPlayers);
	godot::register_method("getPlayerPosition", &ClientManager::getPlayerPosition);
}

void ClientManager::_init() {
	data = "Hello World from C++";
}

godot::String ClientManager::get_data() const {
	return data;
}

void ClientManager::connectToServer(godot::String _ip, godot::String _port) {
	// const char * c_ip = _ip.utf8().get_data();
	// const char * c_port = _port.utf8().get_data();
	char * ip = (char*) _ip.utf8().get_data();
	char * port = (char*) _port.utf8().get_data();
	printf("Connecting to server from ClientManager via to Network::connectToServer\n");
	// malloc(strlen(c_ip) + 10);
	// char * port = (char*)malloc(strlen(c_port) + 1);
	// strcpy(ip, c_ip);
	// strcpy(port, c_port);

	Client::Network::connectToServer(ip, port);
}
// For now it's a mock of real functionality
godot::Array ClientManager::getPlayers() {
	printf("Returning a mock list of player ids\n");
	printf(">> %d <<",Client::Network::test);
	godot::Array players{};
	players.push_back(10);
	players.push_back(21);
	return players;
}
// For now it's a mock of real functionality
godot::Vector2 ClientManager::getPlayerPosition(int id) {
	godot::Vector2 position = godot::Vector2(100, 10*id);
	return position;
}