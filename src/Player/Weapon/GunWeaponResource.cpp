#include "GunWeaponResource.h"
#include "godot_cpp/classes/time.hpp"
#include "godot_cpp/classes/rigid_body3d.hpp"
#include <Player/FPSController.h>
#include <godot_cpp/classes/area3d.hpp>

using namespace godot;

GunWeaponResource::GunWeaponResource() {
    // both pointers will point to the same ray_cast
    //
}

GunWeaponResource::~GunWeaponResource() {}


void GunWeaponResource::Setup(ResourceFSM *owner) {
    WeaponResource::Setup(owner);
    if(reload_ui.is_valid()){
        reload_label = Object::cast_to<Label>(reload_ui->instantiate());
        owner->add_child(reload_label);
    }
}

bool GunWeaponResource::got_hit() {
	return on_hit;
}

void GunWeaponResource::Enter() {
    WeaponResource::Enter();
    reload_label->set_visible(true);
}

void GunWeaponResource::Update(double delta) {
    UtilityFunctions::print("reached A");
    WeaponResource::Update(delta);
    
    Dictionary event_input = input["event_based_actions"];
    Dictionary state_input = input["state_based_actions"];

    if(!has_equiped()) return;
    if(reloading) {
        reload_update();
        return;
    }
    if(reload_label){
        reload_label->set_text(String((std::to_string(current_ammo) + "/" + std::to_string(magazine_capacity) + " Total Ammo: " + std::to_string(reserve_ammo)).c_str()));
       
    }
    UtilityFunctions::print("reached B");
    // Handle primary fire
    if(is_semi_auto){
        if(event_input["left_click"]) {
            trigger_down();
        }
    } else {
        if(state_input["left_click"]) {
            trigger_down();
        }
    }
    
   if(is_semi_auto){
        if(!event_input["left_click"]) {
            trigger_up();
        }
    } else {
        if(!state_input["left_click"]) {
            trigger_up();
        }
    }

    UtilityFunctions::print("reached C");
    // Handle reload
    if (event_input["reload"] ) {
        reload_pressed();
    }
   UtilityFunctions::print("reached D");

}

void GunWeaponResource::Exit() {
    WeaponResource::Exit();
    if(reload_label){
        reload_label->set_visible(false);
    }
    reloading = false;
}

// This function handles when the shoot function should be called, for example, dont call 
// shoot funciton when reloading or switching
void GunWeaponResource::trigger_down() {
    if(Time::get_singleton()->get_ticks_msec() - last_fire_time >= fire_rate && current_ammo > 0){
        shoot();
    }
}

void GunWeaponResource::trigger_up() {
}


void GunWeaponResource::shoot() {
    UtilityFunctions::print("ha");
    play_anim(shoot_anim_name);
    play_audio(shoot_audio);
   


    // Handle bulelt collision
    if (bullet_ray_cast->is_colliding()){

         UtilityFunctions::print("mi");
        Object* obj = bullet_ray_cast->get_collider();
        Vector3 normal = bullet_ray_cast->get_collision_normal();
        Vector3 point = bullet_ray_cast->get_collision_point();
        RigidBody3D* rigidBody = Object::cast_to<RigidBody3D>(obj);
        if(rigidBody) {
            rigidBody->apply_impulse(-normal * 50.0f / rigidBody->get_mass(), point - rigidBody->get_global_position());
        }

        Area3D* area= Object::cast_to<Area3D>(obj);


        if(area){
            FPSController* enemy_player = Object::cast_to<FPSController>(area->get_parent()->get_parent()->get_parent()->get_parent()->get_parent()->get_parent()->get_parent());

            if(enemy_player) {
                 UtilityFunctions::print("mi");
                if(enemy_player != player){
                // Tell gun owner they got a hit 
                player->set_hit(true);
    
                Dictionary combat_report;
                combat_report["kill"] = false;
                combat_report["damage"] = 0;
                combat_report["body_part"]  = "undef";

                if (area->is_in_group("Head")){
                    combat_report["damage"] = gun_damage * 2.0;
                    combat_report["body_part"]  = "head";

                } else if (area->is_in_group("Body")) {
                    combat_report["damage"] = gun_damage;
                    combat_report["body_part"]  = "body";

                } else if (area->is_in_group("Leg")) { 
                    combat_report["damage"] = gun_damage * 0.5;
                    combat_report["body_part"]  = "leg";
                }
               
                if(enemy_player->take_damage(combat_report["damage"])){
                    player->set_kills(player->get_kills() + 1);
                    combat_report["kill"] = true;
                }

                player->add_combat_report(combat_report);
                UtilityFunctions::print("Damage done to player");
                }
                 UtilityFunctions::print("mi");
                
            }
        }
        
    }
    last_fire_time = Time::get_singleton()->get_ticks_msec();
    current_ammo -= 1;
    UtilityFunctions::print("mi");
}

int GunWeaponResource::get_amount_can_reload() {
    return MIN(magazine_capacity - current_ammo, reserve_ammo);
}


/*
This function is there to start the reloading process,
once started the reload_update function waits for the animation to be finished
and then calls the reload functoin appropriately to refill the magzine
*/
void GunWeaponResource::reload_pressed() {
	if(get_amount_can_reload() == 0 || reloading) return;
    else {
        reloading = true;
        // Set reload timer
        play_anim(reload_anim_name, get_anim_length(reload_anim_name) / reload_time );
        UtilityFunctions::print(get_anim_length(reload_anim_name));
        UtilityFunctions::print(reload_time);
        
        play_audio(reload_audio, reload_audio->get_length() / reload_time );
        reload_timer = owner->get_tree()->create_timer(reload_time);
        
    }
}

