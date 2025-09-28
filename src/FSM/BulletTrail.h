#pragma once

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/time.hpp>
#include <godot_cpp/core/class_db.hpp>
#include "BulletTrailConfig.h"
namespace godot {

class BulletTrail : public Node3D {
    GDCLASS(BulletTrail, Node3D);

private:
    Node3D *bullet_trail_start = nullptr;
    Vector3 target_pos = Vector3();
    double speed = 200.0;
    double tracer_length = 1.0;
    uint64_t spawn_time = 0;
    static inline const uint64_t MAX_LIFETIME_MS = 5000;
    Ref<BulletTrailConfig> config;

    bool active = false;

protected:
    static void _bind_methods();

public:
    BulletTrail();
    ~BulletTrail();

    void _ready() override;
    void _process(double delta) override;

    // Exported vars
    void set_bullet_trail_start(Node3D *p_val);
    Node3D *get_bullet_trail_start() const;

    void set_speed(double p_val);
    double get_speed() const;

    void set_tracer_length(double p_val);
    double get_tracer_length() const;

    void set_config(const Ref<BulletTrailConfig> &p_config) { config = p_config; }
    Ref<BulletTrailConfig> get_config() const { return config; }

    // API
    void make(const Vector3 &p_target);
};

} // namespace godot

