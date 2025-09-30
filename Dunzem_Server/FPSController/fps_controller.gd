extends FPSController


const SPEED = 5.0
const JUMP_VELOCITY = 4.5


func _physics_process(delta: float) -> void:
	set_input_dict({"w": 0})
	print(get_input_dict())
