#pragma once

#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/animation_player.hpp>
#include <godot_cpp/classes/texture_rect.hpp>
#include <godot_cpp/classes/audio_stream_player3d.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/time.hpp>

#include "AirPhysics.h"
#include "GroundPhysics.h"
#include "FSM/Gun.h"

using namespace godot;

class Player : public CharacterBody3D {
    GDCLASS(Player, CharacterBody3D);

public:
    Player();
    ~Player();

    // Lifecycle
    void _ready() override;
    void _physics_process(double delta) override;
    void _unhandled_input(const Ref<InputEvent> &p_event) override;

    // Gameplay
    Vector3 calculate_wish_dir();
    void on_hit();
    void on_kill();
    void register_damage(float damage, Object *enemy);

    // Getters/Setters
    void set_air_physics(AirPhysics *p_air) { air_physics = p_air; }
    AirPhysics *get_air_physics() const { return air_physics; }

    void set_ground_physics(GroundPhysics *p_ground) { ground_physics = p_ground; }
    GroundPhysics *get_ground_physics() const { return ground_physics; }

protected:
    static void _bind_methods();

public:
    // Movement constants
    float JUMP_VELOCITY = 4.5f;
    const float CROUCH_HEIGHT = 1.0f;
    const float SLIDE_HEIGHT = 0.5f;
    const float STAND_HEIGHT = 1.5f;
    const float FOV_CHANGE = 1.5f;

    const float SPEED = 7.0f;
    const float SPRINT_SPEED = 8.5f;
    const float SLIDE_SPEED = 20.0f;
    const float SLIDE_SLAM_SPEED = 15.0f;
    float CURR_SLIDE_SPEED = SLIDE_SPEED;

    const float SENSITIVITY = 0.0008f;

    // State vars
    Vector3 wish_dir = Vector3();
    bool is_sliding = false;

    // Onready nodes
    Node3D *head = nullptr;
    Camera3D *camera = nullptr;
    AnimationPlayer *animation_player = nullptr;
    TextureRect *hit_marker = nullptr;
    AudioStreamPlayer3D *hit_effect_sfx = nullptr;
    AudioStreamPlayer3D *kill_effect_sfx = nullptr;

    // Exported (inspector)
    AirPhysics *air_physics = nullptr;
    GroundPhysics *ground_physics = nullptr;
};
