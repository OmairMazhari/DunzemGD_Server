#include "PlayerWeaponManager.h"

#include "Global.hpp"
#include "godot_cpp/classes/resource_loader.hpp"
#include <godot_cpp/classes/dir_access.hpp>
#include <godot_cpp/classes/animation_player.hpp>
#include <godot_cpp/classes/audio_stream_player3d.hpp>

using namespace godot;

PlayerWeaponManager::PlayerWeaponManager() {
}

PlayerWeaponManager::~PlayerWeaponManager() {

}

void PlayerWeaponManager::Update(double delta, Dictionary input) {
    GAME_ONLY

    this->input = input;
    Dictionary event_input = input["event_based_actions"];
    
    // Script
    if(event_input["primary"]){
        change_state(Ref<WeaponResource>(Object::cast_to<WeaponResource>(states[Primary])));
    } if(event_input["secondary"]){
        change_state(Ref<WeaponResource>(Object::cast_to<WeaponResource>(states[Secondary])));
    } if(event_input["melee"]){
        change_state(Ref<WeaponResource>(Object::cast_to<WeaponResource>(states[Melee])));
    }

    // Cast current_weapon
    Ref<WeaponResource> current_weapon = current_state;

    current_weapon->set_input(input);

    if(current_weapon.is_null()) {
        UtilityFunctions::print("ERROR::WEAPON_NULL");
        return;
    }
    // Update current_state
    ResourceFSM::Update(delta);
   
    // Sway current weapon
    sway_and_update(current_weapon->get_view_model_instance(), delta);
}

void PlayerWeaponManager::_process(double delta) {
}

void PlayerWeaponManager::_ready() {
	GAME_ONLY
    // ResourceFSM::_ready();
    
    // Set up scene tree variablesz
    view_model_container = get_node<Node3D>(view_model_container_path);

    // Setup all the weapon states
    for(int i = 0; i < states.size(); i++){
        states[i] = states[i].duplicate();
        Ref<WeaponResource> current_weapon = Ref<WeaponResource>(Object::cast_to<WeaponResource>(states[i]));
        current_weapon->Setup(this);
    }

    // // Set the initial state
    change_state(Ref<WeaponResource>(Object::cast_to<WeaponResource>(states[Primary])));
}

void PlayerWeaponManager::_unhandled_input(const Ref<InputEvent> &event) {
     InputEventMouseMotion* mouseMotionEvent = Object::cast_to<InputEventMouseMotion>(event.ptr());
    if(mouseMotionEvent) {
        mouse_sway(((Ref<WeaponResource>)current_state)->get_view_model_instance(), mouseMotionEvent);
    } 
}

void PlayerWeaponManager::load_from_directory(const String& weapon_resource_dir, int weapon_index) {
    Ref<DirAccess> dir = DirAccess::open(weapon_resource_dir);
    if(dir.is_null()) {
        UtilityFunctions::print("Unable to open weapon resource directory : " + weapon_resource_dir);
    }
    dir->list_dir_begin();
    String file_name = dir->get_next();
    while(!file_name.is_empty()){
        if(!dir->current_is_dir() && file_name.ends_with(".tres")){
            String full_path = weapon_resource_dir + file_name;
            Ref<WeaponResource> resource = ResourceLoader::get_singleton()->load(full_path);
            // Store all weapon types properly in their respective indexed arrays
            if(resource.is_valid()){
                weapon_resource_list[weapon_index].push_back(full_path);
            }
        }
        file_name = dir->get_next();
    }
    dir->list_dir_end();
}

void PlayerWeaponManager::update_view_model(Ref<WeaponResource> weapon) {
    Node3D* view_model = weapon->get_view_model_instance();
    view_model->set_position(weapon->get_view_model_position());
    view_model->set_rotation(weapon->get_view_model_rotation());
    view_model->set_scale(weapon->get_view_model_scale());
}

