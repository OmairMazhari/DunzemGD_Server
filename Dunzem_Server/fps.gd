extends Label


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	# Get current FPS
	var fps = Engine.get_frames_per_second()
	# Update the label text
	text = "FPS: %d" % fps
