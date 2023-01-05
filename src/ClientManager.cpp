#include "ClientManager.hpp"

void ClientManager::_register_methods() {
	godot::register_method("get_data", &ClientManager::get_data);
	godot::register_method("connectToServer", &ClientManager::connectToServer);
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
	printf("Testowy printf\n");
	// malloc(strlen(c_ip) + 10);
	// char * port = (char*)malloc(strlen(c_port) + 1);
	// strcpy(ip, c_ip);
	// strcpy(port, c_port);

	Client::Network::connectToServer(ip, port);
}