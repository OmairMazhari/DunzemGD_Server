#pragma once
#include <godot_cpp/classes/Node.hpp>
#include <godot_cpp/core/class_db.hpp>

#include <FSM/NodeFSM/FSM.h>
#include <Player/FPSController.h>
#include <Player/Movement/PlayerMovementStatsResource.h>

using namespace godot;

class PlayerMovementFSM : public FSM {
    GDCLASS(PlayerMovementFSM, FSM);

public:
    PlayerMovementFSM();
    ~PlayerMovementFSM();
    
    //Overriden functions
    void _process(double delta) override;
    void _physics_process(double delta) override;

    // Class functions
    void Update(double delta, Dictionary input_dict);

protected:
    static void _bind_methods();

private:
    FPSController* player = nullptr;
    Ref<PlayerMovementStatsResource> stats;
    Vector3 wish_dir = Vector3();
    Vector3 cam_aligned_wish_dir = Vector3();
    Dictionary input_dict;

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

    void set_input_dict(Dictionary p_input_dict) { input_dict = p_input_dict; }
    Dictionary get_input_dict() const { return input_dict; }
};