// void PlayerWeaponManager::change_state(Ref<WeaponResource> new_weapon_resource) {
//      if(new_weapon_resource == current_weapon_resource){
//         UtilityFunctions::print("ERROR_CHANGE_STATE::weapon == current_weapon_resource");
//         return;
//     } if(new_weapon_resource.is_null()){ 
//         UtilityFunctions::print("ERROR_CHANGE_STATE::weapon.is_null())");
//         // ERROR NEW STATE EMPTY
//         return;
//     } if(current_weapon_resource.is_valid()){
//         current_weapon_resource->Exit();
//     }
//     new_weapon_resource->Enter();
//     current_weapon_resource = new_weapon_resource;
// }

void PlayerWeaponManager::sway_and_update(Node3D* node, double delta) {
    Vector2 wish_dir = Input::get_singleton()->get_vector("left", "right", "up", "down").normalized();
     node->set_position(Vector3(
        Math::lerp((float)node->get_position().x, ((Ref<WeaponResource>)current_state)->get_view_model_position().x, (float)delta*5.0f),
        Math::lerp((float)node->get_position().y, ((Ref<WeaponResource>)current_state)->get_view_model_position().y, (float)delta*5.0f),
        ((Ref<WeaponResource>)current_state)->get_view_model_position().z
    ));
    node->set_position(node->get_position() + Vector3(wish_dir.x, wish_dir.y, 0.0f) * move_sway_factor * 0.0005);
}

void PlayerWeaponManager::mouse_sway(Node3D *node, InputEventMouseMotion* mouseMotionEvent) { 
    node->set_position(Vector3(
            node->get_position().x + mouseMotionEvent->get_relative().x * 0.0005f * look_sway_factor,
            node->get_position().y + mouseMotionEvent->get_relative().y * 0.0005f * look_sway_factor,
            node->get_position().z
        ));
}

void PlayerWeaponManager::_bind_methods() {
    // weapon_resource_path
    ClassDB::bind_method(D_METHOD("set_weapon_resource_path", "p_weapon_resource_path"), &PlayerWeaponManager::set_weapon_resource_path);
    ClassDB::bind_method(D_METHOD("get_weapon_resource_path"), &PlayerWeaponManager::get_weapon_resource_path);
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "Path for automatic wepaon loading", PROPERTY_HINT_ARRAY_TYPE, "StringName"), "set_weapon_resource_path", "get_weapon_resource_path");
    // view_model_container_path
    ClassDB::bind_method(D_METHOD("set_view_model_container_path", "p_view_model_container_path"), &PlayerWeaponManager::set_view_model_container_path);
    ClassDB::bind_method(D_METHOD("get_view_model_container_path"), &PlayerWeaponManager::get_view_model_container_path);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "view_model_container_path"), "set_view_model_container_path", "get_view_model_container_path");
    // bullet_ray_cast
    ClassDB::bind_method(D_METHOD("set_bullet_ray_cast", "p_bullet_ray_cast"), &PlayerWeaponManager::set_bullet_ray_cast);
    ClassDB::bind_method(D_METHOD("get_bullet_ray_cast"), &PlayerWeaponManager::get_bullet_ray_cast);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "bullet_ray_cast", PROPERTY_HINT_NODE_TYPE, "RayCast3D"), "set_bullet_ray_cast", "get_bullet_ray_cast");
    // look_sway_factor
    ClassDB::bind_method(D_METHOD("set_look_sway_factor", "p_look_sway_factor"), &PlayerWeaponManager::set_look_sway_factor);
    ClassDB::bind_method(D_METHOD("get_look_sway_factor"), &PlayerWeaponManager::get_look_sway_factor);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "look_sway_factor"), "set_look_sway_factor", "get_look_sway_factor");
    // move_sway_factor
    ClassDB::bind_method(D_METHOD("set_move_sway_factor", "p_move_sway_factor"), &PlayerWeaponManager::set_move_sway_factor);
    ClassDB::bind_method(D_METHOD("get_move_sway_factor"), &PlayerWeaponManager::get_move_sway_factor);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "move_sway_factor"), "set_move_sway_factor", "get_move_sway_factor");
     // Update function
    ClassDB::bind_method(D_METHOD("Update", "delta"), &PlayerWeaponManager::Update);

}