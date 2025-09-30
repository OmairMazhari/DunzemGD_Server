#include "NoClip_PMState.h"
#include "godot_cpp/classes/input_event_mouse_button.hpp"


void NoClip_PMState::_unhandled_input(const Ref<InputEvent> &p_event) {
    InputEventMouseButton* mouseButtonEvent = Object::cast_to<InputEventMouseButton>(p_event.ptr());
    if(mouseButtonEvent && mouseButtonEvent->is_pressed()){
        if (mouseButtonEvent->get_button_index() == MOUSE_BUTTON_WHEEL_UP){
            stats->set_no_clip_speed_mult(MIN(100.0, stats->get_no_clip_speed_mult() * 1.1));
        }
        if (mouseButtonEvent->get_button_index() == MOUSE_BUTTON_WHEEL_DOWN){
            stats->set_no_clip_speed_mult(MAX(0.1, stats->get_no_clip_speed_mult() * 0.9));
        }
    }
}

void NoClip_PMState::Enter() {
	UtilityFunctions::print("No Clipping");
    old_no_clip_speed_mult = stats->get_no_clip_speed_mult();
    player->get_collision_shape()->set_disabled(true);
}

void NoClip_PMState::Update(double delta) {
	if(input->is_action_just_pressed("no_clip")){
        UtilityFunctions::print("pressed unclip");
        FSM->change_state(this, "move_pmstate");
    }
    float speed = get_move_speed() * stats->get_no_clip_speed_mult();
    if (input->is_action_pressed("sprint")) {
        speed *= 3.0f;
    }
    player->set_velocity(FSM->get_cam_aligned_wish_dir() * speed);
    player->set_global_position(player->get_global_position() + (player->get_velocity() * delta)); 
}



void NoClip_PMState::Exit() {
    player->get_collision_shape()->set_disabled(false);
    stats->set_no_clip_speed_mult(old_no_clip_speed_mult);
}



void NoClip_PMState::_bind_methods() {

}
