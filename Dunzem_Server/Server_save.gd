extends Node

var world : Node3D
var spawns : Node3D

var player_array: Array = []
var dead_players: Dictionary = {
}
var player_names : Dictionary = {
}

var player_stats : Dictionary = {
}

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

const FPS_CONTROLLER_OLD = preload("res://FPSController/FPSController.tscn")
const SERVER_FPS_CONTROLLER = preload("C:/Users/notxc/OneDrive/Documents/dunzem/DunzemGD_Server/Dunzem_Server/FPSController/FPSController_old.tscn")


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

# Game
enum GameState {
	GAME = 0,
	VOTING = 1
}

var current_game_state : GameState 

var game_length = 120000
var game_time_left = 0
var game_time_started = 0

var voting_time = 9000
var voting_time_left = voting_time
var voting_time_started = 0

const BOXY_MAP = preload("uid://cy1vv6ufvcajd")
const STARTER_SCENE = preload("uid://cumlkudt7n44b")
const LITTLE_TOWN = preload("uid://bnrna4agc6nqh")
const TDM_MAP = preload("uid://b1hytnu5uiwd5")







func _ready():
	var err = server.create_server(8081)
	if err != OK:
		push_error("Server failed: %s" % err)
		return
	
	get_tree().get_multiplayer().multiplayer_peer = server
	print("Server started on port 8081")
	get_tree().get_multiplayer().peer_connected.connect(on_peer_connected)
	get_tree().get_multiplayer().peer_disconnected.connect(on_peer_disconnected)
	
	start_new_game(TDM_MAP)
	stateBuffer.resize(BUFFER_SIZE)
	
	game_time_left = game_length
	game_time_started = Time.get_ticks_msec()
	#inputBuffer.resize(BUFFER_SIZE)
	
func start_new_game(map : PackedScene):
	game_time_left = game_length
	get_tree().call_deferred(&"change_scene_to_packed", map)
	
	get_tree().connect("scene_changed", Callable(self, "_on_scene_changed"))
	pass

func _on_scene_changed() -> void:
	#Initialize nodes
	initialize_nodes()
	
	#Reset and refill player array for new lobby
	player_array = []
	for peer_id in get_tree().get_multiplayer().get_peers():
		on_peer_connected(peer_id)
		
func game_end():
	get_tree().current_scene.queue_free()
	#Switching to Vote state
	current_game_state = GameState.VOTING
	start_voting()
	
	voting_time_started = Time.get_ticks_msec()
	voting_time_left = voting_time
	pass

# Voting State
func start_voting():
	for peer_id in get_tree().get_multiplayer().get_peers():
		rpc_id(peer_id, "on_new_game_state", "voting")
	pass

func voting_phase():
	voting_time_left = voting_time - (Time.get_ticks_msec() - voting_time_started)
	#print("time left"  + str(voting_time_left))
	if(voting_time_left <= 0):
		end_voting()
	pass
	
func end_voting():
	#Switching to game state
	current_game_state = GameState.GAME
	
	
	game_time_started = Time.get_ticks_msec()
	game_time_left = game_length
	
	start_new_game(TDM_MAP)
	pass
	

	
func initialize_nodes():
	world = get_tree().root.get_node("World")
	spawns = world.get_node("Spawns")



func _physics_process(delta: float):
	if current_game_state == GameState.GAME:
		game_time_left = game_length - (Time.get_ticks_msec() - game_time_started)
		print("time left"  + str(game_time_left))
		if(game_time_left <= 0):
			game_end()
		server.poll()
		HandleTick(delta)
		currentTick += 1
	elif current_game_state == GameState.VOTING:
		voting_phase()
	
