extends Node

var server := WebSocketMultiplayerPeer.new()
var players := {} # peer_id -> Vector3
var player_inputs: Dictionary = {}

const SERVER_FPS_CONTROLLER = preload("uid://cqewns2aeaaca")
var world
var player: FPSController
var playerMovement: PlayerMovementFSM

# Client Prediction
var timer: float
var currentTick: int = 0
var peerTick: int = 0
var minTimeBetweenTicks: float

const SERVER_TICK_RATE: float  = 30
const BUFFER_SIZE: int  = 1024

var stateBuffer: Array = []
var inputBuffer: Array = []
var latestServerState: Dictionary
var lastProcessedState: Dictionary

var input_dict := {
	"mouse_x_offset": 0.0,
	"mouse_y_offset": 0.0,
	"event_based_actions": {},
	"state_based_actions": {}
}

var InputPayLoad: Dictionary  = {
	"tick" : 2,
	"input" : input_dict
}

var StatePayLoad: Dictionary  = {
	"tick" : 2,
	"position" : Vector3.ZERO,
	"velocity" : Vector3.ZERO
}

func on_peer_connected(id: int):
	print("Peer connected" + str(id))
	var player: Node3D = SERVER_FPS_CONTROLLER.instantiate()
	var range = 5
	player.global_position = Vector3(7.6, 13.5, 24.3)
	player.rotation = Vector3(0,0,0) 
	world.add_child(player)
	players[id] = player
	
	
func on_peer_disconnected(id: int):
	print("Peer disconnected" + str(id))
	players[id].queue_free()
	
	
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
	
	# Client Prediction
	minTimeBetweenTicks = 1 / SERVER_TICK_RATE
	
	stateBuffer.resize(BUFFER_SIZE)
	#inputBuffer.resize(BUFFER_SIZE)


#func _physics_process(delta: float) -> void:
	#if(player):
		#player.move_and_slide()
func _physics_process(delta: float):
	
	server.poll()
	if(playerMovement):
		playerMovement.set_process(false)
		playerMovement.set_physics_process(false)
	# Client Prediction
	timer += delta
	while timer >= minTimeBetweenTicks:
		timer -= minTimeBetweenTicks
		HandleTick()
		currentTick += 1
		
	
	#for peer_id in get_tree().get_multiplayer().get_peers():
		#var player_controller: FPSController = players[peer_id]
		#var player_cam : Camera3D = player_controller.get_camera()
		#rpc_id(peer_id, "set_self_position", peer_id, players[peer_id].position, player_controller.rotation, player_controller.get_camera().rotation)
	
func HandleTick():
	
	
	var id: int 
	for peer_id in get_tree().get_multiplayer().get_peers():	
		player = players[peer_id]
		id = peer_id
		playerMovement = player.get_node("PlayerMovementFSM")

	
		
	if player:
		var bufferIndex: int = -1
		
		while inputBuffer.size() > 0: 
			var input_pay_load: Dictionary
			if inputBuffer[0]:
				input_pay_load = inputBuffer[0]
				inputBuffer.remove_at(0)
				#print(inputBuffer)
				
				bufferIndex = input_pay_load.tick % BUFFER_SIZE
				#Update player movement
				var input_dict : Dictionary = input_pay_load.duplicate()
				#print("input dict is " + str(input_dict) + " at tick " + str(input_pay_load.tick))
				player.rotation = input_pay_load["rotation"]
				player.get_node("Head/Camera3D").rotation = input_pay_load["cam_rotation"]
				var temp_player_pos = player.position
				
				#print("Position Before "  + str(temp_player_pos) + " Position After " +  str(player.position))	
				#print("Current tick" + str(input_pay_load.tick) + " Player position: " + str(player.position) + "Player rotation" + str(player.rotation) ) 
				#
				var state_pay_load: Dictionary = StatePayLoad.duplicate()
				state_pay_load.tick = input_pay_load.tick
				playerMovement.Update(minTimeBetweenTicks, input_pay_load.input)
				state_pay_load.position = player.position
				state_pay_load.velocity = player.velocity
				stateBuffer[bufferIndex] = state_pay_load
		#send to client
			else:
				print("Tick skipped")
		if bufferIndex != -1:
			rpc_id(id, "on_server_movement_state", id, stateBuffer[bufferIndex])
		else:
			print("Not sending this time")
	pass
	
	
@rpc("any_peer", "unreliable_ordered")
func on_client_input(input_pay_load: Dictionary):
	inputBuffer.push_back(input_pay_load)
	pass
	
@rpc("any_peer", "unreliable_ordered")
func handle_mouse(x, y):
	var id = get_tree().get_multiplayer().get_remote_sender_id()
	var player : FPSController = players[id]
	player.handle_mouse_input(x, y)
	
# Client → Server
@rpc("any_peer", "unreliable_ordered")
func update_input(input_dict: Dictionary):
	#Update the input states of the player who wants to update it
	var id = get_tree().get_multiplayer().get_remote_sender_id()
	player_inputs[id] = input_dict
	# Set the input dict for the player
	players[id].set_input_dict(input_dict)
	 
@rpc("any_peer", "unreliable_ordered")
func get_world_state():
	var id = get_tree().get_multiplayer().get_remote_sender_id()
	for peer_id in get_tree().get_multiplayer().get_peers():	
		if peer_id != id:
			rpc_id(id, "set_remote_position", peer_id, players[peer_id].position)
			print("Giving world state")
		

# Client → Server
@rpc("any_peer", "unreliable_ordered")
func update_position(pos: Vector3):
	var id = get_tree().get_multiplayer().get_remote_sender_id()
	var player: Node3D = players[id]
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

@rpc("authority", "unreliable_ordered")
func on_server_movement_state(peer_id: int, state_pay_load: Dictionary):
	pass
