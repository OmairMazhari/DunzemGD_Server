#pragma once
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/input.hpp>

#include "PlayerMovementState.h"
#include <FSM/ResourceFSM/State.h>

using namespace godot;

class Move_PMState : public PlayerMovementState {
    GDCLASS(Move_PMState, PlayerMovementState);
public:
    Move_PMState() {}
    ~Move_PMState() {};

    // Overriden functions
    void Enter() override;
    void Exit() override;
    void Update(double delta) override;
    void Setup(ResourceFSM* FSM) override;

    // Class functions
    void _handle_air_physics(double delta);
    void _handle_ground_physics(double delta);
    float get_move_speed();

protected:
    static void _bind_methods();

private:
    // Class Variables
    Vector3 wish_dir = Vector3();

    // Export Variables
    // Basic values
    float jump_velocity = 6.0f;
    float walk_speed = 7.0f;
    float sprint_speed = 8.5f;
    float gravity = 9.8f;
    bool auto_bhop = false;
    // Air physics values
    float  air_cap = .25;
    float air_accel = 800.0;
    float air_move_speed = 500.0;
    // Ground physics values
    float ground_accel = 25.0;
    float ground_decel = 10.0;
    float ground_friction = 3.0;

public:

    void set_jump_velocity(float p_jump_velocity) { jump_velocity = p_jump_velocity; }
    float get_jump_velocity() const { return jump_velocity; }

    void set_walk_speed(float p_walk_speed) {walk_speed = p_walk_speed; }
    float get_walk_speed() const { return walk_speed; }

    void set_sprint_speed(float p_sprint_speed) { sprint_speed = p_sprint_speed; }
    float get_sprint_speed() const { return sprint_speed; }
  
    void set_auto_bhop(float p_auto_bhop) { auto_bhop = p_auto_bhop; }
    bool get_auto_bhop() const { return auto_bhop; }

    void set_gravity(float p_gravity) { gravity = p_gravity; }
    float get_gravity() const { return gravity; }

    float get_air_cap() { return air_cap; }
    void set_air_cap(float p_value) { air_cap = p_value; }

    float get_air_accel() { return air_accel; }
    void set_air_accel(float p_value) { air_accel= p_value; }

    float get_air_move_speed() { return air_move_speed; }
    void set_air_move_speed(float p_value) { air_move_speed = p_value; }

     float get_ground_accel() { return ground_accel; }
    void set_ground_accel(float p_value) { ground_accel = p_value; }

    float get_ground_decel() { return ground_decel; }
    void set_ground_decel(float p_value) { ground_decel = p_value; }

    float get_ground_friction() { return ground_friction; }
    void set_ground_friction(float p_value) { ground_friction = p_value; }
   
};