func update_scoreboard():
	# Returns a sorter_peer_array containing peer_id in orders sorted by highest kills
	var peer_array: PackedInt32Array = get_tree().get_multiplayer().get_peers()
	var sorted_peer_array : PackedInt32Array
	for i in range(0, peer_array.size()):
		print(player_stats)
		var max : int = player_stats.get(peer_array[peer_array.size()-i-1]).kills
		var max_id : int = peer_array[peer_array.size()-i-1]
		for j in range(i, peer_array.size()):
			if max < player_stats.get(peer_array[j]).kills:
				max = player_stats.get(peer_array[j]).kills
				max_id = peer_array[j]
		peer_array.erase(max_id)
		sorted_peer_array.append(max_id)
	var scoreboard : Dictionary = { 
		
	}
	for i in range(0, sorted_peer_array.size()):
		
		scoreboard.get_or_add(player_names.get(sorted_peer_array[i]))
		print(sorted_peer_array[i])
		scoreboard.set("name", player_names.get(sorted_peer_array[i]))
		scoreboard.set("stats" , 
			{ 
				"kills" : player_stats[sorted_peer_array[i]].kills,
				"deaths" : player_stats[sorted_peer_array[i]].deaths
			}  
		)
	return scoreboard
		
func HandleTick(delta: float):
	
	# Store everyone's information into the dictionary
	var other_players_info : Dictionary
	for other_player_id in get_tree().get_multiplayer().get_peers():
			
		if !dead_players.has(other_player_id):
			other_players_info[other_player_id] = {
				"position"	: player_array[retrive_player_index(other_player_id)].instance.global_position,
				"rotation"	: player_array[retrive_player_index(other_player_id)].instance.global_rotation,
				"input" : {},
				"dead" : player_array[retrive_player_index(other_player_id)].instance.is_dead(),
				"stats" : {
					"position" : 0,
					"name" : "Player 0",
					"kills" : 0, 
					"deaths" : 0
				}
				}
				
			if player_array[retrive_player_index(other_player_id)].input_buffer.size() > 0: 
				if player_array[retrive_player_index(other_player_id)].input_buffer[0]:
					other_players_info[other_player_id].input = player_array[retrive_player_index(other_player_id)].input_buffer[0].input.key_input
		else: 
			other_players_info[other_player_id] = {
				"position"	: 0,
				"rotation"	: 0,
				"input" : {},
				"dead" : true
				}
			 
						
	for peer_id in get_tree().get_multiplayer().get_peers():
		if(dead_players.has(peer_id)):
			print("time since dead : " + str(Time.get_ticks_msec() - dead_players[peer_id].time_dead))
			if RESPAWN_TIME <= Time.get_ticks_msec() - dead_players[peer_id].time_dead:
				dead_players.erase(peer_id)
				on_peer_connected(peer_id)
				rpc_id(peer_id, "on_respawn")
			else:
				var state_pay_load : Dictionary
				state_pay_load.others = other_players_info
				rpc_id(peer_id, "on_server_movement_state", peer_id, state_pay_load)
				continue
			pass

		var player_state_dict : Dictionary = player_array[retrive_player_index(peer_id)]
		var player : Dictionary = player_array[retrive_player_index(peer_id)]	
		var player_fps_controller : FPSController = player.instance	
		
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
				"on_hit" : 0,
				"dead" : false
			},
			"others" : Dictionary()
		}
				
		#if player_fps_controller.is_dead() && !player_state_dict.time_dead:
			#player_state_dict.respawn_timer = RESPAWN_TIME
			#player_state_dict.time_dead = Time.get_ticks_msec()
			#on_peer_disconnected(peer_id)
			#dead_players[peer_id].time_dead = Time.get_ticks_msec()
			#rpc_id(peer_id, "on_dead")
		#for players in get_tree().get_multiplayer().get_peers(): 
			#if !dead_players.has(players):
				#var fpscontroller : FPSController = player_array[retrive_player_index(peer_id)].instance 
				#if fpscontroller.is_dead():
					#rpc_id(peer_id, "on_dead")
					#dead_players.get_or_add(peer_id)
					#dead_players[peer_id] = { "time_dead" : Time.get_ticks_msec()}
					#on_peer_disconnected(peer_id)
				
		#state_pay_load.self.dead = player_fps_controller.is_dead()
		#
		#if player_state_dict.respawn_timer <= Time.get_ticks_msec() - player_state_dict.time_dead && player_fps_controller.is_dead():
			#player_state_dict.time_dead = 0
			#var spawn_point : Node3D = spawns.get_child(randi_range(0,2))
			#player_fps_controller.set_curr_health(player_fps_controller.max_health)
			#player_fps_controller.global_position = spawn_point.global_position
			

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
				#player_fps_controller.handle_mouse_input(mouse_input_dict.offset.x, mouse_input_dict.offset.y)
				
				#Right now we are making this clinet authoritative for the sake of development progress
				if (player_fps_controller.global_position.distance_to(input_pay_load.input.position) > 0.000001):
					player_fps_controller.global_position = input_pay_load.input.position
					
					
				player_fps_controller.get_node("Head/Camera3D").rotation.x = input_pay_load.input.rotation.x
				player_fps_controller.rotation.y = input_pay_load.input.rotation.y
				print("player rotatoin at tick " + str(currentTick) + "is : " + str(player_fps_controller.get_node("Head/Camera3D").rotation.x) + ", " + str(player_fps_controller.rotation.y))
				
				# Move the player
				if !(!player_fps_controller.is_dead() && player_state_dict.time_dead != 0):
					pass
					
				#Update wish dir ( not used right now )
				player_movement.set_wish_dir(input_pay_load.input.wish_dir)
				print("wish dir is : " + str(input_pay_load.input.wish_dir))
				
				player_movement.Update(delta, key_input_dict, input_pay_load.input.wish_dir)

				player_weapon_manager.Update(delta, key_input_dict)
	
		
				# Store resulting data
				
				
				state_pay_load.tick = input_pay_load.tick
				# Self information
				state_pay_load.self.position = player_fps_controller.global_position
				state_pay_load.self.global_transform = player_fps_controller.global_transform
				state_pay_load.self.velocity = player_fps_controller.velocity
				state_pay_load.self.health = player_fps_controller.curr_health
				state_pay_load.self.combat = player_fps_controller.get_combat_report()
				print('reached x')
				
				var kills_in_tick = 0
				for killed_player in get_tree().get_multiplayer().get_peers(): 
					if !dead_players.has(killed_player):
						var fpscontroller : FPSController = player_array[retrive_player_index(peer_id)].instance 
						if fpscontroller.is_dead():
							kills_in_tick += 1
							player_stats[killed_player].deaths += 1
							rpc_id(peer_id, "on_dead")
							dead_players.get_or_add(peer_id)
							dead_players[peer_id] = { "time_dead" : Time.get_ticks_msec()}
							on_peer_disconnected(peer_id)
							
				player_stats[peer_id].kills = kills_in_tick + player_stats[peer_id].kills
				
				rpc_id(peer_id, "reliable_client_input", player_fps_controller.get_combat_report())
				
				#print("combat report : " + str(state_pay_load.self.combat) + " at tick " + str(state_pay_load.tick))
				player_fps_controller.set_combat_report([])

				# Information about other players
				state_pay_load.others = other_players_info
				player_state_buffer[bufferIndex] = state_pay_load
				
					
		if bufferIndex != -1:
			if(dead_players.has(peer_id)):
				print("DEAD PLAYER")
			rpc_id(peer_id, "on_server_movement_state", peer_id, player_state_buffer[bufferIndex])
			
		rpc_id(peer_id, "on_scoreboard_update", update_scoreboard())	
		
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
	rpc_id(id, "on_new_game_joined", "tdm")
	
	
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
 
@rpc("any_peer", "unreliable_ordered")
func on_connection_info(id: int, player_name : String):
	print("hellosoo")
	player_names[id] = player_name
	player_stats[id] = { 
		"kills" : 0,
		"deaths" : 0
	}
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

@rpc("authority", "unreliable_ordered")
func on_dead():
	pass
	
@rpc("authority", "unreliable_ordered")
func on_respawn():
	pass
	
@rpc("authority", "unreliable_ordered")	
func on_new_game_state(state : String):
	pass

@rpc("authority", "unreliable_ordered")	
func on_scoreboard_update(scoreboard : Dictionary):
	pass
