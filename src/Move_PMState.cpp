#include "Move_PMState.h"
#include "godot_cpp/classes/input.hpp"
#include "godot_cpp/classes/scene_tree.hpp"
#include "godot_cpp/classes/node.hpp"
#include "godot_cpp/classes/engine.hpp"



void Move_PMState::Enter() {
    
   
}

void Move_PMState::Exit() {
}

void Move_PMState::Update(double delta) {
    Dictionary client_input = player->get_input_dict();

    if(client_input["no_clip"]){
        FSM->change_state(this, "noclip_pmstate");
    }

    if(player->is_on_floor()){
        if(client_input["jump"] || (stats->get_auto_bhop() && client_input["jump"])){
            player->set_velocity(Vector3(
                player->get_velocity().x,
                stats->get_jump_velocity(),
                player->get_velocity().z
            ));
        }
        _handle_ground_physics(delta);
    } else {
        _handle_air_physics(delta);
    }
    player->move_and_slide();
}

void Move_PMState::_handle_air_physics(double delta) {
    player->set_velocity(Vector3(
        player->get_velocity().x,
        player->get_velocity().y - (stats->get_gravity() * delta),
        player->get_velocity().z
    ));
    float curr_speed_in_wish_dir = player->get_velocity().dot(FSM->get_wish_dir());
    float capped_speed = UtilityFunctions::min((stats->get_air_move_speed() * FSM->get_wish_dir()).length(), stats->get_air_cap());
    float add_speed_till_cap = capped_speed - curr_speed_in_wish_dir;
    if(add_speed_till_cap > 0){
        float accel_speed = stats->get_air_accel() * stats->get_air_move_speed() * delta;
        accel_speed = UtilityFunctions::min(accel_speed, add_speed_till_cap);
        player->set_velocity(player->get_velocity() + (accel_speed * FSM->get_wish_dir()));
    }
}

void Move_PMState::_handle_ground_physics(double delta) {
   
    // Current speed in desired direction
    float curr_speed_in_wish_dir = player->get_velocity().dot(FSM->get_wish_dir());
    // Amount of speed we can still add
    float add_speed_till_cap = get_move_speed() - curr_speed_in_wish_dir;
    if (add_speed_till_cap > 0.0f) {
        float accel_speed = stats->get_ground_accel() * (float)delta * get_move_speed();
        accel_speed = MIN(accel_speed, add_speed_till_cap);
        player->set_velocity(player->get_velocity() + (accel_speed * FSM->get_wish_dir()));
    }
     float player_velocity_length = player->get_velocity().length();
    // Apply friction
    float control = MAX(player_velocity_length, stats->get_ground_decel());
    float drop = control * stats->get_ground_friction() * (float)delta;
    float new_speed = MAX(player_velocity_length - drop, 0.0f);
    if (player_velocity_length > 0.0f) {
        new_speed /= player_velocity_length;
    }
    player->set_velocity(player->get_velocity() * new_speed);
}



void Move_PMState::_bind_methods(){

}