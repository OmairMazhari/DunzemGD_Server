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
    // hanlde_mouse_input
    ClassDB::bind_method(D_METHOD("handle_mouse_input", "x_offset", "y_offset"), &FPSController::handle_mouse_input);
    // max_health
    ClassDB::bind_method(D_METHOD("set_max_health", "p_val"), &FPSController::set_max_health);
    ClassDB::bind_method(D_METHOD("get_max_health"), &FPSController::get_max_health);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "max_health"), "set_max_health", "get_max_health");
    // curr_health
    ClassDB::bind_method(D_METHOD("set_curr_health", "p_val"), &FPSController::set_curr_health);
    ClassDB::bind_method(D_METHOD("get_curr_health"), &FPSController::get_curr_health);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "curr_health"), "set_curr_health", "get_curr_health");
    // Update
    ClassDB::bind_method(D_METHOD("Update", "delta"), &FPSController::Update);
    // is_dead
    ClassDB::bind_method(D_METHOD("is_dead"), &FPSController::is_dead);

     // get_hit
    ClassDB::bind_method(D_METHOD("get_hit"), &FPSController::get_hit);
     // set_hit
    ClassDB::bind_method(D_METHOD("set_hit"), &FPSController::set_hit);

    // get_combat_report
    ClassDB::bind_method(D_METHOD("get_combat_report"), &FPSController::get_combat_report);
    // set_combat_report
    ClassDB::bind_method(D_METHOD("set_combat_report", "p_val"), &FPSController::set_combat_report);
    // kills
    ClassDB::bind_method(D_METHOD("set_kills", "p_kills"), &FPSController::set_kills);
    ClassDB::bind_method(D_METHOD("get_kills"), &FPSController::get_kills);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "kills"), "set_kills", "get_kills");

    // Died
    ClassDB::add_signal("FPSController", MethodInfo("died"));



}

void FPSController::_ready() {
    // Initialize the relevant scene tree nodes
    worldModel = get_node<Node>("WorldModel");
    camera = get_node<Camera3D>("Head/Camera3D");
    collision_shape = get_node<CollisionShape3D>("CollisionShape3D");
    sub_viewport = get_node<SubViewport>("SubViewportContainer/SubViewport");
    cameraAnchor = get_node<Marker3D> ("CameraAnchor");

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

void FPSController::_unhandled_input(const Ref<InputEvent> &p_event) {


}

void FPSController::Update(double delta) {

}

void FPSController::handle_mouse_input(int x_offset, int y_offset) {
	Input* input = Input::get_singleton();
    if(input->is_action_just_pressed("enter")){
        input->set_mouse_mode(input->MOUSE_MODE_CAPTURED);
    } else if (input->is_action_just_pressed("ui_cancel")){
        input->set_mouse_mode(input->MOUSE_MODE_VISIBLE);
    }
    rotate_y(-x_offset * lookSensitivity);

    camera->rotate_x(-y_offset * lookSensitivity);

    Vector3 camRotation = camera->get_rotation();
    camRotation.x = Math::clamp(camera->get_rotation().x, Math::deg_to_rad(-90.0f), Math::deg_to_rad(90.0f));
    camera->set_rotation(camRotation);
}

bool FPSController::take_damage(int damage) {
    curr_health -= damage;
    if (curr_health <= 0){
        curr_health = 0;
        on_dead();
        emit_signal("died");
        return true;
    }
    return false;
}

void FPSController::on_dead() {
    return;
}

bool FPSController::get_hit() {
	return on_hit;
}

void FPSController::set_hit(bool hit) {
    on_hit = hit;
}

bool FPSController::is_dead() {
	return !curr_health;
}

void FPSController::_process(double delta) {
    
}

void FPSController::_physics_process(double delta) {

}
