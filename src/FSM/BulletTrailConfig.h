#pragma once

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/node3d.hpp>

namespace godot {

class BulletTrailConfig : public Resource {
    GDCLASS(BulletTrailConfig, Resource);

private:
    double speed = 200.0;
    double tracer_length = 1.0;
    Node3D* barrel_node = nullptr;
    Node3D* target_pos = nullptr;

protected:
    static void _bind_methods();

public:
    void set_speed(double p_val) { speed = p_val; }
    double get_speed() const { return speed; }

    void set_tracer_length(double p_val) { tracer_length = p_val; }
    double get_tracer_length() const { return tracer_length; }

    void set_barrel_node(Node3D *p_val) { barrel_node = p_val; }
    Node3D *get_barrel_node() const { return barrel_node; }

    void set_target_pos(Node3D *p_val) { target_pos = p_val; }
    Node3D *get_target_pos() { return target_pos; }
};

} // namespace godot
