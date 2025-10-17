#pragma once
#include <PlayerMovementState.h>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

class NoClip_PMState : public PlayerMovementState {
    GDCLASS(NoClip_PMState, PlayerMovementState);
public:
    NoClip_PMState() {}
    ~NoClip_PMState() {}

    // Overriden functions
	void _unhandled_input(const Ref<InputEvent> &p_event) override;
    void Enter() override;
    void Update(double delta) override;
    void Exit() override;
protected:
    static void _bind_methods();
    float old_no_clip_speed_mult = 0.0f;


};