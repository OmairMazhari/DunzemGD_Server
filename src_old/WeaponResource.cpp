#include "WeaponResource.h"
#include "Global.hpp"
#include "godot_cpp/classes/animation.hpp"
#include "godot_cpp/classes/timer.hpp"

using namespace godot;

WeaponResource::WeaponResource() {

}

WeaponResource::~WeaponResource() {

}

void WeaponResource::setup(PlayerWeaponManager* owner) {
// Weapon view model could be any node, node2d, 1d so we make sure that the root node is node3d before 
        // using it, in the if statement prior we checked if view_model is assigned to anything at all first.
        // We do this check only for the view model and not for container, because view model is a packed scene
        // and the root is unknown but for container we explicitly defined it in code as a Node3D.
        this->owner = owner;
        if(view_model_scene.is_valid()){
            view_model_instance = Object::cast_to<Node3D>(view_model_scene->instantiate());
        }
        if(owner){
            view_model_container = owner->get_view_model_container();
            // Setup Audio: audio player is a child of PlayerWeaponManager
            audio_stream_player = owner->get_node<AudioStreamPlayer>("AudioStreamPlayer3D");
            // Note: shared bullet ray_cast across all weapons
            bullet_ray_cast = owner->get_bullet_ray_cast();
        }
        if(view_model_instance && view_model_container){
            // Setup view model
            view_model_container->add_child(view_model_instance);
            view_model_instance->set_position(view_model_position);
            view_model_instance->set_rotation(view_model_rotation);
            view_model_instance->set_scale(view_model_scale);
            // Setup animation
            anim_player = view_model_instance->get_node<AnimationPlayer>("AnimationPlayer");
            // Not visible initially
            view_model_instance->set_visible(false);
        }
}

void WeaponResource::Enter() {
    // Start timer for equipping
    if(owner){
        equip_timer = owner->get_tree()->create_timer(equip_time);
    } else {
        UtilityFunctions::print("No owner");
    }
    // Show view model
    if(view_model_instance){
        view_model_instance->set_visible(true);
    }
    // Play audio and animation
    play_anim(equip_anim_name, get_anim_length(equip_anim_name) / equip_time);
    play_audio(equip_audio, equip_audio->get_length() / equip_time );
}

void WeaponResource::Update(double delta) {
    SETUP_INPUT
}

void WeaponResource::Exit() {
    play_anim(unequip_anim_name, get_anim_length(unequip_anim_name) / unequip_time);
    // play_audio(unequip_audio, unequip_audio->get_length() / unequip_time);
    if(view_model_instance){
        view_model_instance->set_visible(false);
    }
    // Stop the anim_player if switching weapons, no need to do this with audiostreamplayer because the audiostreamplayer
    // is shared across all the weapons
    anim_player->stop();
}

void WeaponResource::trigger_up() {

}

void WeaponResource::trigger_down() {
}

void WeaponResource::play_anim(String name, float speed_scale) {
    if(anim_player){
        if(anim_player->has_animation(name)){
            if(anim_player->is_playing()){
                anim_player->stop();
            }
            anim_player->play(name, -1.0, speed_scale);
    
        } else {
            UtilityFunctions::print("Animation: " + name + ", does not exist");
        }
    } else {
        UtilityFunctions::print("Animation player null");
    }
}

void WeaponResource::queue_anim(String name, float speed_scale) {
    if(anim_player){
        if(anim_player->has_animation(name)){
            anim_player->set_speed_scale(speed_scale);
            anim_player->queue(name);
            anim_player->set_speed_scale(1.0f);
        } else {
            UtilityFunctions::print("Animation: " + name + ", does not exist");
        }
    } else {
        UtilityFunctions::print("Animation player null");
    }
}

void WeaponResource::play_audio(Ref<AudioStream> stream, float pitch_scale) {
    if(audio_stream_player) {
        if (audio_stream_player->get_stream() != stream){
            audio_stream_player->set_stream(stream);
        }
        audio_stream_player->set_pitch_scale(pitch_scale);
        audio_stream_player->play();
    } else {
        UtilityFunctions::print("Audio Stream Player null");
    }
}


bool WeaponResource::has_equiped() {
	return equip_timer.is_valid() && !equip_timer->get_time_left();
}

float WeaponResource::get_anim_length(String name) {
    if(anim_player){
        if(anim_player->has_animation(name)){
           return anim_player->get_animation(name)->get_length();
        } else {
            UtilityFunctions::print("Animation: " + name + ": does not exist");
        }
    } else {
        UtilityFunctions::print("Animation player null");
    }
    return 1.0f;
}

