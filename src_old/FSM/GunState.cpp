#include "GunState.h"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/timer.hpp>

GunState::GunState() {}
GunState::~GunState() {}

void GunState::_bind_methods() {

    ClassDB::bind_method(D_METHOD("set_gun", "gun"), &GunState::set_gun);
    ClassDB::bind_method(D_METHOD("get_gun"), &GunState::get_gun);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "gun", PROPERTY_HINT_NODE_TYPE, "Gun"),
                 "set_gun", "get_gun");

    // --- AnimationPlayer ---
    ClassDB::bind_method(D_METHOD("set_animation_player", "animation_player"), &GunState::set_animation_player);
    ClassDB::bind_method(D_METHOD("get_animation_player"), &GunState::get_animation_player);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "animation_player", PROPERTY_HINT_NODE_TYPE, "AnimationPlayer"),
                 "set_animation_player", "get_animation_player");
}

void GunState::_ready() {
    if (gun && gun->reload_sfx && gun->reload_sfx->get_stream().is_valid()) {
        float stream_length = gun->reload_sfx->get_stream()->get_length();
        gun->reload_sfx->set_pitch_scale(stream_length * (1.0f / gun->reload_time));
        //gun->gun_holder = Object::cast_to<Player>(get_parent()->get("player"));
    }
}

void GunState::Enter(Player *player) {
    if (!gun) return;

    gun->set_visible(true);
    if (gun->animation_player) {
        gun->animation_player->play("Pull_out", -1, 1.0 / gun->pull_out_time);
    }
    setup_time = gun->pull_out_time;
}

void GunState::Exit(Player *player) {
    if (!gun) return;

    if (gun->animation_player) {
        gun->animation_player->play("Pull_away", -1, 1.0 / gun->pull_out_time);
    }

    gun->set_visible(false);
    ADS = false;
    rate = 0;
    setup_complete = false;

    if (reloading) {
        reloading = false;
        if (gun->reload_sfx) gun->reload_sfx->stop();
        if (gun->animation_player) gun->animation_player->stop();
    }
}

void GunState::Update(double delta, Player *player) {
    if (!gun) return;

    if (!setup_complete) {
        setup(delta);
        return;
    }

    Input *input = Input::get_singleton();

    if (input->is_action_pressed("left_click")) {
        if (!reloading && !gun->clip_empty) {
            gun->shoot(delta);
            if (gun->animation_player)
                gun->animation_player->play("Shoot", -1, gun->fire_rate);
            if (gun->shot && gun->shoot_sfx)
                gun->shoot_sfx->play();
        } else {
            reload();
        }
        if (gun->hit && player) {
            player->on_hit();
        }
    }

    if (input->is_action_just_pressed("right_click")) {
        ADS = !ADS;
    }

    gun->aim(delta, ADS);

    if (input->is_action_just_pressed("reload") && !reloading) {
        reload();
    }
}

void GunState::reload() {
    if (!gun) return;
    if (gun->current_magzine != gun->magzine_size && !reloading) {
        if (gun->animation_player) {
            gun->animation_player->play("Reload", -1, 1.0 / gun->reload_time);
        }
        if (gun->reload_sfx) {
            gun->reload_sfx->play();
        }
        reloading = true;
        reloading = false;
        gun->reload(true);
        UtilityFunctions::print(Engine::get_singleton()->get_frames_per_second());


        // NOTE: In C++ we don’t have `await`, so you would use a Timer node or signal
        // Example: spawn Timer programmatically and connect timeout -> gun->reload(reloading)
    }
}

void GunState::setup(double delta) {
    rate += delta;
    if (rate >= setup_time) {
        if (gun->animation_player && gun->animation_player->is_playing()) {
            UtilityFunctions::print("anim playing and true");
        }
        setup_complete = true;
    } else {
        setup_complete = false;
    }
}
