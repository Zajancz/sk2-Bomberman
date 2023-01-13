extends KinematicBody2D


# Declare member variables here. Examples:
# var a = 2
# var b = "text"


#puppet func do_explosion():
#	$"AnimationPlayer".play("explode")

func done():
	queue_free()
#master func exploaded():
	
#	rpc("do_explosion")
#	do_explosion()
# Called when the node enters the scene tree for the first time.

# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass
