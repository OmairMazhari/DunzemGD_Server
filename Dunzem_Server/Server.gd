extends Node

var world : Node3D
var spawns : Node3D

var player_array: Array = []

var player_dict : Dictionary = {
	"id" : 0,
	
	"instance" : FPSController,
	"respawn_timer" : 0,
	"time_dead" : 0,
	"ARRAY_input_pay_load_buffer" : {
		"input_pay_load" : {
			"key_input" : {
				"state_based" : {
					"up" : false,
					"down" : false,
					"right" : false,
					"left" : false
				},
				"event_based" : {
					"jump" : false,
					"no_clip" : false,
				},
				"mouse_input" : {
					"offset" : Vector2.ZERO,
					"is_processing" : true
				}
			}, 
			"tick" : 0
		}	
	},
	
	"ARRAY_state_pay_load_buffer" : { 
		"state_pay_load": {
			"self" : {
				"position" : Vector3.ZERO
			}, 
			"other" : {
				".id" : {
					"position" : Vector3.ZERO,
					"rotation" : Vector3.ZERO,
				}
			}
		}
	},
} 

var player_inputs: Dictionary = {}

const SERVER_FPS_CONTROLLER = preload("res://FPSController/FPSController.tscn")


# Client Prediction
var timer: float
var currentTick: int = 0
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
# Game Logic Variables
const RESPAWN_TIME : float = 2000


var server := WebSocketMultiplayerPeer.new()

func _ready():
	var err = server.create_server(8081)
	if err != OK:
		push_error("Server failed: %s" % err)
		return
	
	get_tree().get_multiplayer().multiplayer_peer = server
	print("Server started on port 808")
	get_tree().get_multiplayer().peer_connected.connect(on_peer_connected)
	get_tree().get_multiplayer().peer_disconnected.connect(on_peer_disconnected)
	
	initialize_nodes()
	stateBuffer.resize(BUFFER_SIZE)
	#inputBuffer.resize(BUFFER_SIZE)
	
	
func initialize_nodes():
	world = get_tree().root.get_node("World")
	spawns = world.get_node("Spawns")

func _physics_process(delta: float):
	server.poll()
	HandleTick(delta)
	currentTick += 1
	
	
func HandleTick(delta: float):
	
	# Store everyone's information into the dictionary
	var other_players_info : Dictionary
	for other_player_id in get_tree().get_multiplayer().get_peers():
			other_players_info[other_player_id] = {
				"position"	: player_array[retrive_player_index(other_player_id)].instance.global_position,
				"rotation"	: player_array[retrive_player_index(other_player_id)].instance.global_rotation,
				"input" : {}
				}
			if player_array[retrive_player_index(other_player_id)].input_buffer.size() > 0: 
				if player_array[retrive_player_index(other_player_id)].input_buffer[0]:
					other_players_info[other_player_id].input = player_array[retrive_player_index(other_player_id)].input_buffer[0].input.key_input
						
	for peer_id in get_tree().get_multiplayer().get_peers():
		
		var player_state_dict : Dictionary = player_array[retrive_player_index(peer_id)]
		var player : Dictionary = player_array[retrive_player_index(peer_id)]	
		var player_fps_controller : FPSController = player.instance	
				
		if !player_fps_controller.is_dead() && !player_state_dict.time_dead:
			player_state_dict.respawn_timer = RESPAWN_TIME
			player_state_dict.time_dead = Time.get_ticks_msec()
		
					
		
		if player_state_dict.respawn_timer >= Time.get_ticks_msec() - player_state_dict.time_dead:
			player_state_dict.time_dead = 0
			var spawn_point : Node3D = spawns.get_child(randi_range(0,2))
			player_fps_controller.set_curr_health(player_fps_controller.max_health)
			player_fps_controller.global_position = spawn_point.global_position
			

		var player_movement: PlayerMovementFSM = player_fps_controller.get_node("PlayerMovementFSM")
		var player_weapon_manager: PlayerWeaponManager  = player_fps_controller.get_node("PlayerWeaponManager");
		var player_input_buffer : Array = player.input_buffer
		var player_state_buffer : Array = player.state_buffer

		var bufferIndex: int = -1

		while player_input_buffer.size() > 0: 

			if player_input_buffer[0]:
				
				var input_pay_load: Dictionary = player_input_buffer[0]
				player_input_buffer.remove_at(0)
				
				bufferIndex = input_pay_load.tick % BUFFER_SIZE

				var key_input_dict : Dictionary
				key_input_dict  = input_pay_load.input.key_input
					
				var mouse_input_dict : Dictionary
				mouse_input_dict  = input_pay_load.input.mouse_input
			
				
				# Rotate the player first
				player_fps_controller.handle_mouse_input(mouse_input_dict.offset.x, mouse_input_dict.offset.y)
	
				# Move the player
				if !(!player_fps_controller.is_dead() && player_state_dict.time_dead != 0):
					pass
					
				player_movement.Update(delta, key_input_dict)
				player_weapon_manager.Update(delta, key_input_dict)
				
				# Store resulting data
				var state_pay_load: Dictionary
				
				state_pay_load = { 
					"tick" : 0,
					"self" : { 
						"position" : 0,
						"global_transform" : 0,
						"velocity" : 0,
						"respawn_timer" : 0,
						"combat": {
							"on_hit" : 0,
							"part" : 0,
							
						},
						"on_hit" : 0
					},
					"others" : Dictionary()
				}
				
				state_pay_load.tick = input_pay_load.tick
				# Self information
				state_pay_load.self.position = player_fps_controller.global_position
				state_pay_load.self.global_transform = player_fps_controller.global_transform
				state_pay_load.self.velocity = player_fps_controller.velocity
				state_pay_load.self.health = player_fps_controller.curr_health
				state_pay_load.self.combat = player_fps_controller.get_combat_report()
				
				rpc_id(peer_id, "reliable_client_input", player_fps_controller.get_combat_report())
				#print("combat report : " + str(state_pay_load.self.combat) + " at tick " + str(state_pay_load.tick))
				player_fps_controller.set_combat_report([])
				
				
				# Information about other players
				state_pay_load.others = other_players_info
				player_state_buffer[bufferIndex] = state_pay_load
				
					
		if bufferIndex != -1:
			rpc_id(peer_id, "on_server_movement_state", peer_id, player_state_buffer[bufferIndex])
		
		
func retrive_player_index(id : int):
	for i in range(player_array.size()):
		if player_array[i].id == id :
			return i 
	return -1

func on_peer_connected(id: int):
	print("Player Connected: " + str(id))
	var new_player : Dictionary = player_dict.duplicate(true)
	new_player.id = id
	new_player.instance = SERVER_FPS_CONTROLLER.instantiate()
	# Set up the player in world
	world.add_child(new_player.instance)
	new_player.instance.global_position = Vector3(7.6, randi_range(0,10) ,24.3)
	# Set the buffer size
	new_player.input_buffer = Array()
	#new_player.input_buffer.resize(BUFFER_SIZE)

	new_player.state_buffer = Array()
	new_player.state_buffer.resize(BUFFER_SIZE)
	
	player_array.push_back(new_player)
	
	# Inform about the new game
	rpc_id(id, "on_new_game_joined", "boxy_map")
	
	
func on_peer_disconnected(id: int):
	print("Player Disconnected: " + str(id))
	var player_id : int = retrive_player_index(id)
	if player_id != -1:	
		var player : Dictionary =  player_array[player_id]
		if(player.instance) :
			player.instance.queue_free()
		player_array.remove_at(player_id)

@rpc("any_peer", "unreliable_ordered")
func on_client_input(id: int, input_pay_load: Dictionary):
	player_array[retrive_player_index(id)].input_buffer.push_back(input_pay_load)
	pass
 
@rpc("any_peer", "reliable")
func reliable_client_input(peer_id: int, reliable_input_pay_load: Dictionary):
	pass

@rpc("authority", "unreliable_ordered")
func on_server_movement_state( state_pay_load: Dictionary):
	pass
	
@rpc("authority", "unreliable_ordered")
func on_new_game_joined(map):
	pass
