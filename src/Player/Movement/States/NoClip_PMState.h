#pragma once

#include <godot_cpp/core/class_db.hpp>

#include "PlayerMovementState.h"

using namespace godot;

class NoClip_PMState : public PlayerMovementState {
    GDCLASS(NoClip_PMState, PlayerMovementState);
public:
    NoClip_PMState() {}
    ~NoClip_PMState() {}

    // Overriden functions
	void unhandled_state_input(const Ref<InputEvent> &p_event);
    void Enter() override;
    void Update(double delta) override;
    void Exit() override;
    void Setup(ResourceFSM* FSM) override;

    // Class functions
    float get_move_speed();

protected:
    static void _bind_methods();
    float old_no_clip_speed_mult = 0.0f;

private:
    // Class Variables
    Vector3 cam_aligned_wish_dir = Vector3();

    // Export Variables
    // No clip
    float no_clip_speed_mult = 3.0;
    bool no_clip = false;

    float walk_speed = 7.0f;
    float sprint_speed = 8.5f;

public:
    void set_no_clip_speed_mult(float p_no_clip_speed_mult) { no_clip_speed_mult = p_no_clip_speed_mult; }
    float get_no_clip_speed_mult() const { return no_clip_speed_mult; }

    void set_no_clip(bool p_no_clip) { no_clip = p_no_clip; }
    bool  get_no_clip() const { return no_clip; }

    void set_walk_speed(float p_walk_speed) { walk_speed = p_walk_speed; }
    float get_walk_speed() const { return walk_speed; }

    void set_sprint_speed(float p_sprint_speed) { sprint_speed = p_sprint_speed; }
    float get_sprint_speed() const { return sprint_speed; }

};