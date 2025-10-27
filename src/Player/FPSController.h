#pragma once
#include <godot_cpp/classes/Node.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/sub_viewport.hpp>
#include <godot_cpp/classes/marker3d.hpp>

using namespace godot;

class FPSController : public CharacterBody3D {
    GDCLASS(FPSController, CharacterBody3D);

public:
    FPSController();
    ~FPSController();

    // Overriden functions
    void _ready() override;
	void _unhandled_input(const Ref<InputEvent> &p_event) override;
	void _process(double delta) override;
	void _physics_process(double delta) override;
    

    // Class functions
    void Update(double delta);
    void handle_mouse_input(int x_offset, int y_offset);
    void take_damage(int damage);
    void on_dead();

    bool get_hit();
    void set_hit(bool hit);
    bool is_dead();


    // Setters and Getters for Exported Variables
    void set_lookSensitivity(float p_lookSensitivity) { lookSensitivity = p_lookSensitivity; }
    float get_lookSensitivity() const { return lookSensitivity; }

    Camera3D* get_camera() { return camera; }
    CollisionShape3D* get_collision_shape() { return collision_shape; }

    int get_max_health() { return max_health; }
    void set_max_health(int p_val) {
         max_health = p_val;
         curr_health = max_health;
         }

    int get_curr_health() { return curr_health; }
    void set_curr_health(int p_val) { curr_health = p_val; }

    TypedArray<Dictionary> get_combat_report() { return combat_report; }
    void set_combat_report(TypedArray<Dictionary> p_val) { combat_report = p_val; }
    void add_combat_report(Dictionary p_val) {
        combat_report.push_back(p_val);
    }


protected:
    static void _bind_methods();
    
private:
    // Scene tree Variables
    Node* worldModel = nullptr;
    Camera3D *camera = nullptr;
    Node3D* head = nullptr;
    CollisionShape3D* collision_shape = nullptr;
    SubViewport* sub_viewport = nullptr;
    Marker3D* cameraAnchor = nullptr;
    
    // Class Variables
    Vector2 mouse_event;
    Vector2 input_rotation;
    Vector3 head_origin_position;
    bool on_hit = false;
    TypedArray<Dictionary> combat_report;

    // Exported Variables
    float lookSensitivity = 0.005;
    int max_health = 100;
    int curr_health = 100;
};