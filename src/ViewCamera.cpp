#include "ViewCamera.h"
#include "godot_cpp/core/math.hpp"
#include "godot_cpp/classes/input_event_mouse_motion.hpp"
#include "godot_cpp/classes/input.hpp"

using namespace godot;

ViewCamera::ViewCamera() {}

ViewCamera::~ViewCamera() {}

void ViewCamera::_process(double delta) {
    set_global_transform(camera->get_global_transform());
    // Prepare wish_dir to apply movement based sway
    // Vector2 wish_dir = Input::get_singleton()->get_vector("left", "right", "up", "down").normalized();
    // // Apply  sway effect to all the children of the view_model_camera
    // for(int i = 0; i < children.size(); i++){
    //         Node3D* child = Object::cast_to<Node3D>(children[i]);
    //         // Don't sway invisible items for performance and also common sense  
    //         if(child && child->is_visible()) { 
    //             sway(child, delta);
    //             // Move sway
                
    //         }   
    // }
    // children = get_children();
}

void ViewCamera::_ready() {
    camera = get_node<Camera3D>("../../../Head/Camera3D");
}

void ViewCamera::_unhandled_input(const Ref<InputEvent> &event) {
    // // Mouse weapon sway
    // InputEventMouseMotion* mouseMotionEvent = Object::cast_to<InputEventMouseMotion>(event.ptr());
    // if(mouseMotionEvent){
    //     // Apply mouse sway effect to all the children of the view_model_camera
    //     for(int i = 0; i < children.size(); i++){
    //         Node3D* child = Object::cast_to<Node3D>(children[i]);
    //         if(child && child->is_visible()) {
    //             mouse_sway(child, mouseMotionEvent);
    //         }
    //     }
    // }
}

void ViewCamera::sway(Node3D* node, double delta) {
     node->set_position(Vector3(
        Math::lerp((float)node->get_position().x, 0.0f, (float)delta*5.0f),
        Math::lerp((float)node->get_position().y, 0.0f, (float)delta*5.0f),
        node->get_position().z
    ));
}

void ViewCamera::mouse_sway(Node3D *node, InputEventMouseMotion* mouseMotionEvent) { 
    node->set_position(Vector3(
            node->get_position().x + mouseMotionEvent->get_relative().x * 0.0005f * look_sway_factor,
            node->get_position().y + mouseMotionEvent->get_relative().y * 0.0005f * look_sway_factor,
            node->get_position().z
        ));
}

void ViewCamera::_bind_methods() {
    // look_sway_factor
    ClassDB::bind_method(D_METHOD("set_look_sway_factor", "p_look_sway_factor"), &ViewCamera::set_look_sway_factor);
    ClassDB::bind_method(D_METHOD("get_look_sway_factor"), &ViewCamera::get_look_sway_factor);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "look_sway_factor"), "set_look_sway_factor", "get_look_sway_factor");
    // move_sway_factor
    ClassDB::bind_method(D_METHOD("set_move_sway_factor", "p_move_sway_factor"), &ViewCamera::set_move_sway_factor);
    ClassDB::bind_method(D_METHOD("get_move_sway_factor"), &ViewCamera::get_move_sway_factor);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "move_sway_factor"), "set_move_sway_factor", "get_move_sway_factor");
}