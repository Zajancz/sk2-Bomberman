#include "ClientManager.hpp"

void ClientManager::_register_methods() {
	godot::register_method("get_data", &ClientManager::get_data);
	godot::register_method("connectToServer", &ClientManager::connectToServer);
	godot::register_method("setPosition", &ClientManager::setPosition);
	godot::register_method("getEnemies", &ClientManager::getEnemies);
	godot::register_method("getEnemyPosition", &ClientManager::getEnemyPosition);
}

void ClientManager::_init() {
	data = "Hello World from C++";
}

godot::String ClientManager::get_data() const {
	return data;
}

void ClientManager::connectToServer(godot::String _ip, godot::String _port) {
	char * ip = (char*) _ip.utf8().get_data();
	char * port = (char*) _port.utf8().get_data();
	printf("Connecting to server from ClientManager via to Network::connectToServer\n");
	Client::Network::connectToServer(ip, port);
}
/// @brief sends player position to the server
void ClientManager::setPosition(godot::Vector2 position) {
	PlayerPosition pp {(int)position.x, (int)position.y};
	printf("Position: {%d, %d}", pp.x, pp.y);
	Client::MessageHandler(Client::Network::agent, NULL, 0)
		.sendMessage<PlayerPosition>(pp);
}

godot::Array ClientManager::getEnemies() {
	printf("Enemies: ");
	godot::Array enemies{};
	for (auto e : Client::GameManager::enemies) {
		enemies.push_back(e.first); // id (fd) of an enemy's client
		printf("%d,",e.first);
	}
	printf("\n");
	return enemies;
}
// For now it's a mock of real functionality
godot::Vector2 ClientManager::getEnemyPosition(int id) {
	PlayerPosition pp = Client::GameManager::enemies[id].position;
	printf("Enemy %d position is {%d, %d}\n", id, pp.x, pp.y);
	godot::Vector2 position = godot::Vector2(pp.x, pp.y);
	return position;
}