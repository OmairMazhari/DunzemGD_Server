#pragma once
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/core/math.hpp>
#include <godot_cpp/classes/input.hpp>

using namespace godot;




class GroundPhysics: public Node {
    GDCLASS(GroundPhysics, Node);
public:
    GroundPhysics(){
    
    }
    ~GroundPhysics(){

    }
    // Export Variables
    float sprint_speed = 10.0f;
    float speed = 6.0f;

    float ground_accel = 25.0;
    float ground_decel = 10.0;
    float ground_friction = 3.0;

    float get_ground_accel() { return ground_accel; }
    void set_ground_accel(float p_value) { ground_accel = p_value; }

    float get_ground_decel() { return ground_decel; }
    void set_ground_decel(float p_value) { ground_decel = p_value; }

    float get_ground_friction() { return ground_friction; }
    void set_ground_friction(float p_value) { ground_friction = p_value; }

     float get_sprint_speed() const { return sprint_speed; }
    void set_sprint_speed(float p_value) { sprint_speed = p_value; }

    Vector3 calculate(Vector3 velocity, Vector3 wish_dir, double delta) {
        // Current speed in desired direction
        float curr_speed_in_wish_dir = velocity.dot(wish_dir);

        // Amount of speed we can still add
        float add_speed_till_cap = get_move_speed() - curr_speed_in_wish_dir;
        if (add_speed_till_cap > 0.0f) {
            float accel_speed = ground_accel * (float)delta * get_move_speed();
            accel_speed = MIN(accel_speed, add_speed_till_cap);
            velocity += accel_speed * wish_dir;
        }
        // Apply friction
        float control = MAX(velocity.length(), ground_decel);
        float drop = control * ground_friction * (float)delta;
        float new_speed = MAX(velocity.length() - drop, 0.0f);
        if (velocity.length() > 0.0f) {
            new_speed /= velocity.length();
        }
        velocity *= new_speed;

        return velocity;
    }

     float get_move_speed() const {
        if(Input::get_singleton()->is_action_pressed("sprint")){
             return sprint_speed; 
        } 
        return speed;
       
    }

    protected:
        static void _bind_methods(){
        
       

        ClassDB::bind_method(D_METHOD("set_sprint_speed", "sprint_speed"), &GroundPhysics::set_sprint_speed);
        ClassDB::bind_method(D_METHOD("get_sprint_speed"), &GroundPhysics::get_sprint_speed);
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "sprint_speed"), "set_sprint_speed", "get_sprint_speed");

        ClassDB::bind_method(D_METHOD("set_ground_accel", "ground_accel"), &GroundPhysics::set_ground_accel);
        ClassDB::bind_method(D_METHOD("get_ground_accel"), &GroundPhysics::get_ground_accel);

        ClassDB::bind_method(D_METHOD("set_ground_decel", "ground_decel"), &GroundPhysics::set_ground_decel);
        ClassDB::bind_method(D_METHOD("get_ground_decel"), &GroundPhysics::get_ground_decel);

        ClassDB::bind_method(D_METHOD("set_ground_friction", "ground_friction"), &GroundPhysics::set_ground_friction);
        ClassDB::bind_method(D_METHOD("get_ground_friction"), &GroundPhysics::get_ground_friction);

    
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "ground_accel"), "set_ground_accel", "get_ground_accel");
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "ground_decel"), "set_ground_decel", "get_ground_decel");
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "ground_friction"), "set_ground_friction", "get_ground_friction");

    }
};