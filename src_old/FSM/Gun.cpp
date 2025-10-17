
#include "Gun.h"
#include <godot_cpp/core/class_db.hpp>


using namespace godot;

Gun::Gun() {}
Gun::~Gun() {}

// --- Setters & Getters ---
void Gun::set_animation_player(AnimationPlayer *p_val) { animation_player = p_val; }
AnimationPlayer *Gun::get_animation_player() const { return animation_player; }

void Gun::set_fire_rate(float p_val) { fire_rate = p_val; }
float Gun::get_fire_rate() const { return fire_rate; }

void Gun::set_recoil(float p_val) { recoil = p_val; }
float Gun::get_recoil() const { return recoil; }

void Gun::set_aim_ray(RayCast3D *p_val) { aim_ray = p_val; }
RayCast3D *Gun::get_aim_ray() const { return aim_ray; }

void Gun::set_damage(float p_val) { damage = p_val; }
float Gun::get_damage() const { return damage; }

void Gun::set_bullet_trail(BulletTrail *p_val) { bullet_trail = p_val; }
BulletTrail *Gun::get_bullet_trail() const { return bullet_trail; }

void Gun::set_shoot_sfx(AudioStreamPlayer3D *p_val) { shoot_sfx = p_val; }
AudioStreamPlayer3D *Gun::get_shoot_sfx() const { return shoot_sfx; }

void Gun::set_reload_sfx(AudioStreamPlayer3D *p_val) { reload_sfx = p_val; }
AudioStreamPlayer3D *Gun::get_reload_sfx() const { return reload_sfx; }

void Gun::set_magzine_size(int p_val) { magzine_size = p_val; }
int Gun::get_magzine_size() const { return magzine_size; }

void Gun::set_reload_time(float p_val) { reload_time = p_val; }
float Gun::get_reload_time() const { return reload_time; }

void Gun::set_pull_out_time(float p_val) { pull_out_time = p_val; }
float Gun::get_pull_out_time() const { return pull_out_time; }

void Gun::set_default_position(Vector3 p_val) { default_position = p_val; }
Vector3 Gun::get_default_position() const { return default_position; }

void Gun::set_ads_position(Vector3 p_val) { ads_position = p_val; }
Vector3 Gun::get_ads_position() const { return ads_position; }

void Gun::set_ads_acceleration(float p_val) { ads_acceleration = p_val; }
float Gun::get_ads_acceleration() const { return ads_acceleration; }

void Gun::set_default_fov(float p_val) { default_fov = p_val; }
float Gun::get_default_fov() const { return default_fov; }

void Gun::set_ads_fov(float p_val) { ads_fov = p_val; }
float Gun::get_ads_fov() const { return ads_fov; }

void Gun::set_camera(Camera3D *p_val) { camera = p_val; }
Camera3D *Gun::get_camera() const { return camera; }

void Gun::set_gun_camera(Camera3D *p_val) { gun_camera = p_val; }
Camera3D *Gun::get_gun_camera() const { return gun_camera; }

void Gun::set_gun_default_fov(float p_val) { gun_default_fov = p_val; }
float Gun::get_gun_default_fov() const { return gun_default_fov; }

void Gun::set_gun_ads_fov(float p_val) { gun_ads_fov = p_val; }
float Gun::get_gun_ads_fov() const { return gun_ads_fov; }

void Gun::set_magize_label(Label *p_val) { magize_label = p_val; }
Label *Gun::get_magize_label() const { return magize_label; }

void Gun::shoot(double delta) {
    hit = false;
    shot = false;

    if (magize_label) {
        magize_label->set_text(String::num_int64(current_magzine));
    }

    rate += delta;
    if (rate >= 1.0 / fire_rate && !reloading) {
        shot = true;

        if (magize_label) {
            if (current_magzine < 1) {
                clip_empty = true;
            } else {
                current_magzine -= 1;
            }
        }

        // Handle RayCast collision
        if (aim_ray && aim_ray->is_colliding()) {
            Node *collider = Object::cast_to<Node>(aim_ray->get_collider());
            if (collider && collider->is_in_group("enemy")) {
                // Call `_hit` on collider
                collider->call("_hit", gun_holder, damage);

                if (gun_holder) {
                    gun_holder->register_damage(damage, collider);
                }
                hit = true;
            }
        }

        // Draw bullet trail
        if (bullet_trail && aim_ray) {
            Vector3 bullet_target_pos = aim_ray->get_global_transform().xform(aim_ray->get_target_position());
            bullet_trail->make(bullet_target_pos);
        }

        rate = 0;
    }
}

