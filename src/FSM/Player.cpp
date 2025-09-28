#include "Player.h"
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/scene_tree_timer.hpp>


using namespace godot;

Player::Player() {}
Player::~Player() {}

void Player::_bind_methods() {
    // Physics refs
    ClassDB::bind_method(D_METHOD("set_air_physics", "air_physics"), &Player::set_air_physics);
    ClassDB::bind_method(D_METHOD("get_air_physics"), &Player::get_air_physics);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "air_physics", PROPERTY_HINT_NODE_TYPE, "AirPhysics"),
                 "set_air_physics", "get_air_physics");

    ClassDB::bind_method(D_METHOD("set_ground_physics", "ground_physics"), &Player::set_ground_physics);
    ClassDB::bind_method(D_METHOD("get_ground_physics"), &Player::get_ground_physics);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "ground_physics", PROPERTY_HINT_NODE_TYPE, "GroundPhysics"),
                 "set_ground_physics", "get_ground_physics");

    // Functions
    ClassDB::bind_method(D_METHOD("on_hit"), &Player::on_hit);
    ClassDB::bind_method(D_METHOD("on_kill"), &Player::on_kill);
    ClassDB::bind_method(D_METHOD("register_damage", "damage", "enemy"), &Player::register_damage);
}

void Player::_ready() {
     if (Engine::get_singleton()->is_editor_hint()) {
        return; // don't run in editor
    }
  
    Input::get_singleton()->set_mouse_mode(Input::MOUSE_MODE_CAPTURED);

    head = get_node<Node3D>("Head");
    camera = get_node<Camera3D>("Head/Camera3D");
    animation_player = get_node<AnimationPlayer>("Animation/AnimationPlayer");
    hit_marker = get_node<TextureRect>("UI/HitMarker");
    hit_effect_sfx = get_node<AudioStreamPlayer3D>("Audio/hit_effect_sfx");
    kill_effect_sfx = get_node<AudioStreamPlayer3D>("Audio/kill_effect_sfx");
}

void Player::_unhandled_input(const Ref<InputEvent> &p_event) {
    if (p_event.is_null()) return;

    Ref<InputEventMouseMotion> motion = p_event;
    if (motion.is_valid()) {
        if (head) head->rotate_y(-motion->get_relative().x * SENSITIVITY);
        if (camera) {
            camera->rotate_x(-motion->get_relative().y * SENSITIVITY);
            Vector3 rot = camera->get_rotation();
            rot.x = UtilityFunctions::clamp(rot.x, Math::deg_to_rad(-90.0), Math::deg_to_rad(90.0));
            camera->set_rotation(rot);
        }
    }
}



void Player::_physics_process(double delta) {

     if (Engine::get_singleton()->is_editor_hint()) {
        return; // don't run in editor
    }
    calculate_wish_dir();

    // FOV adjustment
    if (camera) {
        float velocity_clamped = Math::clamp(get_velocity().length(), 0.5f, SPEED * 2.0f);
        float target_fov = 68.0f + FOV_CHANGE * velocity_clamped;
        float new_fov = Math::lerp(camera->get_fov(), target_fov, float(delta * 8.0));
        // camera->set_fov(new_fov);
    }
}

Vector3 Player::calculate_wish_dir() {
    Vector2 input_dir = Input::get_singleton()->get_vector("left", "right", "up", "down").normalized();
    if (head) {
        wish_dir = head->get_global_transform().basis.xform(Vector3(input_dir.x, 0, input_dir.y)).normalized();
    }
    return wish_dir;
}

void Player::on_hit() {
    if (hit_effect_sfx) hit_effect_sfx->play();
    if (hit_marker) {
        hit_marker->set_visible(true);

        // Hide after 0.25s
        SceneTree *tree = get_tree();
        if (tree) {
            Ref<SceneTreeTimer> timer = tree->create_timer(0.25);
            timer->connect("timeout", Callable(hit_marker, "set_visible").bindv(Array::make(false)));
        }
    }
}

void Player::on_kill() {
    if (kill_effect_sfx) kill_effect_sfx->play();
}

void Player::register_damage(float damage, Object *enemy) {
    if (!enemy) return;

    Variant health = enemy->get("health");
    UtilityFunctions::print("ENEMY HEALTH: ", String::num(health));

    if ((int)health <= 0) {
        on_kill();
    }
}
