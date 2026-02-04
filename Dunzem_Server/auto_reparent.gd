@tool
extends EditorScript

func _run():
	var root := get_scene().get_tree().edited_scene_root
	if root == null:
		push_error("Open the scene first.")
		return

	# Path to where your PhysicalBone3D nodes live
	var bones_root := root.get_node("Y Bot/Skeleton3D/PhysicalBoneSimulator3D")
	if bones_root == null:
		push_error("Couldn't find PhysicalBoneSimulator3D")
		return

	for bone in bones_root.get_children():
		if not bone is PhysicalBone3D:
			continue

		var area := bone.get_node_or_null("Area3D")
		if area == null:
			continue  # you said areas already exist

		var n = bone.name.to_lower()

		if n.match("*head*") or n.match("*neck*"):
			area.add_to_group("head")
		elif n.match("*arm*") or n.match("*shoulder*") or n.match("*hand*") or n.match("*spine*") or n.match("*hips*"):
			area.add_to_group("body")
		elif n.match("*leg*") or n.match("*foot*"):
			area.add_to_group("leg")

	print("Finished assigning body groups.")


func assign_body_group(bone: PhysicalBone3D, area: Area3D):
	var name := bone.name.to_lower()

	# --------------------
	#   HEAD GROUP
	# --------------------
	if "head" in name or "neck" in name:
		area.add_to_group("head")
		print("HEAD:", bone.name)
		return

	# --------------------
	#   BODY GROUP
	# --------------------
	if (
		"spine" in name
		or "hips" in name
		or "shoulder" in name
		or "arm" in name
		or "hand" in name
		or "chest" in name
	):
		area.add_to_group("body")
		print("BODY:", bone.name)
		return

	# --------------------
	#   LEG GROUP
	# --------------------
	if (
		"leg" in name
		or "up_leg" in name
		or "thigh" in name
		or "foot" in name
	):
		area.add_to_group("leg")
		print("LEG:", bone.name)
		return
