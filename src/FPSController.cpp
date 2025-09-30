#include "FPSController.h"
#include "godot_cpp/classes/visual_instance3d.hpp"
#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/input_event_mouse_button.hpp"
#include "godot_cpp/classes/input_event_mouse_motion.hpp"
#include "godot_cpp/classes/input.hpp"
#include "godot_cpp/core/math.hpp"
#include "godot_cpp/classes/display_server.hpp"


using namespace godot;

FPSController::FPSController() {}

FPSController::~FPSController() {}

void FPSController::_bind_methods() {
    // lookSensitivity
    ClassDB::bind_method(D_METHOD("set_lookSensitivity", "p_lookSensitivity"), &FPSController::set_lookSensitivity);
    ClassDB::bind_method(D_METHOD("get_lookSensitivity"), &FPSController::get_lookSensitivity);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "lookSensitivity"), "set_lookSensitivity", "get_lookSensitivity");

    ClassDB::bind_method(D_METHOD("set_input_dict", "dict"), &FPSController::set_input_dict);
    ClassDB::bind_method(D_METHOD("get_input_dict"), &FPSController::get_input_dict);
}

void FPSController::_ready() {
    UtilityFunctions::print("Ready from server");
    // Initialize the relevant scene tree nodes
    worldModel = get_node<Node>("WorldModel");
    camera = get_node<Camera3D>("Head/Camera3D");
    collision_shape = get_node<CollisionShape3D>("CollisionShape3D");
    sub_viewport = get_node<SubViewport>("SubViewportContainer/SubViewport");

    if(worldModel){
        TypedArray<Node> children = worldModel->find_children("*", "VisualInstance3D"); 
        VisualInstance3D* child = nullptr; 
        for(int i = 0; i < children.size(); i++){
            child = Object::cast_to<VisualInstance3D>(children[i]);
            child->set_layer_mask_value(1, false);
            child->set_layer_mask_value(2, true);
            UtilityFunctions::print("Yes");
        }
    } 
    // Set the weapon viewport to right size
    sub_viewport->set_size(DisplayServer::get_singleton()->window_get_size());
}

void FPSController::_unhandled_input(const Ref<InputEvent> &p_event) {
    // Capture mouse on click
    Input* input = Input::get_singleton();
    InputEventMouseButton* mouseMotionButton = Object::cast_to<InputEventMouseButton>(p_event.ptr());
    if(mouseMotionButton){
        input->set_mouse_mode(input->MOUSE_MODE_CAPTURED);
    } else if (input->is_action_just_pressed("ui_cancel")){
        input->set_mouse_mode(input->MOUSE_MODE_VISIBLE);
    }
    // Control mouse motion
    if (input->get_mouse_mode() == input->MOUSE_MODE_CAPTURED){
        InputEventMouseMotion* mouseMotionEvent = Object::cast_to<InputEventMouseMotion>(p_event.ptr());
        if(mouseMotionEvent){
            rotate_y(-mouseMotionEvent->get_relative().x * lookSensitivity);
            camera->rotate_x(-mouseMotionEvent->get_relative().y * lookSensitivity);
            // Clamp the camera rotation
            Vector3 camRotation = camera->get_rotation();
            camRotation.x = Math::clamp(camera->get_rotation().x, Math::deg_to_rad(-90.0f), Math::deg_to_rad(90.0f));
            camera->set_rotation(camRotation);
        }
    }
}


void FPSController::_process(double delta) {

}

void FPSController::_physics_process(double delta) {

}

Dictionary FPSController::get_input_dict() const {
    return input_dict;
}

void FPSController::set_input_dict(Dictionary dict) {
    input_dict = dict;
}