// Aim function

void Gun::aim(double delta, bool ADS) {
    if (!camera || !gun_camera) return;

    if (ADS) {
        set_position(get_position().lerp(ads_position, ads_acceleration));
        camera->set_fov(Math::lerp(camera->get_fov(), ads_fov, ads_acceleration));
        gun_camera->set_fov(gun_ads_fov);
    } else {
        set_position(get_position().lerp(default_position, ads_acceleration));
        camera->set_fov(Math::lerp(camera->get_fov(), default_fov, ads_acceleration));
        gun_camera->set_fov(gun_default_fov);
    }
}

// Reload function

void Gun::reload(bool can_reload) {
    if (can_reload) {
        reloading = true;
        UtilityFunctions::print("RELOADED");
        current_magzine = magzine_size;
        reloading = false;
        clip_empty = false;
    }
}


// --- Binding for Inspector ---
void Gun::_bind_methods() {

    ClassDB::bind_method(D_METHOD("shoot", "delta"), &Gun::shoot);
    ClassDB::bind_method(D_METHOD("aim", "delta", "ADS"), &Gun::aim);
    ClassDB::bind_method(D_METHOD("reload", "can_reload"), &Gun::reload);

    // Export Variables
    ClassDB::bind_method(D_METHOD("set_animation_player", "animation_player"), &Gun::set_animation_player);
    ClassDB::bind_method(D_METHOD("get_animation_player"), &Gun::get_animation_player);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "animation_player", PROPERTY_HINT_NODE_TYPE, "AnimationPlayer"), "set_animation_player", "get_animation_player");

    ClassDB::bind_method(D_METHOD("set_fire_rate", "fire_rate"), &Gun::set_fire_rate);
    ClassDB::bind_method(D_METHOD("get_fire_rate"), &Gun::get_fire_rate);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "fire_rate"), "set_fire_rate", "get_fire_rate");

    ClassDB::bind_method(D_METHOD("set_recoil", "recoil"), &Gun::set_recoil);
    ClassDB::bind_method(D_METHOD("get_recoil"), &Gun::get_recoil);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "recoil"), "set_recoil", "get_recoil");

    ClassDB::bind_method(D_METHOD("set_aim_ray", "aim_ray"), &Gun::set_aim_ray);
    ClassDB::bind_method(D_METHOD("get_aim_ray"), &Gun::get_aim_ray);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "aim_ray", PROPERTY_HINT_NODE_TYPE, "RayCast3D"), "set_aim_ray", "get_aim_ray");

    ClassDB::bind_method(D_METHOD("set_damage", "damage"), &Gun::set_damage);
    ClassDB::bind_method(D_METHOD("get_damage"), &Gun::get_damage);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "damage"), "set_damage", "get_damage");

    ClassDB::bind_method(D_METHOD("set_bullet_trail", "bullet_trail"), &Gun::set_bullet_trail);
    ClassDB::bind_method(D_METHOD("get_bullet_trail"), &Gun::get_bullet_trail);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "bullet_trail", PROPERTY_HINT_NODE_TYPE, "BulletTrail"), "set_bullet_trail", "get_bullet_trail");

    ClassDB::bind_method(D_METHOD("set_shoot_sfx", "shoot_sfx"), &Gun::set_shoot_sfx);
    ClassDB::bind_method(D_METHOD("get_shoot_sfx"), &Gun::get_shoot_sfx);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "shoot_sfx", PROPERTY_HINT_NODE_TYPE, "AudioStreamPlayer3D"), "set_shoot_sfx", "get_shoot_sfx");

    ClassDB::bind_method(D_METHOD("set_reload_sfx", "reload_sfx"), &Gun::set_reload_sfx);
    ClassDB::bind_method(D_METHOD("get_reload_sfx"), &Gun::get_reload_sfx);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "reload_sfx", PROPERTY_HINT_NODE_TYPE, "AudioStreamPlayer3D"), "set_reload_sfx", "get_reload_sfx");

    ClassDB::bind_method(D_METHOD("set_magzine_size", "magzine_size"), &Gun::set_magzine_size);
    ClassDB::bind_method(D_METHOD("get_magzine_size"), &Gun::get_magzine_size);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "magzine_size"), "set_magzine_size", "get_magzine_size");

    ClassDB::bind_method(D_METHOD("set_reload_time", "reload_time"), &Gun::set_reload_time);
    ClassDB::bind_method(D_METHOD("get_reload_time"), &Gun::get_reload_time);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "reload_time"), "set_reload_time", "get_reload_time");

    ClassDB::bind_method(D_METHOD("set_pull_out_time", "pull_out_time"), &Gun::set_pull_out_time);
    ClassDB::bind_method(D_METHOD("get_pull_out_time"), &Gun::get_pull_out_time);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "pull_out_time"), "set_pull_out_time", "get_pull_out_time");

    ClassDB::bind_method(D_METHOD("set_default_position", "default_position"), &Gun::set_default_position);
    ClassDB::bind_method(D_METHOD("get_default_position"), &Gun::get_default_position);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "default_position"), "set_default_position", "get_default_position");

    ClassDB::bind_method(D_METHOD("set_ads_position", "ads_position"), &Gun::set_ads_position);
    ClassDB::bind_method(D_METHOD("get_ads_position"), &Gun::get_ads_position);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "ads_position"), "set_ads_position", "get_ads_position");

    ClassDB::bind_method(D_METHOD("set_ads_acceleration", "ads_acceleration"), &Gun::set_ads_acceleration);
    ClassDB::bind_method(D_METHOD("get_ads_acceleration"), &Gun::get_ads_acceleration);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "ads_acceleration"), "set_ads_acceleration", "get_ads_acceleration");

    ClassDB::bind_method(D_METHOD("set_default_fov", "default_fov"), &Gun::set_default_fov);
    ClassDB::bind_method(D_METHOD("get_default_fov"), &Gun::get_default_fov);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "default_fov"), "set_default_fov", "get_default_fov");

    ClassDB::bind_method(D_METHOD("set_ads_fov", "ads_fov"), &Gun::set_ads_fov);
    ClassDB::bind_method(D_METHOD("get_ads_fov"), &Gun::get_ads_fov);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "ads_fov"), "set_ads_fov", "get_ads_fov");

    ClassDB::bind_method(D_METHOD("set_camera", "camera"), &Gun::set_camera);
    ClassDB::bind_method(D_METHOD("get_camera"), &Gun::get_camera);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "camera", PROPERTY_HINT_NODE_TYPE, "Camera3D"), "set_camera", "get_camera");

    ClassDB::bind_method(D_METHOD("set_gun_camera", "gun_camera"), &Gun::set_gun_camera);
    ClassDB::bind_method(D_METHOD("get_gun_camera"), &Gun::get_gun_camera);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "gun_camera", PROPERTY_HINT_NODE_TYPE, "Camera3D"), "set_gun_camera", "get_gun_camera");

    ClassDB::bind_method(D_METHOD("set_gun_default_fov", "gun_default_fov"), &Gun::set_gun_default_fov);
    ClassDB::bind_method(D_METHOD("get_gun_default_fov"), &Gun::get_gun_default_fov);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "gun_default_fov"), "set_gun_default_fov", "get_gun_default_fov");

    ClassDB::bind_method(D_METHOD("set_gun_ads_fov", "gun_ads_fov"), &Gun::set_gun_ads_fov);
    ClassDB::bind_method(D_METHOD("get_gun_ads_fov"), &Gun::get_gun_ads_fov);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "gun_ads_fov"), "set_gun_ads_fov", "get_gun_ads_fov");

    ClassDB::bind_method(D_METHOD("set_magize_label", "magize_label"), &Gun::set_magize_label);
    ClassDB::bind_method(D_METHOD("get_magize_label"), &Gun::get_magize_label);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "magize_label", PROPERTY_HINT_NODE_TYPE, "Label"), "set_magize_label", "get_magize_label");
}
