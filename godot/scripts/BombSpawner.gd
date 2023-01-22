extends Area2D

var _timer = null

func _ready():
	# Setting up timer for checking for new players
	_timer = Timer.new()
	add_child(_timer)
	_timer.connect("timeout", self, "_check_bomb_list")
	_timer.set_wait_time(0.01)
	_timer.set_one_shot(false) # Make sure it loops
	_timer.start()
	
func _check_bomb_list():
	print("hello")
	# Determine if there is a new enemy in the game
	var newBombs: Array = Global.clientManager.getBombs()
	print(newBombs)
	for position in newBombs:
		print(position)
		var bomb = preload("res://scenes//bomb.tscn").instance()
		bomb.global_position = position
		get_node("../..").add_child(bomb)
		bomb.get_node("anim").play("anim")
	
#func spawn(position:Vector2):
	#var bomb = preload("res://scenes//bomb.tscn").instance()
	#get_node("../..").add_child(bomb)
