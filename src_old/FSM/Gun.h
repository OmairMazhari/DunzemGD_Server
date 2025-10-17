#pragma once

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/animation_player.hpp>
#include <godot_cpp/classes/ray_cast3d.hpp>
#include <godot_cpp/classes/audio_stream_player3d.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/variant/string.hpp>
#include "./Player.h"
#include "BulletTrail.h"


class Player; // forward declaration

using namespace godot;


class Gun : public Node3D {
    GDCLASS(Gun, Node3D);

public:
    Player *gun_holder = nullptr;
    int current_magzine = 0;
    float rate = 0.0f;
    bool ADS = false;
    bool reloading = false;
    bool shot = false;
    bool hit = false;
    bool clip_empty = false;


    // Exported variables
    AnimationPlayer *animation_player = nullptr;
    float fire_rate = 0.0;
    float recoil = 0.0;
    RayCast3D *aim_ray = nullptr;
    float damage = 0.0;
    BulletTrail *bullet_trail = nullptr;

    AudioStreamPlayer3D *shoot_sfx = nullptr;
    AudioStreamPlayer3D *reload_sfx = nullptr;

    int magzine_size = 0.0;
    float reload_time = 0.0;
    float pull_out_time = 0.0;

    Vector3 default_position = Vector3();
    Vector3 ads_position = Vector3();
    float ads_acceleration = 0.2f;
    float default_fov = 70.0f;
    float ads_fov = 55.0f;
    Camera3D *camera = nullptr;
    Camera3D *gun_camera = nullptr;
    float gun_default_fov = 70.0f;
    float gun_ads_fov = 55.0f;

    Label *magize_label = nullptr;

public:
    Gun();
    ~Gun();

    // --- Getters & Setters ---
    void set_animation_player(AnimationPlayer *p_val);
    AnimationPlayer *get_animation_player() const;

    void set_fire_rate(float p_val);
    float get_fire_rate() const;

    void set_recoil(float p_val);
    float get_recoil() const;

    void set_aim_ray(RayCast3D *p_val);
    RayCast3D *get_aim_ray() const;

    void set_damage(float p_val);
    float get_damage() const;

    void set_bullet_trail(BulletTrail *p_val);
    BulletTrail *get_bullet_trail() const;

    void set_shoot_sfx(AudioStreamPlayer3D *p_val);
    AudioStreamPlayer3D *get_shoot_sfx() const;

    void set_reload_sfx(AudioStreamPlayer3D *p_val);
    AudioStreamPlayer3D *get_reload_sfx() const;

    void set_magzine_size(int p_val);
    int get_magzine_size() const;

    void set_reload_time(float p_val);
    float get_reload_time() const;

    void set_pull_out_time(float p_val);
    float get_pull_out_time() const;

    void set_default_position(Vector3 p_val);
    Vector3 get_default_position() const;

    void set_ads_position(Vector3 p_val);
    Vector3 get_ads_position() const;

    void set_ads_acceleration(float p_val);
    float get_ads_acceleration() const;

    void set_default_fov(float p_val);
    float get_default_fov() const;

    void set_ads_fov(float p_val);
    float get_ads_fov() const;

    void set_camera(Camera3D *p_val);
    Camera3D *get_camera() const;

    void set_gun_camera(Camera3D *p_val);
    Camera3D *get_gun_camera() const;

    void set_gun_default_fov(float p_val);
    float get_gun_default_fov() const;

    void set_gun_ads_fov(float p_val);
    float get_gun_ads_fov() const;

    void set_magize_label(Label *p_val);
    Label *get_magize_label() const;

    void Gun::shoot(double delta);
    void Gun::aim(double delta, bool ADS);
    void Gun::reload(bool can_reload);

protected:
    static void _bind_methods();
};
