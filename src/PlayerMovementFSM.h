#pragma once
#include <godot_cpp/classes/Node.hpp>
#include <godot_cpp/core/class_db.hpp>


#include "FSM.h"
#include "FPSController.h"
#include "PlayerMovementStatsResource.h"

using namespace godot;

class PlayerMovementFSM : public FSM {
    GDCLASS(PlayerMovementFSM, FSM);

public:
    PlayerMovementFSM();
    ~PlayerMovementFSM();
    
    // Overriden functions
    void _process(double delta) override;
    // Class functions

protected:

    static void _bind_methods();

private:
    FPSController* player = nullptr;
    Ref<PlayerMovementStatsResource> stats;
    Vector3 wish_dir = Vector3();
    Vector3 cam_aligned_wish_dir = Vector3();


public:
    // Getters and setters for export variables
    void set_player(Node* p_player) { player = Object::cast_to<FPSController>(p_player); }
    Node* get_player() const { return player; }

    void set_stats(Ref<PlayerMovementStatsResource> p_stats) { stats = p_stats; }
    Ref<PlayerMovementStatsResource> get_stats() const { return stats; }

    void set_wish_dir(Vector3 p_wish_dir) { wish_dir = p_wish_dir; }
    Vector3 get_wish_dir() const { return wish_dir; }

    void set_cam_aligned_wish_dir(Vector3 p_cam_aligned_wish_dir) { cam_aligned_wish_dir = p_cam_aligned_wish_dir; }
    Vector3 get_cam_aligned_wish_dir() const { return cam_aligned_wish_dir; }
};