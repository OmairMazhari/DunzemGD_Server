#include "BulletTrailConfig.h"

using namespace godot;

void BulletTrailConfig::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_speed", "speed"), &BulletTrailConfig::set_speed);
    ClassDB::bind_method(D_METHOD("get_speed"), &BulletTrailConfig::get_speed);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "speed", PROPERTY_HINT_RANGE, "0,1000,1"), "set_speed", "get_speed");

    ClassDB::bind_method(D_METHOD("set_tracer_length", "len"), &BulletTrailConfig::set_tracer_length);
    ClassDB::bind_method(D_METHOD("get_tracer_length"), &BulletTrailConfig::get_tracer_length);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "tracer_length", PROPERTY_HINT_RANGE, "0,100,0.1"), "set_tracer_length", "get_tracer_length");

    ClassDB::bind_method(D_METHOD("set_barrel_node", "start"), &BulletTrailConfig::set_barrel_node);
    ClassDB::bind_method(D_METHOD("get_barrel_node"), &BulletTrailConfig::get_barrel_node);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "barrel_node", PROPERTY_HINT_NODE_TYPE, "Node3D"), "set_barrel_node", "get_barrel_node");

    
    ClassDB::bind_method(D_METHOD("set_target_pos", "len"), &BulletTrailConfig::set_target_pos);
    ClassDB::bind_method(D_METHOD("get_target_pos"), &BulletTrailConfig::get_target_pos);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "target_pos", PROPERTY_HINT_NODE_TYPE, "Node3D"), "set_target_pos", "get_target_pos");



}
