extends Node


const Simple = preload("res://bin/simple.gdns")
onready var simple_instance = Simple.new()

onready var ClientManager = load("res://bin/client_manager.gdns")

func _on_Button_pressed():
	print(OS.get_name())
	$Label.text = "C++ code says: " + simple_instance.get_data()
	ClientManager.new().connectToServer("127.0.0.1", "3333")
