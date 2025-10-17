#pragma once
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/ray_cast3d.hpp>
#include <godot_cpp/classes/label.hpp>


#include "WeaponResource.h"

using namespace godot;

class GunWeaponResource : public WeaponResource {
    GDCLASS(GunWeaponResource, WeaponResource);
public:
    GunWeaponResource();
    ~GunWeaponResource();
    // Overriden functions
    void Enter() override;
    void Update(double delta) override;
    void Exit() override;
    void trigger_down() override;
    void trigger_up() override;
    void setup(PlayerWeaponManager* owner) override;
    
    // Class functions
    int get_amount_can_reload();
    void reload_pressed();
    void reload_update();
    void reload();
    Ref<PackedScene> reload_ui;
    Label* reload_label = nullptr;
    bool reloading = false;

    void shoot();
    void ADS();
    
protected:
    static void _bind_methods();

private:
    // Export Variables
    bool is_semi_auto = false;
    // Gun varaibles
    float fire_rate = 1.0f;
    float gun_damage = 25.0f;

    float reload_time = 1.0f;
    int current_ammo = 30;
    int magazine_capacity = 30;
    int reserve_ammo = 999;
    int max_reserve_ammo = 999;

   
    // Animation
    StringName shoot_anim_name;
    StringName reload_anim_name;

    // Audio
    Ref<AudioStream> shoot_audio;
    Ref<AudioStream> reload_audio;
    
    // Class Variables
    float last_fire_time = 0.0f;
    Ref<SceneTreeTimer> reload_timer;


public:

    void set_is_semi_auto(bool p_value) { is_semi_auto = p_value; }
    bool get_is_semi_auto() const { return is_semi_auto; }

    void set_fire_rate(float p_value) { fire_rate = p_value; }
    float get_fire_rate() const { return fire_rate; }

    void set_shoot_anim_name(const StringName &p_value) { shoot_anim_name = p_value; }
    StringName get_shoot_anim_name() const { return shoot_anim_name; }

    void set_reload_anim_name(const StringName &p_value) { reload_anim_name = p_value; }
    StringName get_reload_anim_name() const { return reload_anim_name; }

    void set_shoot_audio(const Ref<AudioStream> &p_value) { shoot_audio = p_value; }
    Ref<AudioStream> get_shoot_audio() const { return shoot_audio; }

    void set_reload_audio(const Ref<AudioStream> &p_value) { reload_audio = p_value; }
    Ref<AudioStream> get_reload_audio() const { return reload_audio; }  

    void set_gun_damage(float p_gun_damage) { gun_damage = p_gun_damage; }
    float get_gun_damage() const { return gun_damage; }

    // --- reload_time ---
    void set_reload_time(float p_reload_time) { reload_time = p_reload_time; }
    float get_reload_time() const { return reload_time; }

    // --- current_ammo ---
    void set_current_ammo(int p_current_ammo) { current_ammo = p_current_ammo; }
    int get_current_ammo() const { return current_ammo; }

    // --- magazine_capacity ---
    void set_magazine_capacity(int p_magazine_capacity) { magazine_capacity = p_magazine_capacity; }
    int get_magazine_capacity() const { return magazine_capacity; }

    // --- reserve_ammo ---
    void set_reserve_ammo(int p_reserve_ammo) { reserve_ammo = p_reserve_ammo; }
    int get_reserve_ammo() const { return reserve_ammo; }

    // --- max_reserve_ammo ---
    void set_max_reserve_ammo(int p_max_reserve_ammo) { max_reserve_ammo = p_max_reserve_ammo; }
    int get_max_reserve_ammo() const { return max_reserve_ammo; }

    void set_reload_ui(const Ref<PackedScene> &p_reload_ui) { reload_ui = p_reload_ui; }
    Ref<PackedScene> get_reload_ui() const { return reload_ui; }

};