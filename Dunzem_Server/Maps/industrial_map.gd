extends Node3D

var player_scene: PackedScene = preload("res://FPSController/Template_FPSController.tscn")
var player_list: Dictionary = {} # id -> Node3D

func add_player(id: int, playerPos: Vector3):
	if not player_list.has(id):
		var player_instance: Node3D = player_scene.instantiate()
		player_instance.name = str(id)
		add_child(player_instance)
		player_instance.global_position = playerPos
		player_list[id] = player_instance
	else:
		player_list[id].global_position = playerPos

func remove_player(id: int):
	if player_list.has(id):
		player_list[id].queue_free()
		player_list.erase(id)
