extends Node

var threadConnection

# The thread will start here.
func _ready():
	threadConnection = Thread.new()
	threadConnection.start(self, "_thread_connect", ["127.0.0.1", "3333"])

# Function connects to a server, with args = [ip, port]
# Currently on the c++ size we have an infinite loop,
# So for now there is no way to close the thread from godot
func _thread_connect(args):
	print("> Starting a connection thread with: ", args[0], ", ", args[1])
	Global.clientManager.connectToServer(args[0], args[1])
	
func _exit_tree():
	# With current c++ implementation we cannot close the thread from here
	threadConnection.wait_to_finish()

func _on_Button_pressed():
	var players: Array = Global.clientManager.getPlayers()
	print("players: ",players)
	print("position: ",Global.clientManager.getPlayerPosition(players[0]))
