#include "PlayerMovementFSM.h"
#include "godot_cpp/classes/input.hpp"
using namespace godot;

PlayerMovementFSM::PlayerMovementFSM() {}

PlayerMovementFSM::~PlayerMovementFSM() {}

void PlayerMovementFSM::_process(double delta) {
    if(current_state){
        current_state->Update(delta);
    }
    if(player) {
        Vector2 input_dir = Input::get_singleton()->get_vector("left", "right", "up", "down").normalized();
        wish_dir = player->get_global_transform().basis.xform(Vector3(input_dir.x, 0, input_dir.y));
        cam_aligned_wish_dir = player->get_camera()->get_global_transform().basis.xform(Vector3(input_dir.x, 0, input_dir.y));
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
}