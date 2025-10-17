#include <Player/FPSController.h>
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
    ClassDB::bind_method(D_METHOD("handle_mouse_input", "x_offset", "y_offset"), &FPSController::handle_mouse_input);

}

void FPSController::_ready() {
    // Initialize the relevant scene tree nodes
    worldModel = get_node<Node>("WorldModel");
    camera = get_node<Camera3D>("Head/Camera3D");
    collision_shape = get_node<CollisionShape3D>("CollisionShape3D");
    sub_viewport = get_node<SubViewport>("SubViewportContainer/SubViewport");
    cameraAnchor = get_node<Marker3D> ("CameraAnchor");
    // head = get_node<Node3D> ("Head");
    // head_origin_position = head->get_position();

    // head->set_physics_interpolation_mode(Node::PHYSICS_INTERPOLATION_MODE_OFF);
    // head->set_as_top_level(true);


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
    // Capture mouse on click
    // Input* input = Input::get_singleton();
    // InputEventMouseButton* mouseMotionButton = Object::cast_to<InputEventMouseButton>(p_event.ptr());
    // if(mouseMotionButton){
    //     input->set_mouse_mode(input->MOUSE_MODE_CAPTURED);
    // } else if (input->is_action_just_pressed("ui_cancel")){
    //     input->set_mouse_mode(input->MOUSE_MODE_VISIBLE);
    // }
    // // Control mouse motion
    // if (input->get_mouse_mode() == input->MOUSE_MODE_CAPTURED){
    //     InputEventMouseMotion* mouseMotionEvent = Object::cast_to<InputEventMouseMotion>(p_event.ptr());
    //     if(mouseMotionEvent){
    //         // rotate_y(-mouseMotionEvent->get_relative().x * lookSensitivity);
    //         rotate_y(-mouseMotionEvent->get_relative().x * lookSensitivity);
    //         // set_rotation(Vector3(get_rotation().x, UtilityFunctions::round(get_rotation().y * 1000.0 / 1000.0), get_rotation().z));
            
    //         // cameraAnchor->rotate_x(-mouseMotionEvent->get_relative().y * lookSensitivity);
    //         // // Clamp the camera rotation
    //         // Vector3 camRotation = cameraAnchor->get_rotation();
    //         // camRotation.x = Math::clamp(cameraAnchor->get_rotation().x, Math::deg_to_rad(-90.0f), Math::deg_to_rad(90.0f));
    //         // cameraAnchor->set_rotation(camRotation);

    //         camera->rotate_x(-mouseMotionEvent->get_relative().y * lookSensitivity);

    //         // Clamp the camera rotation
    //         Vector3 camRotation = camera->get_rotation();
    //         camRotation.x = Math::clamp(camera->get_rotation().x, Math::deg_to_rad(-90.0f), Math::deg_to_rad(90.0f));
    //         camera->set_rotation(camRotation);
    //     }
    // }

}


void FPSController::handle_mouse_input(int x_offset, int y_offset) {
    // Capture mouse on click
    // InputEventMouseButton* mouseMotionButton = Object::cast_to<InputEventMouseButton>(p_event.ptr());
    // if(mouseMotionButton){
    //     input->set_mouse_mode(input->MOUSE_MODE_CAPTURED);
    // } else if (input->is_action_just_pressed("ui_cancel")){
    //     input->set_mouse_mode(input->MOUSE_MODE_VISIBLE);
    // }
    // Control mouse motion

    //  rotate_y(-x_offset * lookSensitivity);
    
    // // Clamp camera pitch to prevent over-rotation
    // float current_pitch = camera->get_rotation().x;
    // float new_pitch = current_pitch - y_offset * lookSensitivity;
    // new_pitch = UtilityFunctions::clamp(new_pitch, -Math_PI/2, Math_PI/2);
    // camera->set_rotation(Vector3(new_pitch, camera->get_rotation().y, camera->get_rotation().z));


    
    // UtilityFunctions::print("Plyaer Rotation Before: "  + get_rotation());
    rotate_y(-x_offset * lookSensitivity);
    // UtilityFunctions::print(" Player  : " + get_rotation());
    // UtilityFunctions::print("Camera Rotation Before: "  + camera->get_rotation());
    camera->rotate_x(-y_offset * lookSensitivity);
    // UtilityFunctions::print(" After  : " + camera->get_rotation());

    // Clamp the camera rotation
    Vector3 camRotation = camera->get_rotation();
    camRotation.x = Math::clamp(camera->get_rotation().x, Math::deg_to_rad(-90.0f), Math::deg_to_rad(90.0f));
    camera->set_rotation(camRotation);
        

    // if (input->get_mouse_mode() == input->MOUSE_MODE_CAPTURED){
        
    // }
    
}

void FPSController::_process(double delta) {

    /*
    var tr : Transform3D = self.get_global_transform_interpolated()
    camera_mount.global_position = lerp(camera_mount.global_position, 
        tr.origin + self.camera_mount_orig_position, min(delta*15, 1.0)
    )
    camera_mount.global_basis = camera_mount.global_basis.slerp(self.global_basis.orthonormalized(), 15 * delta)
    camera_mount.global_basis = camera_mount.global_basis.orthonormalized()
    self.global_basis = self.global_basis.orthonormalized()
    */ 

    // Transform3D tr = get_global_transform_interpolated();
    // head->set_global_position(UtilityFunctions::lerp(head->get_global_position(), tr.get_origin()  + head_origin_position, 
    //     UtilityFunctions::min(delta*60.0f, 1.0f)));
    // head->set_global_basis(head->get_global_basis().slerp(get_global_basis().orthonormalized(), 60.0f * delta));
    // head->set_global_basis(head->get_global_basis().orthonormalized());
    // set_global_basis(get_global_basis().orthonormalized());
}

void FPSController::_physics_process(double delta) {

}
