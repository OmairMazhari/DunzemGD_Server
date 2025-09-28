#pragma once

#include "State.h"
#include "Gun.h"
#include "Player.h"

#include <godot_cpp/classes/animation_player.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/audio_stream.hpp>

using namespace godot;

class GunState : public State {
    GDCLASS(GunState, State);

public:
    GunState();
    ~GunState();

protected:
    static void _bind_methods();

public:
    // Exported gun & animation player
    Gun *gun = nullptr;
    AnimationPlayer *animation_player = nullptr;

    bool ADS = false;
    bool pull_out_complete = false;
    bool setup_complete = false;

    double rate = 0.0;
    double setup_time = 0.0;
    bool reloading = false;

    // --- State Lifecycle ---
    void _ready();
    void Enter(Player *player) override;
    void Exit(Player *player) override;
    void Update(double delta, Player *player) override;

    void set_gun(Node* p_gun) { gun = Object::cast_to<Gun>(p_gun); }
    Gun* get_gun() const { return gun; }

    // --- AnimationPlayer getter/setter ---
    void set_animation_player(Node* p_anim) { animation_player = Object::cast_to<AnimationPlayer>(p_anim); }
    AnimationPlayer* get_animation_player() const { return animation_player; }

private:
    void reload();
    void setup(double delta);
};
