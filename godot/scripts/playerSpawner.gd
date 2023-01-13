extends Node

# Declare member variables here. Examples:
# var a = 2
# var b = "text"


# Called when the node enters the scene tree for the first time.
func _ready():
	#var spawn_points = {}
	#spawn_points[0] = 0 #[1]
	#var spawn_point_idx = 1
	#for p in players:
	#	spawn_points[p] = spawn_point_idx
	#	spawn_point_idx += 1
		
	#for p in players:
		#pre_start_game(spawn_points)	
		
	#var world = load("res://scenes/Main.tscn").instance()	
	var scene = load("res://Player.tscn")
	#var spawn_pos = world.get_node("SpawnPoints/" + str(spawn_points[0])).position
	var player = scene.instance()
	#player.position = Vector2(100,100)
	#player.position = spawn_pos 
	add_child(player)
	


# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass
