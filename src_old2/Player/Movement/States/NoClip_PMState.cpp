#include "NoClip_PMState.h"

#include <godot_cpp/classes/input_event_mouse_button.hpp>


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
    Dictionary event_input = player->get_input_dict()["event_based_actions"];

	if(event_input["no_clip"]){
        UtilityFunctions::print("pressed unclip");
        FSM->change_state(this, "move_pmstate");
        event_input["no_clip"] = !event_input["no_clip"];
    }
    float speed = get_move_speed() * stats->get_no_clip_speed_mult();
    
     UtilityFunctions::print("Get move speed: " + UtilityFunctions::str(get_move_speed()) + " stats->get_no_clip_speed_mult: "
         + UtilityFunctions::str(stats->get_no_clip_speed_mult()));

    if(event_input.has("sprint")){
        if (event_input["sprint"]) {
          speed *= 3.0f;
        }
    }   
   
    UtilityFunctions::print("cam_aligned_wish_dir" + UtilityFunctions::str(FSM->get_cam_aligned_wish_dir()));
    UtilityFunctions::print("speed" + UtilityFunctions::str(speed));
    player->set_velocity(FSM->get_cam_aligned_wish_dir() * speed);
    player->move_and_slide();
    UtilityFunctions::print("Velocity no clip" + UtilityFunctions::str(player->get_velocity().length()));
}



void NoClip_PMState::Exit() {
    player->get_collision_shape()->set_disabled(false);
    stats->set_no_clip_speed_mult(old_no_clip_speed_mult);
}



void NoClip_PMState::_bind_methods() {

}
