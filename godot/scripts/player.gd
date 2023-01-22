extends KinematicBody2D

const MOTION_SPEED = 190.0

export var stunned = false

func setup_bomb(position):
	var bomb = preload("res://scenes//bomb.tscn").instance()

	bomb.position = position
	
	Global.clientManager.setBomb(position) # send a new bomb to the server
	print("Bomb is planted at %d", position)
	get_node("../..").add_child(bomb)
	bomb.get_node("anim").play("anim")

var prev_bombing = false
var bomb_index = 0


var _timer = null

func _physics_process(_delta):
	var motion = Vector2()
	if Input.is_action_pressed("move_left"):
		motion += Vector2(-1, 0)
	if Input.is_action_pressed("move_right"):
		motion += Vector2(1, 0)
	if Input.is_action_pressed("move_up"):
		motion += Vector2(0, -1)
	if Input.is_action_pressed("move_down"):
		motion += Vector2(0, 1)
		
	var player_position = get_position()

	var bombing = Input.is_action_pressed("add_bomb")

	if stunned:
		bombing = false
		motion = Vector2()
	
	if bombing and not prev_bombing:
		setup_bomb(player_position)
		

	prev_bombing = bombing
	
	move_and_slide(motion * MOTION_SPEED)


func _ready():
	stunned = false
	# Setting up timer for updating player position on the server
	_timer = Timer.new()
	add_child(_timer)
	_timer.connect("timeout", self, "_set_position")
	_timer.set_wait_time(0.25)
	_timer.set_one_shot(false) # Make sure it loops
	_timer.start()


func _set_position():
	Global.clientManager.setPosition(get_global_position())
	# print("Updated player position: ",get_global_position())