void WeaponResource::_bind_methods() {
    
    ADD_GROUP("View Model", "view_");
    // view_model_container
    // ClassDB::bind_method(D_METHOD("set_view_model_container", "p_view_model_container"), &WeaponResource::set_view_model_container);
    // ClassDB::bind_method(D_METHOD("get_view_model_container"), &WeaponResource::get_view_model_container);
    // ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "view_model_container", PROPERTY_HINT_RESOURCE_TYPE, "PackedScene"), "set_view_model_container", "get_view_model_container");
    // view_model_position
    ClassDB::bind_method(D_METHOD("set_view_model_position", "p_view_model_position"), &WeaponResource::set_view_model_position);
    ClassDB::bind_method(D_METHOD("get_view_model_position"), &WeaponResource::get_view_model_position);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "view_model_position"), "set_view_model_position", "get_view_model_position");
    // view_model_rotation
    ClassDB::bind_method(D_METHOD("set_view_model_rotation", "p_view_model_rotation"), &WeaponResource::set_view_model_rotation);
    ClassDB::bind_method(D_METHOD("get_view_model_rotation"), &WeaponResource::get_view_model_rotation);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "view_model_rotation"), "set_view_model_rotation", "get_view_model_rotation");
    // view_model_scale
    ClassDB::bind_method(D_METHOD("set_view_model_scale", "p_view_model_scale"), &WeaponResource::set_view_model_scale);
    ClassDB::bind_method(D_METHOD("get_view_model_scale"), &WeaponResource::get_view_model_scale);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "view_model_scale"), "set_view_model_scale", "get_view_model_scale");
    // view_model_scene
    ClassDB::bind_method(D_METHOD("set_view_model_scene", "p_view_model_scene"), &WeaponResource::set_view_model_scene);
    ClassDB::bind_method(D_METHOD("get_view_model_scene"), &WeaponResource::get_view_model_scene);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "view_model_scene", PROPERTY_HINT_RESOURCE_TYPE, "PackedScene"), "set_view_model_scene", "get_view_model_scene");


    ADD_GROUP("World Model", "world_");
    // world_model_container
    ClassDB::bind_method(D_METHOD("set_world_model_container", "p_world_model_container"), &WeaponResource::set_world_model_container);
    ClassDB::bind_method(D_METHOD("get_world_model_container"), &WeaponResource::get_world_model_container);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "world_model_container", PROPERTY_HINT_NODE_TYPE, "Node"), "set_world_model_container", "get_world_model_container");
    // world_model_position
    ClassDB::bind_method(D_METHOD("set_world_model_position", "p_world_model_position"), &WeaponResource::set_world_model_position);
    ClassDB::bind_method(D_METHOD("get_world_model_position"), &WeaponResource::get_world_model_position);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "world_model_position"), "set_world_model_position", "get_world_model_position");
    // world_model_rotation
    ClassDB::bind_method(D_METHOD("set_world_model_rotation", "p_world_model_rotation"), &WeaponResource::set_world_model_rotation);
    ClassDB::bind_method(D_METHOD("get_world_model_rotation"), &WeaponResource::get_world_model_rotation);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "world_model_rotation"), "set_world_model_rotation", "get_world_model_rotation");
    // world_model_scale
    ClassDB::bind_method(D_METHOD("set_world_model_scale", "p_world_model_scale"), &WeaponResource::set_world_model_scale);
    ClassDB::bind_method(D_METHOD("get_world_model_scale"), &WeaponResource::get_world_model_scale);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "world_model_scale"), "set_world_model_scale", "get_world_model_scale");

    ADD_GROUP("Animation", "_");
    // equip_anim_name
    ClassDB::bind_method(D_METHOD("set_equip_anim_name", "p_name"), &WeaponResource::set_equip_anim_name);
    ClassDB::bind_method(D_METHOD("get_equip_anim_name"), &WeaponResource::get_equip_anim_name);
    ADD_PROPERTY(PropertyInfo(Variant::STRING_NAME, "equip_anim_name"),"set_equip_anim_name", "get_equip_anim_name");
    // unequip_anim_name
    ClassDB::bind_method(D_METHOD("set_unequip_anim_name", "p_name"), &WeaponResource::set_unequip_anim_name);
    ClassDB::bind_method(D_METHOD("get_unequip_anim_name"), &WeaponResource::get_unequip_anim_name);
    ADD_PROPERTY(PropertyInfo(Variant::STRING_NAME, "unequip_anim_name"), "set_unequip_anim_name", "get_unequip_anim_name");
   
    ADD_GROUP("Audio", "world_");
    // equip_audio
    ClassDB::bind_method(D_METHOD("set_equip_audio", "p_audio"), &WeaponResource::set_equip_audio);
    ClassDB::bind_method(D_METHOD("get_equip_audio"), &WeaponResource::get_equip_audio);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "equip_audio", PROPERTY_HINT_RESOURCE_TYPE, "AudioStream"),"set_equip_audio", "get_equip_audio");
    // unequip_audio
    ClassDB::bind_method(D_METHOD("set_unequip_audio", "p_audio"), &WeaponResource::set_unequip_audio);
    ClassDB::bind_method(D_METHOD("get_unequip_audio"), &WeaponResource::get_unequip_audio);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "unequip_audio", PROPERTY_HINT_RESOURCE_TYPE, "AudioStream"), "set_unequip_audio", "get_unequip_audio");

    // equip_time
    ClassDB::bind_method(D_METHOD("set_equip_time", "p_equip_time"), &WeaponResource::set_equip_time);
    ClassDB::bind_method(D_METHOD("get_equip_time"), &WeaponResource::get_equip_time);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "equip_time"), "set_equip_time", "get_equip_time");
    // unequip_time
    ClassDB::bind_method(D_METHOD("set_unequip_time", "p_unequip_time"), &WeaponResource::set_unequip_time);
    ClassDB::bind_method(D_METHOD("get_unequip_time"), &WeaponResource::get_unequip_time);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "unequip_time"), "set_unequip_time", "get_unequip_time");

}