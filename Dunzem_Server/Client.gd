#extends Node
#
#var client := WebSocketMultiplayerPeer.new()
#var remote_players := {} # peer_id -> Node3D
#var fps_controller_scene := preload("res://FPSController/Template_FPSController.tscn")
#var player: FPSController 
#var playerMovement: PlayerMovementFSM
#var camera: Camera3D 
#var head: Node3D 
#
#
#
## Client Prediction
#var timer: float = 0 
#var currentTick: int = 0
#var minTimeBetweenTicks: float
#
#const SERVER_TICK_RATE: float  = 30
#
#
#const BUFFER_SIZE: int  = 1024
#
#var stateBuffer: Array = []
#var inputBuffer: Array = []
#var latestServerState: Dictionary
#var lastProcessedState: Dictionary
#
#var input_dict := {
	#"state_based_actions": {
		#"up" : false, "down" : false, "left" : false, "right" : false
	#},
	#"event_based_actions": {
		#"no_clip" : false, "jump" : false
	#}
#}
#
#var InputPayLoad: Dictionary  = {
	#"tick" : 2,
	#"input" : input_dict,
	#"player_position" : Vector3.ZERO,
	#"player_basis" : Vector3.ZERO,
	#"cam_position" :  Vector3.ZERO,
	#"cam_basis" : Vector3.ZERO,
	#"mouse" : Vector2.ZERO
#}
#
#var StatePayLoad: Dictionary  = {
	#"tick" : 2,
	#"position" : Vector3.ZERO,
	#"velocity" : Vector3.ZERO
#}
#
#func _ready(): 
	#print("READY READY READY")
	#var err = client.create_client("ws://localhost:8081")
	#
	##var err = client.create_client("tkdhe-31-167-27-97.a.free.pinggy.link:36877")
#
	#if err != OK:
		#push_error("Client failed: %s" % err)
		#print("CLIENT FAILED")
		#return
	#get_tree().get_multiplayer().multiplayer_peer = client
	#get_tree().get_multiplayer().connected_to_server.connect(_on_connected)
	#
	#player = get_tree().root.get_node("World/Player")
	#camera = get_tree().root.get_node("World/Player/Head/Camera3D")
	#head = get_tree().root.get_node("World/Player/Head")
	#print("PRINTINC CHILDREN" + str(player.get_children()))
	#playerMovement  = player.get_node("PlayerMovementFSM")
	#
	## Client Prediction
	#minTimeBetweenTicks = 1 / SERVER_TICK_RATE
	#
	#stateBuffer.resize(BUFFER_SIZE)
	#inputBuffer.resize(BUFFER_SIZE)
#
	#
	#
##func _physics_process(delta: float) -> void:
	##player.move_and_slide()
##func _process(delta: float):
	##pass
	#
#func _physics_process(delta: float) -> void:
	#playerMovement.set_process(false)
	#playerMovement.set_physics_process(false)
	#player.set_process(false)
	#player.set_physics_process(false)
	#
	#client.poll()
	## Update input
	#input_dict["state_based_actions"] = {
		#"up" : Input.is_action_pressed("up"),
		#"down" : Input.is_action_pressed("down"),
		#"left" : Input.is_action_pressed("left"),
		#"right" : Input.is_action_pressed("right")
	#}
	#
	#input_dict["event_based_actions"] = {
		#"no_clip" : input_dict.event_based_actions["no_clip"] || Input.is_action_just_pressed("no_clip"), 
		#"jump" :  input_dict.event_based_actions["jump"] || Input.is_action_just_pressed("jump")
	#}
	## Client Prediction
	#
	#timer += delta
	#while timer >= minTimeBetweenTicks:
		#timer -= minTimeBetweenTicks
		#HandleTick()
		#currentTick += 1
		## Reset state based actions after it handling it
		#input_dict["unhandled_input"] = false
		#input_dict.event_based_actions = {
			#"no_clip" : false, 
			#"jump" : false
		#}
