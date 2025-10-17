#pragma once
#include <godot_cpp/classes/Resource.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

class PlayerMovementStatsResource : public Resource {
    GDCLASS(PlayerMovementStatsResource, Resource);

 // Resource Values
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
    // No clip
    float no_clip_speed_mult = 3.0;
    bool no_clip = false;


public:
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

    void set_no_clip_speed_mult(float p_no_clip_speed_mult) { no_clip_speed_mult = p_no_clip_speed_mult; }
    float get_no_clip_speed_mult() const { return no_clip_speed_mult; }

    void set_no_clip(bool p_no_clip) { no_clip = p_no_clip; }
    bool  get_no_clip() const { return no_clip; }
 protected:
    static void _bind_methods() {
        // jump_velocity
        ClassDB::bind_method(D_METHOD("set_jump_velocity", "p_jump_velocity"), &PlayerMovementStatsResource::set_jump_velocity);
        ClassDB::bind_method(D_METHOD("get_jump_velocity"), &PlayerMovementStatsResource::get_jump_velocity);
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "jump_velocity"), "set_jump_velocity", "get_jump_velocity");
        // walk_speed
        ClassDB::bind_method(D_METHOD("set_walk_speed", "p_walk_speed"), &PlayerMovementStatsResource::set_walk_speed);
        ClassDB::bind_method(D_METHOD("get_walk_speed"), &PlayerMovementStatsResource::get_walk_speed);
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "walk_speed"), "set_walk_speed", "get_walk_speed");
        // sprint_speed
        ClassDB::bind_method(D_METHOD("set_sprint_speed", "p_sprint_speed"), &PlayerMovementStatsResource::set_sprint_speed);
        ClassDB::bind_method(D_METHOD("get_sprint_speed"), &PlayerMovementStatsResource::get_sprint_speed);
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "sprint_speed"), "set_sprint_speed", "get_sprint_speed");
        // auto_bhop
        ClassDB::bind_method(D_METHOD("set_auto_bhop", "p_auto_bhop"), &PlayerMovementStatsResource::set_auto_bhop);
        ClassDB::bind_method(D_METHOD("get_auto_bhop"), &PlayerMovementStatsResource::get_auto_bhop);
        ADD_PROPERTY(PropertyInfo(Variant::BOOL, "auto_bhop"), "set_auto_bhop", "get_auto_bhop");
        // gravity
        ClassDB::bind_method(D_METHOD("set_gravity", "p_gravity"), &PlayerMovementStatsResource::set_gravity);
        ClassDB::bind_method(D_METHOD("get_gravity"), &PlayerMovementStatsResource::get_gravity);
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "gravity"), "set_gravity", "get_gravity");

        // ---- Air Physics ----
        ADD_GROUP("Air Physics", "air_");
        // air_cap
        ClassDB::bind_method(D_METHOD("set_air_cap", "air_cap"), &PlayerMovementStatsResource::set_air_cap);
        ClassDB::bind_method(D_METHOD("get_air_cap"), &PlayerMovementStatsResource::get_air_cap);
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "air_cap"), "set_air_cap", "get_air_cap");
        // air_accel
        ClassDB::bind_method(D_METHOD("set_air_accel", "air_accel"), &PlayerMovementStatsResource::set_air_accel);
        ClassDB::bind_method(D_METHOD("get_air_accel"), &PlayerMovementStatsResource::get_air_accel);
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "air_accel"), "set_air_accel", "get_air_accel");
        // air_move_speed
        ClassDB::bind_method(D_METHOD("set_air_move_speed", "air_move_speed"), &PlayerMovementStatsResource::set_air_move_speed);
        ClassDB::bind_method(D_METHOD("get_air_move_speed"), &PlayerMovementStatsResource::get_air_move_speed);
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "air_move_speed"), "set_air_move_speed", "get_air_move_speed");

        // ---- Ground Physics ----
        ADD_GROUP("Ground Physics", "ground_");
        // ground_accel
        ClassDB::bind_method(D_METHOD("set_ground_accel", "ground_accel"), &PlayerMovementStatsResource::set_ground_accel);
        ClassDB::bind_method(D_METHOD("get_ground_accel"), &PlayerMovementStatsResource::get_ground_accel);
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "ground_accel"), "set_ground_accel", "get_ground_accel");
        // ground_decel
        ClassDB::bind_method(D_METHOD("set_ground_decel", "ground_decel"), &PlayerMovementStatsResource::set_ground_decel);
        ClassDB::bind_method(D_METHOD("get_ground_decel"), &PlayerMovementStatsResource::get_ground_decel);
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "ground_decel"), "set_ground_decel", "get_ground_decel");
        // ground_friction
        ClassDB::bind_method(D_METHOD("set_ground_friction", "ground_friction"), &PlayerMovementStatsResource::set_ground_friction);
        ClassDB::bind_method(D_METHOD("get_ground_friction"), &PlayerMovementStatsResource::get_ground_friction);
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "ground_friction"), "set_ground_friction", "get_ground_friction");

        // ---- No Clip ---- 
         ADD_GROUP("No Clip", "no_clip_");
        ClassDB::bind_method(D_METHOD("set_no_clip_speed_mult", "p_no_clip_speed_mult"), &PlayerMovementStatsResource::set_no_clip_speed_mult);
        ClassDB::bind_method(D_METHOD("get_no_clip_speed_mult"), &PlayerMovementStatsResource::get_no_clip_speed_mult);
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "no_clip_speed_mult"), "set_no_clip_speed_mult", "get_no_clip_speed_mult");

        ClassDB::bind_method(D_METHOD("set_no_clip", "p_no_clip"), &PlayerMovementStatsResource::set_no_clip);
        ClassDB::bind_method(D_METHOD("get_no_clip"), &PlayerMovementStatsResource::get_no_clip);
        ADD_PROPERTY(PropertyInfo(Variant::BOOL, "no_clip"), "set_no_clip", "get_no_clip");
    }
public:
    PlayerMovementStatsResource() { }
    ~PlayerMovementStatsResource() { }
    // Setter and Getter
    void set_jump_velocity(float p_jump_velocity) { jump_velocity = p_jump_velocity; }
    float get_jump_velocity() const { return jump_velocity; }
};