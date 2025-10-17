#pragma once

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/node.hpp>
#include "godot_cpp/classes/ray_cast3d.hpp"
#include "godot_cpp/classes/input_event_mouse_motion.hpp"
#include "godot_cpp/classes/input_event.hpp"
#include "godot_cpp/classes/label.hpp"

#include "WeaponResource.h"

#include <FSM/ResourceFSM/FSM.h>

using namespace godot;

class WeaponResource;

class PlayerWeaponManager : public ResourceFSM {
    GDCLASS(PlayerWeaponManager, ResourceFSM);
public:
    PlayerWeaponManager();
    ~PlayerWeaponManager();

    // Overriden functions
    void _process(double delta) override;
    void _ready() override;
    void _unhandled_input(const Ref<InputEvent> &p_event) override;
    
    // Class functions
    void Update(double delta, Dictionary input);
    void load_from_directory(const String& weapon_resource_dir, int weapon_index);
	void update_view_model(Ref<WeaponResource> weapon);
    void sway_and_update(Node3D *node, double delta);
	void mouse_sway(Node3D *node, InputEventMouseMotion *mouseMotionEvent);

    enum Weapon {
        Primary = 0, 
        Secondary = 1,
        Melee = 2
    };

private:
    // Export Variables
    TypedArray<StringName> weapon_resource_path;
    String view_model_container_path;
    String world_model_container_path; // remember to export this

    RayCast3D* bullet_ray_cast = nullptr;

    // Class Variables
    std::vector<std::vector<String>> weapon_resource_list;
    static constexpr float INVENTORY_SIZE = 3;
    Node3D* view_model_container = nullptr;

    float look_sway_factor = 1.0f;
    float move_sway_factor = 1.0f;

    Dictionary input;

public:
    // Setters and getters
    void set_weapon_resource_path(const TypedArray<StringName> p_weapon_resource_path) {
        weapon_resource_path = p_weapon_resource_path; 
        weapon_resource_path.resize(INVENTORY_SIZE);
    }
    TypedArray<StringName> get_weapon_resource_path() const { 
        return weapon_resource_path; 
    }

    void set_bullet_ray_cast(RayCast3D* p_bullet_ray_cast) { bullet_ray_cast = p_bullet_ray_cast; }
    RayCast3D* get_bullet_ray_cast() const { return bullet_ray_cast; }

    void set_view_model_container_path(String p_view_model_container_path) { view_model_container_path = p_view_model_container_path; }
    String get_view_model_container_path() const { return view_model_container_path; }
    
    void set_view_model_container(Node3D* p_view_model_container) { view_model_container = p_view_model_container; }
    Node3D* get_view_model_container() const { return view_model_container; }

    void set_look_sway_factor(float p_look_sway_factor) { look_sway_factor = p_look_sway_factor; }
    float get_look_sway_factor() const { return look_sway_factor; }

    void set_move_sway_factor(float p_move_sway_factor) { move_sway_factor = p_move_sway_factor; }
    float get_move_sway_factor() const { return move_sway_factor; }

protected:
	
	static void _bind_methods();
};