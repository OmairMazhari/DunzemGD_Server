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

    ClassDB::bind_method(D_METHOD("get_camera"), &FPSController::get_camera);

    ClassDB::bind_method(D_METHOD("handle_mouse_input", "x_offset", "y_offset"), &FPSController::handle_mouse_input);
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
        }
    } 
    // Set the weapon viewport to right size
    sub_viewport->set_size(DisplayServer::get_singleton()->window_get_size());
}

void FPSController::handle_mouse_input(float x_offset, float y_offset) {
    // Capture mouse on click
    // InputEventMouseButton* mouseMotionButton = Object::cast_to<InputEventMouseButton>(p_event.ptr());
    // if(mouseMotionButton){
    //     input->set_mouse_mode(input->MOUSE_MODE_CAPTURED);
    // } else if (input->is_action_just_pressed("ui_cancel")){
    //     input->set_mouse_mode(input->MOUSE_MODE_VISIBLE);
    // }
    // Control mouse motion
    rotate_y(-x_offset * lookSensitivity);
    camera->rotate_x(-y_offset * lookSensitivity);


    // Clamp the camera rotation
    Vector3 camRotation = camera->get_rotation();
    camRotation.x = Math::clamp(camera->get_rotation().x, Math::deg_to_rad(-90.0f), Math::deg_to_rad(90.0f));
    camera->set_rotation(camRotation);
        
    // if (input->get_mouse_mode() == input->MOUSE_MODE_CAPTURED){
        
    // }
    
    
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
