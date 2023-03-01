#include "ClientManager.hpp"

/**
 * @brief This function mainly used for Godot engine needs to be able to see such functions in Godot
*/

void ClientManager::_register_methods() {
	godot::register_method("get_data", &ClientManager::get_data);
	godot::register_method("connectToServer", &ClientManager::connectToServer);
	godot::register_method("setPosition", &ClientManager::setPosition);
	godot::register_method("setBomb", &ClientManager::setBomb);
	godot::register_method("getEnemies", &ClientManager::getEnemies);
	godot::register_method("getEnemyPosition", &ClientManager::getEnemyPosition);
	godot::register_method("getBombs", &ClientManager::getBombs);
}

void ClientManager::_init() {
	data = "Hello World from C++";
}

godot::String ClientManager::get_data() const {
	return data;
}

/**
 * @brief connects to the server through Godot engine
*/

void ClientManager::connectToServer(godot::String _ip, godot::String _port) {
	char * ip = (char*) _ip.utf8().get_data();
	char * port = (char*) _port.utf8().get_data();
	printf("Connecting to server from ClientManager via to Network::connectToServer\n");
	Client::Network::connectToServer(ip, port);
}
/**
 * @brief sends player position to the server
 * */
void ClientManager::setPosition(godot::Vector2 position) {
	PlayerPosition pp {(int)position.x, (int)position.y};
	printf("Position: {%d, %d}", pp.x, pp.y);
	Client::MessageHandler(Client::Network::agent, NULL, 0)
		.sendMessage<PlayerPosition>(pp);
}

/**
 * @brief sends new bomb's position to the server 
*/
void ClientManager::setBomb(godot::Vector2 position) {
	PlayerPosition bombPosition {(int)position.x, (int)position.y};
	Bomb bomb {0, bombPosition};
	printf("New bomb position: {%d, %d}\n", bombPosition.x, bombPosition.y);
	Client::MessageHandler(Client::Network::agent, NULL, 0)
		.sendMessage<Bomb>(bomb);
}
/**
 * @brief gets enemies on the map
*/
godot::Array ClientManager::getEnemies() {
	godot::Array enemies{};
	for (auto e : Client::GameManager::enemies) {
		enemies.push_back(e.first); // id (fd) of an enemy's client
		printf("Enemy: %d,",e.first);
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
// For now, only returns a positions, without info about who placed the bomb
godot::Array ClientManager::getBombs() {
	godot::Array bombs{};
	while (true) {
        Bomb bomb = Client::GameManager::getBomb();
        if (bomb.fd == 0) break;
		bombs.push_back(godot::Vector2(bomb.position.x, bomb.position.y));
        printf("Bomb: {%d, {%d, %d}}, ", bomb.fd, bomb.position.x, bomb.position.y);
    }
	printf("\n");
	return bombs;
}