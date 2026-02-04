#pragma once
#include <godot_cpp/classes/Node.hpp>
#include <godot_cpp/core/class_db.hpp>

#include <FSM/ResourceFSM/FSM.h>

#include <Player/FPSController.h>
#include <Player/Movement/PlayerMovementStatsResource.h>
#include <godot_cpp/classes/animation_tree.hpp>
#include <godot_cpp/classes/animation_node_state_machine_playback.hpp>


using namespace godot;

class PlayerMovementFSM : public ResourceFSM { 
    GDCLASS(PlayerMovementFSM, ResourceFSM);

public:
    PlayerMovementFSM();
    ~PlayerMovementFSM();
    
    // Overriden functions
    void _process(double delta) override;
    void _physics_process(double delta) override;
    void _ready() override;

    // Class functions
    void Update(double delta, Dictionary input_dict, Vector3 r_wish_dir);

    enum MovementState{
        Move = 0,
        No_Clip = 1
    };

protected:
    static void _bind_methods();

private:
    // Onready Variables
    AnimationTree* characterAnimationTree = nullptr;
    AnimationNodeStateMachinePlayback* characterAnimationPlayback = nullptr;

    FPSController* player = nullptr;
    Ref<PlayerMovementStatsResource> stats;
    Vector3 wish_dir = Vector3();
    Vector3 cam_aligned_wish_dir = Vector3();
    Dictionary input_dict;

public:
    // Getters and setters for export variables
    void set_player(Node* p_player) { player = Object::cast_to<FPSController>(p_player); }
    Node* get_player() const { return player; }

    void set_input_dict(Dictionary p_input_dict) { input_dict = p_input_dict; }
    Dictionary get_input_dict() const { return input_dict; }

    void set_characterAnimationTree(AnimationTree* p_characterAnimationTree) { characterAnimationTree = p_characterAnimationTree; }
    AnimationTree* get_characterAnimationTree() const { return characterAnimationTree; }

    void set_characterAnimationPlayback(AnimationNodeStateMachinePlayback* p_characterAnimationPlayback) { characterAnimationPlayback = p_characterAnimationPlayback; }
    AnimationNodeStateMachinePlayback* get_characterAnimationPlayback() const { return characterAnimationPlayback; };

    Vector3 get_wish_dir() { return wish_dir; }
    void set_wish_dir(Vector3 r_wish_dir) { wish_dir = r_wish_dir; }
};