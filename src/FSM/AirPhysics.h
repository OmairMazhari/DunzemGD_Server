#pragma once
#include <godot_cpp/classes/node.hpp>
using namespace godot;



class AirPhysics: public Node {
    GDCLASS(AirPhysics, Node);
public:
    AirPhysics(){ 
    
    }
    ~AirPhysics(){

    }
    // Export Variable

    
    float  air_cap = .25;
    float air_accel = 800.0;
    float air_move_speed = 500.0;
    bool  auto_bhop = true;

    float get_air_cap() { return air_cap; }
    void set_air_cap(float p_value) { air_cap = p_value; }

    float get_air_accel() { return air_accel; }
    void set_air_accel(float p_value) { air_accel= p_value; }

    float get_air_move_speed() { return air_move_speed; }
    void set_air_move_speed(float p_value) { air_move_speed = p_value; }

    bool get_auto_bhop() { return auto_bhop; }
    void set_auto_bhop(bool p_value) { auto_bhop = p_value; }



    Vector3 calculate(Vector3 velocity, Vector3 wish_dir, double delta){
        velocity.y -= 25 * delta;
        float curr_speed_in_wish_dir = velocity.dot(wish_dir);
        float capped_speed = UtilityFunctions::min((air_move_speed * wish_dir).length(), air_cap);
        float add_speed_till_cap = capped_speed - curr_speed_in_wish_dir;
        if(add_speed_till_cap > 0){
            float accel_speed = air_accel * air_move_speed * delta;
            accel_speed = UtilityFunctions::min(accel_speed, add_speed_till_cap);
            velocity += accel_speed * wish_dir;
        }
        return velocity;
    }

protected:
    static void _bind_methods(){

        ClassDB::bind_method(D_METHOD("set_air_cap", "air_cap"), &AirPhysics::set_air_cap);
        ClassDB::bind_method(D_METHOD("get_air_cap"), &AirPhysics::get_air_cap);

        ClassDB::bind_method(D_METHOD("set_air_accel", "air_accel"), &AirPhysics::set_air_accel);
        ClassDB::bind_method(D_METHOD("get_air_accel"), &AirPhysics::get_air_accel);

        ClassDB::bind_method(D_METHOD("set_air_move_speed", "air_move_speed"), &AirPhysics::set_air_move_speed);
        ClassDB::bind_method(D_METHOD("get_air_move_speed"), &AirPhysics::get_air_move_speed);

        ClassDB::bind_method(D_METHOD("set_auto_bhop", "auto_bhop"), &AirPhysics::set_auto_bhop);
        ClassDB::bind_method(D_METHOD("get_auto_bhop"), &AirPhysics::get_auto_bhop);

        
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "air_cap"), "set_air_cap", "get_air_cap");
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "air_accel"), "set_air_accel", "get_air_accel");
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "air_move_speed"), "set_air_move_speed", "get_air_move_speed");
        ADD_PROPERTY(PropertyInfo(Variant::BOOL, "auto_bhop"), "set_auto_bhop", "get_auto_bhop");

    }
};