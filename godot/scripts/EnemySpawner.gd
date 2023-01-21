extends Node2D

# Note: Enemy spawner's script does care about enemy's position.
# It operates soley on enemy ids

var id_enemies = [] # ids of already spawned enemies

var Enemy = preload("res://scenes/Enemy.tscn")
var _timer = null

func _ready():
	# Setting up timer for checking for new players
	_timer = Timer.new()
	add_child(_timer)
	_timer.connect("timeout", self, "_check_player_list")
	_timer.set_wait_time(5.0)
	_timer.set_one_shot(false) # Make sure it loops
	_timer.start()

func _check_player_list():
	# Determine if there is a new enemy in the game
	for id in Global.clientManager.getEnemies():
		if !(id in id_enemies):
			spawn(id);

# Spawns an Enemy with given 'id'
func spawn(id:int):
	var enemy = Enemy.instance()
	enemy.init(id)
	add_child(enemy)
	id_enemies.append(id)

#func spawn(enemies):
#	var world = main.instance()
#	if id_enemies != enemies:
#		for i in range(4):
#			var Enemy = enemy.instance()
#			Enemy.position = world.get_node("SpawnPoints").get_child(i).position
#			add_child(Enemy)
#			id_enemies.append(i)
