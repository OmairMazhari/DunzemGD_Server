#include "Move_PMState.h"

#include <godot_cpp/classes/input.hpp>
#include "utils.hpp"

void Move_PMState::Setup(ResourceFSM* FSM) {
    PlayerMovementState::Setup(FSM);
     // Character animation nodes
    characterAnimationTree = this->FSM->get_characterAnimationTree();
    characterAnimationPlayback = this->FSM->get_characterAnimationPlayback();
}

void Move_PMState::Enter() {
   
}

void Move_PMState::Exit() {
}

void Move_PMState::Update(double delta) {
    Dictionary event_input = input["event_based_actions"];
     
    // Calculate wish_dir
    Vector2 input_dir = utils::get_custom_vector(input["state_based_actions"], "left", "right", "up", "down");
    wish_dir =  player->get_global_transform().basis.xform(Vector3(input_dir.x, 0, input_dir.y));   
    // wish_dir = FSM->get_wish_dir();
    
    UtilityFunctions::print("SERver wish dir is " + UtilityFunctions::str(wish_dir));
    Vector3 rel_vel = player->get_global_basis().inverse().xform((player->get_velocity() * Vector3(1,0,1)) / get_move_speed());
    Vector2 rel_vel_xz = Vector2(rel_vel.x, rel_vel.z);

    characterAnimationPlayback->travel("RunBlendSpace2D");
    characterAnimationTree->set("parameters/RunBlendSpace2D/blend_position", rel_vel_xz);

    // Move based on input
    if(event_input["no_clip"]){
        FSM->change_state(Ref<ResourceState>(Object::cast_to<ResourceState>(FSM->get_states()[FSM->No_Clip])));
    }
    if(player->is_on_floor()){
        if(event_input["jump"]|| (auto_bhop && event_input["jump"])){
            player->set_velocity(Vector3(
                player->get_velocity().x,
                jump_velocity,
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
        player->get_velocity().y - (gravity * delta),
        player->get_velocity().z
    ));
    float curr_speed_in_wish_dir = player->get_velocity().dot(wish_dir);
    float capped_speed = UtilityFunctions::min((air_move_speed * wish_dir).length(), air_cap);
    float add_speed_till_cap = capped_speed - curr_speed_in_wish_dir;
    if(add_speed_till_cap > 0){
        float accel_speed = air_accel * air_move_speed * delta;
        accel_speed = UtilityFunctions::min(accel_speed, add_speed_till_cap);
        player->set_velocity(player->get_velocity() + (accel_speed * wish_dir));
    }
}

void Move_PMState::_handle_ground_physics(double delta) {
   
    // Current speed in desired direction
    float curr_speed_in_wish_dir = player->get_velocity().dot(wish_dir);
    // Amount of speed we can still add
    float add_speed_till_cap = get_move_speed() - curr_speed_in_wish_dir;
    if (add_speed_till_cap > 0.0f) {
        float accel_speed = ground_accel * (float)delta * get_move_speed();
        accel_speed = MIN(accel_speed, add_speed_till_cap);
        player->set_velocity(player->get_velocity() + (accel_speed * wish_dir));
    }
     float player_velocity_length = player->get_velocity().length();
    // Apply friction
    float control = MAX(player_velocity_length, ground_decel);
    float drop = control * ground_friction * (float)delta;
    float new_speed = MAX(player_velocity_length - drop, 0.0f);
    if (player_velocity_length > 0.0f) {
        new_speed /= player_velocity_length;
    }
    player->set_velocity(player->get_velocity() * new_speed);
}

float Move_PMState::get_move_speed() {
    Dictionary state_input = (Dictionary)input["state_based_actions"];
	if(state_input["sprint"]){
            return sprint_speed;
        } else {
            return walk_speed;
        }
}

void Move_PMState::_bind_methods(){
// jump_velocity
    ClassDB::bind_method(D_METHOD("set_jump_velocity", "p_jump_velocity"), &Move_PMState::set_jump_velocity);
    ClassDB::bind_method(D_METHOD("get_jump_velocity"), &Move_PMState::get_jump_velocity);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "jump_velocity"), "set_jump_velocity", "get_jump_velocity");
    // walk_speed
    ClassDB::bind_method(D_METHOD("set_walk_speed", "p_walk_speed"), &Move_PMState::set_walk_speed);
    ClassDB::bind_method(D_METHOD("get_walk_speed"), &Move_PMState::get_walk_speed);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "walk_speed"), "set_walk_speed", "get_walk_speed");
    // sprint_speed
    ClassDB::bind_method(D_METHOD("set_sprint_speed", "p_sprint_speed"), &Move_PMState::set_sprint_speed);
    ClassDB::bind_method(D_METHOD("get_sprint_speed"), &Move_PMState::get_sprint_speed);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "sprint_speed"), "set_sprint_speed", "get_sprint_speed");
    // auto_bhop
    ClassDB::bind_method(D_METHOD("set_auto_bhop", "p_auto_bhop"), &Move_PMState::set_auto_bhop);
    ClassDB::bind_method(D_METHOD("get_auto_bhop"), &Move_PMState::get_auto_bhop);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "auto_bhop"), "set_auto_bhop", "get_auto_bhop");
    // gravity
    ClassDB::bind_method(D_METHOD("set_gravity", "p_gravity"), &Move_PMState::set_gravity);
    ClassDB::bind_method(D_METHOD("get_gravity"), &Move_PMState::get_gravity);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "gravity"), "set_gravity", "get_gravity");

    // ---- Air Physics ----
    ADD_GROUP("Air Physics", "air_");
    // air_cap
    ClassDB::bind_method(D_METHOD("set_air_cap", "air_cap"), &Move_PMState::set_air_cap);
    ClassDB::bind_method(D_METHOD("get_air_cap"), &Move_PMState::get_air_cap);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "air_cap"), "set_air_cap", "get_air_cap");
    // air_accel
    ClassDB::bind_method(D_METHOD("set_air_accel", "air_accel"), &Move_PMState::set_air_accel);
    ClassDB::bind_method(D_METHOD("get_air_accel"), &Move_PMState::get_air_accel);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "air_accel"), "set_air_accel", "get_air_accel");
    // air_move_speed
    ClassDB::bind_method(D_METHOD("set_air_move_speed", "air_move_speed"), &Move_PMState::set_air_move_speed);
    ClassDB::bind_method(D_METHOD("get_air_move_speed"), &Move_PMState::get_air_move_speed);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "air_move_speed"), "set_air_move_speed", "get_air_move_speed");

    // ---- Ground Physics ----
    ADD_GROUP("Ground Physics", "ground_");
    // ground_accel
    ClassDB::bind_method(D_METHOD("set_ground_accel", "ground_accel"), &Move_PMState::set_ground_accel);
    ClassDB::bind_method(D_METHOD("get_ground_accel"), &Move_PMState::get_ground_accel);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "ground_accel"), "set_ground_accel", "get_ground_accel");
    // ground_decel
    ClassDB::bind_method(D_METHOD("set_ground_decel", "ground_decel"), &Move_PMState::set_ground_decel);
    ClassDB::bind_method(D_METHOD("get_ground_decel"), &Move_PMState::get_ground_decel);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "ground_decel"), "set_ground_decel", "get_ground_decel");
    // ground_friction
    ClassDB::bind_method(D_METHOD("set_ground_friction", "ground_friction"), &Move_PMState::set_ground_friction);
    ClassDB::bind_method(D_METHOD("get_ground_friction"), &Move_PMState::get_ground_friction);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "ground_friction"), "set_ground_friction", "get_ground_friction");
}