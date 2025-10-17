#include "PlayerMovementFSM.h"

#include <godot_cpp/classes/input.hpp>
#include "utils.hpp"

using namespace godot;

PlayerMovementFSM::PlayerMovementFSM() {}

PlayerMovementFSM::~PlayerMovementFSM() {}

void PlayerMovementFSM::_process(double delta) { 

}

void PlayerMovementFSM::_physics_process(double delta) { 
    
}

void PlayerMovementFSM::Update(double delta, Dictionary input_dict) {
    player->set_input_dict(input_dict);
    this->input_dict = input_dict;
    if(current_state){
        current_state->Update(delta);
    }
    if(player) {
        // left right up down etc are state_based_actions 

        Vector2 input_dir = utils::get_custom_vector(input_dict["state_based_actions"], "left", "right", "up", "down");
        wish_dir = player->get_global_transform().basis.xform(Vector3(input_dir.x, 0, input_dir.y));
        cam_aligned_wish_dir = player->get_camera()->get_global_transform().basis.xform(Vector3(input_dir.x, 0, input_dir.y));
        // UtilityFunctions::print("Wish Dir: " + wish_dir);
        // UtilityFunctions::print("Cam Wish Dir: " + cam_aligned_wish_dir);
        // UtilityFunctions::print("Input Dir: "  + input_dir);
        // UtilityFunctions::print("Camera Transform: "  + player->get_camera()->get_global_transform().basis);
        // UtilityFunctions::print("Player Transform: "  + player->get_global_transform().basis);
    }
}



void PlayerMovementFSM::_bind_methods() {
    // player
    ClassDB::bind_method(D_METHOD("set_player", "p_player"), &PlayerMovementFSM::set_player);
    ClassDB::bind_method(D_METHOD("get_player"), &PlayerMovementFSM::get_player);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "player", PROPERTY_HINT_NODE_TYPE, "FPSController"), "set_player", "get_player");
    // stats
    ClassDB::bind_method(D_METHOD("set_stats", "p_stats"), &PlayerMovementFSM::set_stats);
    ClassDB::bind_method(D_METHOD("get_stats"), &PlayerMovementFSM::get_stats);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "stats", PROPERTY_HINT_RESOURCE_TYPE, "PlayerMovementStatsResource"), "set_stats", "get_stats");
    // Update function
    ClassDB::bind_method(D_METHOD("Update", "delta", "input_dict"), &PlayerMovementFSM::Update);
    // get_wish_dir()
        ClassDB::bind_method(D_METHOD("get_wish_dir"), &PlayerMovementFSM::get_wish_dir);

}