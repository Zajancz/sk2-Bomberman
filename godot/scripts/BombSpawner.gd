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
	var newBombs: Array = Global.clientManager.getBombs()
	for position in newBombs:
		var bomb = preload("res://scenes//bomb.tscn").instance()
		bomb.global_position = position
		bomb.set_scale(Vector2(0.9,0.9))
		get_node("../..").add_child(bomb)
		bomb.get_node("anim").play("anim")

