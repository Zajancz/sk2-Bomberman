extends Node2D

var id_enemies = []
var i = 0 

var enemy = preload("res://scenes/Enemy.tscn")
var main = load("res://scenes/Main.tscn")
	
func _ready():
	var Enemy = enemy.instance()
	add_child(Enemy)

#func spawn(enemies):
#	var world = main.instance()
#	if id_enemies != enemies:
#		for i in range(4):
#			var Enemy = enemy.instance()
#			Enemy.position = world.get_node("SpawnPoints").get_child(i).position
#			add_child(Enemy)
#			id_enemies.append(i)
