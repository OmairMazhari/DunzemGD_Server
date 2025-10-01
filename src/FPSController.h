#pragma once
#include <godot_cpp/classes/Node.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/sub_viewport.hpp>

using namespace godot;

class FPSController : public CharacterBody3D {
    GDCLASS(FPSController, CharacterBody3D);

public:
    FPSController();
    ~FPSController();

    // Overriden functions
    void _ready() override;
	void _process(double delta) override;
	void _physics_process(double delta) override;

    // Class functions
    Dictionary get_input_dict() const;
    void set_input_dict(Dictionary dict);
    void handle_mouse_input(float x_offset, float y_offset);

    // Setters and Getters for Exported Variables
    void set_lookSensitivity(float p_lookSensitivity) { lookSensitivity = p_lookSensitivity; }
    float get_lookSensitivity() const { return lookSensitivity; }

    Camera3D* get_camera() { return camera; }
    CollisionShape3D* get_collision_shape() { return collision_shape; }

protected:
    static void _bind_methods();
    
private:
    // Scene tree Variables
    Node* worldModel = nullptr;
    Camera3D *camera = nullptr;
    CollisionShape3D* collision_shape = nullptr;
    SubViewport* sub_viewport = nullptr;

    // Exported Variables
    float lookSensitivity = 0.005;

    // Class Variables
    Dictionary input_dict;
};