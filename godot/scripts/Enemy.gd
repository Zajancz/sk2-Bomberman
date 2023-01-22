extends Node2D

const MOTION_SPEED = 190.0

export var stunned:bool = false
var enemy_id:int = 0

func init(id):
	enemy_id = id

func setup_bomb(position):
	var bomb = preload("res://scenes//bomb.tscn").instance()

	#bomb.position = position 
	#+ Vector2(100,100)
	#Global.clientManager.setBomb(position)
	get_node("../..").add_child(bomb)
	bomb.get_node("anim").play("anim")

var prev_bombing = false
var bomb_index = 0


var _timer = null

func _ready():
	stunned = false
	# Setting up timer for updating enemy position
	_timer = Timer.new()
	add_child(_timer)
	_timer.connect("timeout", self, "_update_enemy_position")
	_timer.set_wait_time(0.1)
	_timer.set_one_shot(false) # Make sure it loops
	_timer.start()


func _update_enemy_position():
	var pos:Vector2 = Global.clientManager.getEnemyPosition(enemy_id)
	set_global_position(pos)
	# print("Updated enemy position: ",pos)