#
#func _process(delta: float) -> void:
	#pass
	##player.rotation.y = round(player.rotation.y * 1000.0) / 1000.0
	##player.get_node("Head/Camera3D").rotation.x = round(player.get_node("Head/Camera3D").rotation.y * 1000.0) / 1000.0
	##pass
	##print("yes ss")
	##var fps = Engine.get_frames_per_second()
	##var lerp_interval = playerMovement.get_wish_dir() / fps
	##var lerp_position = player.global_transform.origin + lerp_interval
	##
	##var playerMesh = player.get_node("WorldModel");
##
	##if fps > 30:
		##player.top_level = (true)
		##player.global_transform.origin = player.global_transform.origin.lerp(lerp_position, 20 * delta)
	##else:
		###playerMesh.global_transform = global_transform
		##player.top_level = (false)
	#
	#
	#
	##if(client.get_connection_status() == WebSocketMultiplayerPeer.CONNECTION_CONNECTED):
		##rpc_id(1, "update_input", input_dict)
	##if(client.get_connection_status() == WebSocketMultiplayerPeer.CONNECTION_CONNECTED):
		##rpc_id(1, "get_world_state")
		#
	## Make an input_dict of what keys the client is pressing
	#
#var last_pos: Vector3 = Vector3.ZERO
#func HandleTick():
#
	#if lastProcessedState != latestServerState: 
		#HandleServerReconcilliation()
		#pass
	#
	#var buffer_index: int = currentTick % BUFFER_SIZE
	#var input_pay_load: Dictionary = InputPayLoad.duplicate()
	#input_pay_load.tick = currentTick
	#input_pay_load.input = input_dict.duplicate()
	#
	##Update player movement
	#
	#
#
	##wtick pos" + str(stateBuffer[buffer_index-1].tick) + " Player position: " + str(stateBuffer[buffer_index-1].position)) 
#
	#
	#if(latestServerState):
		#print("Tick Difference" + str(latestServerState.tick - currentTick))
	##input_pay_load["rotation"] = player.global_transform.basis
	##var camera = player.get_node("Head")
	##input_pay_load["cam_rotation"] = camera.global_transform.basis
	##var temp_player_pos = player.position
	##input_pay_load["rotation"] = mouse_input.x
	##input_pay_load["cam_rotation"] = mouse_input.y
	##print("Current Tick: " + str(currentTick) + " Player rotation: " + str(player.global_transform.basis))
	#
	#print("Current Tick: " + str(currentTick) + " Camera  Basis: " + str(camera.global_transform.basis))
	#
	#
	##print("Current tick" + str(input_pay_load.tick) + " Player position: " + str(player.position) + "Player rotation" + str(input_pay_load.rotation) + " Cam Rotation" + str(input_pay_load.rotation))
	##Input.action_press("right")
	#
	##player.position = Vector3.ZERO
	##print("TICK:  " + str(currentTick) + "   //////////////////////////////////////")
	##input_pay_load["player_position"] = player.position
	##input_pay_load["player_basis"] = player.global_transform
	##
	##input_pay_load["player_rotation"] = player.rotation
	##
	##input_pay_load["cam_position"] = camera.global_position
	##input_pay_load["cam_basis"] = camera.transform
	##input_pay_load["cam_rotation"] = camera.global_rotation
	##
	##input_pay_load["head_basis"] = head.get_global_transform().basis
	#
	#input_pay_load["mouse"] = mouse_input
	#mouse_input = Vector2.ZERO
	#playerMovement.Update(minTimeBetweenTicks, input_pay_load.input)
	#
	##print(player.position)
	##if last_pos.distance_to(player.position) != 0:
		##print("UnDeterministic " +  "last_pos: " + str(last_pos) + " current_pos : " + str(player.position) + " error : " + str(last_pos.distance_to(player.position)))
	##last_pos = player.position
	#
	##Input.action_release("right")
	##print(" Position After " +  str(player.position))	
	#inputBuffer[buffer_index] = input_pay_load
	#
	#var state_pay_load: Dictionary = StatePayLoad.duplicate()
	#state_pay_load.tick = currentTick
	#
	#
	#
	#state_pay_load.position = player.position
	#stateBuffer[buffer_index] = state_pay_load
	##print("State Buffer" + str(stateBuffer))
	##print("State Buffer" + str(stateBuffer))
	#
	## send to server
	#rpc_id(1, "on_client_input", input_pay_load)
	#
	#pass
	#
