extends Node

var client := WebSocketMultiplayerPeer.new()
var remote_players := {} # peer_id -> Node3D
var fps_controller_scene := preload("res://FPSController/Template_FPSController.tscn")

const SERVER_FPS_CONTROLLER = preload("uid://cqewns2aeaaca")



func _ready():
	var err = client.create_client("ws://127.0.0.1:8081")
	if err != OK:
		push_error("Client failed: %s" % err)
		return
	get_tree().get_multiplayer().multiplayer_peer = client
	get_tree().get_multiplayer().connected_to_server.connect(_on_connected)

func _process(delta):
	client.poll()

	# Send my position every frame (for demo; better = fixed interval)
	if has_node("../World/CharacterBody3D"):
		var pos = get_node("../World/CharacterBody3D").global_transform.origin
		rpc_id(1, "update_position", pos)

func _on_connected():
	print("Connected to server!")

# Server → Client
@rpc("authority", "unreliable_ordered")
func set_remote_position(peer_id: int, pos: Vector3):
	print("updating")
	if not remote_players.has(peer_id):
		var remote_player = fps_controller_scene.instantiate()
		remote_player.name = str(peer_id)
		get_node("../World").add_child(remote_player)
		remote_players[peer_id] = remote_player

	remote_players[peer_id].global_transform.origin = pos

# Declared here too, but only server actually uses it
@rpc("any_peer", "unreliable_ordered")
func update_position(pos: Vector3):
	pass
