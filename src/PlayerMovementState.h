#pragma once

#include <godot_cpp/core/class_db.hpp>

#include "State.h"
#include "PlayerMovementFSM.h"
#include "PlayerMovementStatsResource.h"
#include "FPSController.h"
#include "godot_cpp/classes/input.hpp"

using namespace godot;

class PlayerMovementState : public State {
    GDCLASS(PlayerMovementState, State);
public:
    PlayerMovementState() { }
    ~PlayerMovementState() { }

    void _ready() override {
        input = Input::get_singleton();
        server = get_node<Node>("../../../../Server");
        // player = get_node<FPSController>("../../FPSController");
    }

    void Enter() override {
        emit_signal("state_transition", this, "Move_PMState");
    }

protected:
    PlayerMovementFSM* FSM = nullptr;
    Ref<PlayerMovementStatsResource> stats;
    FPSController* player = nullptr;
    Input* input = nullptr;
    Node* server = nullptr;

    void initialize_values(){
        if(FSM){
            stats = FSM->get_stats();
            player = Object::cast_to<FPSController>(FSM->get_player());
        }
    }

    float get_move_speed() {
        if(input->is_action_pressed("sprint")){
            return stats->get_sprint_speed();
        } else {
            return stats->get_walk_speed();
        }
    }

    static void _bind_methods(){
        ClassDB::bind_method(D_METHOD("set_FSM", "p_FSM"), &PlayerMovementState::set_FSM);
        ClassDB::bind_method(D_METHOD("get_FSM"), &PlayerMovementState::get_FSM);
        ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "FSM", PROPERTY_HINT_NODE_TYPE, "PlayerMovementFSM"), "set_FSM", "get_FSM");
    }

public:
    

    // Setters and Getters
    void set_FSM(Node* p_FSM) {
         FSM = Object::cast_to<PlayerMovementFSM>(p_FSM);
         initialize_values();
    }
    Node* get_FSM() const { return FSM; }
};