extends Node


const Simple = preload("res://bin/simple.gdns")

onready var simple_instance = Simple.new()

func _on_Button_pressed():
	print(OS.get_name())
	$Label.text = "C++ code says: " + simple_instance.get_data()
