extends Area2D

var in_area = []
var from_player



# Called from the animation.
func explode():
	
		#Rock.get_node("AnimationPlayer").play("exploded")
	#if not is_network_master():
		# Explode only on master.
		#return
	print(in_area)
	for p in in_area:
		if p.has_method("done"):
			# Exploded has a master keyword, so it will only be received by the master.
			p.done()
		#Rock.get_node("AnimationPlayer").play("exploded")
		#p.rpc("exploded")


func done():
	queue_free()

#func _on_bomb_body_enter(body):
#func _on_bomb_body_exit(body):

func _on_Area2D_body_entered(body):
	if not body in in_area:
		in_area.append(body)


func _on_Area2D_body_exited(body):
	in_area.erase(body)
