#include "NoClip_PMState.h"

#include <godot_cpp/classes/input_event_mouse_button.hpp>
#include "utils.hpp"


void NoClip_PMState::Setup(ResourceFSM *FSM) {
    PlayerMovementState::Setup(FSM);
}

void NoClip_PMState::unhandled_state_input(const Ref<InputEvent> &p_event) {
    InputEventMouseButton* mouseButtonEvent = Object::cast_to<InputEventMouseButton>(p_event.ptr());
    if(mouseButtonEvent && mouseButtonEvent->is_pressed()){
        if (mouseButtonEvent->get_button_index() == MOUSE_BUTTON_WHEEL_UP){
            no_clip_speed_mult = MIN(100.0, no_clip_speed_mult * 1.1);
        }
        if (mouseButtonEvent->get_button_index() == MOUSE_BUTTON_WHEEL_DOWN){
            no_clip_speed_mult = MAX(0.1, no_clip_speed_mult * 0.9);
        }
    }
}

void NoClip_PMState::Enter() {
    old_no_clip_speed_mult = no_clip_speed_mult;
    player->get_collision_shape()->set_disabled(true);
}

void NoClip_PMState::Update(double delta) {

    // Calculate cam_aligned_wish_dir
    Vector2 input_dir = utils::get_custom_vector(input["state_based_actions"], "left", "right", "up", "down");
    cam_aligned_wish_dir = player->get_camera()->get_global_transform().basis.xform(Vector3(input_dir.x, 0, input_dir.y));   

	if(input["event_based_actions"]["jump"] ){
        FSM->change_state(Ref<ResourceState>(Object::cast_to<ResourceState>(FSM->get_states()[FSM->Move])));
    }
    float speed = get_move_speed() * no_clip_speed_mult;
    
    if (input["state_based_actions"]["sprint"]) {
        speed *= 3.0f;
    }
    player->set_velocity(cam_aligned_wish_dir * speed);
    player->move_and_slide();
}



void NoClip_PMState::Exit() {

    player->get_collision_shape()->set_disabled(false);
    no_clip_speed_mult = old_no_clip_speed_mult;
}

float NoClip_PMState::get_move_speed() {
	if(input["state_based_actions"]["sprint"]){
        return sprint_speed;
    } else {
        return walk_speed;
    }
}

void NoClip_PMState::_bind_methods() {
    // ---- No Clip ---- 
    ADD_GROUP("No Clip", "no_clip_");
    // no_clip_speed_mult
    ClassDB::bind_method(D_METHOD("set_no_clip_speed_mult", "p_no_clip_speed_mult"), &NoClip_PMState::set_no_clip_speed_mult);
    ClassDB::bind_method(D_METHOD("get_no_clip_speed_mult"), &NoClip_PMState::get_no_clip_speed_mult);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "no_clip_speed_mult"), "set_no_clip_speed_mult", "get_no_clip_speed_mult");
    // no_clip
    ClassDB::bind_method(D_METHOD("set_no_clip", "p_no_clip"), &NoClip_PMState::set_no_clip);
    ClassDB::bind_method(D_METHOD("get_no_clip"), &NoClip_PMState::get_no_clip);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "no_clip"), "set_no_clip", "get_no_clip");

    // walk_speed
    ClassDB::bind_method(D_METHOD("set_walk_speed", "p_walk_speed"), &NoClip_PMState::set_walk_speed);
    ClassDB::bind_method(D_METHOD("get_walk_speed"), &NoClip_PMState::get_walk_speed);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "walk_speed"), "set_walk_speed", "get_walk_speed");
    // sprint_speed
    ClassDB::bind_method(D_METHOD("set_sprint_speed", "p_sprint_speed"), &NoClip_PMState::set_sprint_speed);
    ClassDB::bind_method(D_METHOD("get_sprint_speed"), &NoClip_PMState::get_sprint_speed);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "sprint_speed"), "set_sprint_speed", "get_sprint_speed");
}
