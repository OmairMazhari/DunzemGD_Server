extends Node

var server := WebSocketMultiplayerPeer.new()
var players := {} # peer_id -> Vector3
var player_inputs: Dictionary = {}

const SERVER_FPS_CONTROLLER = preload("uid://cqewns2aeaaca")
var world

func _ready():
	var err = server.create_server(8081)
	if err != OK:
		push_error("Server failed: %s" % err)
		return
	get_tree().get_multiplayer().multiplayer_peer = server
	print("Server started on port 8081")
	get_tree().get_multiplayer().peer_connected.connect(on_peer_connected)
	get_tree().get_multiplayer().peer_disconnected.connect(on_peer_disconnected)
	# Have the map node ready
	world = get_tree().root.get_node("World")
	if(world):
		print("NOT NULL")


func on_peer_connected(id: int):
	print("Peer connected" + str(id))
	var player: Node3D = SERVER_FPS_CONTROLLER.instantiate()
	var range = 10
	player.global_position = Vector3(randf_range(-range, range),randf_range(-range, range),randf_range(-range, range)) 
	world.add_child(player)
	players[id] = player
	
func on_peer_disconnected(id: int):
	print("Peer disconnected" + str(id))
	players[id].queue_free()
		
func _process(delta):
	server.poll()
	for peer_id in get_tree().get_multiplayer().get_peers():
		rpc_id(peer_id, "set_self_position", peer_id, players[peer_id].position, players[peer_id].rotation)
	
	
	
	

# Client → Server
@rpc("any_peer", "unreliable_ordered")
func update_input(input_dict: Dictionary):
	#Update the input states of the player who wants to update it
	var id = get_tree().get_multiplayer().get_remote_sender_id()
	player_inputs[id] = input_dict
	# Set the input dict for the player
	players[id].set_input_dict(input_dict)
	
# Client → Server
@rpc("any_peer", "unreliable_ordered")
func update_position(pos: Vector3):
	var id = get_tree().get_multiplayer().get_remote_sender_id()
	players[id] = pos
	# Relay to others
	for peer_id in get_tree().get_multiplayer().get_peers():
		if peer_id != id:
			rpc_id(peer_id, "set_remote_position", id, pos)
			

# Declared here too, but only clients actually use it
@rpc("authority", "unreliable_ordered")
func set_remote_position(peer_id: int, pos: Vector3):
	pass

@rpc("authority", "unreliable_ordered")
func set_self_position(peer_id: int, pos: Vector3):
	pass
