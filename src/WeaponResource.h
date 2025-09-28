#pragma once
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/animation_player.hpp>
#include <godot_cpp/classes/audio_stream.hpp>
#include <godot_cpp/classes/audio_stream_player3d.hpp>
#include <godot_cpp/classes/audio_stream_player.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/scene_tree_timer.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/ray_cast3d.hpp>

#include "Global.hpp"
#include "PlayerWeaponManager.h"


using namespace godot;

class PlayerWeaponManager;

class WeaponResource : public Resource {
    GDCLASS(WeaponResource, Resource);
public:
    WeaponResource();
    ~WeaponResource();

    // Override functions
    virtual void Enter();
    virtual void Update(double delta);
    virtual void Exit();
    virtual void trigger_up();
    virtual void trigger_down();
    virtual void setup(PlayerWeaponManager* owner);

    // Class functions
    void play_anim(String name, float speed_scale = 1.0f);
    void queue_anim(String name, float speed_scale = 1.0f);
    float get_anim_length(String name);
    void play_audio(Ref<AudioStream> stream, float pitch_scale = 1.0f);

    bool has_equiped();
    
protected:
    // Export

    // World model
    Node3D* world_model_container = nullptr;
    Vector3 world_model_position = Vector3();
    Vector3 world_model_rotation = Vector3();
    Vector3 world_model_scale = Vector3(1.0f, 1.0f, 1.0f);
    
    // View model
    Node3D* view_model_container = nullptr;
    Vector3 view_model_position = Vector3();
    Vector3 view_model_rotation = Vector3();
    Vector3 view_model_scale = Vector3(1.0f, 1.0f, 1.0f);
    Ref<PackedScene> view_model_scene;
    Node3D* view_model_instance = nullptr;

    // Animation
    AnimationPlayer* anim_player = nullptr;
    StringName equip_anim_name;
    StringName unequip_anim_name;

    // Audio
    AudioStreamPlayer* audio_stream_player = nullptr;
    Ref<AudioStream> equip_audio;
    Ref<AudioStream> unequip_audio;

    // Weapon 
    float equip_time = 1.0f;
    float unequip_time = 1.0f;
   

    // Class Variables
    bool can_shoot = true;
    Ref<SceneTreeTimer> equip_timer;
    PlayerWeaponManager* owner = nullptr;
    RayCast3D* bullet_ray_cast = nullptr;
    // AnimationPlayer* anim_player;
    // AudioStreamPlayer* equip_sfx;
    // AudioStreamPlayer* unequip_sfx;
     
protected:
    static void _bind_methods();
public:
// Setter and getters

    // void set_view_model_container(Ref<PackedScene> p_view_model_container) { view_model_container = p_view_model_container; }
    // Ref<PackedScene> get_view_model_container() const { return view_model_container; }
    
    void set_world_model_container(Node3D* p_world_model_container) { world_model_container = p_world_model_container; }
    Node3D* get_world_model_container() const { return world_model_container; }

    void set_view_model_position(Vector3 p_view_model_position) { 
        view_model_position = p_view_model_position; 
        UtilityFunctions::print(view_model_position);
    }
    Vector3 get_view_model_position() const { return view_model_position; }

    void set_view_model_rotation(Vector3 p_view_model_rotation) { view_model_rotation = p_view_model_rotation; }
    Vector3 get_view_model_rotation() const { return view_model_rotation; }

    void set_view_model_scale(Vector3 p_view_model_scale) { view_model_scale = p_view_model_scale; }
    Vector3 get_view_model_scale() const { return view_model_scale; }

    void set_world_model_position(Vector3 p_world_model_position) { world_model_position = p_world_model_position; }
    Vector3 get_world_model_position() const { return world_model_position; }

    void set_world_model_rotation(Vector3 p_world_model_rotation) { world_model_rotation = p_world_model_rotation; }
    Vector3 get_world_model_rotation() const { return world_model_rotation; }

    void set_world_model_scale(Vector3 p_world_model_scale) { world_model_scale = p_world_model_scale; }
    Vector3 get_world_model_scale() const { return world_model_scale; }

    void set_view_model_scene(Ref<PackedScene> p_view_model_scene) { view_model_scene = p_view_model_scene; }
    Ref<PackedScene> get_view_model_scene() const { return view_model_scene; }

    // Animation Player
    void set_anim_player(AnimationPlayer* p_player) { anim_player = p_player; }
    AnimationPlayer* get_anim_player() const { return anim_player; }

    // Equip animation name
    void set_equip_anim_name(const StringName &p_name) { equip_anim_name = p_name; }
    StringName get_equip_anim_name() const { return equip_anim_name; }

    // Unequip animation name
    void set_unequip_anim_name(const StringName &p_name) { unequip_anim_name = p_name; }
    StringName get_unequip_anim_name() const { return unequip_anim_name; }

    // Audio Stream Player
    void set_audio_stream_player(AudioStreamPlayer* p_audio_stream_player) { audio_stream_player = p_audio_stream_player; }
    AudioStreamPlayer* get_audio_stream_player() const { return audio_stream_player; }

    // Equip audio
    void set_equip_audio(const Ref<AudioStream> &p_audio) { equip_audio = p_audio; }
    Ref<AudioStream> get_equip_audio() const { return equip_audio; }

    // Unequip audio
    void set_unequip_audio(const Ref<AudioStream> &p_audio) { unequip_audio = p_audio; }
    Ref<AudioStream> get_unequip_audio() const { return unequip_audio; }

    // View Model Instance
    void set_view_model_instance(Node3D* p_instance) { view_model_instance = p_instance; }
    Node3D* get_view_model_instance() { return view_model_instance; }

    void set_equip_time(float p_equip_time) { equip_time = p_equip_time; }
    float get_equip_time() const { return equip_time; }

    void set_unequip_time(float p_unequip_time) { unequip_time = p_unequip_time; }
    float get_unequip_time() const { return unequip_time; }

    void set_owner(PlayerWeaponManager* p_val) { owner = p_val; }
    PlayerWeaponManager* get_owner() { return owner; }

    void set_bullet_ray_cast(RayCast3D* p_bullet_ray_cast) { bullet_ray_cast = p_bullet_ray_cast; }
    RayCast3D* get_bullet_ray_cast() const { return bullet_ray_cast; }

};