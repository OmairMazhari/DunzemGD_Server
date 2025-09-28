#pragma once

#include "State.h"
#include <godot_cpp/classes/input.hpp>
#include "./Player.h"



class StateMove : public State {
    GDCLASS(StateMove, State);
    StateMove(){

    }
    ~StateMove(){

    }
public:
    AirPhysics *air_physics = nullptr;
    GroundPhysics *ground_physics = nullptr;

    void set_air_physics(Node *p) { air_physics = Object::cast_to<AirPhysics>(p); }
    AirPhysics *get_air_physics() const { return air_physics; }

    // --- GroundPhysics ---
    void set_ground_physics(Node *p) { ground_physics = Object::cast_to<GroundPhysics>(p); }
    GroundPhysics *get_ground_physics() const { return ground_physics; }

    int jumps = 0;

    void Enter() override  {
        UtilityFunctions::print("Running in move state");
    }
    void Exit() override  {
        UtilityFunctions::print("Exiting move state");    
    }
    void Update(double delta, Player* player) override  {
        Transform3D camera_transform = player->camera->get_transform();
        camera_transform.origin.y = UtilityFunctions::lerp(player->camera->get_transform().origin.y, player->STAND_HEIGHT, delta * 11);
        player->camera->set_transform(camera_transform);
        if(player->is_on_floor()){
            jumps = 0;
        }
        if (Input::get_singleton()->is_action_just_pressed("jump")){
            jumps += 1;
            if(jumps >=  2){
                emit_signal("state_transition", this, "power_jump");
                jumps = 0;
            }
        }
        if (player->is_on_floor() && 
            (Input::get_singleton()->is_action_just_pressed("jump") ||
            (air_physics->auto_bhop && Input::get_singleton()->is_action_pressed("jump")))) {
                UtilityFunctions::print("JUMPING Now");
                Vector3 playerVelocity = player->get_velocity();
                playerVelocity.y = player->JUMP_VELOCITY * 2.0;
                player->set_velocity(playerVelocity);
        }
        if (player->is_on_floor()){
            player->set_velocity(ground_physics->calculate(player->get_velocity(), player->wish_dir, delta));
        } else { 
           player->set_velocity(air_physics->calculate(player->get_velocity(), player->wish_dir, delta));
        }

        if (Input::get_singleton()->is_action_just_pressed("jump")){
            emit_signal("state_transition", this, "stateslide");
        }
    }
protected:
    static void _bind_methods(){
         // AirPhysics
        ClassDB::bind_method(D_METHOD("set_air_physics", "air_physics"), &StateMove::set_air_physics);
        ClassDB::bind_method(D_METHOD("get_air_physics"), &StateMove::get_air_physics);
        ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "AirPhysics", PROPERTY_HINT_NODE_TYPE, "AirPhysics"), "set_air_physics", "get_air_physics");
        // GroundPhysics 
        ClassDB::bind_method(D_METHOD("set_ground_physics", "ground_physics"), &StateMove::set_ground_physics);
        ClassDB::bind_method(D_METHOD("get_ground_physics"), &StateMove::get_ground_physics);
        ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "GroundPhysics", PROPERTY_HINT_NODE_TYPE, "GroundPhysics"), "set_ground_physics", "get_ground_physics");
    }
};