void GunWeaponResource::reload_update() {
    if(!reload_timer->get_time_left()){
        reload();
    }
}

void GunWeaponResource::reload() {
    int reload_amount = get_amount_can_reload();
    current_ammo += reload_amount;
    reserve_ammo -= reload_amount;
    reloading = false;
}

void GunWeaponResource::ADS() {

}

void GunWeaponResource::_bind_methods() {
    // got_hit
    ClassDB::bind_method(D_METHOD("got_hit"), &GunWeaponResource::got_hit);
    // Bool
    ClassDB::bind_method(D_METHOD("set_is_semi_auto", "value"), &GunWeaponResource::set_is_semi_auto);
    ClassDB::bind_method(D_METHOD("get_is_semi_auto"), &GunWeaponResource::get_is_semi_auto);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_semi_auto"), "set_is_semi_auto", "get_is_semi_auto");
    // Fire rate
    ClassDB::bind_method(D_METHOD("set_fire_rate", "value"), &GunWeaponResource::set_fire_rate);
    ClassDB::bind_method(D_METHOD("get_fire_rate"), &GunWeaponResource::get_fire_rate);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "fire_rate"), "set_fire_rate", "get_fire_rate");

    // Animations
    ClassDB::bind_method(D_METHOD("set_shoot_anim_name", "value"), &GunWeaponResource::set_shoot_anim_name);
    ClassDB::bind_method(D_METHOD("get_shoot_anim_name"), &GunWeaponResource::get_shoot_anim_name);
    ADD_PROPERTY(PropertyInfo(Variant::STRING_NAME, "shoot_anim_name"), "set_shoot_anim_name", "get_shoot_anim_name");

    ClassDB::bind_method(D_METHOD("set_reload_anim_name", "value"), &GunWeaponResource::set_reload_anim_name);
    ClassDB::bind_method(D_METHOD("get_reload_anim_name"), &GunWeaponResource::get_reload_anim_name);
    ADD_PROPERTY(PropertyInfo(Variant::STRING_NAME, "reload_anim_name"), "set_reload_anim_name", "get_reload_anim_name");

    // Audio
    ClassDB::bind_method(D_METHOD("set_shoot_audio", "value"), &GunWeaponResource::set_shoot_audio);
    ClassDB::bind_method(D_METHOD("get_shoot_audio"), &GunWeaponResource::get_shoot_audio);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "shoot_audio", PROPERTY_HINT_RESOURCE_TYPE, "AudioStream"), "set_shoot_audio", "get_shoot_audio");
   
    ClassDB::bind_method(D_METHOD("set_reload_audio", "value"), &GunWeaponResource::set_reload_audio);
    ClassDB::bind_method(D_METHOD("get_reload_audio"), &GunWeaponResource::get_reload_audio);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "reload_audio", PROPERTY_HINT_RESOURCE_TYPE, "AudioStream"), "set_reload_audio", "get_reload_audio");

    ClassDB::bind_method(D_METHOD("set_gun_damage", "p_gun_damage"), &GunWeaponResource::set_gun_damage);
    ClassDB::bind_method(D_METHOD("get_gun_damage"), &GunWeaponResource::get_gun_damage);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "gun_damage"), "set_gun_damage", "get_gun_damage");

    ClassDB::bind_method(D_METHOD("set_reload_time", "p_reload_time"), &GunWeaponResource::set_reload_time);
    ClassDB::bind_method(D_METHOD("get_reload_time"), &GunWeaponResource::get_reload_time);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "reload_time"), "set_reload_time", "get_reload_time");

    // --- current_ammo ---
    ClassDB::bind_method(D_METHOD("set_current_ammo", "p_current_ammo"), &GunWeaponResource::set_current_ammo);
    ClassDB::bind_method(D_METHOD("get_current_ammo"), &GunWeaponResource::get_current_ammo);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "current_ammo"), "set_current_ammo", "get_current_ammo");

    // --- magazine_capacity ---
    ClassDB::bind_method(D_METHOD("set_magazine_capacity", "p_magazine_capacity"), &GunWeaponResource::set_magazine_capacity);
    ClassDB::bind_method(D_METHOD("get_magazine_capacity"), &GunWeaponResource::get_magazine_capacity);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "magazine_capacity"), "set_magazine_capacity", "get_magazine_capacity");

    // --- reserve_ammo ---
    ClassDB::bind_method(D_METHOD("set_reserve_ammo", "p_reserve_ammo"), &GunWeaponResource::set_reserve_ammo);
    ClassDB::bind_method(D_METHOD("get_reserve_ammo"), &GunWeaponResource::get_reserve_ammo);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "reserve_ammo"), "set_reserve_ammo", "get_reserve_ammo");

    // --- max_reserve_ammo ---
    ClassDB::bind_method(D_METHOD("set_max_reserve_ammo", "p_max_reserve_ammo"), &GunWeaponResource::set_max_reserve_ammo);
    ClassDB::bind_method(D_METHOD("get_max_reserve_ammo"), &GunWeaponResource::get_max_reserve_ammo);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "max_reserve_ammo"), "set_max_reserve_ammo", "get_max_reserve_ammo");

   // --- reload_ui ---
    ClassDB::bind_method(D_METHOD("set_reload_ui", "p_reload_ui"), &GunWeaponResource::set_reload_ui);
    ClassDB::bind_method(D_METHOD("get_reload_ui"), &GunWeaponResource::get_reload_ui);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "reload_ui", PROPERTY_HINT_RESOURCE_TYPE, "PackedScene"), "set_reload_ui", "get_reload_ui");


}