#
#func HandleServerReconcilliation():
	#lastProcessedState = latestServerState.duplicate()
	#var serverStateBufferIndex: int = latestServerState.tick % BUFFER_SIZE
	#var position: Vector3 = latestServerState.player_position
	##print("ServerStateBufferIndex: " + str(serverStateBufferIndex))
	##print("Server tick : " + str(latestServerState.tick) +  " Client tick : " + str(stateBuffer[serverStateBufferIndex].tick))
	#
	#var positionError: float = position.distance_to(stateBuffer[serverStateBufferIndex].position)
#
	#
	#print("position error"  + str(positionError))
	##print(position.distance_to(stateBuffer[serverStateBufferIndex].position))
	#if(positionError  > 0.0001):
		##Rewind & Replay
		#print("we have to reconcile bro, error is " + str(positionError) + " at tick " + str(latestServerState.tick))
		#
		#
		#player.global_transform = latestServerState.player_transform # position 
		#player.velocity = latestServerState.velocity
		#
		##player.global_transform.basis = latestServerState.player_basis # basis
		#
		##player.get_node("Head/Camera3D").global_transform.basis = latestServerState.cam_basis
		##player.get_node("Head/Camera3D").position = latestServerState.cam_pos
		##Update buffer at index of latest server state
		#stateBuffer[serverStateBufferIndex] = latestServerState
#
		#var tickToProcess: int = latestServerState.tick + 1;
		#
		#while tickToProcess < currentTick:
			#var bufferIndex = tickToProcess % BUFFER_SIZE
			#var state_pay_load: Dictionary = StatePayLoad.duplicate()
			## Process new movement with reconcilled state
			#
			#playerMovement.Update(minTimeBetweenTicks, inputBuffer[bufferIndex]["input"])
			#state_pay_load.tick = tickToProcess
			#state_pay_load.position = player.position
			#
			#stateBuffer[bufferIndex] = state_pay_load
			#tickToProcess += 1
			#pass
	#pass
#
#func quantize_position(pos: Vector3) -> Vector3:
	#return Vector3(snappedf(pos.x, 0.00000000000001), snappedf(pos.y, 0.00000000000001), snappedf(pos.z, 0.00000000000001))
	#
#var mouse_input: Vector2
#func _unhandled_input(event: InputEvent) -> void:
	#Input.mouse_mode = Input.MOUSE_MODE_CAPTURED
	#
	#if event is InputEventMouseMotion:
		#player.handle_mouse_input(event.get_relative().x, event.get_relative().y)
		#mouse_input.x += event.get_relative().x
		#mouse_input.y += event.get_relative().y
		##print(mouse_input)
	#pass
	#
#func _on_connected():
	#print("Connected to server!")
#
#@rpc("authority", "unreliable_ordered")
#func on_server_movement_state(peer_id: int, state_pay_load: Dictionary):
	#latestServerState = state_pay_load
	#pass
	#
#@rpc("any_peer", "unreliable_ordered")
#func handle_mouse(x, y):
	#pass
	#
## Server → Client
#@rpc("authority", "unreliable_ordered")
#func set_remote_position(peer_id: int, pos: Vector3):
	#if not remote_players.has(peer_id):
		#var remote_player = fps_controller_scene.instantiate()
		#remote_player.name = str(peer_id)
		#get_node("../World").add_child(remote_player)
		#remote_players[peer_id] = remote_player
	#remote_players[peer_id].global_transform.origin = pos
#
#@rpc("authority", "unreliable_ordered")
#func set_self_position(peer_id: int, pos: Vector3, rot_y: Vector3, rot_x: Vector3):
	#player.global_position = pos
	#player.rotation = rot_y
	#player.get_node("Head/Camera3D").rotation = rot_x
#
## Declared here too, but only server actually uses it
#@rpc("any_peer", "unreliable_ordered")
#func update_position(pos: Vector3):
	#pass
	#
#@rpc("any_peer", "unreliable_ordered")
#func update_input(input_dict: Dictionary):
	#pass
	#
#
	#
#@rpc("any_peer", "unreliable_ordered")
#func get_world_state(pos: Vector3):
	#pass
	#
	#
#@rpc("any_peer", "unreliable_ordered")
#func on_client_input(peer_id: int, input_pay_load: Dictionary):
	#pass
