extends Node


# Declare member variables here. Examples:
# var a = 2
# var b = "text"


# Called when the node enters the scene tree for the first time.
func _ready():
	var scene = load("res://Player.tscn")
	var player = scene.instance()
	player.position = Vector2(100,100)
	add_child(player)
	


# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass
