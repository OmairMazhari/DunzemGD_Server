#pragma once
#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/Node3d.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include "godot_cpp/classes/input_event_mouse_motion.hpp"



#include "FPSController.h"

using namespace godot;


class ViewCamera : public Camera3D {
    GDCLASS(ViewCamera, Camera3D);
public:
    ViewCamera();
    ~ViewCamera();
    // Overriden functions
    void _process(double delta) override;
    void _ready() override;
    void _unhandled_input(const Ref<InputEvent> &event) override;
    // Class functions
    void sway(Node3D* node, double delta);
    void mouse_sway(Node3D* node, InputEventMouseMotion* mouseMotionEvent); // find a more appropriate name

protected:
    static void _bind_methods();

private:
    // Scene tree variables
    Camera3D* camera = nullptr;

    // Export Variables
    float look_sway_factor = 1.0f;
    float move_sway_factor = 1.0f;

    // Class Variables
    TypedArray<Node> children;

public:
    // Setters and getters

    void set_look_sway_factor(float p_look_sway_factor) { look_sway_factor = p_look_sway_factor; }
    float get_look_sway_factor() const { return look_sway_factor; }

    void set_move_sway_factor(float p_move_sway_factor) { move_sway_factor = p_move_sway_factor; }
    float get_move_sway_factor() const { return move_sway_factor; }

};
