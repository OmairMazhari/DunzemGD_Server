#include "PlayerWeaponManager.h"

#include "Global.hpp"
#include "godot_cpp/classes/resource_loader.hpp"
#include <godot_cpp/classes/dir_access.hpp>
#include <godot_cpp/classes/animation_player.hpp>
#include <godot_cpp/classes/audio_stream_player3d.hpp>

using namespace godot;

PlayerWeaponManager::PlayerWeaponManager() {
    weapon_resource.resize(INVENTORY_SIZE);
        // Default value initialization 
        for(int i = 0; i < INVENTORY_SIZE; i++){
            weapon_resource[i] = Ref<WeaponResource>();
            // weapon_resource_path[i] = String("Path for weapon slot ");
        }
        // Initilization: this way we can push back into 0 - INVENTORY_SIZE types of gun arrays
        weapon_resource_list.resize(INVENTORY_SIZE);
}

PlayerWeaponManager::~PlayerWeaponManager() {

}

void PlayerWeaponManager::_process(double delta) {
    GAME_ONLY
    SETUP_INPUT
    
    if(current_weapon_resource.is_valid()){
        current_weapon_resource->Update(delta);
    }

    // Script
    if(input->is_action_just_pressed("primary")){
        change_state(Ref<WeaponResource>(Object::cast_to<WeaponResource>(weapon_resource[Primary])));
    } if(input->is_action_just_pressed("secondary")){
        change_state(Ref<WeaponResource>(Object::cast_to<WeaponResource>(weapon_resource[Secondary])));
    } if(input->is_action_just_pressed("melee")){
        change_state(Ref<WeaponResource>(Object::cast_to<WeaponResource>(weapon_resource[Melee])));
    }
    // update_view_model(current_weapon_resource);
    sway_and_update(current_weapon_resource->get_view_model_instance(), delta);
}

void PlayerWeaponManager::_ready() {
    GAME_ONLY
    // Set up scene tree variables
    view_model_container = get_node<Node3D>(view_model_container_path);

    // Load all the weapon reosource names into the weapon_resource_list variable
    for(int i = 0; i < INVENTORY_SIZE; i++){
        load_from_directory(weapon_resource_path[i], i);
        // Setup the all weapon view models
        setup(weapon_resource[i]); 
    }

    // Set the initial state
    change_state(Ref<WeaponResource>(Object::cast_to<WeaponResource>(weapon_resource[Primary])));

    // Print the loaded resource_list
    for(auto x : weapon_resource_list){
        UtilityFunctions::print("New weapon slot");
        for(auto y: x){
            UtilityFunctions::print(y);
        }
    }

}

void PlayerWeaponManager::_unhandled_input(const Ref<InputEvent> &event) {
     InputEventMouseMotion* mouseMotionEvent = Object::cast_to<InputEventMouseMotion>(event.ptr());
    if(mouseMotionEvent) {
        mouse_sway(current_weapon_resource->get_view_model_instance(), mouseMotionEvent);
    } 
}

void PlayerWeaponManager::set_resource(Ref<WeaponResource> resource, int slot) {
       weapon_resource[slot] = resource;
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

void PlayerWeaponManager::setup(Ref<WeaponResource> weapon) {
    if(weapon.is_valid()){
        weapon->setup(this);

        // // Weapon view model could be any node, node2d, 1d so we make sure that the root node is node3d before 
        // // using it, in the if statement prior we checked if view_model is assigned to anything at all first.
        // // We do this check only for the view model and not for container, because view model is a packed scene
        // // and the root is unknown but for container we explicitly defined it in code as a Node3D.
        // Ref<PackedScene> view_model_scene = weapon->get_view_model_scene();
        // Node3D* view_model = Object::cast_to<Node3D>(view_model_scene->instantiate());
        // weapon->set_view_model_instance(view_model);
        // if(view_model && view_model_container){
        //     // Setup view model
        //     view_model_container->add_child(view_model);
        //     view_model->set_position(weapon->get_view_model_position());
        //     view_model->set_rotation(weapon->get_view_model_rotation());
        //     view_model->set_scale(weapon->get_view_model_scale());
        //     // Setup animation
        //     weapon->set_anim_player(view_model->get_node<AnimationPlayer>("AnimationPlayer"));
        //     // Setup audio
        //     weapon->set_audio_stream_player(get_node<AudioStreamPlayer>("AudioStreamPlayer3D"));
        //     // Not visible initially
        //     view_model->set_visible(false);
        //     // Note: shared bullet ray_cast across all weapons
        //     weapon->set_bullet_ray_cast(bullet_ray_cast);
        //     weapon->set_owner(this);
    //     }
    // }
    }
}

void PlayerWeaponManager::update_view_model(Ref<WeaponResource> weapon) {
    Node3D* view_model = weapon->get_view_model_instance();
    view_model->set_position(weapon->get_view_model_position());
    view_model->set_rotation(weapon->get_view_model_rotation());
    view_model->set_scale(weapon->get_view_model_scale());
}

void PlayerWeaponManager::change_state(Ref<WeaponResource> new_weapon_resource) {
     if(new_weapon_resource == current_weapon_resource){
        UtilityFunctions::print("ERROR_CHANGE_STATE::weapon == current_weapon_resource");
        return;
    } if(new_weapon_resource.is_null()){ 
        UtilityFunctions::print("ERROR_CHANGE_STATE::weapon.is_null())");
        // ERROR NEW STATE EMPTY
        return;
    } if(current_weapon_resource.is_valid()){
        current_weapon_resource->Exit();
    }
    new_weapon_resource->Enter();
    current_weapon_resource = new_weapon_resource;
}

void PlayerWeaponManager::sway_and_update(Node3D* node, double delta) {
    Vector2 wish_dir = Input::get_singleton()->get_vector("left", "right", "up", "down").normalized();
     node->set_position(Vector3(
        Math::lerp((float)node->get_position().x, current_weapon_resource->get_view_model_position().x, (float)delta*5.0f),
        Math::lerp((float)node->get_position().y, current_weapon_resource->get_view_model_position().y, (float)delta*5.0f),
        current_weapon_resource->get_view_model_position().z
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
	// weapon_resource
    ClassDB::bind_method(D_METHOD("set_weapon_resource", "p_weapon_resource"), &PlayerWeaponManager::set_weapon_resource);
    ClassDB::bind_method(D_METHOD("get_weapon_resource"), &PlayerWeaponManager::get_weapon_resource);
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "Weapon Inventory Slots", PROPERTY_HINT_ARRAY_TYPE, "WeaponResource"), "set_weapon_resource", "get_weapon_resource");
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

}