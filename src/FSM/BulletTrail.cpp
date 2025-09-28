// BulletTrail.cpp
#include "BulletTrail.h"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/window.hpp>


using namespace godot;

BulletTrail::BulletTrail() {}
BulletTrail::~BulletTrail() {}

void BulletTrail::_ready() {
    spawn_time = Time::get_singleton()->get_ticks_msec();
    config = ResourceLoader::get_singleton()->load("res://BulletTrailConfig.tres");
}

void BulletTrail::_process(double delta) {
    if (!active)
        return;
    UtilityFunctions::print("reached");
    Vector3 diff = target_pos - get_global_position();
    Vector3 add = diff.normalized() * speed * delta;
    if (add.length() > diff.length()) {
        add = diff;
    }
    set_global_position(get_global_position() + add);

    // Lifetime and end condition
    if (diff.length() <= tracer_length ||
        Time::get_singleton()->get_ticks_msec() - spawn_time > MAX_LIFETIME_MS) {
        queue_free();
    }
}

// --- Setters/Getters ---
void BulletTrail::set_bullet_trail_start(Node3D *p_val) { bullet_trail_start = p_val; }
Node3D *BulletTrail::get_bullet_trail_start() const { return bullet_trail_start; }

void BulletTrail::set_speed(double p_val) { speed = p_val; }
double BulletTrail::get_speed() const { return speed; }

void BulletTrail::set_tracer_length(double p_val) { tracer_length = p_val; }
double BulletTrail::get_tracer_length() const { return tracer_length; }

// --- API ---
void BulletTrail::make(const Vector3 &p_target) {
    bullet_trail_start = config->get_barrel_node();


    if (!bullet_trail_start)
        return;
    Vector3 bullet_dir = (p_target - bullet_trail_start->get_global_position()).normalized();
    Vector3 start_pos = bullet_trail_start->get_global_position() + bullet_dir * 0.25;
    if ((p_target - start_pos).length() > 3.0) {
        // Spawn a new BulletTrail instance
        Ref<PackedScene> tracer_scene = ResourceLoader::get_singleton()->load("res://Scenes/Gun/BulletTrails.tscn");
        if (tracer_scene.is_valid()) {
            UtilityFunctions::print("Loaded bullet trail");
            Node *instance = tracer_scene->instantiate();
            if (Node3D *bullet_tracer = Object::cast_to<Node3D>(instance)) {
                get_tree()->get_root()->add_child(bullet_tracer);
  

                BulletTrail *bt = Object::cast_to<BulletTrail>(bullet_tracer);
                if (bt) {
                    bt->set_global_position(start_pos);
                    bt->target_pos = p_target;
                    bt->look_at(p_target);
                    bt->spawn_time = Time::get_singleton()->get_ticks_msec();
                    bt->active = true;
                   
                }
            }
        }
    }
}

// --- Bindings ---
void BulletTrail::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_bullet_trail_start", "start"), &BulletTrail::set_bullet_trail_start);
    ClassDB::bind_method(D_METHOD("get_bullet_trail_start"), &BulletTrail::get_bullet_trail_start);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "bullet_trail_start", PROPERTY_HINT_NODE_TYPE, "Node3D"), "set_bullet_trail_start", "get_bullet_trail_start");

    ClassDB::bind_method(D_METHOD("set_speed", "speed"), &BulletTrail::set_speed);
    ClassDB::bind_method(D_METHOD("get_speed"), &BulletTrail::get_speed);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "speed"), "set_speed", "get_speed");

    ClassDB::bind_method(D_METHOD("set_tracer_length", "len"), &BulletTrail::set_tracer_length);
    ClassDB::bind_method(D_METHOD("get_tracer_length"), &BulletTrail::get_tracer_length);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "tracer_length"), "set_tracer_length", "get_tracer_length");


    ClassDB::bind_method(D_METHOD("make", "target_pos"), &BulletTrail::make);

    ClassDB::bind_method(D_METHOD("set_config", "config"), &BulletTrail::set_config);
    ClassDB::bind_method(D_METHOD("get_config"), &BulletTrail::get_config);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "config", PROPERTY_HINT_RESOURCE_TYPE, "BulletTrailConfig"),
                 "set_config", "get_config");
}
